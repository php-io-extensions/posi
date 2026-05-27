
/* This file was generated automatically by Zephir do not modify it! */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <php.h>

#include "php_ext.h"
#include "posi.h"

#include <ext/standard/info.h>

#include <Zend/zend_operators.h>
#include <Zend/zend_exceptions.h>
#include <Zend/zend_interfaces.h>

#include "kernel/globals.h"
#include "kernel/main.h"
#include "kernel/fcall.h"
#include "kernel/memory.h"



zend_class_entry *posi_memory_ce;
zend_class_entry *posi_system_ce;
zend_class_entry *posi_termios_ce;

ZEND_DECLARE_MODULE_GLOBALS(posi)

PHP_INI_BEGIN()
	
PHP_INI_END()

static PHP_MINIT_FUNCTION(posi)
{
	REGISTER_INI_ENTRIES();
	zephir_module_init();
	ZEPHIR_INIT(Posi_Memory);
	ZEPHIR_INIT(Posi_System);
	ZEPHIR_INIT(Posi_Termios);
	
	return SUCCESS;
}

#ifndef ZEPHIR_RELEASE
static PHP_MSHUTDOWN_FUNCTION(posi)
{
	
	zephir_deinitialize_memory();
	UNREGISTER_INI_ENTRIES();
	return SUCCESS;
}
#endif

/**
 * Initialize globals on each request or each thread started
 */
static void php_zephir_init_globals(zend_posi_globals *posi_globals)
{
	posi_globals->initialized = 0;

	/* Cache Enabled */
	posi_globals->cache_enabled = 1;

	/* Recursive Lock */
	posi_globals->recursive_lock = 0;

	/* Static cache */
	memset(posi_globals->scache, '\0', sizeof(zephir_fcall_cache_entry*) * ZEPHIR_MAX_CACHE_SLOTS);

	
	
}

/**
 * Initialize globals only on each thread started
 */
static void php_zephir_init_module_globals(zend_posi_globals *posi_globals)
{
	
}

static PHP_RINIT_FUNCTION(posi)
{
	zend_posi_globals *posi_globals_ptr;
	posi_globals_ptr = ZEPHIR_VGLOBAL;

	php_zephir_init_globals(posi_globals_ptr);
	zephir_initialize_memory(posi_globals_ptr);

	
	return SUCCESS;
}

static PHP_RSHUTDOWN_FUNCTION(posi)
{
	
	zephir_deinitialize_memory();
	return SUCCESS;
}



static PHP_MINFO_FUNCTION(posi)
{
	php_info_print_box_start(0);
	php_printf("%s", PHP_POSI_DESCRIPTION);
	php_info_print_box_end();

	php_info_print_table_start();
	php_info_print_table_header(2, PHP_POSI_NAME, "enabled");
	php_info_print_table_row(2, "Author", PHP_POSI_AUTHOR);
	php_info_print_table_row(2, "Version", PHP_POSI_VERSION);
	php_info_print_table_row(2, "Build Date", __DATE__ " " __TIME__ );
	php_info_print_table_row(2, "Powered by Zephir", "Version " PHP_POSI_ZEPVERSION);
	php_info_print_table_end();
	
	DISPLAY_INI_ENTRIES();
}

static PHP_GINIT_FUNCTION(posi)
{
#if defined(COMPILE_DL_POSI) && defined(ZTS)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif

	php_zephir_init_globals(posi_globals);
	php_zephir_init_module_globals(posi_globals);
}

static PHP_GSHUTDOWN_FUNCTION(posi)
{
	
}


zend_function_entry php_posi_functions[] = {
	ZEND_FE_END

};

static const zend_module_dep php_posi_deps[] = {
	
	ZEND_MOD_END
};

zend_module_entry posi_module_entry = {
	STANDARD_MODULE_HEADER_EX,
	NULL,
	php_posi_deps,
	PHP_POSI_EXTNAME,
	php_posi_functions,
	PHP_MINIT(posi),
#ifndef ZEPHIR_RELEASE
	PHP_MSHUTDOWN(posi),
#else
	NULL,
#endif
	PHP_RINIT(posi),
	PHP_RSHUTDOWN(posi),
	PHP_MINFO(posi),
	PHP_POSI_VERSION,
	ZEND_MODULE_GLOBALS(posi),
	PHP_GINIT(posi),
	PHP_GSHUTDOWN(posi),
#ifdef ZEPHIR_POST_REQUEST
	PHP_PRSHUTDOWN(posi),
#else
	NULL,
#endif
	STANDARD_MODULE_PROPERTIES_EX
};

/* implement standard "stub" routine to introduce ourselves to Zend */
#ifdef COMPILE_DL_POSI
# ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
# endif
ZEND_GET_MODULE(posi)
#endif
