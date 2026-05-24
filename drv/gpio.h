/*
 * @Author: zenghw
 * @Date: 2026-05-24 21:48:15
 * @Description:  GPIO硬件资源抽象，当前仅考虑输出模式
 * @LastEditTime: 2026-05-24 22:01:12
 */

#ifndef GPIO_H
#define GPIO_H

#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>

typedef enum {
    GPIO_INPUT = 0,
    GPIO_OUTPUT,
} gpio_mode_t;

typedef struct gpio_info {
    volatile uint32_t reg; // LED输出寄存器
    uint32_t pin; // 对于具体的IO来说，偏移值
} gpio_info_t ;

void gpio_init(gpio_info_t *, gpio_mode_t mode);
void gpio_write(gpio_info_t *, int32_t level);

#ifdef __cplusplus
}
#endif

#endif