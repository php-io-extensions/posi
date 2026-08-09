---
type: Trap
title: Do not rebuild ext/ in place
description: Committed ext/ is ship-ready — regenerate only on a disposable copy
resource: /ext
tags: [posi, trap, packaging, zephir]
status: draft
generated: { by: okf-documentation-generator/cursor-grok-4.5, at: "2026-08-09T17:48:43Z" }
sources:
  - id: composer
    resource: /composer.json
    title: composer.json build-path
  - id: install
    resource: /install-macos.sh
    title: install-macos.sh
  - id: php-h
    resource: /ext/php_posi.h
    title: php_posi.h
---

# Trap

The committed `ext/` tree is the **ship-ready** PIE/`phpize` input (`build-path: ext`).[^composer] Running Zephir (`fullclean` / `build`), platform installers that regenerate C, or casual `phpize` workflows **in the primary checkout** can overwrite or dirty that tree with unintended diffs.

# Do

1. Copy the package tree to a disposable directory.
2. Run Zephir / installer / smoke builds **in the copy**.
3. Sync back only intentional artifacts (generated sources you mean to ship, version bumps already decided).
4. Delete the copy.

# Do not

- Run `zephir`, `phpize`, `make`, or `pie install` in the primary folder “just to refresh” `ext/`.
- Treat installer side effects on `ext/` as free rewrites of release C.
- Leave a half-regenerated `ext/` mixed with hand-edited version strings.

Version string bumps (e.g. `PHP_POSI_VERSION` in `ext/php_posi.h`) are an explicit maintainer edit — not a license to regenerate the whole tree in place.[^php-h]

Full steps: [Regenerate committed ext/](/playbooks/regenerate-ext.md).

[^composer]: composer.json build-path
[^install]: install-macos.sh
[^php-h]: php_posi.h
