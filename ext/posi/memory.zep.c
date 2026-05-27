
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
#include "src/memory-api.h"
#include "kernel/operators.h"
#include "kernel/memory.h"
#include "kernel/object.h"
#include "kernel/string.h"


/**
 * Generic native-memory primitives for structs that embed C pointers.
 *
 * Some kernel ioctls (e.g. SPI_IOC_MESSAGE, I2C_RDWR) require a struct
 * whose fields are *pointers* to other user-space buffers.  PHP strings
 * live in the Zend memory manager and cannot expose their addresses.
 * This class allocates named emalloc() buffers, returns their addresses
 * as plain PHP ints, and lets callers embed those ints (via pack('Q',...))
 * inside a struct that is then handed to ioctl().
 *
 * Usage example (SPI full-duplex transfer):
 *
 *   tx = Posi\Memory::alloc(2);
 *   rx = Posi\Memory::alloc(2);
 *   Posi\Memory::write(tx, pack('CC', cmd, 0x00));
 *   // pack spi_ioc_transfer: tx_buf(Q), rx_buf(Q), len(V), ...
 *   struct = pack('QQVVvCCCCC', tx, rx, 2, 0, 0, 8, 0, 0, 0, 0);
 *   ioctl(fd, SPI_IOC_MESSAGE_1, ['data': struct], out);
 *   data = Posi\Memory::read(rx, 2);
 *   Posi\Memory::free(tx);
 *   Posi\Memory::free(rx);
 */
ZEPHIR_INIT_CLASS(Posi_Memory)
{
	ZEPHIR_REGISTER_CLASS(Posi, Memory, posi, memory, posi_memory_method_entry, 0);

	return SUCCESS;
}

/**
 * Allocate a zero-filled native buffer of `size` bytes.
 *
 * Returns the buffer address as a PHP int.  The caller is responsible
 * for calling free() exactly once when done.
 */
PHP_METHOD(Posi_Memory, alloc)
{
	zval *size_param = NULL, _0;
	zend_long size;

	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(size)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &size_param);
	ZVAL_LONG(&_0, size);
	RETURN_LONG(posi_mem_alloc(&_0));
}

/**
 * Free a buffer previously returned by alloc().
 * Passing 0 is a safe no-op.
 */
PHP_METHOD(Posi_Memory, free)
{
	zval *ptr_param = NULL, _0;
	zend_long ptr;

	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(ptr)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &ptr_param);
	ZVAL_LONG(&_0, ptr);
	posi_mem_free(&_0);
}

/**
 * Write the bytes of `data` into the buffer at `ptr + offset`.
 * No bounds checking — caller must ensure the buffer is large enough.
 */
PHP_METHOD(Posi_Memory, write)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval data;
	zval *ptr_param = NULL, *data_param = NULL, *offset_param = NULL, _0, _1;
	zend_long ptr, offset;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&data);
	ZEND_PARSE_PARAMETERS_START(2, 3)
		Z_PARAM_LONG(ptr)
		Z_PARAM_STR(data)
		Z_PARAM_OPTIONAL
		Z_PARAM_LONG(offset)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 1, &ptr_param, &data_param, &offset_param);
	zephir_get_strval(&data, data_param);
	if (!offset_param) {
		offset = 0;
	} else {
		}
	ZVAL_LONG(&_0, ptr);
	ZVAL_LONG(&_1, offset);
	posi_mem_write(&_0, &data, &_1);
	ZEPHIR_MM_RESTORE();
}

/**
 * Read `size` bytes from the buffer at `ptr + offset` and return
 * them as a PHP string.
 */
PHP_METHOD(Posi_Memory, read)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *ptr_param = NULL, *size_param = NULL, *offset_param = NULL, result, _0, _1, _2;
	zend_long ptr, size, offset;

	ZVAL_UNDEF(&result);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZEND_PARSE_PARAMETERS_START(2, 3)
		Z_PARAM_LONG(ptr)
		Z_PARAM_LONG(size)
		Z_PARAM_OPTIONAL
		Z_PARAM_LONG(offset)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 1, &ptr_param, &size_param, &offset_param);
	if (!offset_param) {
		offset = 0;
	} else {
		}
	ZVAL_LONG(&_0, ptr);
	ZVAL_LONG(&_1, size);
	ZVAL_LONG(&_2, offset);
	ZEPHIR_INIT_VAR(&result);
	posi_mem_read(&result, &_0, &_1, &_2);
	RETURN_CCTOR(&result);
}

