/*
 * @Author: zenghw
 * @Date: 2026-05-23 17:04:54
 * @Description: 
 * @LastEditTime: 2026-05-25 16:35:52
 */
#include <stddef.h>
#include "common.h"
#include "list.h"
#include "timer.h"
#include "main_loop.h"

typedef struct timer {
    uint32_t period;
    uint32_t duty;
    list_node_t node;
}timer_t;

/*
 正向遍历
*/
static void dump_timer(list_node_t *head)
{
    list_node_t *tmp_node = head->next;
    while (tmp_node != head) {
        timer_t *timer = OBJECT_HEAD_ADDR(timer_t, node, tmp_node);
        log_info("timer period:%d, duty:%d", timer->period, timer->duty);
        tmp_node = tmp_node->next;
    }
}

void timer_init(void)
{
    #if !TASK_TICK_LOOP
    event_post(EVT_TIMER);
    #endif
}

void timer_loop(void)
{
    list_node_t timer_list;
    list_init(&timer_list);

    timer_t timer1 = {1000, 500, {NULL}};  // node.next初始为NULL
    timer_t timer2 = {200, 20, {NULL}};
    timer_t timer3 = {3000, 300, {NULL}};


    // 插入链表
    list_insert_head(&timer_list, &timer1.node);
    list_insert_head(&timer_list, &timer2.node);
    list_insert_head(&timer_list, &timer3.node);

    log_warn("Forward traversal");
    dump_timer(&timer_list);
}
