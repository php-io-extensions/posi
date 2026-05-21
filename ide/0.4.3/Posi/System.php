<?php

namespace Posi;

class System
{


    /**
     * @param string $path
     * @param int $mode
     * @return int
     */
    public static function chmod(string $path, int $mode): int
    {
    }

    /**
     * @param string $path
     * @param int $owner
     * @param int $group
     * @return int
     */
    public static function chown(string $path, int $owner, int $group): int
    {
    }

    /**
     * @param int $_fd
     * @return int
     */
    public static function close(int $_fd): int
    {
    }

    /**
     * @param int $fd
     * @param int $mode
     * @return int
     */
    public static function fchmod(int $fd, int $mode): int
    {
    }

    /**
     * @param int $fd
     * @param int $owner
     * @param int $group
     * @return int
     */
    public static function fchown(int $fd, int $owner, int $group): int
    {
    }

    /**
     * @return int
     */
    public static function getuid(): int
    {
    }

    /**
     * @param int $uid
     * @return int
     */
    public static function setuid(int $uid): int
    {
    }

    /**
     * @param int $mask
     * @return int
     */
    public static function umask(int $mask): int
    {
    }

    /**
     * @param int $fd
     * @param int $cmd
     * @param mixed $arg
     * @return array
     */
    public static function fcntl(int $fd, int $cmd, $arg = null): array
    {
    }

    /**
     * @param int $fd
     * @param int $cmd
     * @param mixed $arg
     * @return array
     */
    public static function ioctl(int $fd, int $cmd, $arg = null): array
    {
    }

    /**
     * @param int $fd
     * @param int $offset
     * @param int $whence
     * @return int
     */
    public static function lseek(int $fd, int $offset, int $whence): int
    {
    }

    /**
     * @param mixed $device_path
     * @param int $flags
     * @param int $mode
     * @return int
     */
    public static function open($device_path, int $flags, int $mode = 0644): int
    {
    }

    /**
     * @param int $fd
     * @param int $bytes_to_read
     * @return mixed
     */
    public static function read(int $fd, int $bytes_to_read): mixed
    {
    }

    /**
     * @param int $fd
     * @param array $iovecs
     * @return mixed
     */
    public static function readv(int $fd, array $iovecs): mixed
    {
    }

    /**
     * @param int $fd
     * @param int $len
     * @param int $flags
     * @return mixed
     */
    public static function recv(int $fd, int $len, int $flags = 0): mixed
    {
    }

    /**
     * @param mixed $status
     * @return int
     */
    public static function wait($status = null): int
    {
    }

    /**
     * @param int $pid
     * @param mixed $status
     * @param int $options
     * @return int
     */
    public static function waitpid(int $pid, $status = null, int $options = 0): int
    {
    }

    /**
     * @return mixed
     */
    public static function hostname(): mixed
    {
    }

    /**
     * @param int $fd
     * @param mixed $data
     * @param int $bytes_to_write
     * @return int
     */
    public static function write(int $fd, $data, int $bytes_to_write): int
    {
    }
}
