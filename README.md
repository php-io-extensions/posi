# Posi - The POSIX extension to unlock PHP's System Call ability.

This project provides bindings to the UNIX Portable Operating System Interface (POSIX).

## Highlights

* Retrieve file descriptors to replace `fopen`
* Read and write to file descriptors beyond streams, replacing `fread` and `fwrite`
* Direct access to `fcntl`
* Direct access to `ioctl`

## Requirements

* PHP 8.3+

## Compilation Requirements

* A C language toolchain

* Manual installation
  * Zephir PHP
  * ext-zephir_parser

* Automated installation
  * PIE

## Manual compilation instructions

### Zephir
* Clone this repository and checkout the release to use (or master for the latest updates)
* `cd posi`
* Run the installer for your platform
  * Raspberry Pi: `bash install-debian-trixie.sh`
  * macOS: `bash install-macos.sh`
  * Jetson Orin (Nano Super): `bash install-jetpack6.sh`
  * The installer will compile the extension and add it to your PHP ini automatically

### PIE
* NOTE: If using Laravel Herd to serve PHP, use Zephir via the install scripts. 
* Clone this repository and checkout the release to use (or master for the latest updates)
* `cd posi`
* `pie install`

## Automated compilation instructions

* `pie install php-io-extensions/posi`
* The installer should compile and stash the extension where the default PHP binary stores its ini.
* If PIE does not automatically enable the extension, add it to your `php.ini` (module name is typically `pposix`):

  ```ini
  extension=posi
  ```

## Usage

POSIX I/O is invoked through the `Posi\System` class. All methods are static.

```php
<?php

use Posi\System;

$fd = System::open('/dev/null', O_RDWR);
System::close($fd);
```

`open()`, `fcntl()`, and `ioctl()` take the same flag and command constants as C (`O_RDONLY`, `F_GETFL`, and so on). Define them in PHP or load them from your platform headers; values differ by OS.

---

## Static Methods API

### `System::open(string $filepath, int $flags, int $mode = 0644): int`

Opens a path and returns a **file descriptor** (non-negative integer), or `-1` on failure (same semantics as C `open(2)`).

* `$filepath` — device node, pipe, socket path, or regular file path.
* `$flags` — `O_*` access mode and creation flags (e.g. `O_RDWR`, `O_CREAT | O_TRUNC`).
* `$mode` — permission bits when `O_CREAT` is set (default `0644`).

**Example — open an existing file for reading**

```php
<?php

use Posi\System;

// O_RDONLY is platform-specific; 0 on Linux/glibc.
$fd = System::open('/etc/hosts', 0);

if ($fd < 0) {
    throw new RuntimeException('open failed');
}

$chunk = System::read($fd, 256);
System::close($fd);
```

**Example — create and truncate a file for read/write**

```php
<?php

use Posi\System;

// Linux/glibc: O_RDWR | O_CREAT | O_TRUNC
$flags = 2 | 64 | 512;

$fd = System::open('/tmp/posi-demo.txt', $flags, 0644);

if ($fd < 0) {
    throw new RuntimeException('open failed');
}

System::write($fd, "hello\n", 6);
System::close($fd);
```

---

### `System::close(int $fd): int`

Closes a file descriptor. Returns `0` on success and `-1` on failure (same semantics as C `close(2)`).

**Example**

```php
<?php

use Posi\System;

$fd = System::open('/dev/null', O_RDWR);

$result = System::close($fd);
// $result === 0 on success
```

---

### `System::chmod(string $path, int $mode): int`

Sets permission bits on a path. Returns `0` on success and `-1` on failure (same semantics as C `chmod(2)`).

* `$path` — file or directory path.
* `$mode` — permission bits (e.g. `0644`). Combine with `S_ISUID`, `S_ISGID`, and `S_ISVTX` where your platform supports them.

Changing modes on paths you do not own, or setting set-user-ID bits, typically requires appropriate privileges.

**Example**

```php
<?php

use Posi\System;

$path = '/tmp/posi-chmod-demo.txt';
$fd = System::open($path, 2 | 64 | 512, 0644);
System::close($fd);

if (System::chmod($path, 0600) !== 0) {
    throw new RuntimeException('chmod failed');
}
```

---

### `System::chown(string $path, int $owner, int $group): int`

Sets owner and group for a path. Returns `0` on success and `-1` on failure (same semantics as C `chown(2)`).

* `$owner` — numeric user ID (`uid_t`).
* `$group` — numeric group ID (`gid_t`).

This call usually requires superuser privileges or capability `CAP_CHOWN` on Linux.

**Example**

```php
<?php

use Posi\System;

$path = '/tmp/owned-by-me.txt';
$fd = System::open($path, 2 | 64 | 512, 0644);
System::close($fd);

$uid = (int) posix_getuid();
$gid = (int) posix_getgid();

if (System::chown($path, $uid, $gid) !== 0) {
    throw new RuntimeException('chown failed');
}
```

---

### `System::fchmod(int $fd, int $mode): int`

Like `chmod`, but applies to an open file descriptor. Returns `0` on success and `-1` on failure (same semantics as C `fchmod(2)`).

**Example**

```php
<?php

use Posi\System;

$fd = System::open('/tmp/posi-fchmod.txt', 2 | 64 | 512, 0644);

if (System::fchmod($fd, 0600) !== 0) {
    System::close($fd);
    throw new RuntimeException('fchmod failed');
}

System::close($fd);
```

---

### `System::fchown(int $fd, int $owner, int $group): int`

Like `chown`, but applies to the file referred to by an open descriptor. Returns `0` on success and `-1` on failure (same semantics as C `fchown(2)`).

**Example**

```php
<?php

use Posi\System;

$fd = System::open('/tmp/posi-fchown.txt', 2 | 64 | 512, 0644);

$uid = (int) posix_getuid();
$gid = (int) posix_getgid();

if (System::fchown($fd, $uid, $gid) !== 0) {
    System::close($fd);
    throw new RuntimeException('fchown failed');
}

System::close($fd);
```

---

### `System::write(int $fd, string $glob_of_bytes, int $num_bytes_to_write): int`

Writes up to `$num_bytes_to_write` bytes from `$glob_of_bytes` to the descriptor. Returns the number of bytes written, or `-1` on failure (same semantics as C `write(2)`).

**Example — write a line to a file descriptor**

```php
<?php

use Posi\System;

$fd = System::open('/tmp/posi-demo.txt', 2 | 64 | 512, 0644);

$payload = "ping\n";
$written = System::write($fd, $payload, strlen($payload));

// $written === 5 when all bytes were accepted
System::close($fd);
```

**Example — write to `/dev/null`**

```php
<?php

use Posi\System;

$fd = System::open('/dev/null', O_WRONLY);

System::write($fd, str_repeat('x', 1024), 1024);
System::close($fd);
```

---

### `System::read(int $fd, int $amt_of_bytes_to_read): string`

Reads up to `$amt_of_bytes_to_read` bytes from the descriptor. Returns a **binary string** of the bytes read (length may be less than requested). Returns `false` if the underlying `read(2)` fails.

**Example — read the first 512 bytes of a file**

```php
<?php

use Posi\System;

$fd = System::open('/etc/hosts', 0);

$data = System::read($fd, 512);

if ($data === false) {
    throw new RuntimeException('read failed');
}

echo $data;
System::close($fd);
```

**Example — read from a pipe in a loop**

```php
<?php

use Posi\System;

$fd = /* descriptor from pipe, socket, or device */;

while (true) {
    $chunk = System::read($fd, 4096);

    if ($chunk === false) {
        break;
    }

    if ($chunk === '') {
        break; // EOF
    }

    // process $chunk
}
```

---

### `System::getuid(): int`

Returns the real user ID of the calling process (same semantics as C `getuid(2)`). This call always succeeds from the extension’s perspective.

**Example**

```php
<?php

use Posi\System;

$uid = System::getuid();
```

---

### `System::setuid(int $uid): int`

Sets the real user ID of the calling process. Returns `0` on success and `-1` on failure (same semantics as C `setuid(2)`). Changing UID typically requires appropriate privileges.

**Example**

```php
<?php

use Posi\System;

if (System::setuid(65534) !== 0) {
    // not privileged or operation denied
}
```

---

### `System::umask(int $mask): int`

Sets the file mode creation mask. Returns the **previous** umask value (same semantics as C `umask(2)`). Values are numeric (for example octal `022` is the integer `18` on typical platforms).

**Example**

```php
<?php

use Posi\System;

$previous = System::umask(0077);
// restore prior mask
System::umask($previous);
```

---

### `System::lseek(int $fd, int $offset, int $whence): int`

Repositions the offset of the open descriptor. Returns the new offset measured in bytes from the beginning of the file, or `-1` on failure (same semantics as C `lseek(2)`). Use `SEEK_SET`, `SEEK_CUR`, and `SEEK_END` from your platform.

**Example**

```php
<?php

use Posi\System;

$fd = System::open('/etc/hosts', 0);

$pos = System::lseek($fd, 0, SEEK_SET);
System::close($fd);
```

---

### `System::recv(int $fd, int $len, int $flags = 0): string|false`

Receives up to `$len` bytes from a **socket** descriptor (same semantics as C `recv(2)`). Returns a binary string (possibly shorter than `$len`), or `false` on failure. Optional `$flags` are the usual `MSG_*` constants for your platform (`0` to block until data is available).

**Example**

```php
<?php

use Posi\System;

// $sock must be a connected socket FD from System::socket() when implemented,
// or another API that returns a socket descriptor.
// $data = System::recv($sock, 4096, 0);
```

---

### `System::readv(int $fd, array $iovecs): array|false`

Scatter-read into several buffers in one syscall (`readv(2)`). Each element of `$iovecs` is an associative array:

| Key    | Meaning |
|--------|---------|
| `len`  | Required. Maximum bytes to place in this segment. |
| `base` | Optional. If present as a string, up to `min(strlen($base), len)` bytes are copied into the segment before the read (usually you omit this for a fresh read buffer). |

On success, returns:

| Key       | Meaning |
|-----------|---------|
| `res`     | Total bytes read (non-negative), or `0` at EOF. |
| `buffers` | List of binary strings, one per `$iovecs` entry, each truncated to the portion of the read that landed in that segment. |

Returns `false` if the syscall fails or if `$iovecs` is invalid.

**Example — two segments from a regular file**

```php
<?php

use Posi\System;

$fd = System::open('/etc/hosts', 0);

$out = System::readv($fd, [
    ['len' => 4],
    ['len' => 8],
]);

if ($out !== false) {
    // $out['res'] — total bytes read
    // $out['buffers'][0], $out['buffers'][1] — segment contents
}

System::close($fd);
```

---

### `System::fcntl(int $fd, int $command, mixed $arguments = null): array`

Invokes `fcntl(2)` on the descriptor. Always returns an associative array:

| Key   | Meaning |
|-------|---------|
| `res` | Return value from `fcntl` (non-negative on success, `-1` on failure). |
| `val` | Command-specific value: flag integer for getters like `F_GETFL`, `false` on error, `flock` array for lock commands, or echo of the argument for setters. |

`$arguments` may be omitted, an integer, a boolean, or (for `F_GETLK` / `F_SETLK` / `F_SETLKW`) an array with keys: `type`, `whence`, `start`, `len`, `pid`.

**Example — read open-file status flags (`F_GETFL`)**

```php
<?php

use Posi\System;

$fd = System::open('/tmp/posi-demo.txt', 0);

$result = System::fcntl($fd, F_GETFL);

// $result['res'] — fcntl status
// $result['val'] — current O_* flags as integer
$flags = $result['val'];

System::close($fd);
```

**Example — enable non-blocking I/O (`F_SETFL`)**

```php
<?php

use Posi\System;

$fd = System::open('/tmp/posi-demo.txt', 0);

$get = System::fcntl($fd, F_GETFL);
$newFlags = $get['val'] | O_NONBLOCK;

System::fcntl($fd, F_SETFL, $newFlags);
System::close($fd);
```

**Example — advisory lock (`F_SETLK`)**

```php
<?php

use Posi\System;

$fd = System::open('/tmp/posi-demo.txt', 2);

$result = System::fcntl($fd, F_SETLK, [
    'type'   => F_WRLCK,
    'whence' => SEEK_SET,
    'start'  => 0,
    'len'    => 0,
    'pid'    => 0,
]);

// $result['val'] contains flock fields after the operation
System::close($fd);
```

---

### `System::ioctl(int $fd, int $command, mixed $arguments = null): array`

Invokes `ioctl(2)` on the descriptor. Returns the same shape as `fcntl`:

| Key   | Meaning |
|-------|---------|
| `res` | Return value from `ioctl`. |
| `val` | Output value: integer, string buffer, `false` on failure, or argument echo depending on command and `$arguments`. |

`$arguments` may be omitted (`null`), an integer pointer value, a string buffer, or an array:

* `['value' => int]` — read/write a single integer through the ioctl (value updated in `val`).
* `['bytes' => string]` or `['data' => string]` — binary buffer in/out.

**Example — ioctl with no argument (device-specific command)**

```php
<?php

use Posi\System;

$fd = System::open('/dev/null', O_RDWR);

// Replace SOME_IOCTL_CMD with your driver's command constant.
$result = System::ioctl($fd, SOME_IOCTL_CMD);

// $result['res'], $result['val']
System::close($fd);
```

**Example — pass an integer argument**

```php
<?php

use Posi\System;

$fd = System::open('/dev/some-device', O_RDWR);

$result = System::ioctl($fd, SOME_IOCTL_CMD, 0);

// On success, $result['val'] may reflect the argument or driver output.
System::close($fd);
```

**Example — integer in/out via array**

```php
<?php

use Posi\System;

$fd = System::open('/dev/some-device', O_RDWR);

$result = System::ioctl($fd, SOME_IOCTL_CMD, ['value' => 1]);

// $result['val'] holds the updated integer after ioctl
$out = $result['val'];

System::close($fd);
```

**Example — binary buffer in/out**

```php
<?php

use Posi\System;

$fd = System::open('/dev/some-device', O_RDWR);

$buffer = str_repeat("\0", 32);

$result = System::ioctl($fd, SOME_IOCTL_CMD, ['data' => $buffer]);

// $result['val'] is the buffer returned from the driver
$response = $result['val'];

System::close($fd);
```

---

### `System::wait(?int &$status = null): int`

Blocks until any child process exits (implemented with `waitpid(-1, …, 0)`, same idea as `wait(2)`). Returns the child PID on success, or `-1` on error (for example `ECHILD` when there are no children). If you pass `$status` **by reference**, it receives the raw wait status integer for use with `pcntl_wifexited()`, `pcntl_wexitstatus()`, and related helpers (or your own bit tests). If the call fails, `$status` is not updated.

**Example**

```php
<?php

use Posi\System;

$pid = pcntl_fork();
if ($pid === 0) {
    exit(42);
}

$status = 0;
$got = System::wait($status);
// decode with pcntl_wexitstatus($status) when pcntl_wifexited($status)
```

---

### `System::waitpid(int $pid, ?int &$status = null, int $options = 0): int`

Wraps `waitpid(2)`. Use `$pid = -1` to wait for any child (same as `wait()`). `$options` is typically `0` (block) or `WNOHANG` (do not block). Returns the child PID on success when a child was reaped, `0` when `$options` includes `WNOHANG` and no child was ready, or `-1` on error. The raw status word is written to `$status` **by reference** only when the return value is greater than zero (a child was reaped).

**Example**

```php
<?php

use Posi\System;

$status = 0;
$pid = System::waitpid(-1, $status, WNOHANG);
```

---

### `System::hostname(): string|false`

Returns the current host name via `gethostname(2)` (same underlying call as PHP’s `gethostname()`). Returns `false` if the syscall fails.

**Example**

```php
<?php

use Posi\System;

$host = System::hostname();
```

---

## Quick reference

| Method | Signature |
|--------|-----------|
| `open` | `System::open(string $filepath, int $flags, int $mode = 0644): int` |
| `chmod` | `System::chmod(string $path, int $mode): int` |
| `chown` | `System::chown(string $path, int $owner, int $group): int` |
| `fchmod` | `System::fchmod(int $fd, int $mode): int` |
| `fchown` | `System::fchown(int $fd, int $owner, int $group): int` |
| `close` | `System::close(int $fd): int` |
| `write` | `System::write(int $fd, string $glob_of_bytes, int $num_bytes_to_write): int` |
| `read` | `System::read(int $fd, int $amt_of_bytes_to_read): string` |
| `getuid` | `System::getuid(): int` |
| `setuid` | `System::setuid(int $uid): int` |
| `umask` | `System::umask(int $mask): int` |
| `lseek` | `System::lseek(int $fd, int $offset, int $whence): int` |
| `recv` | `System::recv(int $fd, int $len, int $flags = 0): string\|false` |
| `readv` | `System::readv(int $fd, array $iovecs): array\|false` |
| `fcntl` | `System::fcntl(int $fd, int $command, mixed $arguments = null): array` |
| `ioctl` | `System::ioctl(int $fd, int $command, mixed $arguments = null): array` |
| `wait` | `System::wait(?int &$status = null): int` |
| `waitpid` | `System::waitpid(int $pid, ?int &$status = null, int $options = 0): int` |
| `hostname` | `System::hostname(): string\|false` |
