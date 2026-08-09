---
type: API
title: Posi\\Termios
description: Terminal attributes and baud-rate helpers
resource: /posi/termios.zep
tags: [posi, api, termios]
status: draft
generated: { by: okf-documentation-generator/cursor-grok-4.5, at: "2026-08-09T17:48:43Z" }
sources:
  - id: termios-zep
    resource: /posi/termios.zep
    title: termios.zep
  - id: termios-h
    resource: /src/termios-api.h
    title: termios-api.h
---

# Class

`Posi\Termios` — static wrappers around POSIX termios APIs. Attribute bags are PHP assoc arrays (`c_iflag`, `c_oflag`, `c_cflag`, `c_lflag`, `c_cc`).[^termios-zep]

# Implemented methods

| Method | Signature | Notes |
|--------|-----------|--------|
| `tcgetattr` | `(int $fd): array\|false` | Current attributes or `false` |
| `tcsetattr` | `(int $fd, int $action, array $termios): int` | `$action`: TCSANOW/TCSADRAIN/TCSAFLUSH (0/1/2) |
| `cfsetispeed` | `(array $termios, int $speed): array\|false` | Returns updated array |
| `cfsetospeed` | `(array $termios, int $speed): array\|false` | Returns updated array |
| `cfgetispeed` | `(array $termios): int` | Input baud |
| `cfgetospeed` | `(array $termios): int` | Output baud |
| `tcdrain` | `(int $fd): int` | Wait for output drain |
| `tcflush` | `(int $fd, int $queue): int` | TCIFLUSH/TCOFLUSH/TCIOFLUSH |
| `tcflow` | `(int $fd, int $action): int` | TCOOFF/TCOON/TCIOFF/TCION |
| `setBaudRate` | `(int $fd, int $baud): int` | Standard `B*` path, or Linux `BOTHER`/termios2 for nonstandard rates (e.g. 256000) |

# Notes

- Baud / action / queue constants live **outside** the extension.
- `setBaudRate` returns `-1` on non-Linux when the rate has no standard `B*` constant.[^termios-zep][^termios-h]

[^termios-zep]: termios.zep
[^termios-h]: termios-api.h
