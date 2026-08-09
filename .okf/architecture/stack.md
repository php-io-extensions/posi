---
type: Architecture
title: Layered stack
description: Zephir → C ABI → libc POSIX
resource: /config.json
tags: [posi, architecture, zephir, abi]
status: draft
generated: { by: okf-documentation-generator/cursor-grok-4.5, at: "2026-08-09T17:48:43Z" }
sources:
  - id: config
    resource: /config.json
    title: Zephir config
  - id: system-zep
    resource: /posi/system.zep
    title: system.zep
  - id: system-h
    resource: /src/system-api.h
    title: system-api.h
  - id: open-opt
    resource: /optimizers/OpenFileOptimizer.php
    title: OpenFileOptimizer.php
---

# Layers

```text
PHP (Posi\{System,Memory,Termios})
        │  Zephir static methods
        │  (optimizers rewrite helper calls → C)
        ▼
C ABI (src/*-api.{c,h})          posix_* / posi_mem_* entry points
        │
        ▼
libc POSIX                       open, read, ioctl, termios, …
```

This is the same **thin Zephir → C ABI → native** pattern metal later borrowed for ObjC bridges (posi uses C/libc instead of `.m`).[^system-zep][^system-h]

# Source map

| Layer | Path | Role |
|-------|------|------|
| Zephir | `posi/{system,memory,termios}.zep` | Public PHP API |
| Headers / C | `src/{system,termios,memory}-api.{c,h}` | Stable ABI for Zephir (`extra-sources`)[^config] |
| Optimizers | `optimizers/*.php` | Compile-time rewrites of `open_file`, `mem_alloc`, …[^open-opt] |
| Packaging | `composer.json` | `build-path: ext`, Windows excluded |
| Ship-ready C | `ext/` | Pre-generated tree for PIE/`phpize` |

# Design intent

- Keep Zephir thin: one static method ≈ one ABI call.
- Keep libc details behind C headers so Zephir never includes POSIX headers directly.
- Expose FDs and native pointers as PHP `int` — app owns lifecycle (`close` / `Memory::free`).

See [C ABI surface](/architecture/c-abi.md) and [Zephir optimizers](/architecture/zephir-optimizers.md).

[^config]: Zephir config
[^system-zep]: system.zep
[^system-h]: system-api.h
[^open-opt]: OpenFileOptimizer.php
