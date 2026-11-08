<?php

declare(strict_types=1);

it('allocates zeroed buffers and reads back what was written, at offsets', function () {
    $ptr = posi_mem_alloc(8);

    expect(posi_mem_read($ptr, 8))->toBe(str_repeat("\0", 8));

    posi_mem_write($ptr, 'abcd');
    posi_mem_write($ptr, 'XY', 6);

    expect(posi_mem_read($ptr, 8))->toBe("abcd\0\0XY")
        ->and(posi_mem_read($ptr, 2, 2))->toBe('cd');

    posi_mem_free($ptr);
});

it('refuses ranges past the end of the buffer', function () {
    $ptr = posi_mem_alloc(4);

    expect(fn () => posi_mem_write($ptr, 'abcde'))->toThrow(ValueError::class)
        ->and(fn () => posi_mem_write($ptr, 'ab', 3))->toThrow(ValueError::class)
        ->and(fn () => posi_mem_read($ptr, 5))->toThrow(ValueError::class)
        ->and(fn () => posi_mem_read($ptr, 1, -1))->toThrow(ValueError::class)
        ->and(fn () => posi_mem_read($ptr, -1))->toThrow(ValueError::class);

    posi_mem_free($ptr);
});

it('refuses addresses it did not hand out, and a second free', function () {
    $ptr = posi_mem_alloc(4);
    posi_mem_free($ptr);

    expect(fn () => posi_mem_free($ptr))->toThrow(ValueError::class)
        ->and(fn () => posi_mem_read($ptr, 1))->toThrow(ValueError::class)
        ->and(fn () => posi_mem_write(12345, 'x'))->toThrow(ValueError::class);
});

it('treats freeing address 0 as free(NULL)', function () {
    posi_mem_free(0);

    expect(true)->toBeTrue();
});

it('refuses an empty or negative allocation', function () {
    expect(fn () => posi_mem_alloc(0))->toThrow(ValueError::class)
        ->and(fn () => posi_mem_alloc(-1))->toThrow(ValueError::class);
});

it('knows the size of a page, a power of two', function () {
    $page = sysconf(_SC_PAGESIZE);

    expect($page)->toBeGreaterThanOrEqual(4096)->and($page & ($page - 1))->toBe(0)
        ->and(sysconf(_SC_PAGE_SIZE))->toBe($page);
});

it('allocates memory at a page boundary that the posi_mem functions read, write and free', function () {
    $page = sysconf(_SC_PAGESIZE);

    expect(posix_memalign($address, $page, $page * 2))->toBe(0)
        ->and($address)->toBeGreaterThan(0)->and($address % $page)->toBe(0);

    posi_mem_write($address, 'abcd', $page * 2 - 4);

    expect(posi_mem_read($address, 4, $page * 2 - 4))->toBe('abcd')
        ->and(fn () => posi_mem_read($address, 4, $page * 2 - 3))->toThrow(ValueError::class);

    posi_mem_free($address);

    expect(fn () => posi_mem_read($address, 1))->toThrow(ValueError::class)
        ->and(fn () => posi_mem_free($address))->toThrow(ValueError::class);
});

it('answers EINVAL for an alignment that is not a power of two times a pointer, and gives no address', function () {
    expect(posix_memalign($address, 3, 64))->toBe(EINVAL)
        ->and($address)->toBe(0)->and(posi_errno())->toBe(EINVAL);
});

it('refuses a zero or negative alignment or size for posix_memalign', function () {
    expect(fn () => posix_memalign($a, 0, 64))->toThrow(ValueError::class)
        ->and(fn () => posix_memalign($a, 16, 0))->toThrow(ValueError::class)
        ->and(fn () => posix_memalign($a, -16, 64))->toThrow(ValueError::class);
});

it('hands back what sysconf reports for an unknown name as -1', function () {
    expect(sysconf(-12345))->toBe(-1)->and(posi_errno())->toBe(EINVAL);
});
