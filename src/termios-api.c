#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include <termios.h>
#include <unistd.h>
#include <string.h>

/*
 * Do NOT include <sys/ioctl.h> here.
 *
 * On ARM64 Linux (Debian Trixie / Raspberry Pi OS) the include chain
 * sys/ioctl.h → asm/ioctls.h → asm-generic/ioctls.h defines TCGETS2 and
 * TCSETS2 as _IOR/_IOW expressions that reference sizeof(struct termios2).
 * At that point struct termios2 is only forward-declared (incomplete), so
 * the compiler refuses to evaluate the sizeof and emits an error.
 *
 * We declare ioctl() ourselves using its POSIX prototype, which is stable
 * across all Linux architectures.  This keeps the kernel UAPI headers out
 * of this translation unit entirely.
 */
#ifdef __linux__
extern int ioctl(int fd, unsigned long request, ...);
#endif

#include "termios-api.h"

/*
 * Linux BOTHER / termios2 support for non-standard baud rates (e.g. 256000).
 *
 * We define every symbol we need inline.  We deliberately avoid
 * <asm/termios.h> (redefines struct termios, conflicts with <termios.h>)
 * and <sys/ioctl.h> (see above).
 *
 * The struct layout and the ioctl numbers are stable across all Linux
 * architectures that use 32-bit tcflag_t: x86, x86_64, ARM32, ARM64,
 * RISC-V, LoongArch, etc.  BOTHER and CBAUD are identical on all of them.
 *
 * We name the struct posi_termios2 to avoid any forward-declaration
 * collision with a kernel-header-provided 'struct termios2'.
 */
#ifdef __linux__
struct posi_termios2 {
    unsigned int  c_iflag;
    unsigned int  c_oflag;
    unsigned int  c_cflag;
    unsigned int  c_lflag;
    unsigned char c_line;
    unsigned char c_cc[19];
    unsigned int  c_ispeed;
    unsigned int  c_ospeed;
};

/* Force literal constants — do not inherit any _IOR-based definition. */
#undef  BOTHER
#define BOTHER   0x00001000u

#undef  CBAUD
#define CBAUD    0x0000100Fu

#undef  TCGETS2
#define TCGETS2  0x802C542Au

#undef  TCSETS2
#define TCSETS2  0x402C542Bu
#endif /* __linux__ */

/* ---------------------------------------------------------------------------
 * Internal helpers — convert between struct termios and a PHP array.
 *
 * PHP array layout:
 *   [
 *     "c_iflag" => int,        input mode flags
 *     "c_oflag" => int,        output mode flags
 *     "c_cflag" => int,        control mode flags  (baud rate bits live here)
 *     "c_lflag" => int,        local mode flags
 *     "c_cc"    => int[NCCS],  control characters  (VMIN, VTIME, etc.)
 *   ]
 * ------------------------------------------------------------------------- */

static void termios_to_array(const struct termios *t, zval *return_value)
{
    zval c_cc;
    int i;

    array_init(return_value);
    add_assoc_long(return_value, "c_iflag", (zend_long) t->c_iflag);
    add_assoc_long(return_value, "c_oflag", (zend_long) t->c_oflag);
    add_assoc_long(return_value, "c_cflag", (zend_long) t->c_cflag);
    add_assoc_long(return_value, "c_lflag", (zend_long) t->c_lflag);

    array_init(&c_cc);
    for (i = 0; i < NCCS; i++) {
        add_next_index_long(&c_cc, (zend_long) t->c_cc[i]);
    }
    add_assoc_zval(return_value, "c_cc", &c_cc);
}

static int array_to_termios(zval *arr, struct termios *t)
{
    zval *tmp, *entry;
    int i;

    if (Z_TYPE_P(arr) != IS_ARRAY) {
        return -1;
    }

    memset(t, 0, sizeof(*t));

    if ((tmp = zend_hash_str_find(Z_ARRVAL_P(arr), "c_iflag", sizeof("c_iflag") - 1)) != NULL) {
        t->c_iflag = (tcflag_t) zval_get_long(tmp);
    }
    if ((tmp = zend_hash_str_find(Z_ARRVAL_P(arr), "c_oflag", sizeof("c_oflag") - 1)) != NULL) {
        t->c_oflag = (tcflag_t) zval_get_long(tmp);
    }
    if ((tmp = zend_hash_str_find(Z_ARRVAL_P(arr), "c_cflag", sizeof("c_cflag") - 1)) != NULL) {
        t->c_cflag = (tcflag_t) zval_get_long(tmp);
    }
    if ((tmp = zend_hash_str_find(Z_ARRVAL_P(arr), "c_lflag", sizeof("c_lflag") - 1)) != NULL) {
        t->c_lflag = (tcflag_t) zval_get_long(tmp);
    }
    if ((tmp = zend_hash_str_find(Z_ARRVAL_P(arr), "c_cc", sizeof("c_cc") - 1)) != NULL
            && Z_TYPE_P(tmp) == IS_ARRAY) {
        i = 0;
        ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(tmp), entry) {
            if (i >= NCCS) break;
            t->c_cc[i++] = (cc_t) zval_get_long(entry);
        } ZEND_HASH_FOREACH_END();
    }

    return 0;
}

/* ---------------------------------------------------------------------------
 * Public API
 * ------------------------------------------------------------------------- */

void posix_tcgetattr(zval *return_value, zval *fd)
{
    struct termios t;

    if (tcgetattr((int) zval_get_long(fd), &t) != 0) {
        ZVAL_FALSE(return_value);
        return;
    }

    termios_to_array(&t, return_value);
}

int posix_tcsetattr(zval *fd, zval *action, zval *termios)
{
    struct termios t;

    if (array_to_termios(termios, &t) != 0) {
        return -1;
    }

    return tcsetattr((int) zval_get_long(fd), (int) zval_get_long(action), &t);
}

void posix_cfsetispeed(zval *return_value, zval *termios, zval *speed)
{
    struct termios t;

    if (array_to_termios(termios, &t) != 0) {
        ZVAL_FALSE(return_value);
        return;
    }

    if (cfsetispeed(&t, (speed_t) zval_get_long(speed)) != 0) {
        ZVAL_FALSE(return_value);
        return;
    }

    termios_to_array(&t, return_value);
}

void posix_cfsetospeed(zval *return_value, zval *termios, zval *speed)
{
    struct termios t;

    if (array_to_termios(termios, &t) != 0) {
        ZVAL_FALSE(return_value);
        return;
    }

    if (cfsetospeed(&t, (speed_t) zval_get_long(speed)) != 0) {
        ZVAL_FALSE(return_value);
        return;
    }

    termios_to_array(&t, return_value);
}

zend_long posix_cfgetispeed(zval *termios)
{
    struct termios t;

    if (array_to_termios(termios, &t) != 0) {
        return -1;
    }

    return (zend_long) cfgetispeed(&t);
}

zend_long posix_cfgetospeed(zval *termios)
{
    struct termios t;

    if (array_to_termios(termios, &t) != 0) {
        return -1;
    }

    return (zend_long) cfgetospeed(&t);
}

int posix_tcdrain(zval *fd)
{
    return tcdrain((int) zval_get_long(fd));
}

int posix_tcflush(zval *fd, zval *queue)
{
    return tcflush((int) zval_get_long(fd), (int) zval_get_long(queue));
}

int posix_tcflow(zval *fd, zval *action)
{
    return tcflow((int) zval_get_long(fd), (int) zval_get_long(action));
}

int posix_set_baud_rate(zval *fd, zval *baud)
{
    struct termios tty;
    int _fd  = (int) zval_get_long(fd);
    long _baud = (long) zval_get_long(baud);
    speed_t speed;

    if (tcgetattr(_fd, &tty) < 0) {
        return -1;
    }

    switch (_baud) {
        case 50:      speed = B50;      break;
        case 75:      speed = B75;      break;
        case 110:     speed = B110;     break;
        case 134:     speed = B134;     break;
        case 150:     speed = B150;     break;
        case 200:     speed = B200;     break;
        case 300:     speed = B300;     break;
        case 600:     speed = B600;     break;
        case 1200:    speed = B1200;    break;
        case 1800:    speed = B1800;    break;
        case 2400:    speed = B2400;    break;
        case 4800:    speed = B4800;    break;
        case 9600:    speed = B9600;    break;
        case 19200:   speed = B19200;   break;
        case 38400:   speed = B38400;   break;
        case 57600:   speed = B57600;   break;
        case 115200:  speed = B115200;  break;
        case 230400:  speed = B230400;  break;
#ifdef B460800
        case 460800:  speed = B460800;  break;
#endif
#ifdef B500000
        case 500000:  speed = B500000;  break;
#endif
#ifdef B576000
        case 576000:  speed = B576000;  break;
#endif
#ifdef B921600
        case 921600:  speed = B921600;  break;
#endif
#ifdef B1000000
        case 1000000: speed = B1000000; break;
#endif
#ifdef B1152000
        case 1152000: speed = B1152000; break;
#endif
#ifdef B1500000
        case 1500000: speed = B1500000; break;
#endif
#ifdef B2000000
        case 2000000: speed = B2000000; break;
#endif
#ifdef B2500000
        case 2500000: speed = B2500000; break;
#endif
#ifdef B3000000
        case 3000000: speed = B3000000; break;
#endif
#ifdef B3500000
        case 3500000: speed = B3500000; break;
#endif
#ifdef B4000000
        case 4000000: speed = B4000000; break;
#endif
        default:
#ifdef __linux__
            /*
             * No standard B* constant — use the Linux BOTHER/termios2 path,
             * which passes the numeric rate directly to the kernel and accepts
             * any value the UART hardware actually supports (e.g. 256000).
             */
            {
                struct posi_termios2 tio2;
                if (ioctl(_fd, TCGETS2, &tio2) == 0) {
                    tio2.c_cflag &= ~CBAUD;
                    tio2.c_cflag |=  BOTHER;
                    tio2.c_ispeed = (unsigned int) _baud;
                    tio2.c_ospeed = (unsigned int) _baud;
                    return (int) ioctl(_fd, TCSETS2, &tio2);
                }
            }
#endif
            return -1;
    }

    cfsetispeed(&tty, speed);
    cfsetospeed(&tty, speed);
    return tcsetattr(_fd, TCSANOW, &tty);
}
