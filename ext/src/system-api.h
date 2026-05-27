#ifndef PHP_SYSTEM_API_H
#define PHP_SYSTEM_API_H

#include "php.h"

int      posix_open(zval *device_path, zval *flags, zval *mode);
int      posix_close(zval *fd);
void     posix_read(zval *return_value, zval *fd, zval *bytes_to_read);
zend_long posix_write(zval *fd, zval *data, zval *bytes_to_write);
int      posix_fcntl(zval *fd, zval *cmd, zval *arg, zval *return_value);
int      posix_ioctl(zval *fd, zval *cmd, zval *arg, zval *return_value);
int      posix_chmod(zval *path, zval *mode);
int      posix_chown(zval *path, zval *owner, zval *group);
int      posix_fchmod(zval *fd, zval *mode);
int      posix_fchown(zval *fd, zval *owner, zval *group);
zend_long posix_getuid(void);
int      posix_setuid(zval *uid);
zend_long posix_umask(zval *mask);
zend_long posix_lseek(zval *fd, zval *offset, zval *whence);
zend_long posix_ppoll(zval *fd, zval *timeout_ns, zval *events);
void     posix_recv(zval *return_value, zval *fd, zval *len, zval *flags);
void     posix_readv(zval *return_value, zval *fd, zval *iovecs);
zend_long posix_wait(zval *status_out);
zend_long posix_waitpid(zval *pid, zval *status_out, zval *options);
void     posix_hostname(zval *return_value);
void     posix_lstat(zval *return_value, zval *path);

#endif /* PHP_SYSTEM_API_H */