/*
 * @Author: zenghw
 * @Date: 2026-05-23 15:37:41
 * @Description: 
 * @LastEditTime: 2026-05-23 18:25:10
 * Copyright (c) 2023 by 汇创智控科技有限公司, All Rights Reserved
 */

#include <stddef.h>
#include "common.h"
#include "doubly_list.h"
#include "list.h"

#define TASK_NAME_SIZE  (64)
typedef struct task {
    uint8_t id;
    uint8_t priority;
    char name[TASK_NAME_SIZE];
    doubly_list_t node;
} task_t;

/*
 正向遍历
*/
static void dump_task(doubly_list_t *head)
{
    doubly_list_t *tmp_node = head->next;
    while (tmp_node != head) {
        task_t *task = OBJECT_HEAD_ADDR(task_t, node, tmp_node);
        log_info("task id:%d, priority:%d, name:%s", task->id, task->priority, task->name);
        tmp_node = tmp_node->next;
    }
}

/*
 反向遍历
*/
static void dump_reverse_task(doubly_list_t *head)
{
    doubly_list_t *tmp_node = head->prev;
    while (tmp_node != head) {
        task_t *task = OBJECT_HEAD_ADDR(task_t, node, tmp_node);
        log_info("task id:%d, priority:%d, name:%s", task->id, task->priority, task->name);
        tmp_node = tmp_node->prev;
    }
}

void task_loop(void)
{
    doubly_list_t task_list;
    dlist_init(&task_list);

    task_t task1 = {1, 2, "task1", {NULL}};  // node.next初始为NULL
    task_t task2 = {2, 15, "task2", {NULL}};
    task_t task3 = {3, 14, "task3", {NULL}};


    // 插入链表
    dlist_insert_head(&task_list, &task1.node);
    dlist_insert_head(&task_list, &task2.node);
    dlist_insert_head(&task_list, &task3.node);

    log_warn("Forward traversal");
    dump_task(&task_list);

    log_warn("Reverse traversal");
    dump_reverse_task(&task_list);

    dlist_remove(&task3.node);
    log_warn("remove task3, Reverse traversal");
    dump_reverse_task(&task_list);

    dlist_insert_tail(&task_list, &task3.node);
    log_warn("insert tail task3, Reverse traversal");
    dump_reverse_task(&task_list);

    dlist_remove(NULL);
}


