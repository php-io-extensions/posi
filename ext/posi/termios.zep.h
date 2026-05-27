
extern zend_class_entry *posi_termios_ce;

ZEPHIR_INIT_CLASS(Posi_Termios);

PHP_METHOD(Posi_Termios, tcgetattr);
PHP_METHOD(Posi_Termios, tcsetattr);
PHP_METHOD(Posi_Termios, cfsetispeed);
PHP_METHOD(Posi_Termios, cfsetospeed);
PHP_METHOD(Posi_Termios, cfgetispeed);
PHP_METHOD(Posi_Termios, cfgetospeed);
PHP_METHOD(Posi_Termios, tcdrain);
PHP_METHOD(Posi_Termios, tcflush);
PHP_METHOD(Posi_Termios, tcflow);
PHP_METHOD(Posi_Termios, setBaudRate);

ZEND_BEGIN_ARG_INFO_EX(arginfo_posi_termios_tcgetattr, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, fd, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_posi_termios_tcsetattr, 0, 3, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, fd, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, action, IS_LONG, 0)
	ZEND_ARG_ARRAY_INFO(0, termios, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_posi_termios_cfsetispeed, 0, 0, 2)
	ZEND_ARG_ARRAY_INFO(0, termios, 0)
	ZEND_ARG_TYPE_INFO(0, speed, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_posi_termios_cfsetospeed, 0, 0, 2)
	ZEND_ARG_ARRAY_INFO(0, termios, 0)
	ZEND_ARG_TYPE_INFO(0, speed, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_posi_termios_cfgetispeed, 0, 1, IS_LONG, 0)
	ZEND_ARG_ARRAY_INFO(0, termios, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_posi_termios_cfgetospeed, 0, 1, IS_LONG, 0)
	ZEND_ARG_ARRAY_INFO(0, termios, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_posi_termios_tcdrain, 0, 1, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, fd, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_posi_termios_tcflush, 0, 2, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, fd, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, queue, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_posi_termios_tcflow, 0, 2, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, fd, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, action, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_posi_termios_setbaudrate, 0, 2, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, fd, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, baud, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(posi_termios_method_entry) {
	PHP_ME(Posi_Termios, tcgetattr, arginfo_posi_termios_tcgetattr, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Posi_Termios, tcsetattr, arginfo_posi_termios_tcsetattr, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Posi_Termios, cfsetispeed, arginfo_posi_termios_cfsetispeed, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Posi_Termios, cfsetospeed, arginfo_posi_termios_cfsetospeed, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Posi_Termios, cfgetispeed, arginfo_posi_termios_cfgetispeed, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Posi_Termios, cfgetospeed, arginfo_posi_termios_cfgetospeed, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Posi_Termios, tcdrain, arginfo_posi_termios_tcdrain, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Posi_Termios, tcflush, arginfo_posi_termios_tcflush, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Posi_Termios, tcflow, arginfo_posi_termios_tcflow, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Posi_Termios, setBaudRate, arginfo_posi_termios_setbaudrate, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_FE_END
};
