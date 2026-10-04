<?php

declare(strict_types=1);

$darwin = PHP_OS_FAMILY === 'Darwin';

it('registers the platform values of <fcntl.h>, not one platform\'s numbers', function () use ($darwin) {
    expect(O_RDONLY)->toBe(0)
        ->and(O_WRONLY)->toBe(1)
        ->and(O_RDWR)->toBe(2)
        ->and(O_NONBLOCK)->toBe($darwin ? 0x0004 : 0x0800)
        ->and(O_CREAT)->toBe($darwin ? 0x0200 : 0x0040)
        ->and(O_CLOEXEC)->toBe($darwin ? 0x01000000 : 0x80000)
        ->and(F_GETFL)->toBe(3)
        ->and(F_SETFL)->toBe(4)
        ->and(FD_CLOEXEC)->toBe(1);
});

it('registers Linux-only open() flags only where the platform defines them', function () use ($darwin) {
    foreach (['O_DIRECT', 'O_NOATIME', 'O_PATH', 'O_TMPFILE'] as $name) {
        expect(defined($name))->toBe(! $darwin);
    }
});

it('registers <poll.h>, <sys/stat.h> and <termios.h>', function () use ($darwin) {
    expect(POLLIN)->toBe(0x001)
        ->and(POLLOUT)->toBe(0x004)
        ->and(S_IFMT)->toBe(0170000)
        ->and(S_IFCHR)->toBe(0020000)
        ->and(TCSANOW)->toBe(0)
        ->and(B9600)->toBe($darwin ? 9600 : 0000015)
        ->and(VMIN)->toBe($darwin ? 16 : 6)
        ->and(NCCS)->toBe($darwin ? 20 : 32);
});

it('registers ECHO under its keyword name, reachable through constant()', function () {
    expect(constant('ECHO'))->toBe(0000010);
});

it('shares MSG_*, W* and errno names with the extensions that also register them', function () use ($darwin) {
    expect(MSG_PEEK)->toBe(0x2)
        ->and(WNOHANG)->toBe(1)
        ->and(ENOENT)->toBe(2)
        ->and(EAGAIN)->toBe($darwin ? 35 : 11)
        ->and(ENOTTY)->toBe(25);
});
