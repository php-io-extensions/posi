---
type: Playbook
title: Minimal open/read/close
description: README-style file descriptor open, read, and close demo
resource: /README.md
tags: [posi, playbook, demo]
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

# Prerequisites

- Extension loaded: `extension=posi`
- Unix host (Linux or macOS)

# Minimal open / close

```php
<?php

use Posi\System;

$fd = System::open('/dev/null', /* O_RDWR — platform value */ 2);
if ($fd < 0) {
    throw new RuntimeException('open failed');
}
System::close($fd);
```

# Open / read / close

```php
<?php

use Posi\System;

// O_RDONLY is platform-specific; 0 on Linux/glibc.
$fd = System::open('/etc/hosts', 0);

if ($fd < 0) {
    throw new RuntimeException('open failed');
}

try {
    $chunk = System::read($fd, 256);
    if ($chunk === false) {
        throw new RuntimeException('read failed');
    }
    echo $chunk;
} finally {
    System::close($fd);
}
```

Grounded in README usage examples.[^readme][^system-zep]

# Notes

- Replace numeric flags with platform-correct constants / enums — see [Constants trap](/traps/constants-platform-specific.md).
- Optional ergonomics: `composer require microscrap/posix` for global `posix_*` helpers (downstream only).

[^readme]: README.md
[^system-zep]: system.zep
