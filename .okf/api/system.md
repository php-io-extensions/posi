---
type: API
title: Posi\\System
description: Static POSIX FD I/O and syscall wrappers
resource: /posi/system.zep
tags: [posi, api, system]
status: draft
generated: { by: okf-documentation-generator/cursor-grok-4.5, at: "2026-08-09T17:48:43Z" }
sources:
  - id: system-zep
    resource: /posi/system.zep
    title: system.zep
  - id: readme
    resource: /README.md
    title: README.md
  - id: system-h
    resource: /src/system-api.h
    title: system-api.h
---

# Class

`Posi\System` — all methods **static**. Implemented surface grounded in `posi/system.zep`.[^system-zep] README documents most methods with C-ish semantics (`-1` / `false` on failure).[^readme]

# Implemented methods

| Method | Signature (Zephir / README) |
|--------|-----------------------------|
| `chmod` | `(string $path, int $mode): int` |
| `chown` | `(string $path, int $owner, int $group): int` |
| `close` | `(int $fd): int` |
| `fchmod` | `(int $fd, int $mode): int` |
| `fchown` | `(int $fd, int $owner, int $group): int` |
| `getuid` | `(): int` |
| `setuid` | `(int $uid): int` |
| `umask` | `(int $mask): int` |
| `fcntl` | `(int $fd, int $cmd, mixed $arg = null): array` → `res` / `val` |
| `ioctl` | `(int $fd, int $cmd, mixed $arg = null): array` → `res` / `val` |
| `lseek` | `(int $fd, int $offset, int $whence): int` |
| `open` | `(string $filepath, int $flags, int $mode = 0644): int` |
| `ppoll` | `(int $fd, int $timeout_ns, int $events = 0): int` |
| `read` | `(int $fd, int $bytes): string\|false` |
| `readv` | `(int $fd, array $iovecs): array\|false` |
| `recv` | `(int $fd, int $len, int $flags = 0): string\|false` |
| `wait` | `(?int &$status = null): int` |
| `waitpid` | `(int $pid, ?int &$status = null, int $options = 0): int` |
| `hostname` | `(): string\|false` |
| `lstat` | `(string $path): array\|false` |
| `write` | `(int $fd, string $data, int $bytes): int` |

`ppoll` is implemented in Zephir/C but is **not** listed in the README quick-reference table — still a real API.[^system-zep][^system-h]

# Notes

- Flag / command integers (`O_*`, `F_*`, ioctl cmds, `SEEK_*`, `WNOHANG`, …) are **not** defined by the extension — see [Constants outside](/conventions/constants-outside-ext.md).
- Pair every successful `open` with `close` — see [FD ownership](/traps/fd-ownership-close.md).
- Commented scaffolds (socket, mmap, …) are **not** callable — see [Reserved scaffolds](/api/reserved-scaffolds.md).

[^system-zep]: system.zep
[^readme]: README.md
[^system-h]: system-api.h
