<?php

declare(strict_types=1);

it('reads and sets the file status flags with fcntl', function () {
    $fd = track(posix_open('/dev/null', O_RDONLY));

    expect(fcntl($fd, F_GETFL, null, $flags))->toBe($flags)
        ->and($flags & O_NONBLOCK)->toBe(0)
        ->and(fcntl($fd, F_SETFL, $flags | O_NONBLOCK, $set))->toBe(0)
        ->and($set)->toBe($flags | O_NONBLOCK);

    fcntl($fd, F_GETFL, null, $after);

    expect($after & O_NONBLOCK)->toBe(O_NONBLOCK);
});

it('sets and reads close-on-exec with fcntl', function () {
    $fd = track(posix_open('/dev/null', O_RDONLY));

    fcntl($fd, F_SETFD, FD_CLOEXEC, $unused);
    fcntl($fd, F_GETFD, null, $flags);

    expect($flags & FD_CLOEXEC)->toBe(FD_CLOEXEC);
});

it('asks for a record lock as a struct flock array', function () {
    $fd = track(posix_open(scratchPath('locked'), O_RDWR | O_CREAT, 0600));

    expect(fcntl($fd, F_GETLK, ['type' => F_WRLCK, 'whence' => SEEK_SET], $lock))->toBe(0)
        ->and($lock)->toMatchArray(['type' => F_UNLCK]);
});

it('refuses fcntl arguments the command cannot take', function () {
    expect(fn () => fcntl(0, F_GETLK, 1, $v))->toThrow(TypeError::class)
        ->and(fn () => fcntl(0, F_SETFL, 'x', $v))->toThrow(TypeError::class);
});

it('passes ioctl an int by pointer and reads it back', function () {
    [$read, $write] = fifoPair();
    posix_write($write, 'abc', 3);

    expect(ioctl($read, fionread(), ['value' => 0], $waiting))->toBe(0)
        ->and($waiting)->toBe(3);
});

it('passes ioctl a byte buffer and reads the bytes back', function () {
    [$read, $write] = fifoPair();
    posix_write($write, 'abcd', 4);

    ioctl($read, fionread(), pack('l', 0), $raw);
    ioctl($read, fionread(), ['bytes' => pack('l', 0)], $rawFromArray);

    expect(unpack('l', $raw)[1])->toBe(4)
        ->and(unpack('l', $rawFromArray)[1])->toBe(4);
});

it('passes ioctl a NULL argument', function () {
    $fd = track(posix_open('/dev/null', O_RDONLY));

    expect(ioctl($fd, fioclex(), null, $value))->toBe(0)
        ->and($value)->toBeNull();

    fcntl($fd, F_GETFD, null, $flags);

    expect($flags & FD_CLOEXEC)->toBe(FD_CLOEXEC);
});

it('passes ioctl a posi_mem_alloc() address as the argument itself', function () {
    [$read, $write] = fifoPair();
    posix_write($write, 'hello', 5);
    $ptr = posi_mem_alloc(8);

    expect(ioctl($read, fionread(), $ptr, $value))->toBe(0)
        ->and($value)->toBe($ptr)
        ->and(unpack('l', posi_mem_read($ptr, 4))[1])->toBe(5);

    posi_mem_free($ptr);
});

it('reports a failed ioctl as -1, $value false and errno', function () {
    $fd = track(posix_open(scratchPath('plain'), O_RDWR | O_CREAT, 0600));

    expect(ioctl($fd, 0x40045431, ['value' => 0], $value))->toBe(-1)
        ->and($value)->toBeFalse()
        ->and(posi_errno())->toBe(ENOTTY);
});

it('refuses ioctl arrays that say neither value nor bytes', function () {
    expect(fn () => ioctl(0, fionread(), ['size' => 4], $v))->toThrow(ValueError::class)
        ->and(fn () => ioctl(0, fionread(), 1.5, $v))->toThrow(TypeError::class);
});
