---
type: Trap
title: FD ownership / close
description: PHP GC does not close file descriptors opened via System::open
tags: [posi, trap, fd, close]
status: draft
generated: { by: okf-documentation-generator/cursor-grok-4.5, at: "2026-08-09T17:48:43Z" }
sources:
  - id: system-zep
    resource: /posi/system.zep
    title: system.zep
  - id: readme
    resource: /README.md
    title: README.md
---

# Trap

`System::open` returns a raw OS file descriptor as a PHP `int`. When that int goes out of scope, PHP’s garbage collector does **not** call `close(2)`.[^system-zep][^readme]

Leaked FDs exhaust `ulimit -n` in long-running processes (GPIO/SPI loops, servers).

# Do

```php
$fd = System::open($path, $flags);
try {
    // …
} finally {
    System::close($fd);
}
```

Same discipline for `Memory::alloc` → `Memory::free`.

# Do not

- Assume “dropping the variable” is cleanup.
- Call `close` on `-1` after a failed `open`.

[^system-zep]: system.zep
[^readme]: README.md
