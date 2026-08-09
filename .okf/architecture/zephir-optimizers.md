---
type: Architecture
title: Zephir optimizers
description: optimizers/ compile-time rewrites from Zephir helpers to C ABI
resource: /optimizers
tags: [posi, architecture, zephir, optimizers]
status: draft
generated: { by: okf-documentation-generator/cursor-grok-4.5, at: "2026-08-09T17:48:43Z" }
sources:
  - id: config
    resource: /config.json
    title: Zephir config optimizer-dirs
  - id: open-opt
    resource: /optimizers/OpenFileOptimizer.php
    title: OpenFileOptimizer.php
  - id: mem-opt
    resource: /optimizers/MemAllocOptimizer.php
    title: MemAllocOptimizer.php
---

# Role

`config.json` sets `optimizer-dirs: ["optimizers"]`.[^config] Each optimizer maps a Zephir helper call (e.g. `open_file`, `mem_alloc`) to a direct C ABI invocation and adds the matching header via `headersManager`.

Example: `OpenFileOptimizer` requires an `int` result symbol, adds `src/system-api`, and emits `posix_open(...)`.[^open-opt]

# Groups (representative)

| Domain | Optimizer classes (examples) |
|--------|------------------------------|
| System FD I/O | `OpenFileOptimizer`, `CloseFileOptimizer`, `ReadFileOptimizer`, `WriteFileOptimizer`, … |
| fcntl / ioctl | `FcntlFileOptimizer`, `IoctlFileOptimizer` |
| Process / path | `GetuidProcOptimizer`, `WaitpidProcOptimizer`, `LstatPathOptimizer`, … |
| Memory | `MemAllocOptimizer`, `MemFreeOptimizer`, `MemWriteOptimizer`, `MemReadOptimizer`[^mem-opt] |
| Termios | `TcgetattrFileOptimizer`, `SetBaudRateTermiosOptimizer`, `CfsetispeedTermiosOptimizer`, … |

# Maintainer note

Adding a new Zephir helper that calls into `src/` usually requires a matching optimizer under `optimizers/` **and** regenerating `ext/` on a disposable tree copy — see [Regenerate ext](/playbooks/regenerate-ext.md). Do not invent helper names that have no `.zep` callers and no C ABI.

[^config]: Zephir config optimizer-dirs
[^open-opt]: OpenFileOptimizer.php
[^mem-opt]: MemAllocOptimizer.php
