---
type: Trap
title: Constants are platform-specific
description: O_*/F_*/ioctl/baud integer values differ across Linux and macOS
tags: [posi, trap, constants, portability]
status: draft
generated: { by: okf-documentation-generator/cursor-grok-4.5, at: "2026-08-09T17:48:43Z" }
sources:
  - id: readme
    resource: /README.md
    title: README.md
---

# Trap

`open()`, `fcntl()`, `ioctl()`, and termios helpers take the **same integer constants as C**, but those numeric values **differ by OS** (and sometimes by libc).[^readme]

Hard-coding Linux/glibc numbers (as some README examples do for illustration) will misbehave on macOS if copied blindly.

# Do

- Define constants from platform headers, microscrap enums, or runtime-detected values.
- Document which OS an example’s literal flags target.

# Do not

- Bake portable PHP class constants into this extension.
- Assume Linux ioctl command numbers work on Darwin.

See [Constants outside the extension](/conventions/constants-outside-ext.md).

[^readme]: README.md
