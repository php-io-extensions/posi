
/* This file was generated automatically by Zephir do not modify it! */

#ifndef PHP_POSI_H
#define PHP_POSI_H 1

#ifdef PHP_WIN32
#define ZEPHIR_RELEASE 1
#endif

#include "kernel/globals.h"

#define PHP_POSI_NAME        "posi"
#define PHP_POSI_VERSION     "0.4.3"
#define PHP_POSI_EXTNAME     "posi"
#define PHP_POSI_AUTHOR      "Project Saturn Studios, LLC"
#define PHP_POSI_ZEPVERSION  "0.19.0-$Id$"
#define PHP_POSI_DESCRIPTION "A Truly PHP-Controllable POSIX Extension"



ZEND_BEGIN_MODULE_GLOBALS(posi)

	int initialized;

	/** Function cache */
	HashTable *fcache;

	zephir_fcall_cache_entry *scache[ZEPHIR_MAX_CACHE_SLOTS];

	/* Cache enabled */
	unsigned int cache_enabled;

	/* Max recursion control */
	unsigned int recursive_lock;

	
ZEND_END_MODULE_GLOBALS(posi)

#ifdef ZTS
#include "TSRM.h"
#endif

ZEND_EXTERN_MODULE_GLOBALS(posi)

#ifdef ZTS
	#define ZEPHIR_GLOBAL(v) ZEND_MODULE_GLOBALS_ACCESSOR(posi, v)
#else
	#define ZEPHIR_GLOBAL(v) (posi_globals.v)
#endif

#ifdef ZTS
	ZEND_TSRMLS_CACHE_EXTERN()
	#define ZEPHIR_VGLOBAL ((zend_posi_globals *) (*((void ***) tsrm_get_ls_cache()))[TSRM_UNSHUFFLE_RSRC_ID(posi_globals_id)])
#else
	#define ZEPHIR_VGLOBAL &(posi_globals)
#endif

#define ZEPHIR_API ZEND_API

#define zephir_globals_def posi_globals
#define zend_zephir_globals_def zend_posi_globals

extern zend_module_entry posi_module_entry;
#define phpext_posi_ptr &posi_module_entry

#endif
