/*
 * @Author: zenghw
 * @Date: 2026-05-23 23:57:09
 * @Description: 
 * @LastEditTime: 2026-05-24 00:18:04
 */

#include "common.h"
#include "sensor.h"

static void mock_init(void *self);
static void mock_read(void *self, float *, float *);

typedef struct mock_priv{
    float temp;
    float humi;
    int fail_on_read;
}mock_priv_t;

typedef struct mock {
    sensor_t base;
    mock_priv_t priv;
} mock_t;

static sensor_ops_t mock_ops = {
    .init = mock_init,
    .read = mock_read,
};
 
static mock_t gs_mock = {
    .base = {
        .ops = &mock_ops,
        .name = "SENSOR_MOCK",
    },
    .priv = {
        .temp = 40.0,
        .humi = 90.0,
        .fail_on_read = 0,
    },
};

void mock_init(void *self)
{
    mock_t *dev = (mock_t*)self;
    (void )dev;
    // 初始化DHT11传感器
    log_info("MOCK传感器已初始化");
}

void mock_set_read_fail_flag(int flag)
{
    gs_mock.priv.fail_on_read = flag;
}

void mock_read(void *self, float *temperature, float *humidity)
{
    mock_t *dev = (mock_t *)self;
    // 读取DHT11传感器数据，并将温度和湿度值存储在对应的变量中
    log_info("正在读取MOCK传感器数据...");

    if (dev->priv.fail_on_read) {
        log_err("failed read");
        return;
    }

    *temperature = dev->priv.temp;
    *humidity = dev->priv.humi;

    return;
}

void mock_register(void)
{
    sensor_drv_register(&gs_mock.base);
}

static void __attribute__((constructor)) mock_auto_register(void)
{
    sensor_drv_register(&gs_mock.base);
}
