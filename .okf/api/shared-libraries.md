---
type: API
title: Shared libraries
description: dlopen/dlsym/dlclose as ints, the open-handle registry, RTLD_* constants.
resource: posi.c
tags: [posi, dlfcn, loader]
status: draft
generated: { by: claude-sonnet/5.5, at: 2026-11-08T12:00:00Z }
sources:
  - id: posi-c
    resource: posi.c
    title: posi.c
  - id: tests
    resource: tests/SharedLibraryTest.php
    title: SharedLibraryTest
---

# Functions

| Function | Returns |
|---|---|
| `dlopen(string $file, int $flags = RTLD_LAZY)` | handle as int, `false` when the loader cannot open the file |
| `dlsym(int $handle, string $symbol)` | symbol address as int, `0` when absent |
| `dlclose(int $handle)` | `0` on success |

Names follow `dlfcn.h`, not the `posix_` or `posi_` prefix, because the C names are the POSIX names. Constants `RTLD_LAZY`, `RTLD_NOW`, `RTLD_LOCAL`, `RTLD_GLOBAL` come from the header, so their values differ per OS.[^posi-c]

# Registry

`POSI_G(dl_handles)` maps handle address to the number of opens in this request. `dlopen()` adds or increments, `dlclose()` decrements and removes at zero. `dlsym()` and `dlclose()` throw `ValueError` for a handle not in the table. RSHUTDOWN closes each leftover handle once per open. The table is per request and per thread; the loader's own table is process-wide.[^posi-c]

# Use

The address answers "is this the same function": a caller compares `dlsym()` results from two libraries, or against an address another extension returns (for example ext-vulkan `vkGetInstanceProcAddr`), with no FFI.

# Tests

`tests/SharedLibraryTest.php` opens the C library (`/usr/lib/libSystem.B.dylib` on macOS, `libc.so.6` elsewhere).[^tests]

[^posi-c]: posi.c, "Shared libraries" block.
[^tests]: tests/SharedLibraryTest.php.
