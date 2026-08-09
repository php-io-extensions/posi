---
type: Playbook
title: Regenerate committed ext/
description: Maintainer Zephir rebuild on a disposable copy — never in-place on ship-ready ext/
resource: /install-macos.sh
tags: [posi, playbook, packaging, zephir]
status: draft
generated: { by: okf-documentation-generator/cursor-grok-4.5, at: "2026-08-09T17:48:43Z" }
sources:
  - id: install
    resource: /install-macos.sh
    title: install-macos.sh
  - id: debian
    resource: /install-debian-trixie.sh
    title: install-debian-trixie.sh
  - id: config
    resource: /config.json
    title: config.json
  - id: php-h
    resource: /ext/php_posi.h
    title: php_posi.h
  - id: composer
    resource: /composer.json
    title: composer.json
---

# When

Before tagging a Packagist/PIE release, or after changing `.zep` / `src/*-api.{c,h}` / `optimizers/` / `config.json`.

# Critical rule

**Never regenerate ship-ready `ext/` in the primary checkout.** Always work on a **copy** of the tree, install/build there, then sync intentional outputs back and delete the copy.[^install]

# Steps

1. Confirm version targets are **0.7.0** (or the release you intend) in `composer.json`, `config.json`, and plan the matching `PHP_POSI_VERSION` string.[^composer][^config][^php-h]

2. Copy the package:

```bash
SRC=/path/to/php-io-extensions/posi
COPY=/tmp/posi-rebuild-$(date +%Y%m%d)
cp -R "$SRC" "$COPY"
cd "$COPY"
```

3. On Linux or macOS with Zephir + matching PHP, run the platform installer **in the copy** (or equivalent Zephir generate + build):

```bash
bash install-macos.sh
# or
bash install-debian-trixie.sh
```

4. Smoke in the copy:

```bash
php -n -d extension=./ext/modules/posi.so --ri posi
php -r 'use Posi\System; $fd = System::open("/dev/null", 2); System::close($fd); echo "ok\n";'
```

5. Sync **only** intentional artifacts back into the primary checkout (generated `ext/` sources you mean to ship, ABI mirrors under `ext/src/`, version header). Do **not** sync phpize junk (`Makefile`, `modules/*.so`, `autom4te.cache`, …).

6. Delete the copy:

```bash
rm -rf "$COPY"
```

7. Update `.okf` + `log.md` if the public surface or packaging changed.

# Explicit non-goals for agents

- Do not run zephir / phpize / make / `pie install` in the primary folder to “refresh” C.
- Do not regenerate IDE stubs unless Angel asks (current stubs lag at `ide/0.4.x`).

See [Do not rebuild in place](/traps/do-not-rebuild-in-place.md).

[^install]: install-macos.sh
[^debian]: install-debian-trixie.sh
[^config]: config.json
[^php-h]: php_posi.h
[^composer]: composer.json
