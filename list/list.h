/*
 * @Author: zenghw
 * @Date: 2026-05-23 15:08:40
 * @Description: 
 * @LastEditTime: 2026-05-23 16:05:49
 * Copyright (c) 2023 by 汇创智控科技有限公司, All Rights Reserved
 */

#ifndef LIST_H
#define LIST_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct list_node {
    struct list_node *next;
}list_node_t;

void list_init(list_node_t *head);
void list_insert_head(list_node_t *head, list_node_t *node);


#ifdef __cplusplus
}
#endif

#endif