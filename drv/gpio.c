/*
 * @Author: zenghw
 * @Date: 2026-05-24 21:48:11
 * @Description: 仅考虑GPIO输出模式 
 * @LastEditTime: 2026-05-24 22:05:34
 */

#include "common.h"
#include "gpio.h"

void gpio_init(gpio_info_t *gpio, gpio_mode_t mode)
{
    if (mode == GPIO_INPUT) {
        log_warn("gpio 配置成输入模式");
    } else {
        log_warn("gpio 配置成输出模式");
    }
}

void gpio_write(gpio_info_t *gpio, int32_t level)
{
    if (level) {
        log_warn("[0x%x]gpio%d 输出高电平", gpio->reg, gpio->pin);
    } else {
        log_warn("[0x%x]gpio%d 输出低电平", gpio->reg, gpio->pin);
    }
}