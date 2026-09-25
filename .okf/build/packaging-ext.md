---
type: Convention
title: Committed ext/ notes
description: Ship-ready ext/ tree, version header, and version-aligned stubs
resource: /ext/php_posi.h
tags: [posi, build, packaging]
status: draft
generated: { by: openai/gpt-5.6-sol, at: "2026-09-23T23:03:00Z" }
sources:
  - id: php-h
    resource: /ext/php_posi.h
    title: php_posi.h
  - id: config
    resource: /config.json
    title: Zephir config
  - id: composer
    resource: /composer.json
    title: PIE package manifest
  - id: system-api
    resource: /ext/src/system-api.c
    title: ext/src system-api mirror
---

# What ships in `ext/`

PIE/`phpize` builds from the pre-generated C tree under `ext/` (`build-path: ext`).[^composer] That tree is **ship-ready** — treat it as release artifact input, not a scratch build folder.

| Artifact | Role |
|----------|------|
| `ext/php_posi.h` | `PHP_POSI_VERSION` (`0.9.0`)[^php-h] |
| `ext/posi/*.zep.c` | Zephir-generated class C |
| `ext/src/*-api.{c,h}` | Mirrored C ABI sources |
| `ext/kernel/` | Zephir kernel |
| `ext/config.m4` | `--enable-posi` |

# Version alignment (0.9.0)

Keep these in sync for a release: `composer.json` `version`, `config.json` `version`, `PHP_POSI_VERSION` in `ext/php_posi.h`.[^config][^php-h]

# Stubs

On-disk IDE stubs live at `ide/0.9.0/`. Regenerate them only alongside a version bump; see [IDE stub path lag](/traps/ide-stub-path-lag.md).

# Do not commit phpize junk

After local `phpize`/`make`, avoid committing `Makefile`, `configure`, `autom4te.cache`, `modules/*.so`, etc.

# Regeneration policy

Regenerating Zephir output must happen on a **disposable copy** of the tree, never by casually overwriting the primary `ext/` in place. See [Regenerate ext](/playbooks/regenerate-ext.md).

[^php-h]: php_posi.h
[^config]: Zephir config
[^composer]: PIE package manifest
[^system-api]: ext/src system-api mirror
