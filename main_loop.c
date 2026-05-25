/*
 * @Author: zenghw
 * @Date: 2026-05-25 14:40:33
 * @Description: 主循环和各个功能模块对接接口
 * @LastEditTime: 2026-05-25 16:52:58
 */

#include <pthread.h>
#include "common.h"
#include "drv/register.h"
#include "list/timer.h"
#include "queue.h"
#include "main_loop.h"
#include "timer.h"
#include "register.h"


#if TASK_TICK_LOOP
#define LOOP_EVNET_METHOD   0
#else
#define LOOP_EVNET_METHOD   1
#endif

#if LOOP_EVNET_METHOD
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
#else
typedef void (* task_func)(void);
typedef struct task_loop {
    volatile uint32_t period;  // 任务调用周期
    volatile uint32_t count;   // 当前计数
    volatile uint8_t ready;    // 任务状态，1就绪，0其它
    task_func task;   // 任务接口
} task_loop_t;

#define MAX_TASK_COUNT  2
static task_loop_t gs_task_table[MAX_TASK_COUNT] = {
    {.period = 1000, .count = 0, .ready = 0, .task = timer_loop},
    {.period = 3000, .count = 0, .ready = 0, .task = register_test}
};

static pthread_mutex_t main_loop_mutex = PTHREAD_MUTEX_INITIALIZER;
/* tick中断中更新任务调度状态 */
void update_task_state(void)
{
    int idx = 0;
    pthread_mutex_lock(&main_loop_mutex);
    for (; idx < MAX_TASK_COUNT; idx ++) {
        if (++ gs_task_table[idx].count > gs_task_table[idx].period) {
            gs_task_table[idx].count = 0;
            gs_task_table[idx].ready = 1;
        }
    }
    pthread_mutex_unlock(&main_loop_mutex);
}

void main_loop_test(void)
{
    int idx = 0;
    while (1) {
        for (idx = 0; idx < MAX_TASK_COUNT; idx ++) {
            pthread_mutex_lock(&main_loop_mutex);
            if (gs_task_table[idx].ready) {
                gs_task_table[idx].ready = 0;
                pthread_mutex_unlock(&main_loop_mutex);
                gs_task_table[idx].task();
            } else {
                pthread_mutex_unlock(&main_loop_mutex);
            }
        }
    }

}



#endif