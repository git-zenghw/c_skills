/*
 * @Author: zenghw
 * @Date: 2026-05-24 21:35:32
 * @Description: 以LED模块(仅输出）为例，抽象硬件资源
 * @LastEditTime: 2026-05-25 14:57:10
 */
#include <unistd.h>
#include "gpio.h"
#include "common.h"
#include "led.h"

typedef struct interface {
    void (*init)(gpio_info_t *, gpio_mode_t mode);
    void (*write)(gpio_info_t *, int32_t level);
} led_interface_t; 

typedef struct led_handle {
    gpio_info_t gpio;
    led_interface_t interface;
    uint8_t polarity; // io极性，1表示，高电平亮，低电平灭；0反之
    uint8_t state;    // 当前状态，1表示亮，0表示灭
    uint8_t init_flag; // 初始化标记
} led_handle_t; 

void led_init(led_handle_t *led)
{
    log_info("led init");
    led->interface.init(&led->gpio, GPIO_OUTPUT);
    led->init_flag = 1;
}

void led_on(led_handle_t *led)
{
    log_info("LED ON");
    if (led->polarity) {
        led->interface.write(&led->gpio, 1);
    } else {
        led->interface.write(&led->gpio, 0);
    }
    led->state = 1;
}

void led_off(led_handle_t *led)
{
    log_info("LED OFF");
    if (led->polarity) {
        led->interface.write(&led->gpio, 0);
    } else {
        led->interface.write(&led->gpio, 1);
    }
    led->state = 0;
}

void led_toggle(led_handle_t *led)
{
    log_info("LED TOGGLE");
    if (led->state) {
        led_off(led);
    } else {
        led_on(led);
    }
}

void led_test(void)
{
    static led_handle_t s_rled = {
        .gpio = {
            .reg = 0x86000020,
            .pin = 10,
        },
        .interface = {
            .init = gpio_init,
            .write = gpio_write,
        },
        .polarity = 0,
        .state = 0,
    };

    led_init(&s_rled);
    led_on(&s_rled);
    led_off(&s_rled);

    while (1) {
        sleep(1);
        led_toggle(&s_rled);
    }

}
