
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
#include "src/system-api.h"
#include "kernel/operators.h"
#include "kernel/memory.h"
#include "kernel/object.h"
#include "kernel/array.h"
#include "kernel/string.h"


ZEPHIR_INIT_CLASS(Posi_System)
{
	ZEPHIR_REGISTER_CLASS(Posi, System, posi, system, posi_system_method_entry, 0);

	return SUCCESS;
}

PHP_METHOD(Posi_System, chmod)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long mode;
	zval *path_param = NULL, *mode_param = NULL, _0;
	zval path;

	ZVAL_UNDEF(&path);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STR(path)
		Z_PARAM_LONG(mode)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &path_param, &mode_param);
	zephir_get_strval(&path, path_param);
	ZVAL_LONG(&_0, mode);
	RETURN_MM_LONG(posix_chmod(&path, &_0));
}

PHP_METHOD(Posi_System, chown)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long owner, group;
	zval *path_param = NULL, *owner_param = NULL, *group_param = NULL, _0, _1;
	zval path;

	ZVAL_UNDEF(&path);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEND_PARSE_PARAMETERS_START(3, 3)
		Z_PARAM_STR(path)
		Z_PARAM_LONG(owner)
		Z_PARAM_LONG(group)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 3, 0, &path_param, &owner_param, &group_param);
	zephir_get_strval(&path, path_param);
	ZVAL_LONG(&_0, owner);
	ZVAL_LONG(&_1, group);
	RETURN_MM_LONG(posix_chown(&path, &_0, &_1));
}

PHP_METHOD(Posi_System, close)
{
	zval *_fd_param = NULL, _0;
	zend_long _fd, result = 0;

	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(_fd)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &_fd_param);
	ZVAL_LONG(&_0, _fd);
	result = posix_close(&_0);
	RETURN_LONG(result);
}

PHP_METHOD(Posi_System, fchmod)
{
	zval *fd_param = NULL, *mode_param = NULL, _0, _1;
	zend_long fd, mode;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_LONG(fd)
		Z_PARAM_LONG(mode)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(2, 0, &fd_param, &mode_param);
	ZVAL_LONG(&_0, fd);
	ZVAL_LONG(&_1, mode);
	RETURN_LONG(posix_fchmod(&_0, &_1));
}

PHP_METHOD(Posi_System, fchown)
{
	zval *fd_param = NULL, *owner_param = NULL, *group_param = NULL, _0, _1, _2;
	zend_long fd, owner, group;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZEND_PARSE_PARAMETERS_START(3, 3)
		Z_PARAM_LONG(fd)
		Z_PARAM_LONG(owner)
		Z_PARAM_LONG(group)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(3, 0, &fd_param, &owner_param, &group_param);
	ZVAL_LONG(&_0, fd);
	ZVAL_LONG(&_1, owner);
	ZVAL_LONG(&_2, group);
	RETURN_LONG(posix_fchown(&_0, &_1, &_2));
}

PHP_METHOD(Posi_System, getuid)
{

	RETURN_LONG(posix_getuid());
}

PHP_METHOD(Posi_System, setuid)
{
	zval *uid_param = NULL, _0;
	zend_long uid;

	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(uid)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &uid_param);
	ZVAL_LONG(&_0, uid);
	RETURN_LONG(posix_setuid(&_0));
}

PHP_METHOD(Posi_System, umask)
{
	zval *mask_param = NULL, _0;
	zend_long mask;

	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(mask)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &mask_param);
	ZVAL_LONG(&_0, mask);
	RETURN_LONG(posix_umask(&_0));
}

PHP_METHOD(Posi_System, fcntl)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *fd_param = NULL, *cmd_param = NULL, *arg = NULL, arg_sub, __$null, val, _0, _1, _2;
	zend_long fd, cmd, res = 0;

	ZVAL_UNDEF(&arg_sub);
	ZVAL_NULL(&__$null);
	ZVAL_UNDEF(&val);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(2, 3)
		Z_PARAM_LONG(fd)
		Z_PARAM_LONG(cmd)
		Z_PARAM_OPTIONAL
		Z_PARAM_ZVAL_OR_NULL(arg)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 1, &fd_param, &cmd_param, &arg);
	if (!arg) {
		arg = &arg_sub;
		arg = &__$null;
	}
	ZVAL_LONG(&_0, fd);
	ZVAL_LONG(&_1, cmd);
	res = posix_fcntl(&_0, &_1, arg, &val);
	zephir_create_array(return_value, 2, 0);
	ZEPHIR_INIT_VAR(&_2);
	ZVAL_LONG(&_2, res);
	zephir_array_update_string(return_value, SL("res"), &_2, PH_COPY | PH_SEPARATE);
	zephir_array_update_string(return_value, SL("val"), &val, PH_COPY | PH_SEPARATE);
	RETURN_MM();
}

PHP_METHOD(Posi_System, ioctl)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *fd_param = NULL, *cmd_param = NULL, *arg = NULL, arg_sub, __$null, val, _0, _1, _2;
	zend_long fd, cmd, res = 0;

	ZVAL_UNDEF(&arg_sub);
	ZVAL_NULL(&__$null);
	ZVAL_UNDEF(&val);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(2, 3)
		Z_PARAM_LONG(fd)
		Z_PARAM_LONG(cmd)
		Z_PARAM_OPTIONAL
		Z_PARAM_ZVAL_OR_NULL(arg)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 1, &fd_param, &cmd_param, &arg);
	if (!arg) {
		arg = &arg_sub;
		arg = &__$null;
	}
	ZVAL_LONG(&_0, fd);
	ZVAL_LONG(&_1, cmd);
	res = posix_ioctl(&_0, &_1, arg, &val);
	zephir_create_array(return_value, 2, 0);
	ZEPHIR_INIT_VAR(&_2);
	ZVAL_LONG(&_2, res);
	zephir_array_update_string(return_value, SL("res"), &_2, PH_COPY | PH_SEPARATE);
	zephir_array_update_string(return_value, SL("val"), &val, PH_COPY | PH_SEPARATE);
	RETURN_MM();
}

PHP_METHOD(Posi_System, lseek)
{
	zval *fd_param = NULL, *offset_param = NULL, *whence_param = NULL, _0, _1, _2;
	zend_long fd, offset, whence;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZEND_PARSE_PARAMETERS_START(3, 3)
		Z_PARAM_LONG(fd)
		Z_PARAM_LONG(offset)
		Z_PARAM_LONG(whence)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(3, 0, &fd_param, &offset_param, &whence_param);
	ZVAL_LONG(&_0, fd);
	ZVAL_LONG(&_1, offset);
	ZVAL_LONG(&_2, whence);
	RETURN_LONG(posix_lseek(&_0, &_1, &_2));
}

PHP_METHOD(Posi_System, open)
{
	zval dp;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long flags, mode, result = 0;
	zval *device_path = NULL, device_path_sub, *flags_param = NULL, *mode_param = NULL, _0, _1;

	ZVAL_UNDEF(&device_path_sub);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&dp);
	ZEND_PARSE_PARAMETERS_START(2, 3)
		Z_PARAM_ZVAL(device_path)
		Z_PARAM_LONG(flags)
		Z_PARAM_OPTIONAL
		Z_PARAM_LONG(mode)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 1, &device_path, &flags_param, &mode_param);
	if (!mode_param) {
		mode = 0644;
	} else {
		}
	zephir_get_strval(&dp, device_path);
	ZVAL_LONG(&_0, flags);
	ZVAL_LONG(&_1, mode);
	result = posix_open(&dp, &_0, &_1);
	RETURN_MM_LONG(result);
}

PHP_METHOD(Posi_System, read)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *fd_param = NULL, *bytes_to_read_param = NULL, result, _0, _1;
	zend_long fd, bytes_to_read;

	ZVAL_UNDEF(&result);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_LONG(fd)
		Z_PARAM_LONG(bytes_to_read)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &fd_param, &bytes_to_read_param);
	ZVAL_LONG(&_0, fd);
	ZVAL_LONG(&_1, bytes_to_read);
	ZEPHIR_INIT_VAR(&result);
	posix_read(&result, &_0, &_1);
	RETURN_CCTOR(&result);
}

PHP_METHOD(Posi_System, readv)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval iovecs;
	zval *fd_param = NULL, *iovecs_param = NULL, result, _0;
	zend_long fd;

	ZVAL_UNDEF(&result);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&iovecs);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_LONG(fd)
		Z_PARAM_ARRAY(iovecs)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &fd_param, &iovecs_param);
	zephir_get_arrval(&iovecs, iovecs_param);
	ZVAL_LONG(&_0, fd);
	ZEPHIR_INIT_VAR(&result);
	posix_readv(&result, &_0, &iovecs);
	RETURN_CCTOR(&result);
}

PHP_METHOD(Posi_System, recv)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *fd_param = NULL, *len_param = NULL, *flags_param = NULL, result, _0, _1, _2;
	zend_long fd, len, flags;

	ZVAL_UNDEF(&result);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZEND_PARSE_PARAMETERS_START(2, 3)
		Z_PARAM_LONG(fd)
		Z_PARAM_LONG(len)
		Z_PARAM_OPTIONAL
		Z_PARAM_LONG(flags)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 1, &fd_param, &len_param, &flags_param);
	if (!flags_param) {
		flags = 0;
	} else {
		}
	ZVAL_LONG(&_0, fd);
	ZVAL_LONG(&_1, len);
	ZVAL_LONG(&_2, flags);
	ZEPHIR_INIT_VAR(&result);
	posix_recv(&result, &_0, &_1, &_2);
	RETURN_CCTOR(&result);
}

PHP_METHOD(Posi_System, wait)
{
	zval *status = NULL, status_sub, __$null;

	ZVAL_UNDEF(&status_sub);
	ZVAL_NULL(&__$null);
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(0, 1)
		Z_PARAM_OPTIONAL
		Z_PARAM_ZVAL_OR_NULL(status)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(0, 1, &status);
	if (!status) {
		status = &status_sub;
		status = &__$null;
	}
	RETURN_LONG(posix_wait(status));
}

PHP_METHOD(Posi_System, waitpid)
{
	zval *pid_param = NULL, *status = NULL, status_sub, *options_param = NULL, __$null, _0, _1;
	zend_long pid, options;

	ZVAL_UNDEF(&status_sub);
	ZVAL_NULL(&__$null);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(1, 3)
		Z_PARAM_LONG(pid)
		Z_PARAM_OPTIONAL
		Z_PARAM_ZVAL_OR_NULL(status)
		Z_PARAM_LONG(options)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 2, &pid_param, &status, &options_param);
	if (!status) {
		status = &status_sub;
		status = &__$null;
	}
	if (!options_param) {
		options = 0;
	} else {
		}
	ZVAL_LONG(&_0, pid);
	ZVAL_LONG(&_1, options);
	RETURN_LONG(posix_waitpid(&_0, status, &_1));
}

PHP_METHOD(Posi_System, hostname)
{
	zval result;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&result);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&result);
	posix_hostname(&result);
	RETURN_CCTOR(&result);
}

PHP_METHOD(Posi_System, write)
{
	zval payload;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *fd_param = NULL, *data = NULL, data_sub, *bytes_to_write_param = NULL, _0, _1;
	zend_long fd, bytes_to_write, results = 0;

	ZVAL_UNDEF(&data_sub);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&payload);
	ZEND_PARSE_PARAMETERS_START(3, 3)
		Z_PARAM_LONG(fd)
		Z_PARAM_ZVAL(data)
		Z_PARAM_LONG(bytes_to_write)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 3, 0, &fd_param, &data, &bytes_to_write_param);
	zephir_get_strval(&payload, data);
	ZVAL_LONG(&_0, fd);
	ZVAL_LONG(&_1, bytes_to_write);
	results = posix_write(&_0, &payload, &_1);
	RETURN_MM_LONG(results);
}

