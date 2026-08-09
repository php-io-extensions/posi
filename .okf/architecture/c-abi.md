---
type: Architecture
title: C ABI surface
description: src/*-api entry points behind Posi static methods
resource: /src/system-api.h
tags: [posi, abi, c]
status: draft
generated: { by: okf-documentation-generator/cursor-grok-4.5, at: "2026-08-09T17:48:43Z" }
sources:
  - id: system-h
    resource: /src/system-api.h
    title: system-api.h
  - id: memory-h
    resource: /src/memory-api.h
    title: memory-api.h
  - id: termios-h
    resource: /src/termios-api.h
    title: termios-api.h
  - id: config
    resource: /config.json
    title: Zephir config extra-sources
---

# Layout

`config.json` lists `extra-sources`: `src/system-api.c`, `src/termios-api.c`, `src/memory-api.c`.[^config] Headers declare the C functions Zephir optimizers emit.

# System (`system-api.h`)[^system-h]

| C function | Zephir helper / PHP |
|------------|---------------------|
| `posix_open` | `open_file` → `System::open` |
| `posix_close` | `close_file` → `System::close` |
| `posix_read` / `posix_write` | `read_file` / `write_file` |
| `posix_fcntl` / `posix_ioctl` | `fcntl_file` / `ioctl_file` |
| `posix_chmod` / `posix_chown` | path variants |
| `posix_fchmod` / `posix_fchown` | FD variants |
| `posix_getuid` / `posix_setuid` / `posix_umask` | process |
| `posix_lseek` / `posix_ppoll` | FD helpers |
| `posix_recv` / `posix_readv` | socket / scatter read |
| `posix_wait` / `posix_waitpid` | process wait |
| `posix_hostname` / `posix_lstat` | hostname / lstat |

# Memory (`memory-api.h`)[^memory-h]

| C function | PHP |
|------------|-----|
| `posi_mem_alloc` | `Memory::alloc` |
| `posi_mem_free` | `Memory::free` |
| `posi_mem_write` | `Memory::write` |
| `posi_mem_read` | `Memory::read` |

# Termios (`termios-api.h`)[^termios-h]

| C function | PHP |
|------------|-----|
| `posix_tcgetattr` / `posix_tcsetattr` | `Termios::tcgetattr` / `tcsetattr` |
| `posix_cfsetispeed` / `posix_cfsetospeed` | baud setters on termios arrays |
| `posix_cfgetispeed` / `posix_cfgetospeed` | baud getters |
| `posix_tcdrain` / `posix_tcflush` / `posix_tcflow` | queue control |
| `posix_set_baud_rate` | `Termios::setBaudRate` (incl. Linux `BOTHER`) |

PHP mapping: [API index](/api/index.md). Ownership: [Handle ownership](/conventions/handle-ownership.md).

[^system-h]: system-api.h
[^memory-h]: memory-api.h
[^termios-h]: termios-api.h
[^config]: Zephir config extra-sources
