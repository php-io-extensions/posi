# ext-posi

[![Latest Version on Packagist](https://img.shields.io/packagist/v/php-io-extensions/posi.svg)](https://packagist.org/packages/php-io-extensions/posi)
[![License](https://img.shields.io/packagist/l/php-io-extensions/posi.svg)](LICENSE)

POSIX file descriptors, `ioctl()`, `fcntl()`, termios and process status for PHP, written in C against the Zend API.

PHP's streams hide the descriptor, and `ext/posix` stops short of `read(2)`, `ioctl(2)` and termios. Talking to hardware from PHP needs exactly those calls: an `i2c-dev` node takes its slave address through `ioctl()`, a serial port is configured through `tcsetattr()`, and `spidev` and `I2C_RDWR` take structs that point at other buffers. ext-posi binds those calls one to one, with the platform's own constant values, so code written against the C man pages works unchanged in PHP.

```
ext-posi                        POSIX calls, 1:1                       ← this package
  → microscrap/gpio, i2c, spi, uart    libgpiod v2, i2c-dev, spidev, termios in PHP
    → microscrap/scrapyard-linux       the `native` driver
      → scrapyard-io/framework         protocol managers, transports, circuits
```

## Requirements

- PHP 8.4 or newer, NTS or ZTS
- Linux or macOS
- `ext-posix`: `posix_getuid()` and `posix_setuid()` come from it under the same names, so posi requires it and loads after it

## Installation

With [PIE](https://github.com/php/pie):

```bash
pie install php-io-extensions/posi
```

From a checkout, with the bundled installers. Each one builds in a disposable copy, installs `posi.so` into the PHP's `extension_dir`, writes `30-posi.ini` into its conf.d directory, and checks that the extension loads:

```bash
./install-macos.sh                     # Homebrew php@8.4 and php@8.4-zts
./install-debian-trixie.sh             # the php on PATH: Debian trixie, Raspberry Pi OS, Ubuntu 24.04+
./install-macos.sh /path/to/bin/php    # either script takes specific PHP binaries
```

By hand:

```bash
phpize && ./configure --enable-posi && make && make install
echo 'extension=posi' > "$(php -r 'echo PHP_CONFIG_FILE_SCAN_DIR;')/30-posi.ini"
```

## Usage

### Read a register over i2c-dev

`I2C_SLAVE` takes the address itself as the third `ioctl()` argument, so an int goes straight through:

```php
const I2C_SLAVE = 0x0703;

$fd = posix_open('/dev/i2c-1', O_RDWR);
if ($fd < 0) {
    throw new RuntimeException('open /dev/i2c-1: errno ' . posi_errno());
}

ioctl($fd, I2C_SLAVE, 0x53, $unused);
posix_write($fd, "\x00", 1);                       // register 0x00
printf("DEVID 0x%02X\n", ord(posix_read($fd, 1)));  // 0xE5 on an ADXL343
posix_close($fd);
```

### Pass structs that point at other buffers

`I2C_RDWR` and `SPI_IOC_MESSAGE` take a struct holding addresses of further buffers. `posi_mem_alloc()` returns such an address as an int, which `pack('P', …)` places into the struct. The kernel reads and writes those buffers during the call, and `posi_mem_read()` reads the result back:

```php
const I2C_RDWR = 0x0707;
const I2C_M_RD = 0x0001;

$fd = posix_open('/dev/i2c-1', O_RDWR);

$register = posi_mem_alloc(1);
$reply = posi_mem_alloc(1);
posi_mem_write($register, "\x00");

// struct i2c_msg { u16 addr; u16 flags; u16 len; u8 *buf; }: 16 bytes on 64-bit Linux
$messages = posi_mem_alloc(32);
posi_mem_write($messages, pack('vvvxxP', 0x53, 0, 1, $register) . pack('vvvxxP', 0x53, I2C_M_RD, 1, $reply));

// struct i2c_rdwr_ioctl_data { struct i2c_msg *msgs; u32 nmsgs; }
if (ioctl($fd, I2C_RDWR, ['bytes' => pack('PVx4', $messages, 2)], $unused) < 0) {
    throw new RuntimeException('I2C_RDWR: errno ' . posi_errno());
}
printf("DEVID 0x%02X\n", ord(posi_mem_read($reply, 1)));

posi_mem_free($messages);
posi_mem_free($reply);
posi_mem_free($register);
posix_close($fd);
```

### Configure a serial port

```php
$fd = posix_open('/dev/ttyAMA0', O_RDWR | O_NOCTTY | O_NONBLOCK);

$termios = tcgetattr($fd);
$termios['c_iflag'] &= ~(IXON | IXOFF | ICRNL);
$termios['c_oflag'] &= ~OPOST;
$termios['c_lflag'] &= ~(ICANON | constant('ECHO') | ISIG | IEXTEN);
$termios['c_cflag'] = ($termios['c_cflag'] & ~(CSIZE | PARENB | CSTOPB)) | CS8 | CLOCAL | CREAD;
$termios = cfsetispeed($termios, B9600);
$termios = cfsetospeed($termios, B9600);
tcsetattr($fd, TCSANOW, $termios);

if (posix_ppoll($fd, 2_000_000_000, POLLIN) === 1) {   // wait up to 2 s
    $bytes = posix_read($fd, 256);
}
posix_close($fd);
```

`ECHO` is a PHP keyword, so the constant is read with `constant('ECHO')`. `posi_set_baud_rate($fd, $baud)` sets both directions at once. A rate with no `B*` constant, such as 250000, goes through Linux's `termios2` with `BOTHER`; on other systems such a rate returns -1 with `posi_errno()` set to `EINVAL`.

### Check errno

A failed call returns what C returns, -1 or false, and keeps `errno` for `posi_errno()`:

```php
if (posix_open('/nonexistent', O_RDONLY) < 0 && posi_errno() === ENOENT) {
    // no such file
}
```

## Functions

| Function | C call | Returns |
|---|---|---|
| `posix_open(string $device_path, int $flags = O_RDWR, int $mode = 0644)` | `open(2)` | fd, or -1 |
| `posix_close(int $fd)` | `close(2)` | 0, or -1 |
| `posix_read(int $fd, int $bytes_to_read)` | `read(2)` | the bytes read, or false |
| `posix_write(int $fd, string $data, int $bytes_to_write)` | `write(2)` | bytes written, or -1 |
| `posix_lseek(int $fd, int $offset, int $whence)` | `lseek(2)` | new offset, or -1 |
| `posix_readv(int $fd, array $iovecs)` | `readv(2)` | `['res' => n, 'buffers' => [...]]`, or false |
| `posix_recv(int $fd, int $len, int $flags = 0)` | `recv(2)` | the bytes received, or false |
| `fcntl(int $fd, int $cmd, mixed $arg, mixed &$value)` | `fcntl(2)` | the C return value |
| `ioctl(int $fd, int $request, mixed $arg, mixed &$value)` | `ioctl(2)` | the C return value |
| `posix_ppoll(int $fd, int $timeout_ns = 0, int $events = 0)` | `ppoll(2)` on Linux, `poll(2)` on macOS | 1 ready, 0 timed out, -1 |
| `posix_fdopen(int $fd, string $mode)` | `fdopen(3)` | stream resource, or false |
| `posix_chmod`, `posix_fchmod`, `posix_chown`, `posix_fchown` | `chmod(2)` family | 0, or -1 |
| `posix_umask(int $mask)` | `umask(2)` | the previous mask |
| `posix_lstat(string $path)` | `lstat(2)` | stat array, or false |
| `posix_hostname()` | `gethostname(3)` | the name, or false |
| `posix_wait(?int &$status = null)` | `wait(2)` | pid, or -1 |
| `posix_waitpid(int $pid, ?int &$status = null, int $options = 0)` | `waitpid(2)` | pid, 0 under `WNOHANG`, or -1 |
| `posix_wifexited`, `posix_wexitstatus`, `posix_wifsignaled`, `posix_wtermsig`, `posix_wifstopped`, `posix_wstopsig`, `posix_wifcontinued` | `<sys/wait.h>` status macros | bool or int |
| `tcgetattr(int $fd)` | `tcgetattr(3)` | termios array, or false |
| `tcsetattr(int $fd, int $action, array $termios)` | `tcsetattr(3)` | 0, or -1 |
| `cfsetispeed(array $termios, int $speed)`, `cfsetospeed(...)` | `cfset*speed(3)` | the termios array with the speed applied, or false |
| `cfgetispeed(array $termios)`, `cfgetospeed(...)` | `cfget*speed(3)` | the speed |
| `tcdrain(int $fd)`, `tcflush(int $fd, int $queue)`, `tcflow(int $fd, int $action)` | `tc*(3)` | 0, or -1 |
| `posi_set_baud_rate(int $fd, int $baud)` | the matching `B*` speed, else Linux `termios2` with `BOTHER` | 0, or -1 |
| `posi_mem_alloc(int $size)` | zeroed request memory | the address |
| `posi_mem_free(int $ptr)` | releases it | — |
| `posi_mem_write(int $ptr, string $data, int $offset = 0)` | `memcpy` in | — |
| `posi_mem_read(int $ptr, int $size, int $offset = 0)` | `memcpy` out | the bytes |
| `posi_errno()` | `errno` | errno of the last posi call that failed |

## Behaviour

- **Failure** is C's: -1 or false. `errno` is saved for `posi_errno()`; compare it with `ENOENT`, `EAGAIN` and the other errno constants.
- **Arguments** are checked against the C type. An int outside the C type's range, a negative length, or a length past the end of the data throws `ValueError`. Nothing is truncated silently.
- **`fcntl()`** `$arg` is an int, a bool or null, or for `F_GETLK`, `F_SETLK` and `F_SETLKW` a `['type', 'whence', 'start', 'len', 'pid']` array, where missing keys count as 0. `$value` receives the datum for `F_GETFD`, `F_GETFL` and `F_GETOWN`, the resulting lock array for lock commands, and `$arg` otherwise. On failure it receives false.
- **`ioctl()`** `$arg` chooses how the third C argument is passed:
  - `['value' => int]`: a pointer to an int, read back into `$value` after the call.
  - `['bytes' => string]`, `['data' => string]` or a plain string: a pointer to a copy of the bytes, read back into `$value` after the call.
  - an int or bool: passed as the pointer-sized argument itself, such as a slave address or a `posi_mem_alloc()` address.
  - null: passes NULL. `$value` receives a positive return value, else null.
- **`posix_ppoll()`** polls one descriptor. `$events` 0 means `POLLIN`, and a negative timeout waits forever. On macOS, which has no `ppoll()`, the timeout is rounded up to whole milliseconds.
- **`posix_fdopen()`** wraps the descriptor without duplicating it, so `fclose()` closes the descriptor.
- **Termios arrays** carry `c_iflag`, `c_oflag`, `c_cflag`, `c_lflag`, `c_cc` (every index below `NCCS`), `c_ispeed` and `c_ospeed`, and every function that takes one requires all of them. The speeds are included because macOS keeps them outside `c_cflag`. A Linux custom rate set with `BOTHER` survives a `tcgetattr()` → `tcsetattr()` round trip.
- **Native buffers**: every `posi_mem_*` address is checked against the live allocations. A foreign or freed address, a second free, or a range past the end throws `ValueError` before any memory is touched. `posi_mem_free(0)` does nothing, like `free(NULL)`. Buffers still allocated when the request ends are freed then.

## Constants

Each constant takes its value from the platform's own header, and is registered only where the platform defines it:

| Header | Constants |
|---|---|
| `<fcntl.h>` | `F_*`, `FD_CLOEXEC`, `O_*`, plus the Linux-only `O_DIRECT`, `O_NOATIME`, `O_PATH`, `O_TMPFILE` |
| `<poll.h>` | `POLL*` |
| `<sys/stat.h>` | `S_IF*` and the mode bits |
| `<termios.h>` | `TCSA*`, `TC*FLUSH`, `TCO*`/`TCI*`, the `V*` `c_cc` indices and `NCCS`, the flag bits, every `B*` speed |
| `<sys/socket.h>`, `<sys/wait.h>` | `MSG_OOB`, `MSG_PEEK`, `MSG_WAITALL`, `WNOHANG`, `WUNTRACED`, `WCONTINUED` |
| `<errno.h>` | the POSIX errno set |

ext/sockets, ext/pcntl and ext-kqueue register some of the same names with the same values. posi loads after them and registers only the names still missing, so any combination loads without a warning.

## Upgrading from 0.9

0.10 is a rewrite in C that replaces the Zephir posi 0.9 and absorbs `microscrap/posix`. Remove `microscrap/posix` from your `composer.json`: its functions now come from the extension.

| 0.9 | 0.10 |
|---|---|
| `Posi\System::read($fd, $n)` and the other `System` statics | `posix_read($fd, $n)` and the other `microscrap/posix` names |
| `Posi\System::fcntl(...)['res']` / `['val']`, same for `ioctl` | `fcntl($fd, $cmd, $arg, $value)` returns the result and fills `$value` |
| `Posi\Termios::tcgetattr($fd)` and the other `Termios` statics | `tcgetattr($fd)` and the other POSIX names |
| `Posi\Termios::setBaudRate($fd, $baud)` | `posi_set_baud_rate($fd, $baud)` |
| `Posi\Memory::alloc/free/read/write` | `posi_mem_alloc/free/read/write` |
| `FileControlFlag::O_RDWR->value`, `FcntlCommand::F_GETFL`, `PollEvent::POLLIN` | `O_RDWR`, `F_GETFL`, `POLLIN` |

The 0.9 enums carried x86 Linux numbers. The 0.10 constants are correct on every platform: `O_NONBLOCK` is 4 on macOS, and `O_DIRECT` is 0x10000 on arm64 Linux.

## Testing

```bash
composer install
php -d extension=/path/to/modules/posi.so vendor/bin/pest
```

The suite needs no hardware and runs on Linux and macOS. Termios tests run against a pseudo-terminal.

## Security

posi gives PHP code the same access to devices and memory as C code in the same process. See [SECURITY.md](SECURITY.md) for what that means and how to report a vulnerability.

## License

MIT. See [LICENSE](LICENSE).
