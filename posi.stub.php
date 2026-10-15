<?php

/**
 * @generate-class-entries
 */

/* <unistd.h>, <fcntl.h>, <sys/stat.h>: descriptors and paths */

function posix_open(string $device_path, int $flags = O_RDWR, int $mode = 0644): int {}

function posix_close(int $fd): int {}

function posix_read(int $fd, int $bytes_to_read): string|false {}

function posix_write(int $fd, string $data, int $bytes_to_write): int {}

function posix_lseek(int $fd, int $offset, int $whence): int {}

/** @param array $iovecs each element ["len" => int] */
function posix_readv(int $fd, array $iovecs): array|false {}

function posix_recv(int $fd, int $len, int $flags = 0): string|false {}

function fcntl(int $fd, int $cmd, mixed $arg, mixed &$value): int {}

function ioctl(int $fd, int $request, mixed $arg, mixed &$value): int {}

function posix_ppoll(int $fd, int $timeout_ns = 0, int $events = 0): int {}

/** @return resource|false */
function posix_fdopen(int $fd, string $mode) {}

/** @param resource $stream */
function posix_fileno($stream): int|false {}

function posix_dup(int $fd): int {}

/** @return resource|false */
function posix_socket_fdopen(int $fd) {}

function posix_chmod(string $path, int $mode): int {}

function posix_chown(string $path, int $owner, int $group): int {}

function posix_fchmod(int $fd, int $mode): int {}

function posix_fchown(int $fd, int $owner, int $group): int {}

function posix_umask(int $mask): int {}

function posix_lstat(string $path): array|false {}

function posix_hostname(): string|false {}

/* <sys/wait.h> */

function posix_wait(?int &$status = null): int {}

function posix_waitpid(int $pid, ?int &$status = null, int $options = 0): int {}

function posix_wifexited(int $status): bool {}

function posix_wexitstatus(int $status): int {}

function posix_wifsignaled(int $status): bool {}

function posix_wtermsig(int $status): int {}

function posix_wifstopped(int $status): bool {}

function posix_wstopsig(int $status): int {}

#ifdef WIFCONTINUED
function posix_wifcontinued(int $status): bool {}
#endif

/* <termios.h> */

function tcgetattr(int $fd): array|false {}

function tcsetattr(int $fd, int $action, array $termios): int {}

function cfsetispeed(array $termios, int $speed): array|false {}

function cfsetospeed(array $termios, int $speed): array|false {}

function cfgetispeed(array $termios): int {}

function cfgetospeed(array $termios): int {}

function tcdrain(int $fd): int {}

function tcflush(int $fd, int $queue): int {}

function tcflow(int $fd, int $action): int {}

function posi_set_baud_rate(int $fd, int $baud): int {}

/* Native buffers for ioctl() structs that point at other memory */

function posi_mem_alloc(int $size): int {}

function posi_mem_free(int $ptr): void {}

function posi_mem_write(int $ptr, string $data, int $offset = 0): void {}

function posi_mem_read(int $ptr, int $size, int $offset = 0): string {}

/* errno of the last posi call that failed */

function posi_errno(): int {}

/* <fcntl.h>: fcntl() commands, descriptor flags, lock types */

#ifdef F_DUPFD
/**
 * @var int
 * @cvalue F_DUPFD
 */
const F_DUPFD = UNKNOWN;
#endif

#ifdef F_DUPFD_CLOEXEC
/**
 * @var int
 * @cvalue F_DUPFD_CLOEXEC
 */
const F_DUPFD_CLOEXEC = UNKNOWN;
#endif

#ifdef F_GETFD
/**
 * @var int
 * @cvalue F_GETFD
 */
const F_GETFD = UNKNOWN;
#endif

#ifdef F_SETFD
/**
 * @var int
 * @cvalue F_SETFD
 */
const F_SETFD = UNKNOWN;
#endif

#ifdef F_GETFL
/**
 * @var int
 * @cvalue F_GETFL
 */
const F_GETFL = UNKNOWN;
#endif

#ifdef F_SETFL
/**
 * @var int
 * @cvalue F_SETFL
 */
const F_SETFL = UNKNOWN;
#endif

#ifdef F_GETLK
/**
 * @var int
 * @cvalue F_GETLK
 */
const F_GETLK = UNKNOWN;
#endif

#ifdef F_SETLK
/**
 * @var int
 * @cvalue F_SETLK
 */
const F_SETLK = UNKNOWN;
#endif

#ifdef F_SETLKW
/**
 * @var int
 * @cvalue F_SETLKW
 */
const F_SETLKW = UNKNOWN;
#endif

#ifdef F_GETOWN
/**
 * @var int
 * @cvalue F_GETOWN
 */
const F_GETOWN = UNKNOWN;
#endif

#ifdef F_SETOWN
/**
 * @var int
 * @cvalue F_SETOWN
 */
const F_SETOWN = UNKNOWN;
#endif

#ifdef FD_CLOEXEC
/**
 * @var int
 * @cvalue FD_CLOEXEC
 */
const FD_CLOEXEC = UNKNOWN;
#endif

#ifdef F_RDLCK
/**
 * @var int
 * @cvalue F_RDLCK
 */
const F_RDLCK = UNKNOWN;
#endif

#ifdef F_UNLCK
/**
 * @var int
 * @cvalue F_UNLCK
 */
const F_UNLCK = UNKNOWN;
#endif

#ifdef F_WRLCK
/**
 * @var int
 * @cvalue F_WRLCK
 */
const F_WRLCK = UNKNOWN;
#endif

/* <fcntl.h>: open() flags */

#ifdef O_ACCMODE
/**
 * @var int
 * @cvalue O_ACCMODE
 */
const O_ACCMODE = UNKNOWN;
#endif

#ifdef O_RDONLY
/**
 * @var int
 * @cvalue O_RDONLY
 */
const O_RDONLY = UNKNOWN;
#endif

#ifdef O_WRONLY
/**
 * @var int
 * @cvalue O_WRONLY
 */
const O_WRONLY = UNKNOWN;
#endif

#ifdef O_RDWR
/**
 * @var int
 * @cvalue O_RDWR
 */
const O_RDWR = UNKNOWN;
#endif

#ifdef O_APPEND
/**
 * @var int
 * @cvalue O_APPEND
 */
const O_APPEND = UNKNOWN;
#endif

#ifdef O_CLOEXEC
/**
 * @var int
 * @cvalue O_CLOEXEC
 */
const O_CLOEXEC = UNKNOWN;
#endif

#ifdef O_CREAT
/**
 * @var int
 * @cvalue O_CREAT
 */
const O_CREAT = UNKNOWN;
#endif

#ifdef O_DIRECTORY
/**
 * @var int
 * @cvalue O_DIRECTORY
 */
const O_DIRECTORY = UNKNOWN;
#endif

#ifdef O_DSYNC
/**
 * @var int
 * @cvalue O_DSYNC
 */
const O_DSYNC = UNKNOWN;
#endif

#ifdef O_EXCL
/**
 * @var int
 * @cvalue O_EXCL
 */
const O_EXCL = UNKNOWN;
#endif

#ifdef O_NOCTTY
/**
 * @var int
 * @cvalue O_NOCTTY
 */
const O_NOCTTY = UNKNOWN;
#endif

#ifdef O_NOFOLLOW
/**
 * @var int
 * @cvalue O_NOFOLLOW
 */
const O_NOFOLLOW = UNKNOWN;
#endif

#ifdef O_NONBLOCK
/**
 * @var int
 * @cvalue O_NONBLOCK
 */
const O_NONBLOCK = UNKNOWN;
#endif

#ifdef O_RSYNC
/**
 * @var int
 * @cvalue O_RSYNC
 */
const O_RSYNC = UNKNOWN;
#endif

#ifdef O_SYNC
/**
 * @var int
 * @cvalue O_SYNC
 */
const O_SYNC = UNKNOWN;
#endif

#ifdef O_TRUNC
/**
 * @var int
 * @cvalue O_TRUNC
 */
const O_TRUNC = UNKNOWN;
#endif

#ifdef O_TTY_INIT
/**
 * @var int
 * @cvalue O_TTY_INIT
 */
const O_TTY_INIT = UNKNOWN;
#endif

#ifdef O_EXEC
/**
 * @var int
 * @cvalue O_EXEC
 */
const O_EXEC = UNKNOWN;
#endif

#ifdef O_SEARCH
/**
 * @var int
 * @cvalue O_SEARCH
 */
const O_SEARCH = UNKNOWN;
#endif

/* <fcntl.h>: open() flags outside POSIX that this platform defines */

#ifdef O_ASYNC
/**
 * @var int
 * @cvalue O_ASYNC
 */
const O_ASYNC = UNKNOWN;
#endif

#ifdef O_DIRECT
/**
 * @var int
 * @cvalue O_DIRECT
 */
const O_DIRECT = UNKNOWN;
#endif

#ifdef O_NOATIME
/**
 * @var int
 * @cvalue O_NOATIME
 */
const O_NOATIME = UNKNOWN;
#endif

#ifdef O_PATH
/**
 * @var int
 * @cvalue O_PATH
 */
const O_PATH = UNKNOWN;
#endif

#ifdef O_TMPFILE
/**
 * @var int
 * @cvalue O_TMPFILE
 */
const O_TMPFILE = UNKNOWN;
#endif

/* <poll.h>: posix_ppoll() events */

#ifdef POLLIN
/**
 * @var int
 * @cvalue POLLIN
 */
const POLLIN = UNKNOWN;
#endif

#ifdef POLLRDNORM
/**
 * @var int
 * @cvalue POLLRDNORM
 */
const POLLRDNORM = UNKNOWN;
#endif

#ifdef POLLRDBAND
/**
 * @var int
 * @cvalue POLLRDBAND
 */
const POLLRDBAND = UNKNOWN;
#endif

#ifdef POLLPRI
/**
 * @var int
 * @cvalue POLLPRI
 */
const POLLPRI = UNKNOWN;
#endif

#ifdef POLLOUT
/**
 * @var int
 * @cvalue POLLOUT
 */
const POLLOUT = UNKNOWN;
#endif

#ifdef POLLWRNORM
/**
 * @var int
 * @cvalue POLLWRNORM
 */
const POLLWRNORM = UNKNOWN;
#endif

#ifdef POLLWRBAND
/**
 * @var int
 * @cvalue POLLWRBAND
 */
const POLLWRBAND = UNKNOWN;
#endif

#ifdef POLLERR
/**
 * @var int
 * @cvalue POLLERR
 */
const POLLERR = UNKNOWN;
#endif

#ifdef POLLHUP
/**
 * @var int
 * @cvalue POLLHUP
 */
const POLLHUP = UNKNOWN;
#endif

#ifdef POLLNVAL
/**
 * @var int
 * @cvalue POLLNVAL
 */
const POLLNVAL = UNKNOWN;
#endif

/* <sys/stat.h>: file type and mode bits, for posix_lstat() and the chmod family */

#ifdef S_IFMT
/**
 * @var int
 * @cvalue S_IFMT
 */
const S_IFMT = UNKNOWN;
#endif

#ifdef S_IFBLK
/**
 * @var int
 * @cvalue S_IFBLK
 */
const S_IFBLK = UNKNOWN;
#endif

#ifdef S_IFCHR
/**
 * @var int
 * @cvalue S_IFCHR
 */
const S_IFCHR = UNKNOWN;
#endif

#ifdef S_IFIFO
/**
 * @var int
 * @cvalue S_IFIFO
 */
const S_IFIFO = UNKNOWN;
#endif

#ifdef S_IFREG
/**
 * @var int
 * @cvalue S_IFREG
 */
const S_IFREG = UNKNOWN;
#endif

#ifdef S_IFDIR
/**
 * @var int
 * @cvalue S_IFDIR
 */
const S_IFDIR = UNKNOWN;
#endif

#ifdef S_IFLNK
/**
 * @var int
 * @cvalue S_IFLNK
 */
const S_IFLNK = UNKNOWN;
#endif

#ifdef S_IFSOCK
/**
 * @var int
 * @cvalue S_IFSOCK
 */
const S_IFSOCK = UNKNOWN;
#endif

#ifdef S_IRWXU
/**
 * @var int
 * @cvalue S_IRWXU
 */
const S_IRWXU = UNKNOWN;
#endif

#ifdef S_IRUSR
/**
 * @var int
 * @cvalue S_IRUSR
 */
const S_IRUSR = UNKNOWN;
#endif

#ifdef S_IWUSR
/**
 * @var int
 * @cvalue S_IWUSR
 */
const S_IWUSR = UNKNOWN;
#endif

#ifdef S_IXUSR
/**
 * @var int
 * @cvalue S_IXUSR
 */
const S_IXUSR = UNKNOWN;
#endif

#ifdef S_IRWXG
/**
 * @var int
 * @cvalue S_IRWXG
 */
const S_IRWXG = UNKNOWN;
#endif

#ifdef S_IRGRP
/**
 * @var int
 * @cvalue S_IRGRP
 */
const S_IRGRP = UNKNOWN;
#endif

#ifdef S_IWGRP
/**
 * @var int
 * @cvalue S_IWGRP
 */
const S_IWGRP = UNKNOWN;
#endif

#ifdef S_IXGRP
/**
 * @var int
 * @cvalue S_IXGRP
 */
const S_IXGRP = UNKNOWN;
#endif

#ifdef S_IRWXO
/**
 * @var int
 * @cvalue S_IRWXO
 */
const S_IRWXO = UNKNOWN;
#endif

#ifdef S_IROTH
/**
 * @var int
 * @cvalue S_IROTH
 */
const S_IROTH = UNKNOWN;
#endif

#ifdef S_IWOTH
/**
 * @var int
 * @cvalue S_IWOTH
 */
const S_IWOTH = UNKNOWN;
#endif

#ifdef S_IXOTH
/**
 * @var int
 * @cvalue S_IXOTH
 */
const S_IXOTH = UNKNOWN;
#endif

#ifdef S_ISUID
/**
 * @var int
 * @cvalue S_ISUID
 */
const S_ISUID = UNKNOWN;
#endif

#ifdef S_ISGID
/**
 * @var int
 * @cvalue S_ISGID
 */
const S_ISGID = UNKNOWN;
#endif

#ifdef S_ISVTX
/**
 * @var int
 * @cvalue S_ISVTX
 */
const S_ISVTX = UNKNOWN;
#endif

/* <termios.h>: tcsetattr() actions, tcflush() queues, tcflow() actions */

#ifdef TCSANOW
/**
 * @var int
 * @cvalue TCSANOW
 */
const TCSANOW = UNKNOWN;
#endif

#ifdef TCSADRAIN
/**
 * @var int
 * @cvalue TCSADRAIN
 */
const TCSADRAIN = UNKNOWN;
#endif

#ifdef TCSAFLUSH
/**
 * @var int
 * @cvalue TCSAFLUSH
 */
const TCSAFLUSH = UNKNOWN;
#endif

#ifdef TCIFLUSH
/**
 * @var int
 * @cvalue TCIFLUSH
 */
const TCIFLUSH = UNKNOWN;
#endif

#ifdef TCOFLUSH
/**
 * @var int
 * @cvalue TCOFLUSH
 */
const TCOFLUSH = UNKNOWN;
#endif

#ifdef TCIOFLUSH
/**
 * @var int
 * @cvalue TCIOFLUSH
 */
const TCIOFLUSH = UNKNOWN;
#endif

#ifdef TCOOFF
/**
 * @var int
 * @cvalue TCOOFF
 */
const TCOOFF = UNKNOWN;
#endif

#ifdef TCOON
/**
 * @var int
 * @cvalue TCOON
 */
const TCOON = UNKNOWN;
#endif

#ifdef TCIOFF
/**
 * @var int
 * @cvalue TCIOFF
 */
const TCIOFF = UNKNOWN;
#endif

#ifdef TCION
/**
 * @var int
 * @cvalue TCION
 */
const TCION = UNKNOWN;
#endif

/* <termios.h>: c_cc indices */

#ifdef NCCS
/**
 * @var int
 * @cvalue NCCS
 */
const NCCS = UNKNOWN;
#endif

#ifdef VEOF
/**
 * @var int
 * @cvalue VEOF
 */
const VEOF = UNKNOWN;
#endif

#ifdef VEOL
/**
 * @var int
 * @cvalue VEOL
 */
const VEOL = UNKNOWN;
#endif

#ifdef VERASE
/**
 * @var int
 * @cvalue VERASE
 */
const VERASE = UNKNOWN;
#endif

#ifdef VINTR
/**
 * @var int
 * @cvalue VINTR
 */
const VINTR = UNKNOWN;
#endif

#ifdef VKILL
/**
 * @var int
 * @cvalue VKILL
 */
const VKILL = UNKNOWN;
#endif

#ifdef VMIN
/**
 * @var int
 * @cvalue VMIN
 */
const VMIN = UNKNOWN;
#endif

#ifdef VQUIT
/**
 * @var int
 * @cvalue VQUIT
 */
const VQUIT = UNKNOWN;
#endif

#ifdef VSTART
/**
 * @var int
 * @cvalue VSTART
 */
const VSTART = UNKNOWN;
#endif

#ifdef VSTOP
/**
 * @var int
 * @cvalue VSTOP
 */
const VSTOP = UNKNOWN;
#endif

#ifdef VSUSP
/**
 * @var int
 * @cvalue VSUSP
 */
const VSUSP = UNKNOWN;
#endif

#ifdef VTIME
/**
 * @var int
 * @cvalue VTIME
 */
const VTIME = UNKNOWN;
#endif

/* <termios.h>: c_iflag bits */

#ifdef BRKINT
/**
 * @var int
 * @cvalue BRKINT
 */
const BRKINT = UNKNOWN;
#endif

#ifdef ICRNL
/**
 * @var int
 * @cvalue ICRNL
 */
const ICRNL = UNKNOWN;
#endif

#ifdef IGNBRK
/**
 * @var int
 * @cvalue IGNBRK
 */
const IGNBRK = UNKNOWN;
#endif

#ifdef IGNCR
/**
 * @var int
 * @cvalue IGNCR
 */
const IGNCR = UNKNOWN;
#endif

#ifdef IGNPAR
/**
 * @var int
 * @cvalue IGNPAR
 */
const IGNPAR = UNKNOWN;
#endif

#ifdef INLCR
/**
 * @var int
 * @cvalue INLCR
 */
const INLCR = UNKNOWN;
#endif

#ifdef INPCK
/**
 * @var int
 * @cvalue INPCK
 */
const INPCK = UNKNOWN;
#endif

#ifdef ISTRIP
/**
 * @var int
 * @cvalue ISTRIP
 */
const ISTRIP = UNKNOWN;
#endif

#ifdef IXANY
/**
 * @var int
 * @cvalue IXANY
 */
const IXANY = UNKNOWN;
#endif

#ifdef IXOFF
/**
 * @var int
 * @cvalue IXOFF
 */
const IXOFF = UNKNOWN;
#endif

#ifdef IXON
/**
 * @var int
 * @cvalue IXON
 */
const IXON = UNKNOWN;
#endif

#ifdef PARMRK
/**
 * @var int
 * @cvalue PARMRK
 */
const PARMRK = UNKNOWN;
#endif

/* <termios.h>: c_oflag bits */

#ifdef OPOST
/**
 * @var int
 * @cvalue OPOST
 */
const OPOST = UNKNOWN;
#endif

#ifdef ONLCR
/**
 * @var int
 * @cvalue ONLCR
 */
const ONLCR = UNKNOWN;
#endif

#ifdef OCRNL
/**
 * @var int
 * @cvalue OCRNL
 */
const OCRNL = UNKNOWN;
#endif

#ifdef ONOCR
/**
 * @var int
 * @cvalue ONOCR
 */
const ONOCR = UNKNOWN;
#endif

#ifdef ONLRET
/**
 * @var int
 * @cvalue ONLRET
 */
const ONLRET = UNKNOWN;
#endif

#ifdef OFDEL
/**
 * @var int
 * @cvalue OFDEL
 */
const OFDEL = UNKNOWN;
#endif

#ifdef OFILL
/**
 * @var int
 * @cvalue OFILL
 */
const OFILL = UNKNOWN;
#endif

#ifdef NLDLY
/**
 * @var int
 * @cvalue NLDLY
 */
const NLDLY = UNKNOWN;
#endif

#ifdef NL0
/**
 * @var int
 * @cvalue NL0
 */
const NL0 = UNKNOWN;
#endif

#ifdef NL1
/**
 * @var int
 * @cvalue NL1
 */
const NL1 = UNKNOWN;
#endif

#ifdef CRDLY
/**
 * @var int
 * @cvalue CRDLY
 */
const CRDLY = UNKNOWN;
#endif

#ifdef CR0
/**
 * @var int
 * @cvalue CR0
 */
const CR0 = UNKNOWN;
#endif

#ifdef CR1
/**
 * @var int
 * @cvalue CR1
 */
const CR1 = UNKNOWN;
#endif

#ifdef CR2
/**
 * @var int
 * @cvalue CR2
 */
const CR2 = UNKNOWN;
#endif

#ifdef CR3
/**
 * @var int
 * @cvalue CR3
 */
const CR3 = UNKNOWN;
#endif

#ifdef TABDLY
/**
 * @var int
 * @cvalue TABDLY
 */
const TABDLY = UNKNOWN;
#endif

#ifdef TAB0
/**
 * @var int
 * @cvalue TAB0
 */
const TAB0 = UNKNOWN;
#endif

#ifdef TAB1
/**
 * @var int
 * @cvalue TAB1
 */
const TAB1 = UNKNOWN;
#endif

#ifdef TAB2
/**
 * @var int
 * @cvalue TAB2
 */
const TAB2 = UNKNOWN;
#endif

#ifdef TAB3
/**
 * @var int
 * @cvalue TAB3
 */
const TAB3 = UNKNOWN;
#endif

#ifdef BSDLY
/**
 * @var int
 * @cvalue BSDLY
 */
const BSDLY = UNKNOWN;
#endif

#ifdef BS0
/**
 * @var int
 * @cvalue BS0
 */
const BS0 = UNKNOWN;
#endif

#ifdef BS1
/**
 * @var int
 * @cvalue BS1
 */
const BS1 = UNKNOWN;
#endif

#ifdef VTDLY
/**
 * @var int
 * @cvalue VTDLY
 */
const VTDLY = UNKNOWN;
#endif

#ifdef VT0
/**
 * @var int
 * @cvalue VT0
 */
const VT0 = UNKNOWN;
#endif

#ifdef VT1
/**
 * @var int
 * @cvalue VT1
 */
const VT1 = UNKNOWN;
#endif

#ifdef FFDLY
/**
 * @var int
 * @cvalue FFDLY
 */
const FFDLY = UNKNOWN;
#endif

#ifdef FF0
/**
 * @var int
 * @cvalue FF0
 */
const FF0 = UNKNOWN;
#endif

#ifdef FF1
/**
 * @var int
 * @cvalue FF1
 */
const FF1 = UNKNOWN;
#endif

/* <termios.h>: c_cflag bits */

#ifdef CSIZE
/**
 * @var int
 * @cvalue CSIZE
 */
const CSIZE = UNKNOWN;
#endif

#ifdef CS5
/**
 * @var int
 * @cvalue CS5
 */
const CS5 = UNKNOWN;
#endif

#ifdef CS6
/**
 * @var int
 * @cvalue CS6
 */
const CS6 = UNKNOWN;
#endif

#ifdef CS7
/**
 * @var int
 * @cvalue CS7
 */
const CS7 = UNKNOWN;
#endif

#ifdef CS8
/**
 * @var int
 * @cvalue CS8
 */
const CS8 = UNKNOWN;
#endif

#ifdef CSTOPB
/**
 * @var int
 * @cvalue CSTOPB
 */
const CSTOPB = UNKNOWN;
#endif

#ifdef CREAD
/**
 * @var int
 * @cvalue CREAD
 */
const CREAD = UNKNOWN;
#endif

#ifdef PARENB
/**
 * @var int
 * @cvalue PARENB
 */
const PARENB = UNKNOWN;
#endif

#ifdef PARODD
/**
 * @var int
 * @cvalue PARODD
 */
const PARODD = UNKNOWN;
#endif

#ifdef HUPCL
/**
 * @var int
 * @cvalue HUPCL
 */
const HUPCL = UNKNOWN;
#endif

#ifdef CLOCAL
/**
 * @var int
 * @cvalue CLOCAL
 */
const CLOCAL = UNKNOWN;
#endif

/* <termios.h>: c_lflag bits (ECHO is registered in MINIT: the name is a PHP keyword, read it with constant('ECHO')) */

#ifdef ECHOE
/**
 * @var int
 * @cvalue ECHOE
 */
const ECHOE = UNKNOWN;
#endif

#ifdef ECHOK
/**
 * @var int
 * @cvalue ECHOK
 */
const ECHOK = UNKNOWN;
#endif

#ifdef ECHONL
/**
 * @var int
 * @cvalue ECHONL
 */
const ECHONL = UNKNOWN;
#endif

#ifdef ICANON
/**
 * @var int
 * @cvalue ICANON
 */
const ICANON = UNKNOWN;
#endif

#ifdef IEXTEN
/**
 * @var int
 * @cvalue IEXTEN
 */
const IEXTEN = UNKNOWN;
#endif

#ifdef ISIG
/**
 * @var int
 * @cvalue ISIG
 */
const ISIG = UNKNOWN;
#endif

#ifdef NOFLSH
/**
 * @var int
 * @cvalue NOFLSH
 */
const NOFLSH = UNKNOWN;
#endif

#ifdef TOSTOP
/**
 * @var int
 * @cvalue TOSTOP
 */
const TOSTOP = UNKNOWN;
#endif

/* <termios.h>: speeds for cfsetispeed()/cfsetospeed(), every one this platform defines */

#ifdef B0
/**
 * @var int
 * @cvalue B0
 */
const B0 = UNKNOWN;
#endif

#ifdef B50
/**
 * @var int
 * @cvalue B50
 */
const B50 = UNKNOWN;
#endif

#ifdef B75
/**
 * @var int
 * @cvalue B75
 */
const B75 = UNKNOWN;
#endif

#ifdef B110
/**
 * @var int
 * @cvalue B110
 */
const B110 = UNKNOWN;
#endif

#ifdef B134
/**
 * @var int
 * @cvalue B134
 */
const B134 = UNKNOWN;
#endif

#ifdef B150
/**
 * @var int
 * @cvalue B150
 */
const B150 = UNKNOWN;
#endif

#ifdef B200
/**
 * @var int
 * @cvalue B200
 */
const B200 = UNKNOWN;
#endif

#ifdef B300
/**
 * @var int
 * @cvalue B300
 */
const B300 = UNKNOWN;
#endif

#ifdef B600
/**
 * @var int
 * @cvalue B600
 */
const B600 = UNKNOWN;
#endif

#ifdef B1200
/**
 * @var int
 * @cvalue B1200
 */
const B1200 = UNKNOWN;
#endif

#ifdef B1800
/**
 * @var int
 * @cvalue B1800
 */
const B1800 = UNKNOWN;
#endif

#ifdef B2400
/**
 * @var int
 * @cvalue B2400
 */
const B2400 = UNKNOWN;
#endif

#ifdef B4800
/**
 * @var int
 * @cvalue B4800
 */
const B4800 = UNKNOWN;
#endif

#ifdef B9600
/**
 * @var int
 * @cvalue B9600
 */
const B9600 = UNKNOWN;
#endif

#ifdef B19200
/**
 * @var int
 * @cvalue B19200
 */
const B19200 = UNKNOWN;
#endif

#ifdef B38400
/**
 * @var int
 * @cvalue B38400
 */
const B38400 = UNKNOWN;
#endif

#ifdef B57600
/**
 * @var int
 * @cvalue B57600
 */
const B57600 = UNKNOWN;
#endif

#ifdef B115200
/**
 * @var int
 * @cvalue B115200
 */
const B115200 = UNKNOWN;
#endif

#ifdef B230400
/**
 * @var int
 * @cvalue B230400
 */
const B230400 = UNKNOWN;
#endif

#ifdef B460800
/**
 * @var int
 * @cvalue B460800
 */
const B460800 = UNKNOWN;
#endif

#ifdef B500000
/**
 * @var int
 * @cvalue B500000
 */
const B500000 = UNKNOWN;
#endif

#ifdef B576000
/**
 * @var int
 * @cvalue B576000
 */
const B576000 = UNKNOWN;
#endif

#ifdef B921600
/**
 * @var int
 * @cvalue B921600
 */
const B921600 = UNKNOWN;
#endif

#ifdef B1000000
/**
 * @var int
 * @cvalue B1000000
 */
const B1000000 = UNKNOWN;
#endif

#ifdef B1152000
/**
 * @var int
 * @cvalue B1152000
 */
const B1152000 = UNKNOWN;
#endif

#ifdef B1500000
/**
 * @var int
 * @cvalue B1500000
 */
const B1500000 = UNKNOWN;
#endif

#ifdef B2000000
/**
 * @var int
 * @cvalue B2000000
 */
const B2000000 = UNKNOWN;
#endif

#ifdef B2500000
/**
 * @var int
 * @cvalue B2500000
 */
const B2500000 = UNKNOWN;
#endif

#ifdef B3000000
/**
 * @var int
 * @cvalue B3000000
 */
const B3000000 = UNKNOWN;
#endif

#ifdef B3500000
/**
 * @var int
 * @cvalue B3500000
 */
const B3500000 = UNKNOWN;
#endif

#ifdef B4000000
/**
 * @var int
 * @cvalue B4000000
 */
const B4000000 = UNKNOWN;
#endif

