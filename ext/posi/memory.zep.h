
extern zend_class_entry *posi_memory_ce;

ZEPHIR_INIT_CLASS(Posi_Memory);

PHP_METHOD(Posi_Memory, alloc);
PHP_METHOD(Posi_Memory, free);
PHP_METHOD(Posi_Memory, write);
PHP_METHOD(Posi_Memory, read);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_posi_memory_alloc, 0, 1, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, size, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_posi_memory_free, 0, 1, IS_VOID, 0)

	ZEND_ARG_TYPE_INFO(0, ptr, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_posi_memory_write, 0, 2, IS_VOID, 0)

	ZEND_ARG_TYPE_INFO(0, ptr, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, data, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, offset, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_posi_memory_read, 0, 2, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, ptr, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, size, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, offset, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(posi_memory_method_entry) {
	PHP_ME(Posi_Memory, alloc, arginfo_posi_memory_alloc, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Posi_Memory, free, arginfo_posi_memory_free, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Posi_Memory, write, arginfo_posi_memory_write, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Posi_Memory, read, arginfo_posi_memory_read, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_FE_END
};
