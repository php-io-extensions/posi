# Agent guidance — php-io-extensions/posi

1. **Read [`.okf/index.md`](.okf/index.md) first** before changing the API, the C, or packaging. Open only the concepts the change touches.
2. **POSIX calls, 1:1.** One PHP function = one C call, named as `microscrap/posix` named it (`posix_open`, `fcntl`, `ioctl`) or as POSIX names it (`tcgetattr`). No helpers that aren't a system call, no event loop, no throwing on a failed call: failure is C's -1 or false, with errno kept for `posi_errno()`.
3. **POSIX only.** epoll, eventfd, timerfd, signalfd and kqueue never go in posi; they have their own extensions (ext-epoll, ext-kqueue). Name the standard an API comes from before proposing it. See [`.okf/architecture/module.md`](.okf/architecture/module.md).
4. **Plain C on the Zend API.** No Zephir, no FFI. Sources: `posi.c`, `posi_termios.c`, `php_posi.h`.
5. **The stub is the declaration.** Edit `posi.stub.php`, regenerate `posi_arginfo.h` with gen_stub, commit both. Never hand-edit arginfo. See [`.okf/runbooks/build.md`](.okf/runbooks/build.md).
6. **Constants come from headers.** Each one `#ifdef`-guarded with `@cvalue`, never a literal. A name that ext/sockets, ext/pcntl or ext-kqueue may also register goes through `posi_register_if_absent()` in MINIT, not the stub. See [`.okf/api/constants.md`](.okf/api/constants.md).
7. **Arguments are checked, never truncated.** Ints through `POSI_ARG_INT` or an explicit range check; lengths against the data; out of range → `ValueError` naming the argument. By-ref outputs through `ZEND_TRY_ASSIGN_REF_*`.
8. **Native buffers stay registry-checked.** Every `posi_mem_*` address is looked up in `POSI_G(allocations)` before memory is touched. See [`.okf/api/native-buffers.md`](.okf/api/native-buffers.md).
9. **NTS and ZTS.** State lives only in module globals. Every change builds and passes Pest under Homebrew `php@8.4` and `php@8.4-zts` (`php84`, `zhp`) and on Linux (the Pi via `fnk`).
10. **Tests are Pest v4 and hardware-free**: pipes, FIFOs, scratch files, ptys, child processes. Expected values come from the constants, never platform numbers; wait statuses come from real children.
11. **Durable facts go in `.okf`.** Update the matching concept, bump its `generated.at`, append `.okf/log.md`. The bundle documents the package, never a session, and lives at the repo root only.
12. **Versions**: `composer.json` `version` and `PHP_POSI_VERSION` move together, and only past a published tag.
