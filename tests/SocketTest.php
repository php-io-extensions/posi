<?php

declare(strict_types=1);

/** @return array{0: resource, 1: int} a listening TCP stream on 127.0.0.1 and its port */
function listener(): array
{
    $server = stream_socket_server('tcp://127.0.0.1:0', $errno, $error);

    if ($server === false) {
        throw new RuntimeException("listen failed: {$error}");
    }

    return [$server, (int) substr(strrchr((string) stream_socket_get_name($server, false), ':'), 1)];
}

it('returns the descriptor behind a socket stream and a file stream', function () {
    [$a, $b] = stream_socket_pair(STREAM_PF_UNIX, STREAM_SOCK_STREAM, STREAM_IPPROTO_IP);
    $file = fopen(scratchPath('fileno'), 'w+');

    expect(posix_fileno($a))->toBeGreaterThanOrEqual(0)
        ->and(posix_fileno($b))->not->toBe(posix_fileno($a))
        ->and(posix_fileno($file))->toBeGreaterThanOrEqual(0);
});

it('fails with EBADF for a stream with no descriptor', function () {
    expect(posix_fileno(fopen('php://memory', 'r+')))->toBeFalse()
        ->and(posi_errno())->toBe(EBADF);
});

it('duplicates a descriptor that reaches the same open file', function () {
    [$a, $b] = stream_socket_pair(STREAM_PF_UNIX, STREAM_SOCK_STREAM, STREAM_IPPROTO_IP);
    $copy = track(posix_dup(posix_fileno($a)));

    expect($copy)->not->toBe(posix_fileno($a))
        ->and(posix_write($copy, 'via dup', 7))->toBe(7)
        ->and(fread($b, 64))->toBe('via dup');
});

it('fails to duplicate a closed descriptor with EBADF', function () {
    $fd = posix_open(scratchPath('closed'), O_RDWR | O_CREAT, 0600);
    posix_close($fd);

    expect(posix_dup($fd))->toBe(-1)
        ->and(posi_errno())->toBe(EBADF);
});

it('refuses a descriptor outside the int range instead of truncating it', function () {
    expect(fn () => posix_dup(PHP_INT_MAX))->toThrow(ValueError::class)
        ->and(fn () => posix_socket_fdopen(PHP_INT_MAX))->toThrow(ValueError::class);
});

it('opens a listening socket descriptor as a stream that accepts and names itself', function () {
    [$server, $port] = listener();
    $shared = posix_socket_fdopen(posix_dup(posix_fileno($server)));
    $client = stream_socket_client("tcp://127.0.0.1:{$port}");

    $accepted = stream_socket_accept($shared, 1);
    fwrite($client, 'ping');

    expect(stream_socket_get_name($shared, false))->toBe(stream_socket_get_name($server, false))
        ->and($accepted)->toBeResource()
        ->and(fread($accepted, 4))->toBe('ping')
        ->and(stream_socket_get_name($accepted, true))->toBe(stream_socket_get_name($client, false));
});

it('closes only its own descriptor when the stream closes', function () {
    [$server, $port] = listener();
    $shared = posix_socket_fdopen(posix_dup(posix_fileno($server)));

    fclose($shared);
    $client = stream_socket_client("tcp://127.0.0.1:{$port}");

    expect(stream_socket_accept($server, 1))->toBeResource()
        ->and($client)->toBeResource();
});

it('fails with ENOTSOCK for a descriptor that is not a socket', function () {
    $fd = track(posix_open(scratchPath('plain'), O_RDWR | O_CREAT, 0600));

    expect(posix_socket_fdopen($fd))->toBeFalse()
        ->and(posi_errno())->toBe(ENOTSOCK);
});

it('fails with EPROTOTYPE for a datagram socket', function () {
    $udp = stream_socket_server('udp://127.0.0.1:0', $errno, $error, STREAM_SERVER_BIND);

    expect(posix_socket_fdopen(posix_dup(posix_fileno($udp))))->toBeFalse()
        ->and(posi_errno())->toBe(EPROTOTYPE);
});

it('fails with EBADF for a closed descriptor', function () {
    $fd = posix_open(scratchPath('gone'), O_RDWR | O_CREAT, 0600);
    posix_close($fd);

    expect(posix_socket_fdopen($fd))->toBeFalse()
        ->and(posi_errno())->toBe(EBADF);
});

it('completes a TLS handshake on a connection accepted through the opened stream', function () {
    $key = openssl_pkey_new(['private_key_bits' => 2048, 'private_key_type' => OPENSSL_KEYTYPE_RSA]);
    $cert = openssl_csr_sign(openssl_csr_new(['commonName' => '127.0.0.1'], $key), null, $key, 1);
    openssl_x509_export($cert, $certPem);
    openssl_pkey_export($key, $keyPem);
    file_put_contents($pem = scratchPath('tls.pem'), $certPem.$keyPem);

    [$server, $port] = listener();
    $shared = posix_socket_fdopen(posix_dup(posix_fileno($server)));

    $child = pcntl_fork();

    if ($child === 0) {
        $context = stream_context_create(['ssl' => ['verify_peer' => false, 'verify_peer_name' => false]]);
        $client = stream_socket_client("tls://127.0.0.1:{$port}", $errno, $error, 5, STREAM_CLIENT_CONNECT, $context);
        fwrite($client, 'over tls');
        fread($client, 1);
        exit(0);
    }

    $accepted = stream_socket_accept($shared, 5);
    stream_context_set_option($accepted, 'ssl', 'local_cert', $pem);
    $handshake = stream_socket_enable_crypto($accepted, true, STREAM_CRYPTO_METHOD_TLS_SERVER);
    $payload = fread($accepted, 8);
    fwrite($accepted, 'k');
    pcntl_waitpid($child, $status);

    expect($handshake)->toBeTrue()
        ->and($payload)->toBe('over tls')
        ->and(pcntl_wexitstatus($status))->toBe(0);
});

it('shares one listener across parallel threads, each accepting on its own copy', function () {
    [$server, $port] = listener();
    $fd = posix_fileno($server);
    $futures = [];

    for ($i = 0; $i < 4; $i++) {
        $futures[] = (new parallel\Runtime())->run(static function (int $fd): int {
            $listener = posix_socket_fdopen(posix_dup($fd));
            stream_set_blocking($listener, false);
            $accepted = 0;
            $deadline = microtime(true) + 3;

            while (microtime(true) < $deadline) {
                $read = [$listener];
                $write = $except = null;

                if (@stream_select($read, $write, $except, 0, 50_000)) {
                    while ($connection = @stream_socket_accept($listener, 0)) {
                        $accepted++;
                        fclose($connection);
                    }
                }
            }

            return $accepted;
        }, [$fd]);
    }

    usleep(300_000);

    for ($i = 0; $i < 400; $i++) {
        $client = stream_socket_client("tcp://127.0.0.1:{$port}");
        fclose($client);
        usleep(1_000);
    }

    $counts = array_map(fn (parallel\Future $future): int => $future->value(), $futures);

    expect(array_sum($counts))->toBe(400)
        ->and(min($counts))->toBeGreaterThan(0);
})->skip(! extension_loaded('parallel'), 'needs a ZTS PHP with ext-parallel');
