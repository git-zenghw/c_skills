/*
 * @Author: zenghw
 * @Date: 2026-05-23 15:37:41
 * @Description: 
 * @LastEditTime: 2026-05-23 16:36:47
 * Copyright (c) 2023 by 汇创智控科技有限公司, All Rights Reserved
 */

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include "common.h"
#include "list.h"

#define TASK_NAME_SIZE  (64)
typedef struct task {
    uint8_t id;
    char name[TASK_NAME_SIZE];
    list_node_t node;
} task_t;

#define TASK_HEAD_ADDR(type, num, ptr) \
    (type*)((char*)(ptr) - offsetof(type, num))

static void dump_task(list_node_t *head)
{
    list_node_t *tmp_node = head->next;
    while (tmp_node != head) {
        task_t *task = TASK_HEAD_ADDR(task_t, node, tmp_node);
        log_info("task id:%d, task name:%s", task->id, task->name);
        tmp_node = tmp_node->next;
    }
}

void task_loop(void)
{
    list_node_t task_list;
    list_init(&task_list);

    // 定义两个任务（节点已嵌入Task结构体）
    task_t task1 = {1, "task1", {NULL}};  // node.next初始为NULL
    task_t task2 = {2, "task2", {NULL}};

    log_info("offsetof note:%ld", offsetof(task_t, node));

    // 插入链表
    list_insert_head(&task_list, &task1.node);
    list_insert_head(&task_list, &task2.node);

    dump_task(&task_list);
}
