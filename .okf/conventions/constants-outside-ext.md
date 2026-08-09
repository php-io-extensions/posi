---
type: Convention
title: Constants outside the extension
description: No PHP class constants; O_*/F_*/ioctl/baud live in app or microscrap enums
tags: [posi, convention, constants, enums]
status: draft
generated: { by: okf-documentation-generator/cursor-grok-4.5, at: "2026-08-09T17:48:43Z" }
sources:
  - id: readme
    resource: /README.md
    title: README.md
  - id: system-zep
    resource: /posi/system.zep
    title: system.zep
---

# Rule

Zephir classes under `Posi\` expose **methods**, not PHP class constants for POSIX flags, fcntl/ioctl commands, or baud rates.[^system-zep]

Apps define locals (as in README examples that hard-code Linux/glibc numeric flags) or consume **microscrap** / app-level backed enums.[^readme]

```php
// Example — values are platform-specific; do not treat as portable literals.
$O_RDWR = 2;
$O_CREAT = 64;
$O_TRUNC = 512;
```

Prefer int-/string-backed PHP Enums (FULLY UPPERCASE cases) in application or microscrap layers — not new class constants inside this extension.

Same convention as `php-io-extensions/sdl3`. See [Constants are platform-specific](/traps/constants-platform-specific.md).

[^readme]: README.md
[^system-zep]: system.zep
