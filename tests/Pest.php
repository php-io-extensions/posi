<?php

declare(strict_types=1);

if (! extension_loaded('posi')) {
    throw new RuntimeException('The posi extension is not loaded; run pest with -d extension=/path/to/posi.so');
}

/** Descriptors a test opened through track(); closeTracked() closes them after each test. */
$GLOBALS['posiFds'] = [];

function track(int $fd): int
{
    if ($fd < 0) {
        throw new RuntimeException('open failed, errno ' . posi_errno());
    }
    $GLOBALS['posiFds'][] = $fd;

    return $fd;
}

function closeTracked(): void
{
    foreach ($GLOBALS['posiFds'] as $fd) {
        posix_close($fd);
    }
    $GLOBALS['posiFds'] = [];
}

function scratchPath(string $name): string
{
    $dir = sys_get_temp_dir() . '/posi-tests-' . getmypid();
    if (! is_dir($dir)) {
        mkdir($dir);
    }

    return "{$dir}/{$name}";
}

function removeScratch(): void
{
    $dir = sys_get_temp_dir() . '/posi-tests-' . getmypid();
    if (is_dir($dir)) {
        foreach (scandir($dir) as $entry) {
            if ($entry !== '.' && $entry !== '..') {
                unlink("{$dir}/{$entry}");
            }
        }
        rmdir($dir);
    }
}

/** A FIFO opened at both ends without blocking: [read fd, write fd]. */
function fifoPair(): array
{
    $path = scratchPath('fifo-' . bin2hex(random_bytes(4)));
    posix_mkfifo($path, 0600);
    $read = track(posix_open($path, O_RDONLY | O_NONBLOCK));
    $write = track(posix_open($path, O_WRONLY | O_NONBLOCK));

    return [$read, $write];
}

/** FIONREAD from <sys/ioctl.h>: bytes waiting on a descriptor. Not POSIX, so posi does not register it. */
function fionread(): int
{
    return PHP_OS_FAMILY === 'Darwin' ? 0x4004667F : 0x541B;
}

/** FIOCLEX from <sys/ioctl.h>: sets close-on-exec, takes no argument. */
function fioclex(): int
{
    return PHP_OS_FAMILY === 'Darwin' ? 0x20006601 : 0x5451;
}

/**
 * A pty pair: [master fd, slave fd], unlocked the platform's way. macOS uses
 * TIOCPTYGRANT, TIOCPTYUNLK and TIOCPTYGNAME from <sys/ttycom.h>; Linux uses
 * TIOCSPTLCK and TIOCGPTN from <asm-generic/ioctls.h>.
 */
function ptyPair(): array
{
    $master = track(posix_open('/dev/ptmx', O_RDWR | O_NOCTTY));

    if (PHP_OS_FAMILY === 'Darwin') {
        ioctl($master, 0x20007454, null, $unused);
        ioctl($master, 0x20007452, null, $unused);
        ioctl($master, 0x40807453, str_repeat("\0", 128), $name);
        $path = rtrim($name, "\0");
    } else {
        ioctl($master, 0x40045431, ['value' => 0], $unused);
        ioctl($master, 0x80045430, ['value' => 0], $number);
        $path = "/dev/pts/{$number}";
    }

    return [$master, track(posix_open($path, O_RDWR | O_NOCTTY))];
}

/**
 * The descriptor number behind a stream: the /dev/fd entry whose inode and
 * mode match the stream's fstat(). Linux names a socket's entry
 * "socket:[inode]", which stat() cannot follow, so its link name is matched.
 *
 * @param resource $stream
 */
function fdOf($stream): int
{
    clearstatcache();
    $want = fstat($stream);
    set_error_handler(static fn (): bool => true);
    try {
        foreach (scandir('/dev/fd') as $entry) {
            if (! ctype_digit($entry)) {
                continue;
            }
            $st = stat("/dev/fd/{$entry}");
            if ($st !== false && $st['ino'] === $want['ino'] && $st['mode'] === $want['mode']) {
                return (int) $entry;
            }
            if (PHP_OS_FAMILY === 'Linux' && readlink("/dev/fd/{$entry}") === "socket:[{$want['ino']}]") {
                return (int) $entry;
            }
        }
    } finally {
        restore_error_handler();
    }

    throw new RuntimeException('No /dev/fd entry matches the stream');
}

pest()->afterEach(function (): void {
    closeTracked();
    removeScratch();
})->in(__DIR__);
