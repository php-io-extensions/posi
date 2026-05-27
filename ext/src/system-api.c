#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ext.h"
#include "system-api.h"
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <poll.h>
#include <stdint.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/uio.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#ifndef HOST_NAME_MAX
#ifdef _POSIX_HOST_NAME_MAX
#define HOST_NAME_MAX _POSIX_HOST_NAME_MAX
#else
#define HOST_NAME_MAX 256
#endif
#endif

static void posix_wait_assign_status(zval *zv, int wstatus)
{
    zval *target;

    if (!zv) {
        return;
    }

    if (Z_TYPE_P(zv) == IS_NULL) {
        return;
    }

#if PHP_VERSION_ID >= 70300
    if (Z_TYPE_P(zv) == IS_UNDEF) {
        return;
    }
#endif

    if (Z_ISREF_P(zv)) {
        target = Z_REFVAL_P(zv);
    } else {
        target = zv;
    }

    ZVAL_LONG(target, (zend_long) wstatus);
}

static int posix_fcntl_is_lock_cmd(int cmd)
{
#ifdef F_GETLK
    return cmd == F_GETLK || cmd == F_SETLK || cmd == F_SETLKW;
#else
    (void) cmd;
    return 0;
#endif
}

static int posix_fcntl_is_get_cmd(int cmd)
{
    return cmd == F_GETFD || cmd == F_GETFL
#ifdef F_GETOWN
        || cmd == F_GETOWN
#endif
#ifdef F_GETLK
        || cmd == F_GETLK
#endif
        ;
}

#ifdef F_GETLK
static int posix_fcntl_array_to_flock(zval *arg, struct flock *fl)
{
    zval *zv;

    memset(fl, 0, sizeof(*fl));

    if (Z_TYPE_P(arg) != IS_ARRAY) {
        return -1;
    }

    zv = zend_hash_str_find(Z_ARRVAL_P(arg), "type", sizeof("type") - 1);
    if (zv) {
        fl->l_type = (short) zval_get_long(zv);
    }

    zv = zend_hash_str_find(Z_ARRVAL_P(arg), "whence", sizeof("whence") - 1);
    if (zv) {
        fl->l_whence = (short) zval_get_long(zv);
    }

    zv = zend_hash_str_find(Z_ARRVAL_P(arg), "start", sizeof("start") - 1);
    if (zv) {
        fl->l_start = (off_t) zval_get_long(zv);
    }

    zv = zend_hash_str_find(Z_ARRVAL_P(arg), "len", sizeof("len") - 1);
    if (zv) {
        fl->l_len = (off_t) zval_get_long(zv);
    }

    zv = zend_hash_str_find(Z_ARRVAL_P(arg), "pid", sizeof("pid") - 1);
    if (zv) {
        fl->l_pid = (pid_t) zval_get_long(zv);
    }

    return 0;
}

static void posix_fcntl_flock_to_array(struct flock *fl, zval *return_value)
{
    array_init(return_value);
    add_assoc_long(return_value, "type", (zend_long) fl->l_type);
    add_assoc_long(return_value, "whence", (zend_long) fl->l_whence);
    add_assoc_long(return_value, "start", (zend_long) fl->l_start);
    add_assoc_long(return_value, "len", (zend_long) fl->l_len);
    add_assoc_long(return_value, "pid", (zend_long) fl->l_pid);
}
#endif

static void posix_fcntl_set_val(int cmd, int ret, zval *arg, zval *return_value)
{
    if (ret < 0) {
        ZVAL_FALSE(return_value);
        return;
    }

    if (posix_fcntl_is_get_cmd(cmd) && !posix_fcntl_is_lock_cmd(cmd)) {
        ZVAL_LONG(return_value, (zend_long) ret);
        return;
    }

    if (Z_TYPE_P(arg) == IS_NULL) {
        ZVAL_NULL(return_value);
        return;
    }

    ZVAL_COPY(return_value, arg);
}

int posix_close(zval *fd) {
    return close((int) Z_LVAL_P(fd));
}

int posix_open(zval *dp, zval *flags, zval *mode) {
    int fl = (int) Z_LVAL_P(flags);

    if (fl & O_CREAT) {
        return open(Z_STRVAL_P(dp), fl, (mode_t) Z_LVAL_P(mode));
    }

    return open(Z_STRVAL_P(dp), fl);
}

void posix_read(zval *return_value, zval *fd, zval *bytes_to_read) {
    size_t n   = (size_t) Z_LVAL_P(bytes_to_read);
    char  *buf = emalloc(n + 1);
    ssize_t read_n = read((int) Z_LVAL_P(fd), buf, n);
    if (read_n < 0) {
        efree(buf);
        ZVAL_FALSE(return_value);
    } else {
        buf[read_n] = '\0';
        ZVAL_STRINGL(return_value, buf, (size_t) read_n);
        efree(buf);
    }
}

zend_long posix_write(zval *fd, zval *data, zval *bytes_to_write) {
    return (zend_long) write(
        (int)         Z_LVAL_P(fd),
        (const char*) Z_STRVAL_P(data),
        (size_t)      Z_LVAL_P(bytes_to_write)
    );
}

int posix_chmod(zval *path, zval *mode)
{
    return chmod(Z_STRVAL_P(path), (mode_t) Z_LVAL_P(mode));
}

int posix_chown(zval *path, zval *owner, zval *group)
{
    return chown(Z_STRVAL_P(path), (uid_t) Z_LVAL_P(owner), (gid_t) Z_LVAL_P(group));
}

int posix_fchmod(zval *fd, zval *mode)
{
    return fchmod((int) Z_LVAL_P(fd), (mode_t) Z_LVAL_P(mode));
}

int posix_fchown(zval *fd, zval *owner, zval *group)
{
    return fchown((int) Z_LVAL_P(fd), (uid_t) Z_LVAL_P(owner), (gid_t) Z_LVAL_P(group));
}

zend_long posix_getuid(void)
{
    return (zend_long) getuid();
}

int posix_setuid(zval *uid)
{
    return setuid((uid_t) Z_LVAL_P(uid));
}

zend_long posix_umask(zval *mask)
{
    return (zend_long) umask((mode_t) Z_LVAL_P(mask));
}

zend_long posix_lseek(zval *fd, zval *offset, zval *whence)
{
    off_t ret = lseek((int) Z_LVAL_P(fd), (off_t) Z_LVAL_P(offset), (int) Z_LVAL_P(whence));

    if (ret == (off_t) -1) {
        return (zend_long) -1;
    }

    return (zend_long) ret;
}

static int ppoll_file(int fd, int64_t timeout_ns, int events, int *revents)
{
    struct pollfd pfd;
    int ret;

    memset(&pfd, 0, sizeof(pfd));
    pfd.fd = fd;
    pfd.events = (short) (events == 0 ? POLLIN : events);

#if defined(__linux__)
    {
        struct timespec ts;
        struct timespec *ts_ptr = NULL;

        if (timeout_ns >= 0) {
            ts.tv_sec = (time_t) (timeout_ns / 1000000000LL);
            ts.tv_nsec = (long) (timeout_ns % 1000000000LL);
            ts_ptr = &ts;
        }

        ret = ppoll(&pfd, 1, ts_ptr, NULL);
    }
#else
    {
        int timeout_ms = -1;

        if (timeout_ns >= 0) {
            int64_t timeout_ms64 = (timeout_ns + 999999LL) / 1000000LL;

            if (timeout_ms64 > INT_MAX) {
                timeout_ms64 = INT_MAX;
            }
            timeout_ms = (int) timeout_ms64;
        }

        ret = poll(&pfd, 1, timeout_ms);
    }
#endif

    if (ret <= 0) {
        if (revents) {
            *revents = 0;
        }
        return ret;
    }

    if (revents) {
        *revents = (int) pfd.revents;
    }

    return 1;
}

zend_long posix_ppoll(zval *fd, zval *timeout_ns, zval *events)
{
    int revents = 0;

    return (zend_long) ppoll_file(
        (int) zval_get_long(fd),
        (int64_t) zval_get_long(timeout_ns),
        (int) zval_get_long(events),
        &revents
    );
}

void posix_recv(zval *return_value, zval *fd, zval *len, zval *flags)
{
    size_t   n      = (size_t) Z_LVAL_P(len);
    int      fl     = (int) Z_LVAL_P(flags);
    char    *buf    = emalloc(n + 1);
    ssize_t recv_n = recv((int) Z_LVAL_P(fd), buf, n, fl);

    if (recv_n < 0) {
        efree(buf);
        ZVAL_FALSE(return_value);
    } else {
        buf[recv_n] = '\0';
        ZVAL_STRINGL(return_value, buf, (size_t) recv_n);
        efree(buf);
    }
}

void posix_readv(zval *return_value, zval *fd, zval *iovecs)
{
    zval *elem;
    struct iovec *iov = NULL;
    zend_ulong i, num;
    ssize_t nread;
    ssize_t remaining;
    zval buffers;

    if (Z_TYPE_P(iovecs) != IS_ARRAY) {
        ZVAL_FALSE(return_value);
        return;
    }

    num = zend_hash_num_elements(Z_ARRVAL_P(iovecs));
    if (num == 0) {
        array_init(return_value);
        add_assoc_long(return_value, "res", 0);
        array_init(&buffers);
        add_assoc_zval(return_value, "buffers", &buffers);
        return;
    }

#ifdef IOV_MAX
    if (num > (zend_ulong) IOV_MAX) {
        ZVAL_FALSE(return_value);
        return;
    }
#endif

    iov = safe_emalloc(num, sizeof(struct iovec), 0);

    i = 0;
    ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(iovecs), elem) {
        zval *zv_len;
        zval *zv_base;
        zend_long iov_len_long;
        size_t iov_len;
        char *buf;

        if (Z_TYPE_P(elem) != IS_ARRAY) {
            goto fail_free_iov;
        }

        zv_len = zend_hash_str_find(Z_ARRVAL_P(elem), "len", sizeof("len") - 1);
        if (!zv_len) {
            goto fail_free_iov;
        }

        iov_len_long = zval_get_long(zv_len);
        if (iov_len_long < 0) {
            goto fail_free_iov;
        }

        iov_len = (size_t) iov_len_long;

        zv_base = zend_hash_str_find(Z_ARRVAL_P(elem), "base", sizeof("base") - 1);
        buf     = emalloc(iov_len > 0 ? iov_len : 1);

        if (zv_base && Z_TYPE_P(zv_base) == IS_STRING && Z_STRLEN_P(zv_base) > 0 && iov_len > 0) {
            size_t copy = Z_STRLEN_P(zv_base);

            if (copy > iov_len) {
                copy = iov_len;
            }
            memcpy(buf, Z_STRVAL_P(zv_base), copy);
        }

        iov[i].iov_base = buf;
        iov[i].iov_len  = iov_len;
        i++;
    }
    ZEND_HASH_FOREACH_END();

    nread = readv((int) Z_LVAL_P(fd), iov, (int) num);
    if (nread < 0) {
        goto fail_free_iov;
    }

    array_init(return_value);
    add_assoc_long(return_value, "res", (zend_long) nread);

    array_init(&buffers);
    remaining = nread;
    for (i = 0; i < num; i++) {
        size_t cap   = iov[i].iov_len;
        size_t chunk = 0;

        if (remaining > 0) {
            chunk = (size_t) remaining;
            if (chunk > cap) {
                chunk = cap;
            }
        }

        add_next_index_stringl(&buffers, (char *) iov[i].iov_base, chunk);
        if (remaining > 0) {
            remaining -= (ssize_t) chunk;
        }
    }
    add_assoc_zval(return_value, "buffers", &buffers);

    for (i = 0; i < num; i++) {
        efree(iov[i].iov_base);
    }
    efree(iov);

    return;

fail_free_iov:
    if (iov) {
        zend_ulong j;

        for (j = 0; j < i; j++) {
            efree(iov[j].iov_base);
        }
        efree(iov);
    }
    ZVAL_FALSE(return_value);
}

int posix_fcntl(zval *fd, zval *cmd, zval *arg, zval *return_value)
{
    int f = (int) Z_LVAL_P(fd);
    int c = (int) Z_LVAL_P(cmd);
    int ret;

#ifdef F_GETLK
    if (posix_fcntl_is_lock_cmd(c)) {
        struct flock fl;

        if (posix_fcntl_array_to_flock(arg, &fl) != 0) {
            ZVAL_FALSE(return_value);
            return -1;
        }

        ret = fcntl(f, c, &fl);
        if (ret < 0) {
            ZVAL_FALSE(return_value);
            return -1;
        }

        posix_fcntl_flock_to_array(&fl, return_value);
        return ret;
    }
#else
    if (posix_fcntl_is_lock_cmd(c)) {
        ZVAL_FALSE(return_value);
        return -1;
    }
#endif

    if (Z_TYPE_P(arg) == IS_NULL) {
        ret = fcntl(f, c);
    } else if (Z_TYPE_P(arg) == IS_LONG || Z_TYPE_P(arg) == IS_DOUBLE) {
        ret = fcntl(f, c, (int) zval_get_long(arg));
    } else if (Z_TYPE_P(arg) == IS_TRUE || Z_TYPE_P(arg) == IS_FALSE) {
        ret = fcntl(f, c, Z_TYPE_P(arg) == IS_TRUE ? 1 : 0);
    } else {
        ZVAL_FALSE(return_value);
        return -1;
    }

    posix_fcntl_set_val(c, ret, arg, return_value);
    return ret;
}

static void posix_ioctl_set_val(int ret, zval *arg, zval *return_value)
{
    if (ret < 0) {
        ZVAL_FALSE(return_value);
        return;
    }

    if (Z_TYPE_P(arg) == IS_NULL) {
        if (ret > 0) {
            ZVAL_LONG(return_value, (zend_long) ret);
        } else {
            ZVAL_NULL(return_value);
        }
        return;
    }

    if (Z_TYPE_P(arg) == IS_LONG || Z_TYPE_P(arg) == IS_DOUBLE
        || Z_TYPE_P(arg) == IS_TRUE || Z_TYPE_P(arg) == IS_FALSE) {
        ZVAL_COPY(return_value, arg);
        return;
    }

    ZVAL_COPY(return_value, arg);
}

static int posix_ioctl_array(int f, unsigned long c, zval *arg, zval *return_value)
{
    zval *zv;
    int ret;

    zv = zend_hash_str_find(Z_ARRVAL_P(arg), "value", sizeof("value") - 1);
    if (zv) {
        int v = (int) zval_get_long(zv);

        ret = ioctl(f, c, &v);
        if (ret < 0) {
            ZVAL_FALSE(return_value);
            return -1;
        }

        ZVAL_LONG(return_value, (zend_long) v);
        return ret;
    }

    zv = zend_hash_str_find(Z_ARRVAL_P(arg), "bytes", sizeof("bytes") - 1);
    if (!zv) {
        zv = zend_hash_str_find(Z_ARRVAL_P(arg), "data", sizeof("data") - 1);
    }

    if (zv && Z_TYPE_P(zv) == IS_STRING) {
        size_t len = Z_STRLEN_P(zv);
        char  *buf = emalloc(len + 1);

        if (len > 0) {
            memcpy(buf, Z_STRVAL_P(zv), len);
        }
        buf[len] = '\0';

        ret = ioctl(f, c, buf);
        if (ret < 0) {
            efree(buf);
            ZVAL_FALSE(return_value);
            return -1;
        }

        ZVAL_STRINGL(return_value, buf, len);
        efree(buf);
        return ret;
    }

    ZVAL_FALSE(return_value);
    return -1;
}

int posix_ioctl(zval *fd, zval *cmd, zval *arg, zval *return_value)
{
    int f = (int) Z_LVAL_P(fd);
    unsigned long c = (unsigned long) zval_get_long(cmd);
    int ret;

    if (Z_TYPE_P(arg) == IS_ARRAY) {
        return posix_ioctl_array(f, c, arg, return_value);
    }

    if (Z_TYPE_P(arg) == IS_STRING) {
        size_t len = Z_STRLEN_P(arg);
        char  *buf = emalloc(len + 1);

        if (len > 0) {
            memcpy(buf, Z_STRVAL_P(arg), len);
        }
        buf[len] = '\0';

        ret = ioctl(f, c, buf);
        if (ret < 0) {
            efree(buf);
            ZVAL_FALSE(return_value);
            return -1;
        }

        ZVAL_STRINGL(return_value, buf, len);
        efree(buf);
        return ret;
    }

    if (Z_TYPE_P(arg) == IS_NULL) {
        ret = ioctl(f, c, NULL);
        posix_ioctl_set_val(ret, arg, return_value);
        return ret;
    }

    if (Z_TYPE_P(arg) == IS_LONG || Z_TYPE_P(arg) == IS_DOUBLE) {
        ret = ioctl(f, c, (void *) (intptr_t) zval_get_long(arg));
        posix_ioctl_set_val(ret, arg, return_value);
        return ret;
    }

    if (Z_TYPE_P(arg) == IS_TRUE || Z_TYPE_P(arg) == IS_FALSE) {
        ret = ioctl(f, c, (void *) (intptr_t) (Z_TYPE_P(arg) == IS_TRUE ? 1 : 0));
        posix_ioctl_set_val(ret, arg, return_value);
        return ret;
    }

    ZVAL_FALSE(return_value);
    return -1;
}

zend_long posix_wait(zval *status_out)
{
    int      wstatus = 0;
    pid_t    pid;

    pid = waitpid(-1, &wstatus, 0);

    if (pid > 0) {
        posix_wait_assign_status(status_out, wstatus);
    }

    return (zend_long) pid;
}

zend_long posix_waitpid(zval *pid, zval *status_out, zval *options)
{
    int      wstatus = 0;
    pid_t    ret;
    pid_t    p     = (pid_t) Z_LVAL_P(pid);
    int      opts  = (int) Z_LVAL_P(options);

    ret = waitpid(p, &wstatus, opts);

    if (ret > 0) {
        posix_wait_assign_status(status_out, wstatus);
    }

    return (zend_long) ret;
}

void posix_hostname(zval *return_value)
{
    char buf[HOST_NAME_MAX + 1];

    if (gethostname(buf, sizeof(buf)) != 0) {
        ZVAL_FALSE(return_value);
        return;
    }

    buf[HOST_NAME_MAX] = '\0';
    ZVAL_STRING(return_value, buf);
}

void posix_lstat(zval *return_value, zval *path)
{
    struct stat st;

    if (lstat(Z_STRVAL_P(path), &st) != 0) {
        ZVAL_FALSE(return_value);
        return;
    }

    array_init(return_value);
    add_assoc_long(return_value, "dev",     (zend_long) st.st_dev);
    add_assoc_long(return_value, "ino",     (zend_long) st.st_ino);
    add_assoc_long(return_value, "mode",    (zend_long) st.st_mode);
    add_assoc_long(return_value, "nlink",   (zend_long) st.st_nlink);
    add_assoc_long(return_value, "uid",     (zend_long) st.st_uid);
    add_assoc_long(return_value, "gid",     (zend_long) st.st_gid);
    add_assoc_long(return_value, "rdev",    (zend_long) st.st_rdev);
    add_assoc_long(return_value, "size",    (zend_long) st.st_size);
    add_assoc_long(return_value, "blksize", (zend_long) st.st_blksize);
    add_assoc_long(return_value, "blocks",  (zend_long) st.st_blocks);
    add_assoc_long(return_value, "atime",   (zend_long) st.st_atime);
    add_assoc_long(return_value, "mtime",   (zend_long) st.st_mtime);
    add_assoc_long(return_value, "ctime",   (zend_long) st.st_ctime);
}

