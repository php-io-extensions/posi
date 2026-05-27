#ifndef PHP_MEMORY_API_H
#define PHP_MEMORY_API_H

#include "php.h"

zend_long posi_mem_alloc(zval *size);
void      posi_mem_free(zval *ptr);
void      posi_mem_write(zval *ptr, zval *data, zval *offset);
void      posi_mem_read(zval *return_value, zval *ptr, zval *size, zval *offset);

#endif /* PHP_MEMORY_API_H */
