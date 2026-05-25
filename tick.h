/*
 * @Author: zenghw
 * @Date: 2026-05-25 15:26:03
 * @Description: 软件试下的延时接口
 * @LastEditTime: 2026-05-25 15:45:23
 */

#ifndef TICK_H
#define TICK_H

#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>


int sys_tick_init(void);
uint32_t get_sys_tick(void);
uint32_t tick_diff(uint32_t old_tick);
void delay_ms(uint32_t ms);
void dump_date(void);

#ifdef __cplusplus
}
#endif

#endif
