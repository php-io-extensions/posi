---
type: API
title: Descriptors and paths
description: open/read/write family, readv, recv, fcntl, ioctl argument modes, ppoll, fdopen, fileno, dup, socket fdopen, chmod family, lstat, hostname.
resource: posi.c
tags: [posi, fd, ioctl, fcntl]
status: draft
generated: { by: claude-opus/5.5, at: 2026-10-15T00:04:15Z }
sources:
  - id: stub
    resource: posi.stub.php
    title: posi.stub.php
  - id: posi-c
    resource: posi.c
    title: posi.c
---

# Functions

Names are `microscrap/posix`'s. Signatures in `posi.stub.php`.[^stub]

| Function | C | Success | Failure |
|---|---|---|---|
| `posix_open($path, $flags = O_RDWR, $mode = 0644)` | `open` | fd | -1 |
| `posix_close($fd)` | `close` | 0 | -1 |
| `posix_read($fd, $n)` | `read` | string (≤ n bytes, "" at EOF) | false |
| `posix_write($fd, $data, $n)` | `write` | bytes written | -1; `$n` > `strlen($data)` → `ValueError` |
| `posix_lseek($fd, $offset, $whence)` | `lseek` | offset | -1 |
| `posix_readv($fd, [['len' => n], …])` | `readv` | `['res' => total, 'buffers' => [string, …]]`, each buffer cut to bytes landed | false; > `IOV_MAX` entries or bad `len` → `ValueError` |
| `posix_recv($fd, $len, $flags = 0)` | `recv` | string | false |
| `posix_ppoll($fd, $timeout_ns = 0, $events = 0)` | `ppoll` Linux, `poll` macOS | 1 ready, 0 timeout | -1 |
| `posix_fdopen($fd, $mode)` | `php_stream_fopen_from_fd` | stream | false (bad fd → errno saved) |
| `posix_fileno($stream)` | `php_stream_cast` (`AS_SOCKETD`, then `AS_FD`) | fd | false, errno `EBADF` |
| `posix_dup($fd)` | `dup` | new fd | -1 |
| `posix_socket_fdopen($fd)` | registered `tcp` transport factory, fd installed | socket stream | false: `EBADF` closed, `ENOTSOCK` not a socket, `EPROTOTYPE` not `SOCK_STREAM` |
| `posix_chmod/fchmod/chown/fchown` | same | 0 | -1 |
| `posix_umask($mask)` | `umask` | previous mask | — |
| `posix_lstat($path)` | `lstat` | stat array | false |
| `posix_hostname()` | `gethostname` | name | false |

`posix_ppoll`: events 0 → `POLLIN`; negative timeout → wait forever; macOS rounds timeout up to whole ms, caps at `INT_MAX` ms. Result 1 whenever `revents` nonzero, including `POLLHUP`/`POLLERR`.[^posi-c]

`posix_fdopen`: no `dup()`. Stream owns fd; `fclose()` closes it. Flags untouched. `F_GETFD` probe first, so a closed fd fails with errno instead of a stream over nothing.[^posi-c]

`posix_fileno`: no `dup()`; fd still belongs to the stream. `php://memory` and other descriptor-less streams → false, `EBADF`.[^posi-c]

`posix_socket_fdopen`: no `dup()`; stream owns fd, `fclose()` closes it. Built by the factory registered for `tcp` (ext/openssl overrides it, so the stream gets TLS-capable ops when openssl loads), then the fd goes into the `php_netstream_data_t` that opens both core and openssl abstracts; `is_blocked` follows the fd's `O_NONBLOCK`. `php_stream_sock_open_from_socket()` can't stand in: its `generic_socket` ops have no accept and no crypto. openssl's factory copies `*timeout`, so a `default_socket_timeout` timeval is passed, never NULL. Purpose: ext-parallel threads can't share streams but do share descriptors; each thread opens its own stream over a `dup()` of one listener. Measured 2026-10-14, four threads on one shared listener, 400 connections: macOS 65/131/72/132, Linux (Pi) 1/23/374/2; Linux spreads accepts only across per-thread `SO_REUSEPORT` listeners (98/100/115/87), macOS hands every connection to the last `SO_REUSEPORT` binder (0/0/0/400).[^posi-c]

# fcntl($fd, $cmd, $arg, &$value): int

Returns C result. `$arg`:
- `F_GETLK`/`F_SETLK`/`F_SETLKW`: array `type, whence, start, len, pid` (missing = 0) → `struct flock`; `$value` = resulting lock array. Non-array → `TypeError`.
- else int (int-range checked), bool (1/0), null (no third arg). Other types → `TypeError`.

`$value`: false on failure; datum for `F_GETFD`, `F_GETFL`, `F_GETOWN`; else copy of `$arg`.[^posi-c]

# ioctl($fd, $request, $arg, &$value): int

`$request` passes as `unsigned long` unchanged, so Linux `_IOR`/`_IOW` numbers above `INT_MAX` work. `$arg` picks third argument:

| `$arg` | C receives | `$value` after success |
|---|---|---|
| `['value' => int]` | `int *` | the int after call |
| `['bytes' => string]` / `['data' => string]` / string | `char *` to private copy | copy's bytes after call |
| int / bool | the number as pointer-sized arg | copy of `$arg` |
| null | `NULL` | return value if > 0, else null |

Failure: `$value` = false, errno saved. Array without `value`/`bytes`/`data` → `ValueError`; other types → `TypeError`.[^posi-c]

Int mode carries addresses from [native buffers](/api/native-buffers.md) and plain-number requests (`I2C_SLAVE` address). Kernel dereferences whatever number arrives; posi cannot check requests it does not know. Recorded in `SECURITY.md`.

[^stub]: posi.stub.php
[^posi-c]: posi.c
