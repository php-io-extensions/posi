---
okf_version: "0.2"
---

# ext-posi

* [Module](architecture/module.md) - Scope, file layout, module globals, load order, errno and argument conventions.

# API

* [Descriptors and paths](api/descriptors.md) - open/read/write family, readv, recv, fcntl, ioctl argument modes, ppoll, fdopen, fileno, dup, socket fdopen, listen, chmod family, lstat, hostname.
* [Processes](api/processes.md) - posix_wait, posix_waitpid and the sys/wait.h status macros.
* [Termios](api/termios.md) - The termios array shape, speeds carried in it, BOTHER round trips, posi_set_baud_rate and its hand-declared termios2.
* [Native buffers](api/native-buffers.md) - posi_mem_alloc/free/read/write, the live-allocation registry, and how addresses reach ioctl() structs.
* [Constants](api/constants.md) - Header-valued constants, #ifdef guards, register-if-absent for names other extensions share, and ECHO.

# Runbooks

* [Build, install, test](runbooks/build.md) - gen_stub after a stub edit, scratch builds, the two installers, Pest on Mac and Pi, PIE.

# Reference

* [Upgrade from 0.9](reference/upgrade-from-0-9.md) - Zephir posi 0.9 plus microscrap/posix to C posi 0.10, call by call.
