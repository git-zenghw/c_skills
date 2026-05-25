/*
 * @Author: zenghw
 * @Date: 2026-05-23 22:14:01
 * @Description: 
 * @LastEditTime: 2026-05-24 00:15:54
 */

#include <string.h>
#include "common.h"
#include "sensor.h"
#include "sensor_mock.h"

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

void sensor_mock_test(void)
{
    const char *sensor_name = "SENSOR_MOCK";
    sensor_t *sensor = sensor_factory_find(sensor_name);

    if (NULL == sensor) {
        log_err("failed find sensor: %s", sensor_name);
        return;
    }

    float t, h;
    sensor->ops->read(sensor, &t, &h);
    log_info("read succ, t:%f, h:%f", t, h);

    mock_set_read_fail_flag(1);
    sensor->ops->read(sensor, &t, &h);

}
