/*
 * @Author: zenghw
 * @Date: 2026-05-24 12:20:01
 * @Description: 
 * @LastEditTime: 2026-05-24 16:04:05
 */
#include <unistd.h>
#include "common.h"
#include "traffic.h"

typedef enum {
    TRAFFIC_LIGHT_RED = 0,
    TRAFFIC_LIGHT_R_YELLOW, // 红灯转绿灯的等待时间
    TRAFFIC_LIGHT_GREEN,
    TRAFFIC_LIGHT_G_YELLOW, // 绿灯转红灯的等待时间
    TRAFFIC_LIGHT_ANY,
    TRAFFIC_LIGHT_UNKNOWN
} traffic_state_t;

/* Traffic event types: 自动超时或按键按下 */
typedef enum {
    EVENT_TIMER_EXPIRED = 0,
    EVENT_EMERGENCY_BUTTON,
    EVENT_RED_BUTTON,
    EVENT_GREEN_BUTTON,
    EVENT_YELLOW_BUTTON,
    EVENT_RESET_BUTTON, // 用于还原回自动切换
} traffic_event_t;

typedef struct {
    traffic_state_t state;  // 当前交通灯状态
    uint32_t timer;        // 当前状态的计时器
    uint32_t allow_timer;  // 是否关闭定时器事件,1开，0关
} traffic_light_t;

typedef void (*action_func)(traffic_light_t *);
typedef struct transition {
    traffic_state_t from;
    traffic_state_t to;
    traffic_event_t event;
    uint32_t allow_timer;  // 是否关闭定时器事件,1开，0关
    action_func func;
} transition_t;

static void entry_yellow_light(traffic_light_t *state);
static void entry_red_light(traffic_light_t *state);
static void entry_green_light(traffic_light_t *state);
static transition_t gs_trans_table[] = {
    {TRAFFIC_LIGHT_RED, TRAFFIC_LIGHT_R_YELLOW, EVENT_TIMER_EXPIRED, 1, entry_yellow_light},
    {TRAFFIC_LIGHT_R_YELLOW, TRAFFIC_LIGHT_GREEN, EVENT_TIMER_EXPIRED, 1, entry_green_light},
    {TRAFFIC_LIGHT_GREEN, TRAFFIC_LIGHT_G_YELLOW, EVENT_TIMER_EXPIRED, 1, entry_yellow_light},
    {TRAFFIC_LIGHT_G_YELLOW, TRAFFIC_LIGHT_RED, EVENT_TIMER_EXPIRED, 1, entry_red_light},
    {TRAFFIC_LIGHT_ANY, TRAFFIC_LIGHT_RED, EVENT_EMERGENCY_BUTTON, 0, entry_red_light},
    {TRAFFIC_LIGHT_ANY, TRAFFIC_LIGHT_RED, EVENT_RED_BUTTON, 0, entry_red_light},
    {TRAFFIC_LIGHT_ANY, TRAFFIC_LIGHT_GREEN, EVENT_GREEN_BUTTON, 0, entry_green_light},
    {TRAFFIC_LIGHT_ANY, TRAFFIC_LIGHT_G_YELLOW, EVENT_YELLOW_BUTTON, 0, entry_yellow_light},
    {TRAFFIC_LIGHT_ANY, TRAFFIC_LIGHT_RED, EVENT_RESET_BUTTON, 1, entry_red_light},
};

static void entry_yellow_light(traffic_light_t *light)
{
    log_warn("YELLOW light");
    light->timer = 3;
}

static void entry_red_light(traffic_light_t *light)
{
    log_warn("RED light");
    light->timer = 10;
}

static void entry_green_light(traffic_light_t *light)
{
    log_warn("GREEN light");
    light->timer = 15;
}

void traffic_dispatch(traffic_light_t *light, traffic_event_t evt)
{
    for (size_t i = 0; i < sizeof(gs_trans_table)/sizeof(gs_trans_table[0]); i++) {

        if ((gs_trans_table[i].from == light->state ||
            gs_trans_table[i].from == TRAFFIC_LIGHT_ANY) &&
            gs_trans_table[i].event == evt) {

            light->state = gs_trans_table[i].to;
            light->allow_timer = gs_trans_table[i].allow_timer;

            if (gs_trans_table[i].func) {
                gs_trans_table[i].func(light);
            }
            return;
        }
    }
}

void traffic_tick(traffic_light_t *light)
{
    log_info("time :%d", light->timer);
    if (--light->timer <= 0) {
        traffic_dispatch(light,  EVENT_TIMER_EXPIRED);
    }
}

void traffic_trigger_reset(traffic_light_t *light)
{
    log_warn("reset event");
    traffic_dispatch(light, EVENT_RESET_BUTTON);
}

void traffic_trigger_emergency(traffic_light_t *light)
{
    log_warn("emergency event");
    traffic_dispatch(light, EVENT_EMERGENCY_BUTTON);
}

void traffic_trigger_red_button(traffic_light_t *light)
{
    log_warn("red_button event");
    traffic_dispatch(light, EVENT_RED_BUTTON);
}

void traffic_trigger_yellow_button(traffic_light_t *light)
{
    log_warn("yellow_button event");
    traffic_dispatch(light, EVENT_YELLOW_BUTTON);
}

void traffic_trigger_green_button(traffic_light_t *light)
{
    log_warn("green_button event");
    traffic_dispatch(light, EVENT_GREEN_BUTTON);
}

void traffic_loop(void)
{
    uint32_t loop_count = 0;
    traffic_light_t light = {TRAFFIC_LIGHT_RED, 10, 1};
    entry_red_light(&light);

    while (1) {
        loop_count ++;
        if (loop_count > 60 && loop_count <= 61) { // 60s后触发紧急事件
            log_info("loop_count:%d", loop_count);
            traffic_trigger_emergency(&light);
        }

        if (loop_count > 100 && loop_count <= 101) { // 100s后触发黄灯事件
            log_info("loop_count:%d", loop_count);
            traffic_trigger_yellow_button(&light);
        }

        if (loop_count > 130) {
            log_info("loop_count:%d", loop_count);
            loop_count = 0;
            traffic_trigger_reset(&light);
        }

        sleep(1);
        if (light.allow_timer) {
            traffic_tick(&light);
        }
    }

}
