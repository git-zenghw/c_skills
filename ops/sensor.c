/*
 * @Author: zenghw
 * @Date: 2026-05-23 22:14:01
 * @Description: 
 * @LastEditTime: 2026-05-23 23:30:02
 * Copyright (c) 2023 by 汇创智控科技有限公司, All Rights Reserved
 */

#include <string.h>
#include "common.h"
#include "sensor.h"

static sensor_t *gs_sensor_list = NULL;

void sensor_drv_register(sensor_t *drv)
{
    if (!drv) {
        return;
    }   
    drv->next = gs_sensor_list;
    gs_sensor_list = drv;
}

sensor_t* sensor_factory_find(const char *name)
{
    sensor_t *current = gs_sensor_list;
    while (current) {
        if (strcmp(current->name, name) == 0) {
            return current;
        }
        current = current->next;
    }

    return NULL; // 未找到匹配的传感器驱动
}

int sensor_get_count(void)
{
    int count = 0;
    sensor_t *current = gs_sensor_list;
    while (current) {
        count++;
        current = current->next;
    }
    log_info("当前注册的传感器驱动数量: %d", count);
    return count;
}

void sensor_dump(void)
{
    sensor_t *current = gs_sensor_list;
    log_warn("已注册的传感器驱动列表:");
    while (current) {
        log_info(" --- %s", current->name);
        current = current->next;
    }
}

void sensor_loop(void)
{
    int sensor_count = sensor_get_count();
    sensor_t *sensor = sensor_factory_find("DHT11");
    float temp = 37.0f;
    float humi = 80.0f;
    sensor->ops->init(sensor);
    sensor->ops->read(sensor, &temp, &humi);
}
