PHP_ARG_ENABLE([posi],
  [whether to enable posi support],
  [AS_HELP_STRING([--enable-posi], [Enable POSIX descriptor, process and termios bindings])],
  [no])

if test "$PHP_POSI" != "no"; then
  AC_CHECK_HEADERS([fcntl.h poll.h termios.h sys/uio.h sys/wait.h], [],
    [AC_MSG_ERROR([posi requires the POSIX headers fcntl.h, poll.h, termios.h, sys/uio.h and sys/wait.h])])
  PHP_NEW_EXTENSION([posi], [posi.c posi_termios.c], [$ext_shared],, [-DZEND_ENABLE_STATIC_TSRMLS_CACHE=1])
  PHP_ADD_EXTENSION_DEP([posi], [posix])
fi
