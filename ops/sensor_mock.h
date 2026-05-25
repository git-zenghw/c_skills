/*
 * @Author: zenghw
 * @Date: 2026-05-23 23:57:14
 * @Description: 传感器mock
 * @LastEditTime: 2026-05-24 00:16:05
 */

#ifndef SENSOR_MOCK_H
#define SENSOR_MOCK_H

#ifdef __cplusplus
extern "C" {
#endif

#define FAIL_READ   1
#define SUCC_READ   0
void mock_set_read_fail_flag(int flag);

#ifdef __cplusplus
}
#endif

#endif