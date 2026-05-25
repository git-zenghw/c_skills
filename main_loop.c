/*
 * @Author: zenghw
 * @Date: 2026-05-25 14:40:33
 * @Description: 主循环和各个功能模块对接接口
 * @LastEditTime: 2026-05-25 15:20:05
 */

#include "common.h"
#include "list/timer.h"
#include "queue.h"
#include <stdint.h>
#include "main_loop.h"
#include "timer.h"

typedef struct event_param {
    loop_event_t evt; // 事件类型
    // ... 如果有接口参数，填写接口参数，但最好统一
} event_param_t;

#define EVE_MAX_QUEUE_SIZE  (32)
static event_param_t event_cache[EVE_MAX_QUEUE_SIZE] = {0};
static queue_t gs_loop_queue = {0};
typedef void (* loop_func)(void);
static loop_func gs_func_tabble[EVT_MAX] = { NULL };

void main_init(void)
{
    queue_init(&gs_loop_queue, &event_cache, EVE_MAX_QUEUE_SIZE, sizeof(event_param_t));
}

/* func_param仅传递参数指针，参数由调用者维护 */
void event_post(loop_event_t evt)
{
    event_param_t data;
    data.evt = evt;
    queue_enqueue(&gs_loop_queue, &data);
}

static int event_get(event_param_t *evt_param)
{
    return queue_dequeue(&gs_loop_queue, evt_param);
}

void main_loop_evt_register(loop_event_t evt, loop_func func)
{
    if (evt < EVT_MAX) {
        gs_func_tabble[evt] = func;
    }
}

void main_loop_dispatch(event_param_t *evt_param)
{
    if ((evt_param->evt < EVT_MAX) && gs_func_tabble[evt_param->evt]) {
        gs_func_tabble[evt_param->evt]();
    }
}

void main_loop(void)
{
    event_param_t event;
    if (0 == event_get(&event)) {
        main_loop_dispatch(&event);
    }
}

void main_loop_test(void)
{
    main_init();
  
    main_loop_evt_register(EVT_TIMER, timer_loop);

    timer_init(); // 仿中断
    while (1) {
        main_loop();
    }
}