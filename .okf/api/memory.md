---
type: API
title: Posi\\Memory
description: Native emalloc buffers for ioctl structs that embed C pointers
resource: /posi/memory.zep
tags: [posi, api, memory]
status: draft
generated: { by: okf-documentation-generator/cursor-grok-4.5, at: "2026-08-09T17:48:43Z" }
sources:
  - id: memory-zep
    resource: /posi/memory.zep
    title: memory.zep
  - id: memory-h
    resource: /src/memory-api.h
    title: memory-api.h
---

# Class

`Posi\Memory` — static helpers for named native buffers when an ioctl struct needs **pointer fields** (e.g. SPI/I2C message transfers). PHP strings cannot expose stable addresses; these APIs return buffer addresses as PHP `int`.[^memory-zep]

# Implemented methods

| Method | Signature | Notes |
|--------|-----------|--------|
| `alloc` | `(int $size): int` | Zero-filled buffer; caller must `free` exactly once |
| `free` | `(int $ptr): void` | `0` is a safe no-op |
| `write` | `(int $ptr, string $data, int $offset = 0): void` | No bounds checking |
| `read` | `(int $ptr, int $size, int $offset = 0): string` | Copy out as PHP string |

# Pattern

1. `alloc` TX/RX buffers.
2. `write` payload into TX.
3. `pack(...)` struct fields with buffer addresses as integers.
4. `System::ioctl($fd, $cmd, ['data' => $struct])`.
5. `read` RX buffer; `free` both.

See class docblock in `memory.zep` for the SPI full-duplex sketch.[^memory-zep]

# Ownership

Caller owns every non-zero pointer from `alloc` until `free`. PHP GC does **not** free native buffers — see [Handle ownership](/conventions/handle-ownership.md).

[^memory-zep]: memory.zep
[^memory-h]: memory-api.h
