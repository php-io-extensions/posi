---
type: API
title: Reserved scaffolds
description: Commented System method signatures — not implemented public API
resource: /posi/system.zep
tags: [posi, api, scaffolds]
status: draft
generated: { by: okf-documentation-generator/cursor-grok-4.5, at: "2026-08-09T17:48:43Z" }
sources:
  - id: system-zep
    resource: /posi/system.zep
    title: system.zep
---

# Rule

`posi/system.zep` contains many **commented-out** `public static function …()` signatures. They are a roadmap / reserved name list only. They are **not** callable PHP APIs and must not be documented as implemented.[^system-zep]

# Commented scaffolds (names only)

`accept`, `access`, `adjtime`, `bind`, `chdir`, `chflags`, `chroot`, `connect`, `dup`, `dup2`, `fchdir`, `fchflags`, `flock`, `fpathconf`, `fstat`, `fstatfs`, `fsync`, `ftruncate`, `getdirentries`, `getdomainname`, `getegid`, `geteuid`, `getfh`, `getfsstat`, `getgid`, `gethostname`, `getpeername`, `getpgid`, `getpgrp`, `getpid`, `getppid`, `getrlimit`, `getsid`, `getsockname`, `getsockopt`, `gettimeofday`, `issetugid`, `kill`, `link`, `listen`, `mkdir`, `mkfifo`, `mknod`, `mmap`, `mount`, `mq_close`, `mq_getattr`, `mq_open`, `mq_receive`, `mq_send`, `mq_unlink`, `mq_setattr`, `munmap`, `nfssvc`, `nvramapi`, `pathconf`, `pipe`, `posix_spawn`, `posix_spawnp`, `readlink`, `recvfrom`, `recvmsg`, `rename`, `rmdir`, `select`, `send`, `sendmsg`, `sendto`, `setdomainname`, `setegid`, `seteuid`, `setgid`, `sethostname`, `setpgid`, `setpgrp`, `setrlimit`, `setsid`, `setsockopt`, `settimeofday`, `shm_open`, `shm_unlink`, `shutdown`, `sigaction`, `sigpending`, `sigprocmask`, `sigqueue`, `sigsuspend`, `sigtimedwait`, `sigwait`, `sigwaitinfo`, `socket`, `socketpair`, `stat`, `statfs`, `swapon`, `symlink`, `sync`, `truncate`, `unlink`, `unmount`, `utimes`, `writev`.

Note: implemented `hostname()` covers host-name retrieval; the commented `gethostname` scaffold is separate and unused.

# Agent rule

Do not invent signatures, return types, or behaviors for these names. When one is implemented, move it to [System](/api/system.md), add C ABI + optimizer, regenerate `ext/` on a **copy**, and update this list.

[^system-zep]: system.zep
