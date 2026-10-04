# Security Policy

## Supported versions

ext-posi is pre-1.0. No 0.x release receives security fixes or advisories; fixes land in the
next release line. Security support starts with 1.0.

| Version | Security fixes |
|---------|----------------|
| < 1.0   | No             |

## Reporting a vulnerability

Please don't open a public issue for a security problem.

Report it privately through GitHub: the **Report a vulnerability** button on the
[php-io-extensions/posi](https://github.com/php-io-extensions/posi) repository's **Security** tab.
If that isn't available, email **info@projectsaturnstudios.com**.

Include what you found, the affected version, your platform and PHP build (NTS or ZTS), and steps
to reproduce. Reports are read and weighed for the release line in development; before 1.0 there
is no response-time commitment.

## Security model

ext-posi binds POSIX calls one to one. Each function performs the C call with the privileges of
the PHP process, the same as C code linked into that process. Load it only where every PHP script
the process runs is trusted, such as a CLI program driving hardware. Do not load it into a shared
web host.

- **Descriptors and paths.** `posix_open()`, `posix_chmod()`, `posix_chown()`, `posix_lstat()` and
  the other path functions call the system directly. They do not consult `open_basedir`, and any
  descriptor number the process holds can be read, written or closed. Restrict them with
  `disable_functions`, or don't load the extension.
- **`ioctl()`.** When `$arg` is an int, posi passes it to the kernel as the pointer-sized argument
  itself. A request that treats that argument as an address makes the kernel read or write process
  memory at that address. The same holds for addresses packed inside a struct passed as bytes.
  posi cannot know what a request does with its argument; the kernel checks only that the address
  lies in the process's own memory.
- **Native buffers.** `posi_mem_read()`, `posi_mem_write()` and `posi_mem_free()` accept only
  addresses `posi_mem_alloc()` returned and has not released, and only ranges inside them, so a
  stale or foreign address throws `ValueError` before any memory is touched. Buffers still
  allocated when the request ends are freed.
- **Arguments.** Every int argument is checked against the C type, and every length against the
  data it describes. Out-of-range values throw `ValueError`; nothing is truncated into a different
  value.

A report is in scope when posi itself reads or writes memory it should not: a length check that
can be bypassed, a use after free in the buffer registry, a crash reachable from well-formed PHP
arguments. Effects that follow from performing a valid system call the script asked for, such as
an `ioctl()` request with a chosen address, are the documented behaviour.
