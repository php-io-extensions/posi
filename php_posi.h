#ifndef PHP_POSI_H
#define PHP_POSI_H

#include <errno.h>
#include <limits.h>

extern zend_module_entry posi_module_entry;
#define phpext_posi_ptr &posi_module_entry

#define PHP_POSI_VERSION "0.10.6"

ZEND_BEGIN_MODULE_GLOBALS(posi)
	int last_errno;
	/* Live posi_mem_alloc() buffers: address => size in bytes. */
	HashTable allocations;
ZEND_END_MODULE_GLOBALS(posi)

ZEND_EXTERN_MODULE_GLOBALS(posi)
#define POSI_G(v) ZEND_MODULE_GLOBALS_ACCESSOR(posi, v)

#if defined(ZTS) && defined(COMPILE_DL_POSI)
ZEND_TSRMLS_CACHE_EXTERN()
#endif

/* Keeps errno across the PHP/C boundary; posi_errno() reads it back. */
#define POSI_RECORD_ERRNO() (POSI_G(last_errno) = errno)

/* A zend_long argument that the C call takes as int: out of range throws instead of truncating. */
#define POSI_ARG_INT(dest, src, argnum) do { \
		if (ZEND_LONG_EXCEEDS_INT(src)) { \
			zend_argument_value_error((argnum), "must be between %d and %d", INT_MIN, INT_MAX); \
			RETURN_THROWS(); \
		} \
		(dest) = (int) (src); \
	} while (0)

/* Defined in posi_termios.c; the function table lives in posi.c. */
ZEND_FUNCTION(tcgetattr);
ZEND_FUNCTION(tcsetattr);
ZEND_FUNCTION(cfsetispeed);
ZEND_FUNCTION(cfsetospeed);
ZEND_FUNCTION(cfgetispeed);
ZEND_FUNCTION(cfgetospeed);
ZEND_FUNCTION(tcdrain);
ZEND_FUNCTION(tcflush);
ZEND_FUNCTION(tcflow);
ZEND_FUNCTION(posi_set_baud_rate);

#endif /* PHP_POSI_H */
