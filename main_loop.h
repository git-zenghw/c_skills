/*
 * @Author: zenghw
 * @Date: 2026-05-25 14:41:59
 * @Description: 
 * @LastEditTime: 2026-05-25 16:36:24
 */

#ifndef MAIN_LOOP_H
#define MAIN_LOOP_H

#ifdef __cplusplus
extern "C" {
#endif
/* 裸机的主循环事件, 一般由中断触发事件，然后循环执行事件动作 */
typedef enum loop_event {
    EVT_NONE = 0,
    EVT_KEY_PRESSED,
    EVT_KEY_RELEASE,
    EVT_ADC_READ,
    EVT_TIMER,
    EVT_MAX,
} loop_event_t;

#define TASK_TICK_LOOP    1

/* func_param仅传递参数指针，参数由调用者维护 */
#if !TASK_TICK_LOOP
void event_post(loop_event_t evt);
void main_init(void);
void main_loop(void);
#else
void update_task_state(void);
#endif
void main_loop_test(void);

#ifdef __cplusplus
}
#endif

#endif