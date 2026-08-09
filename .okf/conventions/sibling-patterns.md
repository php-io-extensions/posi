---
type: Convention
title: Sibling patterns
description: Shared php-io-extensions packaging style (and what posi pioneered)
tags: [posi, convention, packaging]
status: draft
generated: { by: okf-documentation-generator/cursor-grok-4.5, at: "2026-08-09T17:48:43Z" }
sources:
  - id: composer
    resource: /composer.json
    title: posi composer.json
  - id: config
    resource: /config.json
    title: posi config.json
---

# Patterns this package owns / shares

| Pattern | How posi uses it | Borrowed by |
|---------|------------------|-------------|
| Thin Zephir + C ABI in `src/` | `posi/*.zep` → `src/*-api.{c,h}` → libc | metal (ObjC behind C headers) |
| PIE `type: php-ext`, `build-path: ext` | Same layout | sdl3 / glfw / metal |
| Static namespaced classes | Flat `Posi\{System,Memory,Termios}` | Nested `Sdl3\SDL\…`, `Metal\MTL\…` |
| Opaque `int` handles | FDs + Memory pointers | SDL / Metal opaque objects |
| Constants outside extension | App / microscrap enums | sdl3 |
| Windows excluded | `os-families-exclude: ["windows"]` | sdl3 |
| Zephir `optimizer-dirs` | `optimizers/` | (posi-specific density) |
| Version line **0.7.0** | Alignment pass from 0.5.0 | ecosystem 0.7.x |

# Not shared

- No third-party pkg-config library (unlike SDL3) — only libc / POSIX headers.
- No nested PHP namespace folders under `Posi\` — three flat classes only.
- No ObjC / frameworks.

Keep peer packages as composition boundaries only — no Composer `require` on sdl3/metal/microscrap from this extension.

[^composer]: posi composer.json
[^config]: posi config.json
