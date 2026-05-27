
#ifdef HAVE_CONFIG_H
#include "../ext_config.h"
#endif

#include <php.h>
#include "../php_ext.h"
#include "../ext.h"

#include <Zend/zend_operators.h>
#include <Zend/zend_exceptions.h>
#include <Zend/zend_interfaces.h>

#include "kernel/main.h"
#include "kernel/array.h"
#include "kernel/memory.h"
#include "src/termios-api.h"
#include "kernel/operators.h"
#include "kernel/object.h"


ZEPHIR_INIT_CLASS(Posi_Termios)
{
	ZEPHIR_REGISTER_CLASS(Posi, Termios, posi, termios, posi_termios_method_entry, 0);

	return SUCCESS;
}

/**
 * Get terminal attributes for the open file descriptor.
 * Returns an associative array with keys c_iflag, c_oflag, c_cflag, c_lflag, c_cc,
 * or false on failure (same semantics as C tcgetattr(2)).
 */
PHP_METHOD(Posi_Termios, tcgetattr)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *fd_param = NULL, result, _0;
	zend_long fd;

	ZVAL_UNDEF(&result);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(fd)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &fd_param);
	ZVAL_LONG(&_0, fd);
	ZEPHIR_INIT_VAR(&result);
	posix_tcgetattr(&result, &_0);
	RETURN_CCTOR(&result);
}

/**
 * Set terminal attributes for the open file descriptor.
 * $action must be one of TCSANOW (0), TCSADRAIN (1), or TCSAFLUSH (2).
 * $termios must be an array previously returned by tcgetattr().
 * Returns 0 on success, -1 on failure (same semantics as C tcsetattr(2)).
 */
PHP_METHOD(Posi_Termios, tcsetattr)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval termios;
	zval *fd_param = NULL, *action_param = NULL, *termios_param = NULL, _0, _1;
	zend_long fd, action;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&termios);
	ZEND_PARSE_PARAMETERS_START(3, 3)
		Z_PARAM_LONG(fd)
		Z_PARAM_LONG(action)
		Z_PARAM_ARRAY(termios)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 3, 0, &fd_param, &action_param, &termios_param);
	zephir_get_arrval(&termios, termios_param);
	ZVAL_LONG(&_0, fd);
	ZVAL_LONG(&_1, action);
	RETURN_MM_LONG(posix_tcsetattr(&_0, &_1, &termios));
}

/**
 * Set the input baud rate in a termios array.
 * Returns a new termios array with the updated speed, or false on failure
 * (same semantics as C cfsetispeed(3)).
 */
PHP_METHOD(Posi_Termios, cfsetispeed)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long speed;
	zval *termios_param = NULL, *speed_param = NULL, result, _0;
	zval termios;

	ZVAL_UNDEF(&termios);
	ZVAL_UNDEF(&result);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ARRAY(termios)
		Z_PARAM_LONG(speed)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &termios_param, &speed_param);
	zephir_get_arrval(&termios, termios_param);
	ZVAL_LONG(&_0, speed);
	ZEPHIR_INIT_VAR(&result);
	posix_cfsetispeed(&result, &termios, &_0);
	RETURN_CCTOR(&result);
}

/**
 * Set the output baud rate in a termios array.
 * Returns a new termios array with the updated speed, or false on failure
 * (same semantics as C cfsetospeed(3)).
 */
PHP_METHOD(Posi_Termios, cfsetospeed)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long speed;
	zval *termios_param = NULL, *speed_param = NULL, result, _0;
	zval termios;

	ZVAL_UNDEF(&termios);
	ZVAL_UNDEF(&result);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ARRAY(termios)
		Z_PARAM_LONG(speed)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &termios_param, &speed_param);
	zephir_get_arrval(&termios, termios_param);
	ZVAL_LONG(&_0, speed);
	ZEPHIR_INIT_VAR(&result);
	posix_cfsetospeed(&result, &termios, &_0);
	RETURN_CCTOR(&result);
}

/**
 * Get the input baud rate from a termios array.
 * Returns the speed_t value (same semantics as C cfgetispeed(3)).
 */
PHP_METHOD(Posi_Termios, cfgetispeed)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *termios_param = NULL;
	zval termios;

	ZVAL_UNDEF(&termios);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ARRAY(termios)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &termios_param);
	zephir_get_arrval(&termios, termios_param);
	RETURN_MM_LONG(posix_cfgetispeed(&termios));
}

/**
 * Get the output baud rate from a termios array.
 * Returns the speed_t value (same semantics as C cfgetospeed(3)).
 */
PHP_METHOD(Posi_Termios, cfgetospeed)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *termios_param = NULL;
	zval termios;

	ZVAL_UNDEF(&termios);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ARRAY(termios)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &termios_param);
	zephir_get_arrval(&termios, termios_param);
	RETURN_MM_LONG(posix_cfgetospeed(&termios));
}

/**
 * Wait until all output written to the file descriptor has been transmitted.
 * Returns 0 on success, -1 on failure (same semantics as C tcdrain(2)).
 */
PHP_METHOD(Posi_Termios, tcdrain)
{
	zval *fd_param = NULL, _0;
	zend_long fd;

	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(fd)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &fd_param);
	ZVAL_LONG(&_0, fd);
	RETURN_LONG(posix_tcdrain(&_0));
}

/**
 * Discard data in the terminal I/O queues.
 * $queue: TCIFLUSH (0), TCOFLUSH (1), or TCIOFLUSH (2).
 * Returns 0 on success, -1 on failure (same semantics as C tcflush(2)).
 */
PHP_METHOD(Posi_Termios, tcflush)
{
	zval *fd_param = NULL, *queue_param = NULL, _0, _1;
	zend_long fd, queue;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_LONG(fd)
		Z_PARAM_LONG(queue)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(2, 0, &fd_param, &queue_param);
	ZVAL_LONG(&_0, fd);
	ZVAL_LONG(&_1, queue);
	RETURN_LONG(posix_tcflush(&_0, &_1));
}

/**
 * Suspend or resume terminal I/O transmission.
 * $action: TCOOFF (0), TCOON (1), TCIOFF (2), or TCION (3).
 * Returns 0 on success, -1 on failure (same semantics as C tcflow(2)).
 */
PHP_METHOD(Posi_Termios, tcflow)
{
	zval *fd_param = NULL, *action_param = NULL, _0, _1;
	zend_long fd, action;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_LONG(fd)
		Z_PARAM_LONG(action)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(2, 0, &fd_param, &action_param);
	ZVAL_LONG(&_0, fd);
	ZVAL_LONG(&_1, action);
	RETURN_LONG(posix_tcflow(&_0, &_1));
}

/**
 * Set input and output baud rate atomically using a plain numeric value.
 *
 * For rates with a standard POSIX B* constant (50 through 4000000) this
 * calls cfsetispeed/cfsetospeed + tcsetattr.  For any other value — such
 * as the LD2410C default of 256000 — it falls back to the Linux
 * BOTHER/termios2 ioctl path, which passes the exact numeric rate to the
 * kernel driver and accepts whatever the hardware supports.
 *
 * Returns 0 on success, -1 on failure or on non-Linux when the rate has
 * no standard B* constant.
 */
PHP_METHOD(Posi_Termios, setBaudRate)
{
	zval *fd_param = NULL, *baud_param = NULL, _0, _1;
	zend_long fd, baud;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_LONG(fd)
		Z_PARAM_LONG(baud)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(2, 0, &fd_param, &baud_param);
	ZVAL_LONG(&_0, fd);
	ZVAL_LONG(&_1, baud);
	RETURN_LONG(posix_set_baud_rate(&_0, &_1));
}

