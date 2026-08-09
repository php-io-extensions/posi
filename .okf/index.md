---
okf_version: "0.2"
---

# php-io-extensions/posi

Cross-platform (Linux + macOS) PHP extension: Zephir static classes under flat `Posi\` (`System`, `Memory`, `Termios`) that call a thin C ABI in `src/*-api.{c,h}`, which wraps libc POSIX (`open`, `read`, `ioctl`, `termios`, …). File descriptors and native buffer pointers are PHP `int`s. Windows is excluded. No FFI. Version **0.7.0** (0.7.x reconstitution / version-alignment pass; prior line was 0.5.0).

**Prefer** concepts with `status: stable` when present; content is currently `draft` pending Angel’s human verification of the OKF docs (implementation facts are grounded in shipped Zephir/`src/`/`ext/`/README).

# Orientation

* [Package overview](orientation/overview.md) - What posi is, version targets, and what it deliberately is not
* [Stack segmentation](orientation/stack-segmentation.md) - Boundaries vs microscrap/posix and GPIO/I2C/SPI drivers

# Architecture

* [Layered stack](architecture/stack.md) - Zephir → C ABI → libc POSIX
* [C ABI surface](architecture/c-abi.md) - `src/*-api.{c,h}` entry points
* [Zephir optimizers](architecture/zephir-optimizers.md) - `optimizers/` compile-time call rewrites

# Public PHP API

* [Posi\\System](api/system.md) - FD I/O, fcntl/ioctl, wait, lstat, hostname, …
* [Posi\\Memory](api/memory.md) - Native `emalloc` buffers for ioctl pointer structs
* [Posi\\Termios](api/termios.md) - Terminal attributes and baud helpers
* [Reserved scaffolds](api/reserved-scaffolds.md) - Commented `System` signatures — not implemented

# Build & packaging

* [Zephir + PIE install](build/zephir-and-pie.md) - installers, PIE, phpize from `ext/`
* [Committed ext/ notes](build/packaging-ext.md) - Ship-ready tree, version headers, stubs lag

# Conventions

* [Sibling patterns](conventions/sibling-patterns.md) - Shared php-io-extensions packaging style
* [Handle ownership](conventions/handle-ownership.md) - FD close + Memory free
* [No FFI](conventions/no-ffi.md) - Extension-only binding
* [Constants outside the extension](conventions/constants-outside-ext.md) - No PHP class constants; app/microscrap enums

# Traps

* [Windows excluded](traps/windows-excluded.md) - PIE `os-families-exclude: windows`
* [Do not rebuild ext/ in place](traps/do-not-rebuild-in-place.md) - Copy → build → sync → delete copy
* [IDE stub path lag](traps/ide-stub-path-lag.md) - `ide/0.4.8` / `ide/0.4.11` lag behind 0.7.0
* [Constants are platform-specific](traps/constants-platform-specific.md) - `O_*` / `F_*` / ioctl values differ by OS
* [FD ownership / close](traps/fd-ownership-close.md) - PHP GC does not close descriptors

# Playbooks

* [Minimal open/read/close](playbooks/demo-open-read-close.md) - README-style FD demo
* [Regenerate committed ext/](playbooks/regenerate-ext.md) - Maintainer steps on a **copy** of the tree

# Indexes

* [Orientation](orientation/) — start here
* [Architecture](architecture/)
* [API](api/)
* [Build](build/)
* [Conventions](conventions/)
* [Traps](traps/)
* [Playbooks](playbooks/)
