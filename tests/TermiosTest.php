<?php

declare(strict_types=1);

it('reads a terminal\'s attributes as the full struct termios shape', function () {
    [, $slave] = ptyPair();

    $termios = tcgetattr($slave);

    expect($termios)->toHaveKeys(['c_iflag', 'c_oflag', 'c_cflag', 'c_lflag', 'c_cc', 'c_ispeed', 'c_ospeed'])
        ->and($termios['c_cc'])->toHaveCount(NCCS);
});

it('round-trips attributes without dropping the speed', function () {
    [, $slave] = ptyPair();
    $termios = cfsetospeed(cfsetispeed(tcgetattr($slave), B9600), B9600);

    expect(tcsetattr($slave, TCSANOW, $termios))->toBe(0);

    $after = tcgetattr($slave);

    expect(cfgetispeed($after))->toBe(B9600)
        ->and(cfgetospeed($after))->toBe(B9600);

    expect(tcsetattr($slave, TCSANOW, $after))->toBe(0)
        ->and(cfgetospeed(tcgetattr($slave)))->toBe(B9600);
});

it('applies flag and control-character edits made to the array', function () {
    [, $slave] = ptyPair();
    $termios = tcgetattr($slave);
    $termios['c_lflag'] &= ~(ICANON | constant('ECHO'));
    $termios['c_cc'][VMIN] = 1;
    $termios['c_cc'][VTIME] = 0;

    tcsetattr($slave, TCSANOW, $termios);
    $after = tcgetattr($slave);

    expect($after['c_lflag'] & (ICANON | constant('ECHO')))->toBe(0)
        ->and($after['c_cc'][VMIN])->toBe(1)
        ->and($after['c_cc'][VTIME])->toBe(0);
});

it('sets a standard rate with posi_set_baud_rate', function () {
    [, $slave] = ptyPair();

    expect(posi_set_baud_rate($slave, 115200))->toBe(0)
        ->and(cfgetospeed(tcgetattr($slave)))->toBe(B115200);
});

it('sets a rate without a B* constant through BOTHER on Linux, and refuses it elsewhere', function () {
    [, $slave] = ptyPair();

    if (PHP_OS_FAMILY === 'Linux') {
        expect(posi_set_baud_rate($slave, 256000))->toBe(0);

        $termios = tcgetattr($slave);
        $termios['c_cc'][VMIN] = 1;

        expect(tcsetattr($slave, TCSANOW, $termios))->toBe(0);
    } else {
        expect(posi_set_baud_rate($slave, 256000))->toBe(-1)
            ->and(posi_errno())->toBe(EINVAL);
    }
});

it('drains, flushes and controls flow on a terminal', function () {
    [, $slave] = ptyPair();

    expect(tcdrain($slave))->toBe(0)
        ->and(tcflush($slave, TCIOFLUSH))->toBe(0)
        ->and(tcflow($slave, TCOON))->toBe(0);
});

it('reports ENOTTY for a descriptor that is not a terminal', function () {
    $fd = track(posix_open(scratchPath('plain'), O_RDWR | O_CREAT, 0600));

    expect(tcgetattr($fd))->toBeFalse()
        ->and(posi_errno())->toBe(ENOTTY)
        ->and(tcdrain($fd))->toBe(-1)
        ->and(posi_errno())->toBe(ENOTTY);
});

it('refuses an array missing part of the struct', function () {
    [, $slave] = ptyPair();
    $termios = tcgetattr($slave);

    $noSpeed = $termios;
    unset($noSpeed['c_ospeed']);
    $shortCc = $termios;
    unset($shortCc['c_cc'][NCCS - 1]);

    expect(fn () => tcsetattr($slave, TCSANOW, $noSpeed))->toThrow(ValueError::class)
        ->and(fn () => cfgetispeed($shortCc))->toThrow(ValueError::class)
        ->and(fn () => cfsetispeed(['c_iflag' => 0], B9600))->toThrow(ValueError::class);
});
