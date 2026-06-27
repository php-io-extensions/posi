<?php

namespace Posi;

class Termios
{


    /**
     * Get terminal attributes for the open file descriptor.
     * Returns an associative array with keys c_iflag, c_oflag, c_cflag, c_lflag, c_cc,
     * or false on failure (same semantics as C tcgetattr(2)).
     *
     * @param int $fd
     * @return mixed
     */
    public static function tcgetattr(int $fd): mixed
    {
    }

    /**
     * Set terminal attributes for the open file descriptor.
     * $action must be one of TCSANOW (0), TCSADRAIN (1), or TCSAFLUSH (2).
     * $termios must be an array previously returned by tcgetattr().
     * Returns 0 on success, -1 on failure (same semantics as C tcsetattr(2)).
     *
     * @param int $fd
     * @param int $action
     * @param array $termios
     * @return int
     */
    public static function tcsetattr(int $fd, int $action, array $termios): int
    {
    }

    /**
     * Set the input baud rate in a termios array.
     * Returns a new termios array with the updated speed, or false on failure
     * (same semantics as C cfsetispeed(3)).
     *
     * @param array $termios
     * @param int $speed
     * @return mixed
     */
    public static function cfsetispeed(array $termios, int $speed): mixed
    {
    }

    /**
     * Set the output baud rate in a termios array.
     * Returns a new termios array with the updated speed, or false on failure
     * (same semantics as C cfsetospeed(3)).
     *
     * @param array $termios
     * @param int $speed
     * @return mixed
     */
    public static function cfsetospeed(array $termios, int $speed): mixed
    {
    }

    /**
     * Get the input baud rate from a termios array.
     * Returns the speed_t value (same semantics as C cfgetispeed(3)).
     *
     * @param array $termios
     * @return int
     */
    public static function cfgetispeed(array $termios): int
    {
    }

    /**
     * Get the output baud rate from a termios array.
     * Returns the speed_t value (same semantics as C cfgetospeed(3)).
     *
     * @param array $termios
     * @return int
     */
    public static function cfgetospeed(array $termios): int
    {
    }

    /**
     * Wait until all output written to the file descriptor has been transmitted.
     * Returns 0 on success, -1 on failure (same semantics as C tcdrain(2)).
     *
     * @param int $fd
     * @return int
     */
    public static function tcdrain(int $fd): int
    {
    }

    /**
     * Discard data in the terminal I/O queues.
     * $queue: TCIFLUSH (0), TCOFLUSH (1), or TCIOFLUSH (2).
     * Returns 0 on success, -1 on failure (same semantics as C tcflush(2)).
     *
     * @param int $fd
     * @param int $queue
     * @return int
     */
    public static function tcflush(int $fd, int $queue): int
    {
    }

    /**
     * Suspend or resume terminal I/O transmission.
     * $action: TCOOFF (0), TCOON (1), TCIOFF (2), or TCION (3).
     * Returns 0 on success, -1 on failure (same semantics as C tcflow(2)).
     *
     * @param int $fd
     * @param int $action
     * @return int
     */
    public static function tcflow(int $fd, int $action): int
    {
    }

    /**
     * Set input and output baud rate atomically using a plain numeric value.
     *
     * For rates with a standard POSIX B constant (50 through 4000000) this
     * calls cfsetispeed/cfsetospeed + tcsetattr.  For any other value — such
     * as the LD2410C default of 256000 — it falls back to the Linux
     * BOTHER/termios2 ioctl path, which passes the exact numeric rate to the
     * kernel driver and accepts whatever the hardware supports.
     *
     * Returns 0 on success, -1 on failure or on non-Linux when the rate has
     * no standard B constant.
     *
     * @param int $fd
     * @param int $baud
     * @return int
     */
    public static function setBaudRate(int $fd, int $baud): int
    {
    }
}
