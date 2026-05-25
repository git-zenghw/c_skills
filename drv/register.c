/*
 * @Author: zenghw
 * @Date: 2026-05-23 19:46:45
 * @Description: 
 * @LastEditTime: 2026-05-25 14:57:25
 */

 #include "common.h"
 #include "register.h"

typedef struct uart_addr {
    volatile uint32_t CR;
    volatile uint32_t SR;
    volatile uint32_t DR;
    volatile uint32_t BRR;
} uart_addr_t;

uint32_t addr[4] ={0};
static uart_addr_t *uart_addr = (uart_addr_t *)&addr;


void register_test(void)
{
    addr[0] = 0x11223344;
    addr[3] = 0xaabbccdd;


    log_info("CR:0x%x, SR:0x%x, DR:0x%x, BRR:0x%x\n", \
        uart_addr->CR, uart_addr->SR, uart_addr->DR, uart_addr->BRR);
}