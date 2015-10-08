#include <dev_uart.h>
#include <io.h>

uint32_t dev_uart_fr_rxfe(uint32_t flag)
{
    return flag & 0x10;
}

uint32_t dev_uart_fr_txff(uint32_t flag)
{
    return flag & 0x20;
}

uint32_t dev_uart_fr_rxff(uint32_t flag)
{
    return flag & 0x40;
}

uint32_t dev_uart_fr_txfe(uint32_t flag)
{
    return flag & 0x80;
}

void dev_uart_disable(dev_uart* dev_uart)
{
    io_writeh(dev_uart->cr, 0);
}

void dev_uart_enalbe(dev_uart* dev_uart)
{
    io_writeh(dev_uart->cr, 0x301);
}

/*
* baud rate    => 38400
* bit width    => 8
* stop bit     => 1
* parity       => no
* bit stream   => FIFO
*/
void dev_uart_init(dev_uart* dev_uart, uint32_t base_address)
{
    dev_uart->base_address = base_address;
    dev_uart->dr = base_address;
    dev_uart->fr = base_address + 0x18;
    dev_uart->ibrd = base_address + 0x24;
    dev_uart->fbrd = base_address + 0x28;
    dev_uart->lcrh = base_address + 0x2C;

    dev_uart_disable(dev_uart);

    /* set baud rate to be 38400 and UART_CLK to be 24MHz */
    /* The setup values are calculated by ourself instead of CPU */
    /* Be careful of the setup values */
    io_writeh(dev_uart->ibrd, 39); // IBRD = UART_CLK / (16 * baud rate)
    io_writeh(dev_uart->fbrd, 4); // FBRD = round(64 * (UART_CLK % (16 * baud rate)) / (16 * baud rate))

    /* set to 8-bits mode, 1 stop bit, no parity, FIFO enable */
    io_writeb(dev_uart->lcrh, 0x70);

    dev_uart_enalbe(dev_uart);
}

void dev_uart_wait_txff(dev_uart* dev_uart)
{
    uint32_t flag;

    do
    {
        flag = io_readh(dev_uart->fr);
    }
    while(dev_uart_fr_txff(flag));
}

void dev_uart_wait_rxfe(dev_uart* dev_uart)
{
    uint32_t flag;
    do
    {
        flag = io_readh(dev_uart->fr);
    }
    while(dev_uart_fr_rxfe(flag));
}

void dev_uart_tx_char(dev_uart* dev_uart, char data)
{
    if(data == '\n')
    {
        dev_uart_wait_txff(dev_uart);
        io_writeb(dev_uart->dr, '\r');
    }

    dev_uart_wait_txff(dev_uart);
    io_writeb(dev_uart->dr, data);
}

int dev_uart_tx_string(dev_uart* dev_uart, const char* str)
{
    int count = 0;

    while(*str != '\0')
    {
        dev_uart_tx_char(dev_uart, *str);
        str++;
        count++;
    }

    return count;
}

char dev_uart_rx_char(dev_uart* dev_uart)
{
    dev_uart_wait_rxfe(dev_uart);

    char c = io_readb(dev_uart->dr);

    return c;
}
