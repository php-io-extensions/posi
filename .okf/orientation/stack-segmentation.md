---
type: Orientation
title: Stack segmentation
description: Boundaries vs microscrap/posix and GPIO/I2C/SPI drivers
tags: [posi, orientation, boundaries]
status: draft
generated: { by: okf-documentation-generator/cursor-grok-4.5, at: "2026-08-09T17:48:43Z" }
sources:
  - id: composer
    resource: /composer.json
    title: PIE package manifest
  - id: readme
    resource: /README.md
    title: Package README
  - id: memory-zep
    resource: /posi/memory.zep
    title: memory.zep
---

# Who owns what

| Concern | Package | Notes |
|---------|---------|--------|
| Native POSIX C API in PHP | **`php-io-extensions/posi`** | This package — Zephir extension |
| Global `posix_*` helpers wrapping `Posi\System` | `microscrap/posix` | Downstream; not a dependency of this extension[^readme] |
| GPIO / I2C / SPI driver packages | sibling drivers / circuits | Compose via `System::ioctl` + `Memory` pointer buffers[^memory-zep] |
| Flag / ioctl / baud constants | app locals or microscrap enums | Not compiled into this extension |
| Display / window / Metal / SDL | `php-io-extensions/{metal,sdl3,…}` | Peers — not deps of posi |

# Composition sketch

```text
PHP app / tubes / drivers
  ├─ microscrap/posix          → optional global helpers (posix_open, …)
  ├─ GPIO / I2C / SPI drivers  → pack structs, ioctl, Memory buffers
  └─ php-io-extensions/posi    → Posi\{System,Memory,Termios}
         └─ libc POSIX (Unix)
```

# Hard rules

1. Do **not** document `microscrap/posix` APIs inside this OKF — only the composition boundary.
2. Do **not** add microscrap or display extensions as runtime Composer deps of this package.
3. Do **not** nest a second `.okf` under `posi/` or `src/`.
4. Keep Windows out of PIE (`os-families-exclude: ["windows"]`).[^composer]
5. Commented method signatures in `system.zep` are scaffolds — not public API until implemented.

[^composer]: PIE package manifest
[^readme]: Package README
[^memory-zep]: memory.zep
