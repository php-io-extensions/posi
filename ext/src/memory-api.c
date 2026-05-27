#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ext.h"
#include "memory-api.h"
#include <stdint.h>
#include <string.h>

/*
 * Allocate a native (emalloc) buffer of `size` bytes, zero-initialise it,
 * and return its address as a zend_long so PHP can hold it as an int.
 *
 * The returned integer is an opaque handle — it must be passed back to
 * posi_mem_free(), posi_mem_read(), and posi_mem_write().  Callers are
 * responsible for freeing every allocation exactly once.
 */
zend_long posi_mem_alloc(zval *size)
{
    size_t n = (size_t) Z_LVAL_P(size);

    if (n == 0) {
        return 0;
    }

    void *ptr = emalloc(n);
    memset(ptr, 0, n);

    return (zend_long)(uintptr_t) ptr;
}

/*
 * Free a buffer previously returned by posi_mem_alloc().
 * Passing 0 (NULL) is a no-op.
 */
void posi_mem_free(zval *ptr)
{
    void *p = (void *)(uintptr_t)(zend_long) Z_LVAL_P(ptr);

    if (p) {
        efree(p);
    }
}

/*
 * Copy the bytes of the PHP string `data` into the native buffer at
 * `ptr + offset`.  No bounds checking is performed — the caller must
 * ensure the buffer is large enough.
 */
void posi_mem_write(zval *ptr, zval *data, zval *offset)
{
    char       *p   = (char *)(uintptr_t)(zend_long) Z_LVAL_P(ptr);
    size_t      off = (size_t) Z_LVAL_P(offset);
    const char *src = Z_STRVAL_P(data);
    size_t      len = Z_STRLEN_P(data);

    memcpy(p + off, src, len);
}

/*
 * Read `size` bytes from the native buffer at `ptr + offset` and return
 * them as a PHP string.  No bounds checking is performed.
 */
void posi_mem_read(zval *return_value, zval *ptr, zval *size, zval *offset)
{
    const char *p   = (const char *)(uintptr_t)(zend_long) Z_LVAL_P(ptr);
    size_t      off = (size_t) Z_LVAL_P(offset);
    size_t      n   = (size_t) Z_LVAL_P(size);

    ZVAL_STRINGL(return_value, p + off, n);
}
