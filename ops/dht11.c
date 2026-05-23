/*
 * @Author: zenghw
 * @Date: 2026-05-23 22:18:08
 * @Description: 
 * @LastEditTime: 2026-05-23 23:37:36
 * Copyright (c) 2023 by 汇创智控科技有限公司, All Rights Reserved
 */

#include "common.h"
 #include "sensor.h"

 static void dht11_init(void *self);
 static void dht11_read(void *self, float *, float *);

 typedef struct dht11_param {
    char i2c_bus;
    char i2c_addr;
 }dht11_param_t;

 typedef struct dht11 {
    sensor_t base;
    dht11_param_t priv;
 } dht11_t;

 static sensor_ops_t dht11_ops = {
    .init = dht11_init,
    .read = dht11_read,
 };
 
 static dht11_t gs_dht11 = {
    .base = {
        .ops = &dht11_ops,
        .name = "DHT11",
    },
    .priv = {
        .i2c_bus = 1,
        .i2c_addr = 2,
    },
 };

void dht11_init(void *self)
{
    dht11_t *dev = (dht11_t *)self;
    (void )dev;
    // 初始化DHT11传感器
    log_info("DHT11传感器已初始化");
}

void dht11_read(void *self, float *temperature, float *humidity)
{
    dht11_t *dev = (dht11_t *)self;
    // 读取DHT11传感器数据，并将温度和湿度值存储在对应的变量中
    log_info("正在读取DHT11传感器数据...,i2c_bus:%d, i2c_addr:%d", \
        dev->priv.i2c_bus, dev->priv.i2c_addr);
}

void dht11_register(void)
{
    sensor_drv_register(&gs_dht11.base);
}

static void __attribute__((constructor)) dht11_auto_register(void)
{
    sensor_drv_register(&gs_dht11.base);
}
