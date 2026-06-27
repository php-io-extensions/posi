<?php

namespace Posi;

/**
 * Generic native-memory primitives for structs that embed C pointers.
 *
 * Some kernel ioctls (e.g. SPI_IOC_MESSAGE, I2C_RDWR) require a struct
 * whose fields arepointers to other user-space buffers.  PHP strings
 * live in the Zend memory manager and cannot expose their addresses.
 * This class allocates named emalloc() buffers, returns their addresses
 * as plain PHP ints, and lets callers embed those ints (via pack('Q',...))
 * inside a struct that is then handed to ioctl().
 *
 * Usage example (SPI full-duplex transfer):
 *
 *   tx = Posi\Memory::alloc(2);
 *   rx = Posi\Memory::alloc(2);
 *   Posi\Memory::write(tx, pack('CC', cmd, 0x00));
 *   // pack spi_ioc_transfer: tx_buf(Q), rx_buf(Q), len(V), ...
 *   struct = pack('QQVVvCCCCC', tx, rx, 2, 0, 0, 8, 0, 0, 0, 0);
 *   ioctl(fd, SPI_IOC_MESSAGE_1, ['data': struct], out);
 *   data = Posi\Memory::read(rx, 2);
 *   Posi\Memory::free(tx);
 *   Posi\Memory::free(rx);
 */
class Memory
{


    /**
     * Allocate a zero-filled native buffer of `size` bytes.
     *
     * Returns the buffer address as a PHP int.  The caller is responsible
     * for calling free() exactly once when done.
     *
     * @param int $size
     * @return int
     */
    public static function alloc(int $size): int
    {
    }

    /**
     * Free a buffer previously returned by alloc().
     * Passing 0 is a safe no-op.
     *
     * @param int $ptr
     * @return void
     */
    public static function free(int $ptr): void
    {
    }

    /**
     * Write the bytes of `data` into the buffer at `ptr + offset`.
     * No bounds checking — caller must ensure the buffer is large enough.
     *
     * @param int $ptr
     * @param string $data
     * @param int $offset
     * @return void
     */
    public static function write(int $ptr, string $data, int $offset = 0): void
    {
    }

    /**
     * Read `size` bytes from the buffer at `ptr + offset` and return
     * them as a PHP string.
     *
     * @param int $ptr
     * @param int $size
     * @param int $offset
     * @return string
     */
    public static function read(int $ptr, int $size, int $offset = 0): string
    {
    }
}
