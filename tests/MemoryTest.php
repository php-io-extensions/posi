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
