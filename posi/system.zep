namespace Posi;

class System
{
    // public static function accept()
    // public static function access()
    // public static function adjtime()
    // public static function bind()
    // public static function chdir()
    // public static function chflags()
    public static function chmod(string path, int mode) -> int
    {
        return chmod_path(path, mode);
    }

    public static function chown(string path, int owner, int group) -> int
    {
        return chown_path(path, owner, group);
    }

    // public static function chroot()

    public static function close(int _fd) -> int
    {
        int result;

        let result = close_file(_fd);

        return result;
    }

    // public static function connect()
    // public static function dup()
    // public static function dup2()
    // public static function fchdir()
    // public static function fchflags()
    public static function fchmod(int fd, int mode) -> int
    {
        return fchmod_file(fd, mode);
    }

    public static function fchown(int fd, int owner, int group) -> int
    {
        return fchown_file(fd, owner, group);
    }

    public static function getuid() -> int
    {
        return getuid_proc();
    }

    public static function setuid(int uid) -> int
    {
        return setuid_proc(uid);
    }

    public static function umask(int mask) -> int
    {
        return umask_proc(mask);
    }

    public static function fcntl(int fd, int cmd, var arg = null) -> array
    {
        var val;
        int res;

        let res = fcntl_file(fd, cmd, arg, val);

        return [
            "res": res,
            "val": val
        ];
    }
    // public static function flock()
    // public static function fpathconf()
    // public static function fstat()
    // public static function fstatfs()
    // public static function fsync()
    // public static function ftruncate()
    // public static function getdirentries()
    // public static function getdomainname()
    // public static function getegid()
    // public static function geteuid()
    // public static function getfh()
    // public static function getfsstat()
    // public static function getgid()
    // public static function gethostname()
    // public static function getpeername()
    // public static function getpgid()
    // public static function getpgrp()
    // public static function getpid()
    // public static function getppid()
    // public static function getrlimit()
    // public static function getsid()
    // public static function getsockname()
    // public static function getsockopt()
    // public static function gettimeofday()
    public static function ioctl(int fd, int cmd, var arg = null) -> array
    {
        var val;
        int res;

        let res = ioctl_file(fd, cmd, arg, val);

        return [
            "res": res,
            "val": val
        ];
    }
    // public static function issetugid()
    // public static function kill()
    // public static function link()
    // public static function listen()
    public static function lseek(int fd, int offset, int whence) -> int
    {
        return lseek_file(fd, offset, whence);
    }

    // public static function mkdir()
    // public static function mkfifo()
    // public static function mknod()
    // public static function mmap()
    // public static function mount()
    // public static function mq_close()
    // public static function mq_getattr()
    // public static function mq_open()
    // public static function mq_receive()
    // public static function mq_send()
    // public static function mq_unlink()
    // public static function mq_setattr()
    // public static function munmap()
    // public static function nfssvc()
    // public static function nvramapi()

    public static function open(var device_path, int flags, int mode = 0644) -> int
    {
        int result;
        string dp = device_path;

        let result = open_file(dp, flags, mode);

        return result;
    }

    // public static function pathconf()
    // public static function pipe()
    public static function ppoll(int fd, int timeout_ns, int events = 0) -> int
    {
        return ppoll_file(fd, timeout_ns, events);
    }
    // public static function posix_spawn()
    // public static function posix_spawnp()
    public static function read(int fd, int bytes_to_read) -> var
    {
        var result;

        let result = read_file(fd, bytes_to_read);

        return result;
    }
    // public static function readlink()
    public static function readv(int fd, array iovecs) -> var
    {
        var result;

        let result = readv_file(fd, iovecs);

        return result;
    }

    public static function recv(int fd, int len, int flags = 0) -> var
    {
        var result;

        let result = recv_file(fd, len, flags);

        return result;
    }

    // public static function recvfrom()
    // public static function recvmsg()
    // public static function rename()
    // public static function rmdir()
    // public static function select()
    // public static function send()
    // public static function sendmsg()
    // public static function sendto()
    // public static function setdomainname()
    // public static function setegid()
    // public static function seteuid()
    // public static function setgid()
    // public static function sethostname()
    // public static function setpgid()
    // public static function setpgrp()
    // public static function setrlimit()
    // public static function setsid()
    // public static function setsockopt()
    // public static function settimeofday()
    // public static function shm_open()
    // public static function shm_unlink()
    // public static function shutdown()
    // public static function sigaction()
    // public static function sigpending()
    // public static function sigprocmask()
    // public static function sigqueue()
    // public static function sigsuspend()
    // public static function sigtimedwait()
    // public static function sigwait()
    // public static function sigwaitinfo()
    // public static function socket()
    // public static function socketpair()
    // public static function stat()
    // public static function statfs()
    // public static function swapon()
    // public static function symlink()
    // public static function sync()
    // public static function truncate()
    // public static function unlink()
    // public static function unmount()
    // public static function utimes()
    public static function wait(var status = null) -> int
    {
        return wait_proc(status);
    }

    public static function waitpid(int pid, var status = null, int options = 0) -> int
    {
        return waitpid_proc(pid, status, options);
    }

    public static function hostname() -> var
    {
        var result;

        let result = hostname_sys();

        return result;
    }

    public static function lstat(string path) -> var
    {
        var result;

        let result = lstat_path(path);

        return result;
    }

    public static function write(int fd, var data, int bytes_to_write) -> int
    {
        int results;
        string payload = data;

        let results = write_file(fd, payload, bytes_to_write);

        return results;
    }
    // public static function writev()

}
