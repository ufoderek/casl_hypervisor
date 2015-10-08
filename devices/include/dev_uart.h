#ifndef _UART_H_
#define _UART_H_

#include <stdint.h>

typedef struct dev_uart
{
    uint32_t base_address;
    uint32_t dr;
    uint32_t fr;
    uint32_t ibrd;
    uint32_t fbrd;
    uint32_t lcrh;
    uint32_t cr;
} dev_uart;

uint32_t dev_uart_fr_rxfe(uint32_t flag);
uint32_t dev_uart_fr_txff(uint32_t flag);
uint32_t dev_uart_fr_rxff(uint32_t flag);
uint32_t dev_uart_fr_txfe(uint32_t flag);

void dev_uart_disable(dev_uart* dev_uart);
void dev_uart_enable(dev_uart* dev_uart);
void dev_uart_init(dev_uart* dev_uart, uint32_t base_address);

void dev_uart_wait_txff(dev_uart* dev_uart);
void dev_uart_wait_rxfe(dev_uart* dev_uart);

void dev_uart_tx_char(dev_uart* dev_uart, char data);
int dev_uart_tx_string(dev_uart* dev_uart, const char* str);

char dev_uart_rx_char(dev_uart* dev_uart);


#endif
