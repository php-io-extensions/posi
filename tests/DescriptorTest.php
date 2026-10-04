<?php

declare(strict_types=1);

it('opens, writes, seeks, reads and closes a file', function () {
    $fd = posix_open(scratchPath('file'), O_RDWR | O_CREAT | O_TRUNC, 0600);

    expect($fd)->toBeGreaterThanOrEqual(0)
        ->and(posix_write($fd, 'hello world', 11))->toBe(11)
        ->and(posix_lseek($fd, 6, SEEK_SET))->toBe(6)
        ->and(posix_read($fd, 64))->toBe('world')
        ->and(posix_read($fd, 64))->toBe('')
        ->and(posix_close($fd))->toBe(0);
});

it('writes only the first $bytes_to_write bytes', function () {
    $fd = track(posix_open(scratchPath('file'), O_RDWR | O_CREAT, 0600));

    posix_write($fd, 'abcdef', 3);
    posix_lseek($fd, 0, SEEK_SET);

    expect(posix_read($fd, 64))->toBe('abc');
});

it('refuses to write past the end of $data', function () {
    $fd = track(posix_open(scratchPath('file'), O_RDWR | O_CREAT, 0600));

    expect(fn () => posix_write($fd, 'abc', 4))->toThrow(ValueError::class)
        ->and(fn () => posix_write($fd, 'abc', -1))->toThrow(ValueError::class);
});

it('refuses a negative read length', function () {
    expect(fn () => posix_read(0, -1))->toThrow(ValueError::class);
});

it('refuses an fd outside the int range instead of truncating it', function () {
    expect(fn () => posix_close(PHP_INT_MAX))->toThrow(ValueError::class);
});

it('creates files with the mode given, under the umask', function () {
    $previous = posix_umask(0022);
    $path = scratchPath('mode');

    try {
        track(posix_open($path, O_WRONLY | O_CREAT, 0666));

        expect(posix_lstat($path)['mode'] & 0777)->toBe(0644);
    } finally {
        posix_umask($previous);
    }
});

it('reports failures as -1 or false with errno kept for posi_errno()', function () {
    expect(posix_open(scratchPath('missing/nowhere'), O_RDONLY))->toBe(-1)
        ->and(posi_errno())->toBe(ENOENT)
        ->and(posix_read(-1, 1))->toBeFalse()
        ->and(posi_errno())->toBe(EBADF);
});

it('reads into several buffers at once with readv', function () {
    $fd = track(posix_open(scratchPath('file'), O_RDWR | O_CREAT, 0600));
    posix_write($fd, 'hello world', 11);
    posix_lseek($fd, 0, SEEK_SET);

    expect(posix_readv($fd, [['len' => 5], ['len' => 1], ['len' => 10]]))
        ->toBe(['res' => 11, 'buffers' => ['hello', ' ', 'world']])
        ->and(posix_readv($fd, []))->toBe(['res' => 0, 'buffers' => []]);
});

it('refuses readv buffers without a non-negative int len', function () {
    expect(fn () => posix_readv(0, [['len' => -1]]))->toThrow(ValueError::class)
        ->and(fn () => posix_readv(0, [['size' => 4]]))->toThrow(ValueError::class);
});

it('receives from a socket, peeking first', function () {
    [$a, $b] = stream_socket_pair(STREAM_PF_UNIX, STREAM_SOCK_STREAM, STREAM_IPPROTO_IP);
    fwrite($a, 'ping');

    $fd = fdOf($b);

    expect(posix_recv($fd, 16, MSG_PEEK))->toBe('ping')
        ->and(posix_recv($fd, 16))->toBe('ping');

    fclose($a);
    fclose($b);
});

it('polls a descriptor for readiness, with a timeout', function () {
    [$read, $write] = fifoPair();

    $started = hrtime(true);
    expect(posix_ppoll($read, 50_000_000))->toBe(0)
        ->and(hrtime(true) - $started)->toBeGreaterThanOrEqual(45_000_000)
        ->and(posix_ppoll($write, 0, POLLOUT))->toBe(1);

    posix_write($write, 'x', 1);

    expect(posix_ppoll($read, 0))->toBe(1)
        ->and(posix_ppoll($read, 0, POLLIN))->toBe(1);
});

it('wraps an fd as a stream without copying it, and fclose() closes that fd', function () {
    $fd = posix_open('/dev/null', O_RDONLY);

    $stream = posix_fdopen($fd, 'r');

    expect($stream)->toBeResource()
        ->and(get_resource_type($stream))->toBe('stream');

    fclose($stream);

    expect(fcntl($fd, F_GETFD, null, $flags))->toBe(-1)
        ->and($flags)->toBeFalse();
});

it('keeps the close-on-exec flag the fd was opened with', function () {
    $fd = posix_open('/dev/null', O_RDONLY | O_CLOEXEC);

    $stream = posix_fdopen($fd, 'r');
    fcntl($fd, F_GETFD, null, $flags);

    expect($flags & FD_CLOEXEC)->toBe(FD_CLOEXEC);

    fclose($stream);
});

it('hands stream_select an fdopen() stream', function () {
    $stream = posix_fdopen(posix_open('/dev/null', O_RDONLY), 'r');
    $read = [$stream];
    $write = $except = null;

    expect(stream_select($read, $write, $except, 0, 0))->toBe(1);

    fclose($stream);
});

it('refuses to fdopen an fd that is not open', function () {
    $fd = posix_open('/dev/null', O_RDONLY);
    posix_close($fd);

    expect(posix_fdopen($fd, 'r'))->toBeFalse()
        ->and(posi_errno())->toBe(EBADF);
});
