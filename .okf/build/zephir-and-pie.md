---
type: Playbook
title: Zephir + PIE install
description: Platform installers, PIE, and phpize builds on Linux and macOS
resource: /composer.json
tags: [posi, build, pie, zephir]
status: draft
generated: { by: okf-documentation-generator/cursor-grok-4.5, at: "2026-08-09T17:48:43Z" }
sources:
  - id: composer
    resource: /composer.json
    title: PIE package manifest
  - id: install
    resource: /install-macos.sh
    title: install-macos.sh
  - id: debian
    resource: /install-debian-trixie.sh
    title: install-debian-trixie.sh
  - id: pre
    resource: /pre-install.sh
    title: pre-install.sh
  - id: readme
    resource: /README.md
    title: Package README
---

# Requirements

| Component | Notes |
|-----------|--------|
| OS | Linux or macOS (Windows excluded) |
| PHP | composer `>= 8.0`; README recommends **8.3+** |
| Zephir | Required by platform installers when regenerating C; not required for PIE/`phpize` from committed `ext/` |
| Compiler | C toolchain (`clang` / `gcc`) |

# PIE (consumers)

```bash
pie install php-io-extensions/posi
```

Uses `type: php-ext`, `extension-name: posi`, `build-path: "ext"`, `--enable-posi`.[^composer]

Enable if needed:

```ini
extension=posi
```

(README occasionally mentions a legacy `pposix` module name in prose — the PIE/`composer.json` name is **`posi`**.)[^readme]

# Platform installers (on disk)

```bash
bash install-macos.sh            # macOS (+ Herd-aware PHP resolution)
bash install-debian-trixie.sh    # Debian Trixie / Raspberry Pi OS
```

`pre-install.sh` supports installer plumbing.[^pre][^install][^debian]

**Gap:** README also cites `install-jetpack6.sh`, but that script is **not** present in this tree — do not invent JetPack steps here.

# Manual build from committed `ext/`

Consumers and CI may `phpize` / `configure` / `make` **from the ship-ready `ext/`** without Zephir. That compiles the committed C; it does **not** regenerate Zephir output.

```bash
cd ext
phpize
./configure --enable-posi
make
sudo make install
```

# Critical agent rule

Do **not** run Zephir / full installers inside the primary checkout when the goal is to refresh generated C. Work on a **copy** — see [Do not rebuild in place](/traps/do-not-rebuild-in-place.md) and [Regenerate ext](/playbooks/regenerate-ext.md).

[^composer]: PIE package manifest
[^install]: install-macos.sh
[^debian]: install-debian-trixie.sh
[^pre]: pre-install.sh
[^readme]: Package README
