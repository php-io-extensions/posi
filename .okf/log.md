# Log

## 2026-11-08

* **Version**: 0.11.1 in `composer.json` and `PHP_POSI_VERSION` (`php_posi.h`); nothing else changed.

* **Aligned memory**: `posix_memalign(?int &$memptr, int $alignment, int $size): int`, `sysconf(int $name): int`, `_SC_PAGESIZE` and `_SC_PAGE_SIZE` (`stdlib.h`, `unistd.h`), so a caller can have memory at a page boundary (a Metal buffer that wraps it without a copy). `posix_memalign` memory is in the same registry as `posi_mem_alloc` ([native buffers](/api/native-buffers.md)): the `posi_mem_*` functions read, write and free it; a second table marks it as the C library's, so it is freed with `free`. Tests: MemoryTest 5 new; the suite from its own repo setup 82 passed, 1 skipped (ZTS); NTS builds and a smoke run passes. No version bump.

* **Shared libraries**: `dlopen`, `dlsym`, `dlclose` and `RTLD_LAZY/NOW/LOCAL/GLOBAL` added, `dlfcn.h` 1:1, with a handle registry ([shared libraries](/api/shared-libraries.md)). `config.m4` checks `dlfcn.h` and links libdl where `dlopen` lives there. SharedLibraryTest 6 passed on Homebrew PHP 8.4 ZTS; NTS builds and loads. No version bump.

## 2026-10-28

* **Version**: 0.11.0 in `composer.json` and `PHP_POSI_VERSION` (`php_posi.h`); `branch-alias` `dev-main` is `0.11.x-dev`. Promoted with the Venusian 0.11 ecosystem. No API or behaviour change; built and loaded on Homebrew PHP 8.4 NTS and ZTS.

## 2026-10-20

* **listen**: `posix_listen` added, `listen(2)` 1:1, with `SOMAXCONN` registered if absent ([descriptors](/api/descriptors.md), [constants](/api/constants.md)). SocketTest 16 passed + 1 skipped on php84, zhp and the Pi. Ships in 0.10.6, still untagged.

## 2026-10-14

* **Socket descriptors**: `posix_fileno`, `posix_dup`, `posix_socket_fdopen` added for sharing one listening socket across ext-parallel threads ([descriptors](/api/descriptors.md)). Suite 67 passed on zhp and the Pi (66 + 1 skipped on php84, which has no ext-parallel), TLS handshake and four-thread accept included. `PHP_POSI_VERSION` and the composer.json `version` move to 0.10.6.

## 2026-10-11

* **README rewritten** for 0.10.5 to the php-io-extensions template: centered header with badges, problem/solution intro, two-part Basic usage sketch (ioctl on i2c-dev, with a serial-port example), per-function API in the 0.4.0 style with every hardware-free example run on php84, zhp and the Pi, requirements table, both installers, test-suite run proven from a fresh clone on php84, zhp and the Pi (55 passed, 173 assertions). No code change; `PHP_POSI_VERSION` and the composer.json `version` move to 0.10.5. Concepts untouched.

## 2026-10-04

* **Creation**: Bundle created for ext-posi 0.10.0, the plain-C rewrite: [module](/architecture/module.md), [descriptors](/api/descriptors.md), [processes](/api/processes.md), [termios](/api/termios.md), [native buffers](/api/native-buffers.md), [constants](/api/constants.md), [build](/runbooks/build.md), [upgrade from 0.9](/reference/upgrade-from-0-9.md).
