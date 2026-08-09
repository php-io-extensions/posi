---
type: Orientation
title: Package overview
description: What posi is, version targets, and what it deliberately is not
resource: /composer.json
tags: [posi, orientation, php-ext, posix]
status: draft
generated: { by: okf-documentation-generator/cursor-grok-4.5, at: "2026-08-09T17:48:43Z" }
sources:
  - id: composer
    resource: /composer.json
    title: PIE package manifest
  - id: config
    resource: /config.json
    title: Zephir config
  - id: readme
    resource: /README.md
    title: README
  - id: php-h
    resource: /ext/php_posi.h
    title: php_posi.h
  - id: system-zep
    resource: /posi/system.zep
    title: system.zep
---

# Summary

`php-io-extensions/posi` is a **Linux + macOS** PHP extension (`type: php-ext`) that exposes POSIX syscalls to PHP through flat static classes under `Posi\`. Zephir methods call a thin C ABI (`src/*-api.{c,h}`), which wraps libc. Opaque file descriptors and native buffer addresses are PHP `int`s. There is **no FFI**.[^composer][^readme][^system-zep]

| Fact | Value |
|------|--------|
| Package | `php-io-extensions/posi` |
| Extension name | `posi` |
| Version | `0.7.0` |
| Prior release line | `0.5.0` (this cut is 0.7.x reconstitution / version alignment) |
| PHP (composer) | `>= 8.0` |
| PHP (README) | `8.3+` (document both; README is the stricter consumer guidance) |
| OS | **Unix** — Linux + macOS; Windows excluded (`os-families-exclude`) |
| Namespace | `Posi\` — classes `System`, `Memory`, `Termios` (flat, not nested) |
| Author | Project Saturn Studios, LLC |
| License | MIT |

Version strings are aligned at **0.7.0** in `composer.json`, `config.json`, and `PHP_POSI_VERSION` in `ext/php_posi.h`.[^composer][^config][^php-h]

# End capability

1. Open / read / write / close raw file descriptors (`System::open`, `read`, `write`, `close`).
2. Drive `fcntl` / `ioctl` with platform command integers and flexible arg shapes.
3. Terminal control via `Termios` (`tcgetattr` / `tcsetattr` / baud helpers including Linux `BOTHER`).
4. Allocate native buffers for ioctl structs that embed C pointers (`Memory::alloc` / `write` / `read` / `free`).
5. Process helpers: `wait` / `waitpid`, `getuid` / `setuid`, `umask`, `hostname`, `lstat`, `ppoll`.

# What it is not

- Not a Windows extension.
- Not an FFI or pure-PHP polyfill.
- Not a full POSIX surface — many syscalls remain **commented scaffolds** in `system.zep` (see [Reserved scaffolds](/api/reserved-scaffolds.md)).
- Not the ergonomic helper layer — that is optional downstream `microscrap/posix`.

# Public namespace

| Class | Zephir | Role |
|-------|--------|------|
| `Posi\System` | `posi/system.zep` | FD I/O and syscalls |
| `Posi\Memory` | `posi/memory.zep` | Native buffer primitives |
| `Posi\Termios` | `posi/termios.zep` | Terminal attributes |

IDE stubs currently live under `ide/0.4.8/` and `ide/0.4.11/` — they **lag** 0.7.0 (see [IDE stub path lag](/traps/ide-stub-path-lag.md)); do not regenerate stubs as part of routine agent work.

See [Stack segmentation](/orientation/stack-segmentation.md) and [Layered stack](/architecture/stack.md).

[^composer]: PIE package manifest
[^config]: Zephir config
[^readme]: README
[^php-h]: php_posi.h
[^system-zep]: system.zep
