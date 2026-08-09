---
type: Convention
title: Handle ownership
description: Caller owns FDs and Memory pointers; PHP GC does not free them
tags: [posi, convention, memory, fd]
status: draft
generated: { by: okf-documentation-generator/cursor-grok-4.5, at: "2026-08-09T17:48:43Z" }
sources:
  - id: system-zep
    resource: /posi/system.zep
    title: system.zep
  - id: memory-zep
    resource: /posi/memory.zep
    title: memory.zep
  - id: readme
    resource: /README.md
    title: README.md
---

# Rules

| Resource | Create | Release |
|----------|--------|---------|
| File descriptor | `System::open` (and any future openers) | `System::close` |
| Native buffer | `Memory::alloc` | `Memory::free` (once; `0` no-op) |

# Semantics

- Successful `open` returns a non-negative FD; `-1` means failure — do not `close` a failed open.[^readme]
- Dropping an FD int without `close` **leaks** the descriptor until process exit.[^system-zep]
- Dropping a Memory pointer without `free` **leaks** native heap.[^memory-zep]
- `Memory::write` / `read` do not transfer ownership; they copy bytes.
- After `close`, do not reuse the FD int.

# Checklist

1. Pair every successful `open` with `close` (preferably in `finally`).
2. Pair every `Memory::alloc` with `Memory::free`.
3. Free Memory buffers **after** the ioctl that referenced them completes.
4. Do not share FDs or native pointers across process forks without a clear plan.

See also [FD ownership trap](/traps/fd-ownership-close.md).

[^system-zep]: system.zep
[^memory-zep]: memory.zep
[^readme]: README.md
