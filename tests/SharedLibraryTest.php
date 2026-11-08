<?php

declare(strict_types=1);

/** A library every process has: the C library. */
function cLibrary(): string
{
    return PHP_OS_FAMILY === 'Darwin' ? '/usr/lib/libSystem.B.dylib' : 'libc.so.6';
}

it('opens a library, finds a symbol by name, and closes it', function () {
    $handle = dlopen(cLibrary(), RTLD_LAZY | RTLD_LOCAL);

    expect($handle)->toBeInt()->toBeGreaterThan(0);
    $getpid = dlsym($handle, 'getpid');
    expect($getpid)->toBeGreaterThan(0)
        ->and(dlsym($handle, 'getpid'))->toBe($getpid)
        ->and(dlclose($handle))->toBe(0);
});

it('answers 0 for a symbol the library does not have', function () {
    $handle = dlopen(cLibrary());

    expect(dlsym($handle, 'posi_no_such_symbol'))->toBe(0);

    dlclose($handle);
});

it('answers false for a library that does not exist', function () {
    expect(dlopen('/no/such/library.so'))->toBeFalse();
});

it('finds the same address through two opens, and needs a close for each', function () {
    $first = dlopen(cLibrary(), RTLD_NOW);
    $second = dlopen(cLibrary(), RTLD_NOW);

    expect($second)->toBe($first)
        ->and(dlsym($first, 'getpid'))->toBe(dlsym($second, 'getpid'))
        ->and(dlclose($first))->toBe(0)
        ->and(dlsym($first, 'getpid'))->toBeGreaterThan(0)
        ->and(dlclose($second))->toBe(0)
        ->and(fn () => dlsym($first, 'getpid'))->toThrow(ValueError::class);
});

it('refuses a handle dlopen() did not return, or one already closed', function () {
    $handle = dlopen(cLibrary());
    dlclose($handle);

    expect(fn () => dlsym($handle, 'getpid'))->toThrow(ValueError::class, 'dlopen() returned')
        ->and(fn () => dlclose($handle))->toThrow(ValueError::class)
        ->and(fn () => dlsym(12345, 'getpid'))->toThrow(ValueError::class)
        ->and(fn () => dlclose(0))->toThrow(ValueError::class);
});

it('takes its flags from the constants, which are distinct', function () {
    expect(RTLD_LAZY)->not->toBe(RTLD_NOW)
        ->and(RTLD_LOCAL | RTLD_GLOBAL)->toBeInt()
        ->and(fn () => dlopen(cLibrary(), PHP_INT_MAX))->toThrow(ValueError::class);
});
