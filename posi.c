/*
 * posi: POSIX descriptor, path, process and termios bindings (termios lives in
 * posi_termios.c), native buffers for ioctl() structs that point at other
 * memory, and posi_errno() to carry errno across the PHP/C boundary.
 */

/* ppoll() is declared by glibc only under _GNU_SOURCE. */
#if defined(__linux__) && !defined(_GNU_SOURCE)
# define _GNU_SOURCE 1
#endif

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "php.h"
#include "ext/standard/file.h"
#include "ext/standard/info.h"
#include "php_posi.h"
#include "php_network.h"

#include <fcntl.h>
#include <poll.h>
#include <stdint.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/param.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/wait.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

#include "posi_arginfo.h"

#if defined(HOST_NAME_MAX)
# define POSI_HOST_NAME_MAX HOST_NAME_MAX
#elif defined(MAXHOSTNAMELEN)
# define POSI_HOST_NAME_MAX MAXHOSTNAMELEN
#else
# define POSI_HOST_NAME_MAX _POSIX_HOST_NAME_MAX
#endif

ZEND_DECLARE_MODULE_GLOBALS(posi)

static bool posi_mode_ok(zend_long mode, uint32_t argnum)
{
	if (mode < 0 || mode > 07777) {
		zend_argument_value_error(argnum, "must be between 0 and 0o7777");
		return false;
	}
	return true;
}

/* uid_t/gid_t, with -1 meaning "leave unchanged" as chown(2) defines it. */
static bool posi_id_ok(zend_long id, uint32_t argnum)
{
	if (id < -1 || id > (zend_long) UINT32_MAX) {
		zend_argument_value_error(argnum, "must be between -1 and %u", UINT32_MAX);
		return false;
	}
	return true;
}

static bool posi_length_ok(zend_long length, uint32_t argnum)
{
	if (length < 0) {
		zend_argument_value_error(argnum, "must be greater than or equal to 0");
		return false;
	}
	return true;
}

/* Hands back the first `used` bytes of a buffer read into from C. */
static zend_string *posi_string_finish(zend_string *buf, size_t used)
{
	if (used < ZSTR_LEN(buf)) {
		buf = zend_string_truncate(buf, used, 0);
	}
	ZSTR_VAL(buf)[used] = '\0';
	return buf;
}

/* {{{ <unistd.h>, <fcntl.h>, <sys/stat.h> */

ZEND_FUNCTION(posix_open)
{
	zend_string *path;
	zend_long flags = O_RDWR;
	zend_long mode = 0644;
	int fl;

	ZEND_PARSE_PARAMETERS_START(1, 3)
		Z_PARAM_PATH_STR(path)
		Z_PARAM_OPTIONAL
		Z_PARAM_LONG(flags)
		Z_PARAM_LONG(mode)
	ZEND_PARSE_PARAMETERS_END();

	POSI_ARG_INT(fl, flags, 2);
	if (!posi_mode_ok(mode, 3)) {
		RETURN_THROWS();
	}

	/* open(2) reads mode only for flags that create a file (O_CREAT, O_TMPFILE). */
	int fd = open(ZSTR_VAL(path), fl, (mode_t) mode);
	if (fd < 0) {
		POSI_RECORD_ERRNO();
	}
	RETURN_LONG(fd);
}

ZEND_FUNCTION(posix_close)
{
	zend_long fd_l;
	int fd;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(fd_l)
	ZEND_PARSE_PARAMETERS_END();

	POSI_ARG_INT(fd, fd_l, 1);

	int ret = close(fd);
	if (ret < 0) {
		POSI_RECORD_ERRNO();
	}
	RETURN_LONG(ret);
}

ZEND_FUNCTION(posix_read)
{
	zend_long fd_l, length;
	int fd;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_LONG(fd_l)
		Z_PARAM_LONG(length)
	ZEND_PARSE_PARAMETERS_END();

	POSI_ARG_INT(fd, fd_l, 1);
	if (!posi_length_ok(length, 2)) {
		RETURN_THROWS();
	}

	zend_string *buf = zend_string_alloc((size_t) length, 0);
	ssize_t n = read(fd, ZSTR_VAL(buf), (size_t) length);
	if (n < 0) {
		POSI_RECORD_ERRNO();
		zend_string_efree(buf);
		RETURN_FALSE;
	}
	RETURN_NEW_STR(posi_string_finish(buf, (size_t) n));
}

ZEND_FUNCTION(posix_write)
{
	zend_long fd_l, length;
	zend_string *data;
	int fd;

	ZEND_PARSE_PARAMETERS_START(3, 3)
		Z_PARAM_LONG(fd_l)
		Z_PARAM_STR(data)
		Z_PARAM_LONG(length)
	ZEND_PARSE_PARAMETERS_END();

	POSI_ARG_INT(fd, fd_l, 1);
	if (length < 0 || (size_t) length > ZSTR_LEN(data)) {
		zend_argument_value_error(3, "must be between 0 and the length of argument #2 ($data)");
		RETURN_THROWS();
	}

	ssize_t n = write(fd, ZSTR_VAL(data), (size_t) length);
	if (n < 0) {
		POSI_RECORD_ERRNO();
	}
	RETURN_LONG((zend_long) n);
}

ZEND_FUNCTION(posix_lseek)
{
	zend_long fd_l, offset, whence_l;
	int fd, whence;

	ZEND_PARSE_PARAMETERS_START(3, 3)
		Z_PARAM_LONG(fd_l)
		Z_PARAM_LONG(offset)
		Z_PARAM_LONG(whence_l)
	ZEND_PARSE_PARAMETERS_END();

	POSI_ARG_INT(fd, fd_l, 1);
	POSI_ARG_INT(whence, whence_l, 3);

	off_t ret = lseek(fd, (off_t) offset, whence);
	if (ret == (off_t) -1) {
		POSI_RECORD_ERRNO();
	}
	RETURN_LONG((zend_long) ret);
}

ZEND_FUNCTION(posix_readv)
{
	zend_long fd_l;
	HashTable *vectors;
	zval *entry, buffers;
	int fd;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_LONG(fd_l)
		Z_PARAM_ARRAY_HT(vectors)
	ZEND_PARSE_PARAMETERS_END();

	POSI_ARG_INT(fd, fd_l, 1);

	uint32_t count = zend_hash_num_elements(vectors);
#ifdef IOV_MAX
	if (count > IOV_MAX) {
		zend_argument_value_error(2, "must hold at most %d buffers", IOV_MAX);
		RETURN_THROWS();
	}
#endif

	if (count == 0) {
		array_init_size(return_value, 2);
		add_assoc_long(return_value, "res", 0);
		array_init(&buffers);
		add_assoc_zval(return_value, "buffers", &buffers);
		return;
	}

	struct iovec *iov = safe_emalloc(count, sizeof(*iov), 0);
	zend_string **bufs = safe_emalloc(count, sizeof(*bufs), 0);
	uint32_t filled = 0;

	ZEND_HASH_FOREACH_VAL(vectors, entry) {
		zval *len;

		ZVAL_DEREF(entry);
		if (Z_TYPE_P(entry) != IS_ARRAY
			|| (len = zend_hash_str_find_deref(Z_ARRVAL_P(entry), "len", sizeof("len") - 1)) == NULL
			|| Z_TYPE_P(len) != IS_LONG || Z_LVAL_P(len) < 0) {
			break;
		}
		bufs[filled] = zend_string_alloc((size_t) Z_LVAL_P(len), 0);
		iov[filled].iov_base = ZSTR_VAL(bufs[filled]);
		iov[filled].iov_len = (size_t) Z_LVAL_P(len);
		filled++;
	} ZEND_HASH_FOREACH_END();

	ssize_t n = 0;
	if (filled == count) {
		n = readv(fd, iov, (int) count);
	}
	if (filled < count || n < 0) {
		if (filled == count) {
			POSI_RECORD_ERRNO();
		}
		for (uint32_t i = 0; i < filled; i++) {
			zend_string_efree(bufs[i]);
		}
		efree(bufs);
		efree(iov);
		if (filled < count) {
			zend_argument_value_error(2, "must contain arrays with a non-negative int \"len\"");
			RETURN_THROWS();
		}
		RETURN_FALSE;
	}

	array_init_size(return_value, 2);
	add_assoc_long(return_value, "res", (zend_long) n);
	array_init_size(&buffers, count);
	size_t remaining = (size_t) n;
	for (uint32_t i = 0; i < count; i++) {
		size_t chunk = remaining < iov[i].iov_len ? remaining : iov[i].iov_len;

		remaining -= chunk;
		add_next_index_str(&buffers, posi_string_finish(bufs[i], chunk));
	}
	add_assoc_zval(return_value, "buffers", &buffers);

	efree(bufs);
	efree(iov);
}

ZEND_FUNCTION(posix_recv)
{
	zend_long fd_l, length, flags_l = 0;
	int fd, flags;

	ZEND_PARSE_PARAMETERS_START(2, 3)
		Z_PARAM_LONG(fd_l)
		Z_PARAM_LONG(length)
		Z_PARAM_OPTIONAL
		Z_PARAM_LONG(flags_l)
	ZEND_PARSE_PARAMETERS_END();

	POSI_ARG_INT(fd, fd_l, 1);
	if (!posi_length_ok(length, 2)) {
		RETURN_THROWS();
	}
	POSI_ARG_INT(flags, flags_l, 3);

	zend_string *buf = zend_string_alloc((size_t) length, 0);
	ssize_t n = recv(fd, ZSTR_VAL(buf), (size_t) length, flags);
	if (n < 0) {
		POSI_RECORD_ERRNO();
		zend_string_efree(buf);
		RETURN_FALSE;
	}
	RETURN_NEW_STR(posi_string_finish(buf, (size_t) n));
}

/* {{{ fcntl() */

static bool posi_fcntl_is_lock(int cmd)
{
	return cmd == F_GETLK || cmd == F_SETLK || cmd == F_SETLKW;
}

/* Commands whose return value is the datum asked for, not a status. */
static bool posi_fcntl_returns_datum(int cmd)
{
	return cmd == F_GETFD || cmd == F_GETFL
#ifdef F_GETOWN
		|| cmd == F_GETOWN
#endif
		;
}

static zend_long posi_flock_field(HashTable *ht, const char *key, size_t len)
{
	zval *zv = zend_hash_str_find_deref(ht, key, len);

	return zv ? zval_get_long(zv) : 0;
}

ZEND_FUNCTION(fcntl)
{
	zend_long fd_l, cmd_l;
	zval *arg, *value;
	int fd, cmd, ret;

	ZEND_PARSE_PARAMETERS_START(4, 4)
		Z_PARAM_LONG(fd_l)
		Z_PARAM_LONG(cmd_l)
		Z_PARAM_ZVAL(arg)
		Z_PARAM_ZVAL(value)
	ZEND_PARSE_PARAMETERS_END();

	POSI_ARG_INT(fd, fd_l, 1);
	POSI_ARG_INT(cmd, cmd_l, 2);

	if (posi_fcntl_is_lock(cmd)) {
		/* struct flock as ["type", "whence", "start", "len", "pid"]; absent keys are 0. */
		struct flock fl;
		zval out;

		if (Z_TYPE_P(arg) != IS_ARRAY) {
			zend_argument_type_error(3, "must be of type array for a lock command, %s given", zend_zval_value_name(arg));
			RETURN_THROWS();
		}
		memset(&fl, 0, sizeof(fl));
		fl.l_type = (short) posi_flock_field(Z_ARRVAL_P(arg), "type", sizeof("type") - 1);
		fl.l_whence = (short) posi_flock_field(Z_ARRVAL_P(arg), "whence", sizeof("whence") - 1);
		fl.l_start = (off_t) posi_flock_field(Z_ARRVAL_P(arg), "start", sizeof("start") - 1);
		fl.l_len = (off_t) posi_flock_field(Z_ARRVAL_P(arg), "len", sizeof("len") - 1);
		fl.l_pid = (pid_t) posi_flock_field(Z_ARRVAL_P(arg), "pid", sizeof("pid") - 1);

		ret = fcntl(fd, cmd, &fl);
		if (ret < 0) {
			POSI_RECORD_ERRNO();
			ZEND_TRY_ASSIGN_REF_FALSE(value);
			RETURN_LONG(ret);
		}

		array_init_size(&out, 5);
		add_assoc_long(&out, "type", (zend_long) fl.l_type);
		add_assoc_long(&out, "whence", (zend_long) fl.l_whence);
		add_assoc_long(&out, "start", (zend_long) fl.l_start);
		add_assoc_long(&out, "len", (zend_long) fl.l_len);
		add_assoc_long(&out, "pid", (zend_long) fl.l_pid);
		ZEND_TRY_ASSIGN_REF_TMP(value, &out);
		RETURN_LONG(ret);
	}

	switch (Z_TYPE_P(arg)) {
		case IS_NULL:
			ret = fcntl(fd, cmd);
			break;
		case IS_LONG: {
			int a;

			POSI_ARG_INT(a, Z_LVAL_P(arg), 3);
			ret = fcntl(fd, cmd, a);
			break;
		}
		case IS_TRUE:
			ret = fcntl(fd, cmd, 1);
			break;
		case IS_FALSE:
			ret = fcntl(fd, cmd, 0);
			break;
		default:
			zend_argument_type_error(3, "must be of type int|bool|null, %s given", zend_zval_value_name(arg));
			RETURN_THROWS();
	}

	if (ret < 0) {
		POSI_RECORD_ERRNO();
		ZEND_TRY_ASSIGN_REF_FALSE(value);
	} else if (posi_fcntl_returns_datum(cmd)) {
		ZEND_TRY_ASSIGN_REF_LONG(value, ret);
	} else {
		ZEND_TRY_ASSIGN_REF_COPY(value, arg);
	}
	RETURN_LONG(ret);
}

/* }}} */

/* {{{ ioctl() */

/* The request reads and writes a copy of `in`; $value receives the bytes after the call. */
static int posi_ioctl_buffer(int fd, unsigned long request, zend_string *in, zval *value)
{
	zend_string *buf = zend_string_init(ZSTR_VAL(in), ZSTR_LEN(in), 0);
	int ret = ioctl(fd, request, ZSTR_VAL(buf));

	if (ret < 0) {
		POSI_RECORD_ERRNO();
		zend_string_efree(buf);
		ZEND_TRY_ASSIGN_REF_FALSE(value);
		return ret;
	}
	ZEND_TRY_ASSIGN_REF_NEW_STR(value, buf);
	return ret;
}

/*
 * $arg selects how the third ioctl(2) argument is passed:
 *   ["value" => int]               pointer to an int, read back into $value
 *   ["bytes"|"data" => string], or a string
 *                                  pointer to a copy of the bytes, read back into $value
 *   int, bool                      passed as the pointer-sized argument itself (posi_mem_alloc() addresses)
 *   null                           NULL; $value receives a positive return value, else null
 */
ZEND_FUNCTION(ioctl)
{
	zend_long fd_l, request_l;
	zval *arg, *value, *in;
	int fd, ret;

	ZEND_PARSE_PARAMETERS_START(4, 4)
		Z_PARAM_LONG(fd_l)
		Z_PARAM_LONG(request_l)
		Z_PARAM_ZVAL(arg)
		Z_PARAM_ZVAL(value)
	ZEND_PARSE_PARAMETERS_END();

	POSI_ARG_INT(fd, fd_l, 1);
	unsigned long request = (unsigned long) request_l;

	switch (Z_TYPE_P(arg)) {
		case IS_ARRAY:
			in = zend_hash_str_find_deref(Z_ARRVAL_P(arg), "value", sizeof("value") - 1);
			if (in) {
				int v;

				if (Z_TYPE_P(in) != IS_LONG) {
					zend_argument_type_error(3, "\"value\" must be of type int, %s given", zend_zval_value_name(in));
					RETURN_THROWS();
				}
				POSI_ARG_INT(v, Z_LVAL_P(in), 3);
				ret = ioctl(fd, request, &v);
				if (ret < 0) {
					break;
				}
				ZEND_TRY_ASSIGN_REF_LONG(value, v);
				RETURN_LONG(ret);
			}
			in = zend_hash_str_find_deref(Z_ARRVAL_P(arg), "bytes", sizeof("bytes") - 1);
			if (!in) {
				in = zend_hash_str_find_deref(Z_ARRVAL_P(arg), "data", sizeof("data") - 1);
			}
			if (!in || Z_TYPE_P(in) != IS_STRING) {
				zend_argument_value_error(3, "must hold an int \"value\", or a string \"bytes\" or \"data\"");
				RETURN_THROWS();
			}
			RETURN_LONG(posi_ioctl_buffer(fd, request, Z_STR_P(in), value));
		case IS_STRING:
			RETURN_LONG(posi_ioctl_buffer(fd, request, Z_STR_P(arg), value));
		case IS_NULL:
			ret = ioctl(fd, request, NULL);
			if (ret < 0) {
				break;
			}
			if (ret > 0) {
				ZEND_TRY_ASSIGN_REF_LONG(value, ret);
			} else {
				ZEND_TRY_ASSIGN_REF_NULL(value);
			}
			RETURN_LONG(ret);
		case IS_LONG:
		case IS_TRUE:
		case IS_FALSE:
			ret = ioctl(fd, request, (void *) (intptr_t) zval_get_long(arg));
			if (ret < 0) {
				break;
			}
			ZEND_TRY_ASSIGN_REF_COPY(value, arg);
			RETURN_LONG(ret);
		default:
			zend_argument_type_error(3, "must be of type array|string|int|bool|null, %s given", zend_zval_value_name(arg));
			RETURN_THROWS();
	}

	POSI_RECORD_ERRNO();
	ZEND_TRY_ASSIGN_REF_FALSE(value);
	RETURN_LONG(ret);
}

/* }}} */

/*
 * One descriptor, one poll: 1 when it is ready, 0 on timeout, -1 on error.
 * $events 0 means POLLIN; a negative $timeout_ns waits forever. Linux calls
 * ppoll(2) with the full nanosecond timeout; macOS has no ppoll() and calls
 * poll(2) with the timeout rounded up to whole milliseconds.
 */
ZEND_FUNCTION(posix_ppoll)
{
	zend_long fd_l, timeout_ns = 0, events = 0;
	int fd, ret;

	ZEND_PARSE_PARAMETERS_START(1, 3)
		Z_PARAM_LONG(fd_l)
		Z_PARAM_OPTIONAL
		Z_PARAM_LONG(timeout_ns)
		Z_PARAM_LONG(events)
	ZEND_PARSE_PARAMETERS_END();

	POSI_ARG_INT(fd, fd_l, 1);
	if (events < 0 || events > SHRT_MAX) {
		zend_argument_value_error(3, "must be between 0 and %d", SHRT_MAX);
		RETURN_THROWS();
	}

	struct pollfd pfd = { .fd = fd, .events = (short) (events == 0 ? POLLIN : events), .revents = 0 };

#if defined(__linux__)
	struct timespec ts, *tsp = NULL;

	if (timeout_ns >= 0) {
		ts.tv_sec = (time_t) (timeout_ns / 1000000000);
		ts.tv_nsec = (long) (timeout_ns % 1000000000);
		tsp = &ts;
	}
	ret = ppoll(&pfd, 1, tsp, NULL);
#else
	int timeout_ms = -1;

	if (timeout_ns >= 0) {
		zend_long ms = timeout_ns / 1000000 + (timeout_ns % 1000000 != 0);

		timeout_ms = ms > INT_MAX ? INT_MAX : (int) ms;
	}
	ret = poll(&pfd, 1, timeout_ms);
#endif

	if (ret < 0) {
		POSI_RECORD_ERRNO();
	}
	RETURN_LONG(ret > 0 ? 1 : ret);
}

/* fdopen(3): a stream over the caller's fd, no dup. fclose() closes the fd; its flags are untouched. */
ZEND_FUNCTION(posix_fdopen)
{
	zend_long fd_l;
	zend_string *mode;
	int fd;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_LONG(fd_l)
		Z_PARAM_STR(mode)
	ZEND_PARSE_PARAMETERS_END();

	POSI_ARG_INT(fd, fd_l, 1);
	if (ZSTR_LEN(mode) == 0) {
		zend_argument_must_not_be_empty_error(2);
		RETURN_THROWS();
	}

	if (fcntl(fd, F_GETFD) < 0) {
		POSI_RECORD_ERRNO();
		RETURN_FALSE;
	}

	php_stream *stream = php_stream_fopen_from_fd(fd, ZSTR_VAL(mode), NULL);
	if (stream == NULL) {
		RETURN_FALSE;
	}
	php_stream_to_zval(stream, return_value);
}

/* fileno(3) for a PHP stream: the socket or file descriptor it wraps, no dup. EBADF when it wraps none. */
ZEND_FUNCTION(posix_fileno)
{
	zval *zstream;
	php_stream *stream;
	php_socket_t sock;
	int fd;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_RESOURCE(zstream)
	ZEND_PARSE_PARAMETERS_END();

	php_stream_from_zval(stream, zstream);

	if (php_stream_can_cast(stream, PHP_STREAM_AS_SOCKETD) == SUCCESS
		&& php_stream_cast(stream, PHP_STREAM_AS_SOCKETD, (void **) &sock, 0) == SUCCESS) {
		RETURN_LONG((zend_long) sock);
	}

	if (php_stream_can_cast(stream, PHP_STREAM_AS_FD) == SUCCESS
		&& php_stream_cast(stream, PHP_STREAM_AS_FD, (void **) &fd, 0) == SUCCESS) {
		RETURN_LONG(fd);
	}

	POSI_G(last_errno) = EBADF;
	RETURN_FALSE;
}

/* dup(2): a new descriptor for the same open file description. */
ZEND_FUNCTION(posix_dup)
{
	zend_long fd_l;
	int fd, copy;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(fd_l)
	ZEND_PARSE_PARAMETERS_END();

	POSI_ARG_INT(fd, fd_l, 1);

	copy = dup(fd);
	if (copy < 0) {
		POSI_RECORD_ERRNO();
	}
	RETURN_LONG(copy);
}

ZEND_FUNCTION(posix_listen)
{
	zend_long fd_l, backlog_l;
	int fd, backlog;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_LONG(fd_l)
		Z_PARAM_LONG(backlog_l)
	ZEND_PARSE_PARAMETERS_END();

	POSI_ARG_INT(fd, fd_l, 1);
	POSI_ARG_INT(backlog, backlog_l, 2);

	int ret = listen(fd, backlog);
	if (ret < 0) {
		POSI_RECORD_ERRNO();
	}
	RETURN_LONG(ret);
}

/* fdopen(3) for a stream socket: a stream over the caller's socket fd, no dup. The stream comes from
 * the registered "tcp" transport, so it carries that transport's ops (ext/openssl's TLS-capable ones
 * when loaded): stream_socket_accept(), stream_socket_get_name() and stream_socket_enable_crypto()
 * work on it and on what it accepts. php_stream_sock_open_from_socket() can't stand in: its
 * generic_socket ops neither accept nor encrypt. fclose() closes the fd. */
ZEND_FUNCTION(posix_socket_fdopen)
{
	zend_long fd_l;
	int fd, type, fl;
	socklen_t type_len = sizeof(type);
	struct stat st;
	php_stream_transport_factory factory;
	php_stream *stream;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(fd_l)
	ZEND_PARSE_PARAMETERS_END();

	POSI_ARG_INT(fd, fd_l, 1);

	if (fstat(fd, &st) < 0) {
		POSI_RECORD_ERRNO();
		RETURN_FALSE;
	}
	if (!S_ISSOCK(st.st_mode)) {
		POSI_G(last_errno) = ENOTSOCK;
		RETURN_FALSE;
	}
	if (getsockopt(fd, SOL_SOCKET, SO_TYPE, &type, &type_len) < 0) {
		POSI_RECORD_ERRNO();
		RETURN_FALSE;
	}
	if (type != SOCK_STREAM) {
		POSI_G(last_errno) = EPROTOTYPE;
		RETURN_FALSE;
	}
	fl = fcntl(fd, F_GETFL);
	if (fl < 0) {
		POSI_RECORD_ERRNO();
		RETURN_FALSE;
	}

	factory = zend_hash_str_find_ptr(php_stream_xport_get_hash(), "tcp", sizeof("tcp") - 1);
	if (factory == NULL) {
		POSI_G(last_errno) = EPROTONOSUPPORT;
		RETURN_FALSE;
	}

	/* ext/openssl's factory copies *timeout into its connect timeout, so it can't be NULL. */
	struct timeval timeout = { .tv_sec = (time_t) FG(default_socket_timeout), .tv_usec = 0 };

	stream = factory("tcp", sizeof("tcp") - 1, "", 0, NULL, 0, 0, &timeout, NULL STREAMS_CC);
	if (stream == NULL) {
		POSI_G(last_errno) = ENOMEM;
		RETURN_FALSE;
	}

	/* Both core tcp and ext/openssl abstracts begin with php_netstream_data_t. */
	php_netstream_data_t *sock = (php_netstream_data_t *) stream->abstract;
	sock->socket = fd;
	sock->is_blocked = (fl & O_NONBLOCK) == 0;
	stream->flags |= PHP_STREAM_FLAG_AVOID_BLOCKING;

	php_stream_to_zval(stream, return_value);
}

ZEND_FUNCTION(posix_chmod)
{
	zend_string *path;
	zend_long mode;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_PATH_STR(path)
		Z_PARAM_LONG(mode)
	ZEND_PARSE_PARAMETERS_END();

	if (!posi_mode_ok(mode, 2)) {
		RETURN_THROWS();
	}

	int ret = chmod(ZSTR_VAL(path), (mode_t) mode);
	if (ret < 0) {
		POSI_RECORD_ERRNO();
	}
	RETURN_LONG(ret);
}

ZEND_FUNCTION(posix_chown)
{
	zend_string *path;
	zend_long owner, group;

	ZEND_PARSE_PARAMETERS_START(3, 3)
		Z_PARAM_PATH_STR(path)
		Z_PARAM_LONG(owner)
		Z_PARAM_LONG(group)
	ZEND_PARSE_PARAMETERS_END();

	if (!posi_id_ok(owner, 2) || !posi_id_ok(group, 3)) {
		RETURN_THROWS();
	}

	int ret = chown(ZSTR_VAL(path), (uid_t) owner, (gid_t) group);
	if (ret < 0) {
		POSI_RECORD_ERRNO();
	}
	RETURN_LONG(ret);
}

ZEND_FUNCTION(posix_fchmod)
{
	zend_long fd_l, mode;
	int fd;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_LONG(fd_l)
		Z_PARAM_LONG(mode)
	ZEND_PARSE_PARAMETERS_END();

	POSI_ARG_INT(fd, fd_l, 1);
	if (!posi_mode_ok(mode, 2)) {
		RETURN_THROWS();
	}

	int ret = fchmod(fd, (mode_t) mode);
	if (ret < 0) {
		POSI_RECORD_ERRNO();
	}
	RETURN_LONG(ret);
}

ZEND_FUNCTION(posix_fchown)
{
	zend_long fd_l, owner, group;
	int fd;

	ZEND_PARSE_PARAMETERS_START(3, 3)
		Z_PARAM_LONG(fd_l)
		Z_PARAM_LONG(owner)
		Z_PARAM_LONG(group)
	ZEND_PARSE_PARAMETERS_END();

	POSI_ARG_INT(fd, fd_l, 1);
	if (!posi_id_ok(owner, 2) || !posi_id_ok(group, 3)) {
		RETURN_THROWS();
	}

	int ret = fchown(fd, (uid_t) owner, (gid_t) group);
	if (ret < 0) {
		POSI_RECORD_ERRNO();
	}
	RETURN_LONG(ret);
}

ZEND_FUNCTION(posix_umask)
{
	zend_long mask;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(mask)
	ZEND_PARSE_PARAMETERS_END();

	if (!posi_mode_ok(mask, 1)) {
		RETURN_THROWS();
	}

	RETURN_LONG((zend_long) umask((mode_t) mask));
}

ZEND_FUNCTION(posix_lstat)
{
	zend_string *path;
	struct stat st;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_PATH_STR(path)
	ZEND_PARSE_PARAMETERS_END();

	if (lstat(ZSTR_VAL(path), &st) != 0) {
		POSI_RECORD_ERRNO();
		RETURN_FALSE;
	}

	array_init_size(return_value, 13);
	add_assoc_long(return_value, "dev", (zend_long) st.st_dev);
	add_assoc_long(return_value, "ino", (zend_long) st.st_ino);
	add_assoc_long(return_value, "mode", (zend_long) st.st_mode);
	add_assoc_long(return_value, "nlink", (zend_long) st.st_nlink);
	add_assoc_long(return_value, "uid", (zend_long) st.st_uid);
	add_assoc_long(return_value, "gid", (zend_long) st.st_gid);
	add_assoc_long(return_value, "rdev", (zend_long) st.st_rdev);
	add_assoc_long(return_value, "size", (zend_long) st.st_size);
	add_assoc_long(return_value, "blksize", (zend_long) st.st_blksize);
	add_assoc_long(return_value, "blocks", (zend_long) st.st_blocks);
	add_assoc_long(return_value, "atime", (zend_long) st.st_atime);
	add_assoc_long(return_value, "mtime", (zend_long) st.st_mtime);
	add_assoc_long(return_value, "ctime", (zend_long) st.st_ctime);
}

ZEND_FUNCTION(posix_hostname)
{
	char buf[POSI_HOST_NAME_MAX + 1];

	ZEND_PARSE_PARAMETERS_NONE();

	if (gethostname(buf, sizeof(buf)) != 0) {
		POSI_RECORD_ERRNO();
		RETURN_FALSE;
	}
	buf[POSI_HOST_NAME_MAX] = '\0';
	RETURN_STRING(buf);
}

/* }}} */

/* {{{ <sys/wait.h> */

ZEND_FUNCTION(posix_wait)
{
	zval *status = NULL;
	int wstatus = 0;

	ZEND_PARSE_PARAMETERS_START(0, 1)
		Z_PARAM_OPTIONAL
		Z_PARAM_ZVAL(status)
	ZEND_PARSE_PARAMETERS_END();

	pid_t pid = wait(&wstatus);
	if (pid < 0) {
		POSI_RECORD_ERRNO();
	} else if (status) {
		ZEND_TRY_ASSIGN_REF_LONG(status, wstatus);
	}
	RETURN_LONG((zend_long) pid);
}

ZEND_FUNCTION(posix_waitpid)
{
	zend_long pid_l, options_l = 0;
	zval *status = NULL;
	int pid, options, wstatus = 0;

	ZEND_PARSE_PARAMETERS_START(1, 3)
		Z_PARAM_LONG(pid_l)
		Z_PARAM_OPTIONAL
		Z_PARAM_ZVAL(status)
		Z_PARAM_LONG(options_l)
	ZEND_PARSE_PARAMETERS_END();

	POSI_ARG_INT(pid, pid_l, 1);
	POSI_ARG_INT(options, options_l, 3);

	/* 0 under WNOHANG means no child changed state, and there is no status to report. */
	pid_t ret = waitpid((pid_t) pid, &wstatus, options);
	if (ret < 0) {
		POSI_RECORD_ERRNO();
	} else if (ret > 0 && status) {
		ZEND_TRY_ASSIGN_REF_LONG(status, wstatus);
	}
	RETURN_LONG((zend_long) ret);
}

/* The <sys/wait.h> status macros, over a status posix_wait()/posix_waitpid() reported. */
#define POSI_STATUS_FUNCTION(name, macro, RETURN_AS) \
	ZEND_FUNCTION(name) \
	{ \
		zend_long status_l; \
		int status; \
		ZEND_PARSE_PARAMETERS_START(1, 1) \
			Z_PARAM_LONG(status_l) \
		ZEND_PARSE_PARAMETERS_END(); \
		POSI_ARG_INT(status, status_l, 1); \
		RETURN_AS(macro(status)); \
	}

POSI_STATUS_FUNCTION(posix_wifexited, WIFEXITED, RETURN_BOOL)
POSI_STATUS_FUNCTION(posix_wexitstatus, WEXITSTATUS, RETURN_LONG)
POSI_STATUS_FUNCTION(posix_wifsignaled, WIFSIGNALED, RETURN_BOOL)
POSI_STATUS_FUNCTION(posix_wtermsig, WTERMSIG, RETURN_LONG)
POSI_STATUS_FUNCTION(posix_wifstopped, WIFSTOPPED, RETURN_BOOL)
POSI_STATUS_FUNCTION(posix_wstopsig, WSTOPSIG, RETURN_LONG)
#ifdef WIFCONTINUED
POSI_STATUS_FUNCTION(posix_wifcontinued, WIFCONTINUED, RETURN_BOOL)
#endif

/* }}} */

/* {{{ Native buffers
 *
 * ioctl() structs such as i2c_rdwr_ioctl_data and spi_ioc_transfer carry
 * addresses of other buffers. posi_mem_alloc() returns such an address as an
 * int; read, write and free check it against the live allocations, so a stale
 * or foreign address, or a range past the end, throws instead of touching memory.
 * Buffers still live at request end are freed then.
 */

static zval *posi_mem_find(zend_long ptr, uint32_t argnum)
{
	zval *size = zend_hash_index_find(&POSI_G(allocations), (zend_ulong) ptr);

	if (size == NULL) {
		zend_argument_value_error(argnum, "must be an address posi_mem_alloc() returned and posi_mem_free() has not released");
	}
	return size;
}

static bool posi_mem_range_ok(zend_long offset, zend_long length, zend_long size, uint32_t offset_argnum)
{
	if (offset < 0 || offset > size || length > size - offset) {
		zend_argument_value_error(offset_argnum, "puts the range past the end of the " ZEND_LONG_FMT "-byte buffer", size);
		return false;
	}
	return true;
}

ZEND_FUNCTION(posi_mem_alloc)
{
	zend_long size;
	zval entry;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(size)
	ZEND_PARSE_PARAMETERS_END();

	if (size < 1) {
		zend_argument_value_error(1, "must be greater than 0");
		RETURN_THROWS();
	}

	void *p = ecalloc(1, (size_t) size);
	ZVAL_LONG(&entry, size);
	zend_hash_index_add_new(&POSI_G(allocations), (zend_ulong) (uintptr_t) p, &entry);
	RETURN_LONG((zend_long) (uintptr_t) p);
}

ZEND_FUNCTION(posi_mem_free)
{
	zend_long ptr;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(ptr)
	ZEND_PARSE_PARAMETERS_END();

	/* free(NULL) is a no-op. */
	if (ptr == 0) {
		return;
	}
	if (posi_mem_find(ptr, 1) == NULL) {
		RETURN_THROWS();
	}
	efree((void *) (uintptr_t) ptr);
	zend_hash_index_del(&POSI_G(allocations), (zend_ulong) ptr);
}

ZEND_FUNCTION(posi_mem_write)
{
	zend_long ptr, offset = 0;
	zend_string *data;
	zval *size;

	ZEND_PARSE_PARAMETERS_START(2, 3)
		Z_PARAM_LONG(ptr)
		Z_PARAM_STR(data)
		Z_PARAM_OPTIONAL
		Z_PARAM_LONG(offset)
	ZEND_PARSE_PARAMETERS_END();

	if ((size = posi_mem_find(ptr, 1)) == NULL
		|| !posi_mem_range_ok(offset, (zend_long) ZSTR_LEN(data), Z_LVAL_P(size), 3)) {
		RETURN_THROWS();
	}
	memcpy((char *) (uintptr_t) ptr + offset, ZSTR_VAL(data), ZSTR_LEN(data));
}

ZEND_FUNCTION(posi_mem_read)
{
	zend_long ptr, length, offset = 0;
	zval *size;

	ZEND_PARSE_PARAMETERS_START(2, 3)
		Z_PARAM_LONG(ptr)
		Z_PARAM_LONG(length)
		Z_PARAM_OPTIONAL
		Z_PARAM_LONG(offset)
	ZEND_PARSE_PARAMETERS_END();

	if ((size = posi_mem_find(ptr, 1)) == NULL) {
		RETURN_THROWS();
	}
	if (!posi_length_ok(length, 2) || !posi_mem_range_ok(offset, length, Z_LVAL_P(size), 3)) {
		RETURN_THROWS();
	}
	RETURN_STRINGL((const char *) (uintptr_t) ptr + offset, (size_t) length);
}

/* }}} */

ZEND_FUNCTION(posi_errno)
{
	ZEND_PARSE_PARAMETERS_NONE();

	RETURN_LONG(POSI_G(last_errno));
}

/*
 * Constants other extensions also register, with the same <header> values:
 * ext/sockets (MSG_*), ext/pcntl (W*) and ext-kqueue (the E* kevent() reports).
 * posi registers each only when no other extension got there first. ECHO is a
 * PHP keyword, which a stub cannot declare.
 */
static void posi_register_if_absent(const char *name, size_t len, zend_long value, int module_number)
{
	if (!zend_hash_str_exists(EG(zend_constants), name, len)) {
		zend_register_long_constant(name, len, value, CONST_PERSISTENT, module_number);
	}
}

#define POSI_REGISTER_IF_ABSENT(c) posi_register_if_absent(#c, sizeof(#c) - 1, (zend_long) (c), module_number)

static PHP_GINIT_FUNCTION(posi)
{
#if defined(COMPILE_DL_POSI) && defined(ZTS)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif
	posi_globals->last_errno = 0;
	memset(&posi_globals->allocations, 0, sizeof(posi_globals->allocations));
}

PHP_MINIT_FUNCTION(posi)
{
	register_posi_symbols(module_number);

	POSI_REGISTER_IF_ABSENT(MSG_OOB);
	POSI_REGISTER_IF_ABSENT(MSG_PEEK);
	POSI_REGISTER_IF_ABSENT(MSG_WAITALL);
	POSI_REGISTER_IF_ABSENT(SOMAXCONN);
	POSI_REGISTER_IF_ABSENT(WNOHANG);
	POSI_REGISTER_IF_ABSENT(WUNTRACED);
#ifdef WCONTINUED
	POSI_REGISTER_IF_ABSENT(WCONTINUED);
#endif
	REGISTER_LONG_CONSTANT("ECHO", ECHO, CONST_PERSISTENT);

	/* <errno.h>: values posi_errno() returns. */
#ifdef E2BIG
	POSI_REGISTER_IF_ABSENT(E2BIG);
#endif
#ifdef EACCES
	POSI_REGISTER_IF_ABSENT(EACCES);
#endif
#ifdef EADDRINUSE
	POSI_REGISTER_IF_ABSENT(EADDRINUSE);
#endif
#ifdef EADDRNOTAVAIL
	POSI_REGISTER_IF_ABSENT(EADDRNOTAVAIL);
#endif
#ifdef EAFNOSUPPORT
	POSI_REGISTER_IF_ABSENT(EAFNOSUPPORT);
#endif
#ifdef EAGAIN
	POSI_REGISTER_IF_ABSENT(EAGAIN);
#endif
#ifdef EALREADY
	POSI_REGISTER_IF_ABSENT(EALREADY);
#endif
#ifdef EBADF
	POSI_REGISTER_IF_ABSENT(EBADF);
#endif
#ifdef EBADMSG
	POSI_REGISTER_IF_ABSENT(EBADMSG);
#endif
#ifdef EBUSY
	POSI_REGISTER_IF_ABSENT(EBUSY);
#endif
#ifdef ECANCELED
	POSI_REGISTER_IF_ABSENT(ECANCELED);
#endif
#ifdef ECHILD
	POSI_REGISTER_IF_ABSENT(ECHILD);
#endif
#ifdef ECONNABORTED
	POSI_REGISTER_IF_ABSENT(ECONNABORTED);
#endif
#ifdef ECONNREFUSED
	POSI_REGISTER_IF_ABSENT(ECONNREFUSED);
#endif
#ifdef ECONNRESET
	POSI_REGISTER_IF_ABSENT(ECONNRESET);
#endif
#ifdef EDEADLK
	POSI_REGISTER_IF_ABSENT(EDEADLK);
#endif
#ifdef EDESTADDRREQ
	POSI_REGISTER_IF_ABSENT(EDESTADDRREQ);
#endif
#ifdef EDOM
	POSI_REGISTER_IF_ABSENT(EDOM);
#endif
#ifdef EDQUOT
	POSI_REGISTER_IF_ABSENT(EDQUOT);
#endif
#ifdef EEXIST
	POSI_REGISTER_IF_ABSENT(EEXIST);
#endif
#ifdef EFAULT
	POSI_REGISTER_IF_ABSENT(EFAULT);
#endif
#ifdef EFBIG
	POSI_REGISTER_IF_ABSENT(EFBIG);
#endif
#ifdef EHOSTUNREACH
	POSI_REGISTER_IF_ABSENT(EHOSTUNREACH);
#endif
#ifdef EIDRM
	POSI_REGISTER_IF_ABSENT(EIDRM);
#endif
#ifdef EILSEQ
	POSI_REGISTER_IF_ABSENT(EILSEQ);
#endif
#ifdef EINPROGRESS
	POSI_REGISTER_IF_ABSENT(EINPROGRESS);
#endif
#ifdef EINTR
	POSI_REGISTER_IF_ABSENT(EINTR);
#endif
#ifdef EINVAL
	POSI_REGISTER_IF_ABSENT(EINVAL);
#endif
#ifdef EIO
	POSI_REGISTER_IF_ABSENT(EIO);
#endif
#ifdef EISCONN
	POSI_REGISTER_IF_ABSENT(EISCONN);
#endif
#ifdef EISDIR
	POSI_REGISTER_IF_ABSENT(EISDIR);
#endif
#ifdef ELOOP
	POSI_REGISTER_IF_ABSENT(ELOOP);
#endif
#ifdef EMFILE
	POSI_REGISTER_IF_ABSENT(EMFILE);
#endif
#ifdef EMLINK
	POSI_REGISTER_IF_ABSENT(EMLINK);
#endif
#ifdef EMSGSIZE
	POSI_REGISTER_IF_ABSENT(EMSGSIZE);
#endif
#ifdef EMULTIHOP
	POSI_REGISTER_IF_ABSENT(EMULTIHOP);
#endif
#ifdef ENAMETOOLONG
	POSI_REGISTER_IF_ABSENT(ENAMETOOLONG);
#endif
#ifdef ENETDOWN
	POSI_REGISTER_IF_ABSENT(ENETDOWN);
#endif
#ifdef ENETRESET
	POSI_REGISTER_IF_ABSENT(ENETRESET);
#endif
#ifdef ENETUNREACH
	POSI_REGISTER_IF_ABSENT(ENETUNREACH);
#endif
#ifdef ENFILE
	POSI_REGISTER_IF_ABSENT(ENFILE);
#endif
#ifdef ENOBUFS
	POSI_REGISTER_IF_ABSENT(ENOBUFS);
#endif
#ifdef ENODEV
	POSI_REGISTER_IF_ABSENT(ENODEV);
#endif
#ifdef ENOENT
	POSI_REGISTER_IF_ABSENT(ENOENT);
#endif
#ifdef ENOEXEC
	POSI_REGISTER_IF_ABSENT(ENOEXEC);
#endif
#ifdef ENOLCK
	POSI_REGISTER_IF_ABSENT(ENOLCK);
#endif
#ifdef ENOLINK
	POSI_REGISTER_IF_ABSENT(ENOLINK);
#endif
#ifdef ENOMEM
	POSI_REGISTER_IF_ABSENT(ENOMEM);
#endif
#ifdef ENOMSG
	POSI_REGISTER_IF_ABSENT(ENOMSG);
#endif
#ifdef ENOPROTOOPT
	POSI_REGISTER_IF_ABSENT(ENOPROTOOPT);
#endif
#ifdef ENOSPC
	POSI_REGISTER_IF_ABSENT(ENOSPC);
#endif
#ifdef ENOSYS
	POSI_REGISTER_IF_ABSENT(ENOSYS);
#endif
#ifdef ENOTCONN
	POSI_REGISTER_IF_ABSENT(ENOTCONN);
#endif
#ifdef ENOTDIR
	POSI_REGISTER_IF_ABSENT(ENOTDIR);
#endif
#ifdef ENOTEMPTY
	POSI_REGISTER_IF_ABSENT(ENOTEMPTY);
#endif
#ifdef ENOTRECOVERABLE
	POSI_REGISTER_IF_ABSENT(ENOTRECOVERABLE);
#endif
#ifdef ENOTSOCK
	POSI_REGISTER_IF_ABSENT(ENOTSOCK);
#endif
#ifdef ENOTSUP
	POSI_REGISTER_IF_ABSENT(ENOTSUP);
#endif
#ifdef ENOTTY
	POSI_REGISTER_IF_ABSENT(ENOTTY);
#endif
#ifdef ENXIO
	POSI_REGISTER_IF_ABSENT(ENXIO);
#endif
#ifdef EOPNOTSUPP
	POSI_REGISTER_IF_ABSENT(EOPNOTSUPP);
#endif
#ifdef EOVERFLOW
	POSI_REGISTER_IF_ABSENT(EOVERFLOW);
#endif
#ifdef EOWNERDEAD
	POSI_REGISTER_IF_ABSENT(EOWNERDEAD);
#endif
#ifdef EPERM
	POSI_REGISTER_IF_ABSENT(EPERM);
#endif
#ifdef EPIPE
	POSI_REGISTER_IF_ABSENT(EPIPE);
#endif
#ifdef EPROTO
	POSI_REGISTER_IF_ABSENT(EPROTO);
#endif
#ifdef EPROTONOSUPPORT
	POSI_REGISTER_IF_ABSENT(EPROTONOSUPPORT);
#endif
#ifdef EPROTOTYPE
	POSI_REGISTER_IF_ABSENT(EPROTOTYPE);
#endif
#ifdef ERANGE
	POSI_REGISTER_IF_ABSENT(ERANGE);
#endif
#ifdef EROFS
	POSI_REGISTER_IF_ABSENT(EROFS);
#endif
#ifdef ESPIPE
	POSI_REGISTER_IF_ABSENT(ESPIPE);
#endif
#ifdef ESRCH
	POSI_REGISTER_IF_ABSENT(ESRCH);
#endif
#ifdef ESTALE
	POSI_REGISTER_IF_ABSENT(ESTALE);
#endif
#ifdef ETIMEDOUT
	POSI_REGISTER_IF_ABSENT(ETIMEDOUT);
#endif
#ifdef ETXTBSY
	POSI_REGISTER_IF_ABSENT(ETXTBSY);
#endif
#ifdef EWOULDBLOCK
	POSI_REGISTER_IF_ABSENT(EWOULDBLOCK);
#endif
#ifdef EXDEV
	POSI_REGISTER_IF_ABSENT(EXDEV);
#endif

	return SUCCESS;
}

PHP_RINIT_FUNCTION(posi)
{
#if defined(COMPILE_DL_POSI) && defined(ZTS)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif
	POSI_G(last_errno) = 0;
	zend_hash_init(&POSI_G(allocations), 8, NULL, NULL, 0);

	return SUCCESS;
}

PHP_RSHUTDOWN_FUNCTION(posi)
{
	zend_ulong address;

	ZEND_HASH_FOREACH_NUM_KEY(&POSI_G(allocations), address) {
		efree((void *) (uintptr_t) address);
	} ZEND_HASH_FOREACH_END();
	zend_hash_destroy(&POSI_G(allocations));

	return SUCCESS;
}

PHP_MINFO_FUNCTION(posi)
{
	php_info_print_table_start();
	php_info_print_table_row(2, "posi support", "enabled");
	php_info_print_table_row(2, "Version", PHP_POSI_VERSION);
	php_info_print_table_end();
}

/*
 * posix: posix_getuid() and posix_setuid() come from ext/posix under the same names.
 * pcntl, sockets, kqueue: loaded first when present, so their shared constants win (see MINIT).
 */
static const zend_module_dep posi_deps[] = {
	ZEND_MOD_REQUIRED("posix")
	ZEND_MOD_OPTIONAL("pcntl")
	ZEND_MOD_OPTIONAL("sockets")
	ZEND_MOD_OPTIONAL("kqueue")
	ZEND_MOD_END
};

zend_module_entry posi_module_entry = {
	STANDARD_MODULE_HEADER_EX,
	NULL,
	posi_deps,
	"posi",
	ext_functions,
	PHP_MINIT(posi),
	NULL,
	PHP_RINIT(posi),
	PHP_RSHUTDOWN(posi),
	PHP_MINFO(posi),
	PHP_POSI_VERSION,
	PHP_MODULE_GLOBALS(posi),
	PHP_GINIT(posi),
	NULL,
	NULL,
	STANDARD_MODULE_PROPERTIES_EX
};

#ifdef COMPILE_DL_POSI
# ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
# endif
ZEND_GET_MODULE(posi)
#endif
