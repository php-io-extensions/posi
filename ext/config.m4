PHP_ARG_ENABLE(posi, whether to enable posi, [ --enable-posi   Enable Posi])

if test "$PHP_POSI" = "yes"; then

	

	if ! test "x" = "x"; then
		PHP_EVAL_LIBLINE(, POSI_SHARED_LIBADD)
	fi

	AC_DEFINE(HAVE_POSI, 1, [Whether you have Posi])
	CFLAGS="$CFLAGS -Wno-error=incompatible-pointer-types -Wno-incompatible-pointer-types -Wno-pointer-compare -Wno-error=int-conversion"
	posi_sources="posi.c kernel/main.c kernel/memory.c kernel/exception.c kernel/debug.c kernel/backtrace.c kernel/object.c kernel/array.c kernel/string.c kernel/fcall.c kernel/require.c kernel/file.c kernel/operators.c kernel/math.c kernel/concat.c kernel/variables.c kernel/filter.c kernel/iterator.c kernel/time.c kernel/exit.c posi/system.zep.c src/system-api.c"
	PHP_NEW_EXTENSION(posi, $posi_sources, $ext_shared,, )
	PHP_ADD_BUILD_DIR([$ext_builddir/kernel/])
	for dir in "posi" "src"; do
		PHP_ADD_BUILD_DIR([$ext_builddir/$dir])
	done
	PHP_SUBST(POSI_SHARED_LIBADD)

	old_CPPFLAGS=$CPPFLAGS
	CPPFLAGS="$CPPFLAGS $INCLUDES"

	AC_CHECK_DECL(
		[HAVE_BUNDLED_PCRE],
		[
			AC_CHECK_HEADERS(
				[ext/pcre/php_pcre.h],
				[
					PHP_ADD_EXTENSION_DEP([posi], [pcre])
					AC_DEFINE([ZEPHIR_USE_PHP_PCRE], [1], [Whether PHP pcre extension is present at compile time])
				],
				,
				[[#include "main/php.h"]]
			)
		],
		,
		[[#include "php_config.h"]]
	)

	AC_CHECK_DECL(
		[HAVE_JSON],
		[
			AC_CHECK_HEADERS(
				[ext/json/php_json.h],
				[
					PHP_ADD_EXTENSION_DEP([posi], [json])
					AC_DEFINE([ZEPHIR_USE_PHP_JSON], [1], [Whether PHP json extension is present at compile time])
				],
				,
				[[#include "main/php.h"]]
			)
		],
		,
		[[#include "php_config.h"]]
	)

	CPPFLAGS=$old_CPPFLAGS

	PHP_INSTALL_HEADERS([ext/posi], [php_POSI.h])

fi
