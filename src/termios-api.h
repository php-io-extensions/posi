#ifndef PHP_TERMIOS_API_H
#define PHP_TERMIOS_API_H

#include "php.h"

/*
 * tcgetattr_file(fd) → array|false
 * Fills return_value with a PHP array of termios fields, or ZVAL_FALSE on error.
 */
void      posix_tcgetattr(zval *return_value, zval *fd);

/*
 * tcsetattr_file(fd, action, termios) → int
 * Converts PHP termios array to struct termios and calls tcsetattr(2).
 * Returns 0 on success, -1 on error.
 */
int       posix_tcsetattr(zval *fd, zval *action, zval *termios);

/*
 * cfsetispeed_termios(termios, speed) → array|false
 * Applies cfsetispeed(3) and returns the updated termios array, or ZVAL_FALSE on error.
 */
void      posix_cfsetispeed(zval *return_value, zval *termios, zval *speed);

/*
 * cfsetospeed_termios(termios, speed) → array|false
 * Applies cfsetospeed(3) and returns the updated termios array, or ZVAL_FALSE on error.
 */
void      posix_cfsetospeed(zval *return_value, zval *termios, zval *speed);

/*
 * cfgetispeed_termios(termios) → int
 * Returns the input baud rate encoded in the termios array.
 */
zend_long posix_cfgetispeed(zval *termios);

/*
 * cfgetospeed_termios(termios) → int
 * Returns the output baud rate encoded in the termios array.
 */
zend_long posix_cfgetospeed(zval *termios);

/*
 * tcdrain_file(fd) → int
 * Blocks until all output on fd has been transmitted. Returns 0 or -1.
 */
int       posix_tcdrain(zval *fd);

/*
 * tcflush_file(fd, queue) → int
 * Discards queued I/O on fd. Returns 0 or -1.
 */
int       posix_tcflush(zval *fd, zval *queue);

/*
 * tcflow_file(fd, action) → int
 * Suspends or resumes I/O on fd. Returns 0 or -1.
 */
int       posix_tcflow(zval *fd, zval *action);

/*
 * set_baud_rate_termios(fd, baud) → int
 * Sets input and output baud rate atomically.
 * For rates with a standard B* constant (50–4000000), uses cfsetispeed/cfsetospeed.
 * For any other rate (e.g. 256000, 128000), falls back to the Linux BOTHER/termios2
 * ioctl path, which accepts any numeric rate the hardware supports.
 * Returns 0 on success, -1 on failure or unrecognised rate on non-Linux.
 */
int       posix_set_baud_rate(zval *fd, zval *baud);

#endif /* PHP_TERMIOS_API_H */
