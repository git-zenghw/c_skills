/*
 * @Author: zenghw
 * @Date: 2026-05-23 16:48:06
 * @Description: 
 * @LastEditTime: 2026-05-23 17:02:26
 * Copyright (c) 2023 by 汇创智控科技有限公司, All Rights Reserved
 */

#ifndef DOUBLY_LIST_H
#define DOUBLY_LIST_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct doubly_list {
    struct doubly_list *next;
    struct doubly_list *prev;
} doubly_list_t;


void dlist_init(doubly_list_t *head);
void dlist_insert_head(doubly_list_t *head, doubly_list_t *node);
void dlist_insert_tail(doubly_list_t *head, doubly_list_t *node);
void dlist_remove(doubly_list_t *node);

#ifdef __cplusplus
}
#endif
#endif 
