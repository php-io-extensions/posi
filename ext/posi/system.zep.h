
extern zend_class_entry *posi_system_ce;

ZEPHIR_INIT_CLASS(Posi_System);

PHP_METHOD(Posi_System, chmod);
PHP_METHOD(Posi_System, chown);
PHP_METHOD(Posi_System, close);
PHP_METHOD(Posi_System, fchmod);
PHP_METHOD(Posi_System, fchown);
PHP_METHOD(Posi_System, getuid);
PHP_METHOD(Posi_System, setuid);
PHP_METHOD(Posi_System, umask);
PHP_METHOD(Posi_System, fcntl);
PHP_METHOD(Posi_System, ioctl);
PHP_METHOD(Posi_System, lseek);
PHP_METHOD(Posi_System, open);
PHP_METHOD(Posi_System, read);
PHP_METHOD(Posi_System, readv);
PHP_METHOD(Posi_System, recv);
PHP_METHOD(Posi_System, wait);
PHP_METHOD(Posi_System, waitpid);
PHP_METHOD(Posi_System, hostname);
PHP_METHOD(Posi_System, write);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_posi_system_chmod, 0, 2, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, path, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, mode, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_posi_system_chown, 0, 3, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, path, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, owner, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, group, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_posi_system_close, 0, 1, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, _fd, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_posi_system_fchmod, 0, 2, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, fd, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, mode, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_posi_system_fchown, 0, 3, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, fd, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, owner, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, group, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_posi_system_getuid, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_posi_system_setuid, 0, 1, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, uid, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_posi_system_umask, 0, 1, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, mask, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_posi_system_fcntl, 0, 2, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(0, fd, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, cmd, IS_LONG, 0)
	ZEND_ARG_INFO(0, arg)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_posi_system_ioctl, 0, 2, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(0, fd, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, cmd, IS_LONG, 0)
	ZEND_ARG_INFO(0, arg)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_posi_system_lseek, 0, 3, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, fd, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, offset, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, whence, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_posi_system_open, 0, 2, IS_LONG, 0)
	ZEND_ARG_INFO(0, device_path)
	ZEND_ARG_TYPE_INFO(0, flags, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, mode, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_posi_system_read, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, fd, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, bytes_to_read, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_posi_system_readv, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, fd, IS_LONG, 0)
	ZEND_ARG_ARRAY_INFO(0, iovecs, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_posi_system_recv, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, fd, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, len, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, flags, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_posi_system_wait, 0, 0, IS_LONG, 0)
	ZEND_ARG_INFO(0, status)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_posi_system_waitpid, 0, 1, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, pid, IS_LONG, 0)
	ZEND_ARG_INFO(0, status)
	ZEND_ARG_TYPE_INFO(0, options, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_posi_system_hostname, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_posi_system_write, 0, 3, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, fd, IS_LONG, 0)
	ZEND_ARG_INFO(0, data)
	ZEND_ARG_TYPE_INFO(0, bytes_to_write, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(posi_system_method_entry) {
	PHP_ME(Posi_System, chmod, arginfo_posi_system_chmod, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Posi_System, chown, arginfo_posi_system_chown, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Posi_System, close, arginfo_posi_system_close, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Posi_System, fchmod, arginfo_posi_system_fchmod, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Posi_System, fchown, arginfo_posi_system_fchown, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Posi_System, getuid, arginfo_posi_system_getuid, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Posi_System, setuid, arginfo_posi_system_setuid, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Posi_System, umask, arginfo_posi_system_umask, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Posi_System, fcntl, arginfo_posi_system_fcntl, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Posi_System, ioctl, arginfo_posi_system_ioctl, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Posi_System, lseek, arginfo_posi_system_lseek, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Posi_System, open, arginfo_posi_system_open, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Posi_System, read, arginfo_posi_system_read, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Posi_System, readv, arginfo_posi_system_readv, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Posi_System, recv, arginfo_posi_system_recv, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Posi_System, wait, arginfo_posi_system_wait, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Posi_System, waitpid, arginfo_posi_system_waitpid, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
PHP_ME(Posi_System, hostname, arginfo_posi_system_hostname, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Posi_System, write, arginfo_posi_system_write, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_FE_END
};
