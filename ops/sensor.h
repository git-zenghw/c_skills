/*
 * @Author: zenghw
 * @Date: 2026-05-23 22:14:16
 * @Description: 
 * @LastEditTime: 2026-05-25 15:00:45
 */

#ifndef SENSOR_H
#define SENSOR_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct sensor_ops {
    void (*init)(void *self);
    void (*read)(void *self, float *temperature, float *humidity);
} sensor_ops_t;

/* 接口放最前面，sensor_ops地址和sensor地址相同 */
typedef struct sensor  {
    const sensor_ops_t *ops;
    const char *name;
    struct sensor *next; // 传感器链表地址, 也可以用list_node_t 侵入式链表
} sensor_t;

typedef enum {
    SENSOR_DHT11 = 0,
    SENSOR_MAX,
}sensor_type_t;


void sensor_drv_register(sensor_t *drv);

sensor_t* sensor_factory_find(const char *name);
int sensor_get_count(void);
void sensor_dump(void);
void sensor_test(void);;
void sensor_mock_test(void);

#ifdef __cplusplus
}
#endif
#endif