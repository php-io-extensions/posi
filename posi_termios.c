/*
 * posi termios: <termios.h> bindings over a PHP array shaped like struct termios,
 * plus posi_set_baud_rate().
 *
 * Array shape (tcgetattr() returns it, every function taking one requires all of it):
 *   c_iflag, c_oflag, c_cflag, c_lflag  int
 *   c_cc                                int[NCCS], indexed by VMIN, VTIME, ...
 *   c_ispeed, c_ospeed                  int, cfgetispeed()/cfgetospeed() of the struct
 *
 * The speeds travel in the array because macOS keeps them in their own struct
 * fields rather than in c_cflag; without them a round trip would apply B0 and
 * hang up the line. On Linux they mirror the c_cflag speed bits.
 */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "php.h"
#include "php_posi.h"

#include <string.h>
#include <termios.h>
#include <unistd.h>

/*
 * Linux rates without a B* constant go through termios2 with BOTHER, which hands
 * the kernel the number itself. The struct and request numbers are written out
 * here because <asm/termbits.h> redefines struct termios against <termios.h>,
 * and <sys/ioctl.h> on arm64 defines TCGETS2 as sizeof(struct termios2) over an
 * incomplete type. This translation unit includes neither and declares ioctl(2)
 * by its POSIX prototype. The layout and numbers below hold on every Linux
 * architecture with the generic ioctl encoding, which the guard lists.
 */
#if defined(__linux__) && (defined(__x86_64__) || defined(__i386__) || defined(__aarch64__) \
	|| defined(__arm__) || defined(__riscv) || defined(__loongarch__))
# define POSI_HAVE_TERMIOS2 1

extern int ioctl(int fd, unsigned long request, ...);

struct posi_termios2 {
	unsigned int c_iflag;
	unsigned int c_oflag;
	unsigned int c_cflag;
	unsigned int c_lflag;
	unsigned char c_line;
	unsigned char c_cc[19];
	unsigned int c_ispeed;
	unsigned int c_ospeed;
};

# define POSI_BOTHER 0x00001000u
# define POSI_CBAUD 0x0000100Fu
# define POSI_TCGETS2 0x802C542Aul
# define POSI_TCSETS2 0x402C542Bul
#endif

static void posi_termios_to_array(const struct termios *t, zval *out)
{
	zval cc;

	array_init_size(out, 7);
	add_assoc_long(out, "c_iflag", (zend_long) t->c_iflag);
	add_assoc_long(out, "c_oflag", (zend_long) t->c_oflag);
	add_assoc_long(out, "c_cflag", (zend_long) t->c_cflag);
	add_assoc_long(out, "c_lflag", (zend_long) t->c_lflag);

	array_init_size(&cc, NCCS);
	for (int i = 0; i < NCCS; i++) {
		add_next_index_long(&cc, (zend_long) t->c_cc[i]);
	}
	add_assoc_zval(out, "c_cc", &cc);

	add_assoc_long(out, "c_ispeed", (zend_long) cfgetispeed(t));
	add_assoc_long(out, "c_ospeed", (zend_long) cfgetospeed(t));
}

static zval *posi_termios_field(HashTable *ht, const char *key, size_t len, uint32_t argnum)
{
	zval *zv = zend_hash_str_find_deref(ht, key, len);

	if (zv == NULL || Z_TYPE_P(zv) != IS_LONG) {
		zend_argument_value_error(argnum, "must hold an int \"%s\"", key);
		return NULL;
	}
	return zv;
}

/*
 * Rebuilds a struct termios from the array shape above; throws and returns false
 * on a missing or mistyped field. A speed is applied only where it differs from
 * the one the flags already carry, so a Linux custom rate (BOTHER in c_cflag)
 * survives the round trip untouched.
 */
static bool posi_array_to_termios(HashTable *ht, uint32_t argnum, struct termios *t)
{
	zval *zv, *cc;

	memset(t, 0, sizeof(*t));

#define POSI_TERMIOS_FLAG(field) \
	if ((zv = posi_termios_field(ht, #field, sizeof(#field) - 1, argnum)) == NULL) { \
		return false; \
	} \
	t->field = (tcflag_t) Z_LVAL_P(zv);

	POSI_TERMIOS_FLAG(c_iflag)
	POSI_TERMIOS_FLAG(c_oflag)
	POSI_TERMIOS_FLAG(c_cflag)
	POSI_TERMIOS_FLAG(c_lflag)
#undef POSI_TERMIOS_FLAG

	cc = zend_hash_str_find_deref(ht, "c_cc", sizeof("c_cc") - 1);
	if (cc == NULL || Z_TYPE_P(cc) != IS_ARRAY) {
		zend_argument_value_error(argnum, "must hold an array \"c_cc\"");
		return false;
	}
	for (zend_ulong i = 0; i < NCCS; i++) {
		zv = zend_hash_index_find_deref(Z_ARRVAL_P(cc), i);
		if (zv == NULL || Z_TYPE_P(zv) != IS_LONG) {
			zend_argument_value_error(argnum, "must hold \"c_cc\" ints at indices 0 to %d", NCCS - 1);
			return false;
		}
		t->c_cc[i] = (cc_t) Z_LVAL_P(zv);
	}

	zval *ispeed = posi_termios_field(ht, "c_ispeed", sizeof("c_ispeed") - 1, argnum);
	zval *ospeed = ispeed ? posi_termios_field(ht, "c_ospeed", sizeof("c_ospeed") - 1, argnum) : NULL;
	if (ospeed == NULL) {
		return false;
	}
	if ((zend_long) cfgetispeed(t) != Z_LVAL_P(ispeed) && cfsetispeed(t, (speed_t) Z_LVAL_P(ispeed)) != 0) {
		zend_argument_value_error(argnum, "holds a \"c_ispeed\" this platform does not support");
		return false;
	}
	if ((zend_long) cfgetospeed(t) != Z_LVAL_P(ospeed) && cfsetospeed(t, (speed_t) Z_LVAL_P(ospeed)) != 0) {
		zend_argument_value_error(argnum, "holds a \"c_ospeed\" this platform does not support");
		return false;
	}
	return true;
}

ZEND_FUNCTION(tcgetattr)
{
	zend_long fd_l;
	int fd;
	struct termios t;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(fd_l)
	ZEND_PARSE_PARAMETERS_END();

	POSI_ARG_INT(fd, fd_l, 1);

	if (tcgetattr(fd, &t) != 0) {
		POSI_RECORD_ERRNO();
		RETURN_FALSE;
	}
	posi_termios_to_array(&t, return_value);
}

ZEND_FUNCTION(tcsetattr)
{
	zend_long fd_l, action_l;
	HashTable *ht;
	int fd, action;
	struct termios t;

	ZEND_PARSE_PARAMETERS_START(3, 3)
		Z_PARAM_LONG(fd_l)
		Z_PARAM_LONG(action_l)
		Z_PARAM_ARRAY_HT(ht)
	ZEND_PARSE_PARAMETERS_END();

	POSI_ARG_INT(fd, fd_l, 1);
	POSI_ARG_INT(action, action_l, 2);
	if (!posi_array_to_termios(ht, 3, &t)) {
		RETURN_THROWS();
	}

	int ret = tcsetattr(fd, action, &t);
	if (ret != 0) {
		POSI_RECORD_ERRNO();
	}
	RETURN_LONG(ret);
}

/* cfsetispeed() and cfsetospeed(): the array with the speed applied, or false when the platform refuses it. */
static void posi_cfset(INTERNAL_FUNCTION_PARAMETERS, int (*set)(struct termios *, speed_t))
{
	HashTable *ht;
	zend_long speed;
	struct termios t;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ARRAY_HT(ht)
		Z_PARAM_LONG(speed)
	ZEND_PARSE_PARAMETERS_END();

	if (!posi_array_to_termios(ht, 1, &t)) {
		RETURN_THROWS();
	}
	if (speed < 0 || set(&t, (speed_t) speed) != 0) {
		POSI_G(last_errno) = speed < 0 ? EINVAL : errno;
		RETURN_FALSE;
	}
	posi_termios_to_array(&t, return_value);
}

ZEND_FUNCTION(cfsetispeed)
{
	posi_cfset(INTERNAL_FUNCTION_PARAM_PASSTHRU, cfsetispeed);
}

ZEND_FUNCTION(cfsetospeed)
{
	posi_cfset(INTERNAL_FUNCTION_PARAM_PASSTHRU, cfsetospeed);
}

static void posi_cfget(INTERNAL_FUNCTION_PARAMETERS, speed_t (*get)(const struct termios *))
{
	HashTable *ht;
	struct termios t;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ARRAY_HT(ht)
	ZEND_PARSE_PARAMETERS_END();

	if (!posi_array_to_termios(ht, 1, &t)) {
		RETURN_THROWS();
	}
	RETURN_LONG((zend_long) get(&t));
}

ZEND_FUNCTION(cfgetispeed)
{
	posi_cfget(INTERNAL_FUNCTION_PARAM_PASSTHRU, cfgetispeed);
}

ZEND_FUNCTION(cfgetospeed)
{
	posi_cfget(INTERNAL_FUNCTION_PARAM_PASSTHRU, cfgetospeed);
}

ZEND_FUNCTION(tcdrain)
{
	zend_long fd_l;
	int fd;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(fd_l)
	ZEND_PARSE_PARAMETERS_END();

	POSI_ARG_INT(fd, fd_l, 1);

	int ret = tcdrain(fd);
	if (ret != 0) {
		POSI_RECORD_ERRNO();
	}
	RETURN_LONG(ret);
}

/* tcflush() and tcflow(): fd plus one int selector. */
static void posi_tc_selector(INTERNAL_FUNCTION_PARAMETERS, int (*call)(int, int))
{
	zend_long fd_l, selector_l;
	int fd, selector;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_LONG(fd_l)
		Z_PARAM_LONG(selector_l)
	ZEND_PARSE_PARAMETERS_END();

	POSI_ARG_INT(fd, fd_l, 1);
	POSI_ARG_INT(selector, selector_l, 2);

	int ret = call(fd, selector);
	if (ret != 0) {
		POSI_RECORD_ERRNO();
	}
	RETURN_LONG(ret);
}

ZEND_FUNCTION(tcflush)
{
	posi_tc_selector(INTERNAL_FUNCTION_PARAM_PASSTHRU, tcflush);
}

ZEND_FUNCTION(tcflow)
{
	posi_tc_selector(INTERNAL_FUNCTION_PARAM_PASSTHRU, tcflow);
}

/* The B* constant for a numeric rate, or false when the platform defines none. */
static bool posi_speed_for(zend_long baud, speed_t *speed)
{
	switch (baud) {
		case 50: *speed = B50; return true;
		case 75: *speed = B75; return true;
		case 110: *speed = B110; return true;
		case 134: *speed = B134; return true;
		case 150: *speed = B150; return true;
		case 200: *speed = B200; return true;
		case 300: *speed = B300; return true;
		case 600: *speed = B600; return true;
		case 1200: *speed = B1200; return true;
		case 1800: *speed = B1800; return true;
		case 2400: *speed = B2400; return true;
		case 4800: *speed = B4800; return true;
		case 9600: *speed = B9600; return true;
		case 19200: *speed = B19200; return true;
		case 38400: *speed = B38400; return true;
#ifdef B57600
		case 57600: *speed = B57600; return true;
#endif
#ifdef B115200
		case 115200: *speed = B115200; return true;
#endif
#ifdef B230400
		case 230400: *speed = B230400; return true;
#endif
#ifdef B460800
		case 460800: *speed = B460800; return true;
#endif
#ifdef B500000
		case 500000: *speed = B500000; return true;
#endif
#ifdef B576000
		case 576000: *speed = B576000; return true;
#endif
#ifdef B921600
		case 921600: *speed = B921600; return true;
#endif
#ifdef B1000000
		case 1000000: *speed = B1000000; return true;
#endif
#ifdef B1152000
		case 1152000: *speed = B1152000; return true;
#endif
#ifdef B1500000
		case 1500000: *speed = B1500000; return true;
#endif
#ifdef B2000000
		case 2000000: *speed = B2000000; return true;
#endif
#ifdef B2500000
		case 2500000: *speed = B2500000; return true;
#endif
#ifdef B3000000
		case 3000000: *speed = B3000000; return true;
#endif
#ifdef B3500000
		case 3500000: *speed = B3500000; return true;
#endif
#ifdef B4000000
		case 4000000: *speed = B4000000; return true;
#endif
		default: return false;
	}
}

/*
 * Sets both directions of fd to a numeric rate, now (TCSANOW). A rate with a B*
 * constant goes through cfsetispeed/cfsetospeed/tcsetattr; any other rate goes
 * through termios2/BOTHER on Linux and fails with EINVAL elsewhere.
 */
ZEND_FUNCTION(posi_set_baud_rate)
{
	zend_long fd_l, baud;
	int fd;
	speed_t speed;
	struct termios tty;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_LONG(fd_l)
		Z_PARAM_LONG(baud)
	ZEND_PARSE_PARAMETERS_END();

	POSI_ARG_INT(fd, fd_l, 1);
	if (baud < 1 || baud > (zend_long) UINT32_MAX) {
		zend_argument_value_error(2, "must be between 1 and %u", UINT32_MAX);
		RETURN_THROWS();
	}

	if (posi_speed_for(baud, &speed)) {
		if (tcgetattr(fd, &tty) != 0 || cfsetispeed(&tty, speed) != 0 || cfsetospeed(&tty, speed) != 0
			|| tcsetattr(fd, TCSANOW, &tty) != 0) {
			POSI_RECORD_ERRNO();
			RETURN_LONG(-1);
		}
		RETURN_LONG(0);
	}

#ifdef POSI_HAVE_TERMIOS2
	struct posi_termios2 tio2;

	if (ioctl(fd, POSI_TCGETS2, &tio2) != 0) {
		POSI_RECORD_ERRNO();
		RETURN_LONG(-1);
	}
	tio2.c_cflag &= ~POSI_CBAUD;
	tio2.c_cflag |= POSI_BOTHER;
	tio2.c_ispeed = (unsigned int) baud;
	tio2.c_ospeed = (unsigned int) baud;
	if (ioctl(fd, POSI_TCSETS2, &tio2) != 0) {
		POSI_RECORD_ERRNO();
		RETURN_LONG(-1);
	}
	RETURN_LONG(0);
#else
	POSI_G(last_errno) = EINVAL;
	RETURN_LONG(-1);
#endif
}
