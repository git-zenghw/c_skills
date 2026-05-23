/*
 * @Author: zenghw
 * @Date: 2026-05-23 17:04:54
 * @Description: 
 * @LastEditTime: 2026-05-23 17:39:49
 * Copyright (c) 2023 by 汇创智控科技有限公司, All Rights Reserved
 */
#include <stddef.h>
#include "common.h"
#include "doubly_list.h"

typedef struct timer {
    uint32_t period;
    uint32_t duty;
    doubly_list_t node;
}timer_t;

/*
 正向遍历
*/
static void dump_timer(doubly_list_t *head)
{
    doubly_list_t *tmp_node = head->next;
    while (tmp_node != head) {
        timer_t *timer = OBJECT_HEAD_ADDR(timer_t, node, tmp_node);
        log_info("timer period:%d, duty:%d", timer->period, timer->duty);
        tmp_node = tmp_node->next;
    }
}

/*
 反向遍历
*/
static void dump_reverse_timer(doubly_list_t *head)
{
    doubly_list_t *tmp_node = head->prev;
    while (tmp_node != head) {
        timer_t *timer = OBJECT_HEAD_ADDR(timer_t, node, tmp_node);
        log_info("timer period:%d, duty:%d", timer->period, timer->duty);
        tmp_node = tmp_node->prev;
    }
}

void timer_loop(void)
{
    doubly_list_t timer_list;
    dlist_init(&timer_list);

    timer_t timer1 = {1000, 500, {NULL}};  // node.next初始为NULL
    timer_t timer2 = {200, 20, {NULL}};
    timer_t timer3 = {3000, 300, {NULL}};


    // 插入链表
    dlist_insert_head(&timer_list, &timer1.node);
    dlist_insert_head(&timer_list, &timer2.node);
    dlist_insert_head(&timer_list, &timer3.node);

    log_warn("Forward traversal");
    dump_timer(&timer_list);

    log_warn("Reverse traversal");
    dump_reverse_timer(&timer_list);

    dlist_remove(&timer3.node);
    log_warn("remove timer3, Reverse traversal");
    dump_reverse_timer(&timer_list);

    dlist_insert_tail(&timer_list, &timer3.node);
    log_warn("insert tail timer3, Reverse traversal");
    dump_reverse_timer(&timer_list);

    dlist_remove(NULL);
}