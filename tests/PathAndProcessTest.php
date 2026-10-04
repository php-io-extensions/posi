<?php

declare(strict_types=1);

it('changes a mode by path and by descriptor', function () {
    $path = scratchPath('chmod');
    $fd = track(posix_open($path, O_RDWR | O_CREAT, 0600));

    expect(posix_chmod($path, 0640))->toBe(0)
        ->and(posix_lstat($path)['mode'] & 07777)->toBe(0640)
        ->and(posix_fchmod($fd, 0604))->toBe(0)
        ->and(posix_lstat($path)['mode'] & 07777)->toBe(0604);
});

it('refuses modes outside 0o7777', function () {
    expect(fn () => posix_chmod('/tmp', 010000))->toThrow(ValueError::class)
        ->and(fn () => posix_umask(-1))->toThrow(ValueError::class);
});

it('changes ownership to the current owner, and leaves an id alone with -1', function () {
    $path = scratchPath('chown');
    $fd = track(posix_open($path, O_RDWR | O_CREAT, 0600));
    $uid = posix_getuid();
    $gid = posix_getgid();

    expect(posix_chown($path, $uid, $gid))->toBe(0)
        ->and(posix_fchown($fd, -1, -1))->toBe(0)
        ->and(posix_lstat($path))->toMatchArray(['uid' => $uid, 'gid' => $gid]);
});

it('stats a symlink itself, not its target', function () {
    $target = scratchPath('target');
    $link = scratchPath('link');
    track(posix_open($target, O_WRONLY | O_CREAT, 0600));
    symlink($target, $link);

    expect(posix_lstat($link)['mode'] & S_IFMT)->toBe(S_IFLNK)
        ->and(posix_lstat($target)['mode'] & S_IFMT)->toBe(S_IFREG)
        ->and(posix_lstat(scratchPath('missing')))->toBeFalse()
        ->and(posi_errno())->toBe(ENOENT);
});

it('names the host the way gethostname() does', function () {
    expect(posix_hostname())->toBe(gethostname());
});

it('swaps the umask and returns the previous one', function () {
    $previous = posix_umask(0077);

    expect(posix_umask($previous))->toBe(0077);
});

it('takes posix_getuid() and posix_setuid() from ext/posix', function () {
    expect((new ReflectionFunction('posix_getuid'))->getExtensionName())->toBe('posix')
        ->and((new ReflectionFunction('posix_setuid'))->getExtensionName())->toBe('posix');
});

it('waits for a child and decodes its exit status', function () {
    $process = proc_open(['sh', '-c', 'exit 3'], [], $pipes);
    $pid = proc_get_status($process)['pid'];
    $status = null;

    expect(posix_waitpid($pid, $status))->toBe($pid)
        ->and(posix_wifexited($status))->toBeTrue()
        ->and(posix_wexitstatus($status))->toBe(3)
        ->and(posix_wifsignaled($status))->toBeFalse();

    proc_close($process);
});

it('waits for any child with wait and decodes a signal', function () {
    $process = proc_open(['sh', '-c', 'kill -TERM $$'], [], $pipes);
    $pid = proc_get_status($process)['pid'];
    $status = null;

    expect(posix_wait($status))->toBe($pid)
        ->and(posix_wifsignaled($status))->toBeTrue()
        ->and(posix_wtermsig($status))->toBe(SIGTERM)
        ->and(posix_wifexited($status))->toBeFalse();

    proc_close($process);
});

it('returns 0 under WNOHANG while the child runs, and leaves status alone', function () {
    $process = proc_open(['sleep', '5'], [], $pipes);
    $pid = proc_get_status($process)['pid'];
    $status = 'untouched';

    expect(posix_waitpid($pid, $status, WNOHANG))->toBe(0)
        ->and($status)->toBe('untouched');

    proc_terminate($process, SIGKILL);
    posix_waitpid($pid);
    proc_close($process);
});

it('reports ECHILD with no child to wait for', function () {
    expect(posix_wait())->toBe(-1)
        ->and(posi_errno())->toBe(ECHILD);
});

it('reports a stopped child under WUNTRACED and a continued one under WCONTINUED', function () {
    $process = proc_open(['sleep', '5'], [], $pipes);
    $pid = proc_get_status($process)['pid'];

    posix_kill($pid, SIGSTOP);

    expect(posix_waitpid($pid, $stopped, WUNTRACED))->toBe($pid)
        ->and(posix_wifstopped($stopped))->toBeTrue()
        ->and(posix_wstopsig($stopped))->toBe(SIGSTOP)
        ->and(posix_wifexited($stopped))->toBeFalse();

    posix_kill($pid, SIGCONT);

    expect(posix_waitpid($pid, $continued, WCONTINUED))->toBe($pid)
        ->and(posix_wifcontinued($continued))->toBeTrue()
        ->and(posix_wifstopped($continued))->toBeFalse();

    proc_terminate($process, SIGKILL);
    posix_waitpid($pid);
    proc_close($process);
});
