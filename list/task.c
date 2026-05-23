/*
 * @Author: zenghw
 * @Date: 2026-05-23 15:37:41
 * @Description: 
 * @LastEditTime: 2026-05-23 18:49:01
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

void list_insert_by_priority(doubly_list_t *head, task_t *task)
{
    doubly_list_t *tmp_node = head->next;

    // 找到第一个优先级比当前任务低的节点
    while (tmp_node != head && 
           ((task_t *)OBJECT_HEAD_ADDR(task_t, node, tmp_node))->priority >= task->priority) {
        tmp_node = tmp_node->next;
    }

    // 插入到 tmp_node 前面
    task->node.next = tmp_node;
    task->node.prev = tmp_node->prev;

    tmp_node->prev->next = &task->node;
    tmp_node->prev = &task->node;
}

/* 出链表头, 并返回出链表后对象的首地址 */
task_t *list_pop_head(doubly_list_t *head)
{
    if (head->next == head)
        return NULL;  // 空队列

    doubly_list_t *tmp_node = head->next;
    dlist_remove(tmp_node);

    return OBJECT_HEAD_ADDR(task_t, node, tmp_node);
}

void dump_task_node(task_t *task)
{
    if (NULL == task) {
        log_err("task is null");
        return;
    }
    log_info("task id:%d, priority:%d, name:%s", task->id, task->priority, task->name);
}

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
    doubly_list_t task_priority;
    dlist_init(&task_list);
    dlist_init(&task_priority);

    task_t task1 = {1, 2, "task1", {NULL}};  // node.next初始为NULL
    task_t task2 = {2, 15, "task2", {NULL}};
    task_t task3 = {3, 14, "task3", {NULL}};

    list_insert_by_priority(&task_priority, &task1);
    list_insert_by_priority(&task_priority, &task2);
    list_insert_by_priority(&task_priority, &task3);
    log_warn("Forward traversal by priority");
    dump_task(&task_priority);

    task_t *pop_task = list_pop_head(&task_priority);
    log_warn("pop task");
    dump_task_node(pop_task);
    log_warn("pop after, Forward traversal by priority");
    dump_task(&task_priority);

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


