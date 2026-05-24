/*
 * @Author: zenghw
 * @Date: 2026-05-22 23:21:24
 * @Description: 
 * @LastEditTime: 2026-05-24 22:15:41
 * Copyright (c) 2023 by 汇创智控科技有限公司, All Rights Reserved
 */

#include "common.h"
#include "drv/register.h"
#include "ops/sensor.h"
#include "state/traffic.h"
#include "task.h"
#include "timer.h"
#include "register.h"
#include "led.h"


#define TEST_TASK    0
#define TEST_TIMER   0
#define TEST_REGISTER  0
#define TEST_SENSOR  0
#define TEST_TRAFFIC   0
#define TEST_LED        1

int main(int argc, char *argv[])
{

    log_info("linux c skills demo");
#if TEST_TASK
    task_loop();
#endif

#if TEST_TIMER
   timer_loop();
#endif

#if TEST_REGISTER
   register_loop();
#endif

#if TEST_SENSOR
   // sensor_loop();
   sensor_mock_test();
#endif

#if TEST_TRAFFIC
    traffic_loop();
#endif

#if TEST_LED
    led_loop();
#endif

    return 0;
}