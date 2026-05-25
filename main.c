/*
 * @Author: zenghw
 * @Date: 2026-05-22 23:21:24
 * @Description: 
 * @LastEditTime: 2026-05-25 15:56:53
 */

#include "common.h"
#include "tick.h"
#include "drv/register.h"
#include "ops/sensor.h"
#include "state/traffic.h"
#include "task.h"
#include "timer.h"
#include "register.h"
#include "led.h"
#include "main_loop.h"

#define TEST_LOOP    0
#define TEST_TASK    0
#define TEST_TIMER   0
#define TEST_REGISTER  0
#define TEST_SENSOR  0
#define TEST_TRAFFIC   0
#define TEST_LED       0
#define TEST_TICK      0

int main(int argc, char *argv[])
{

    sys_tick_init();
    log_info("linux c skills demo");
#if TEST_TICK
    dump_date();
    delay_ms(1000);
    dump_date();
#endif
#if TEST_LOOP
    main_loop_test();
#endif
#if TEST_TASK
    task_loop();
#endif

#if TEST_TIMER
   timer_loop();
#endif

#if TEST_REGISTER
   register_test();
#endif

#if TEST_SENSOR
   // sensor_test();
   sensor_mock_test();
#endif

#if TEST_TRAFFIC
    traffic_test();
#endif

#if TEST_LED
    led_test();
#endif

    return 0;
}