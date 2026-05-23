/*
 * @Author: zenghw
 * @Date: 2026-05-23 22:18:08
 * @Description: 
 * @LastEditTime: 2026-05-23 22:31:13
 * Copyright (c) 2023 by 汇创智控科技有限公司, All Rights Reserved
 */

#include "common.h"
 #include "sensor.h"

 static void dht11_init(void);
 static void dht11_read(float *, float *);
 static sensor_ops_t dht11_ops = {
    .name = "DHT11",
    .init = dht11_init,
    .read = dht11_read,
 };

void dht11_init(void)
{
    // 初始化DHT11传感器
    log_info("DHT11传感器已初始化");
}

void dht11_read(float *temperature, float *humidity)
{
    // 读取DHT11传感器数据，并将温度和湿度值存储在对应的变量中
    log_info("正在读取DHT11传感器数据...");
}

void dht11_register(void)
{
    sensor_drv_register(&dht11_ops);
}

static void __attribute__((constructor)) dht11_auto_register(void)
{
    sensor_drv_register(&dht11_ops);
}
