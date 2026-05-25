/*
 * @Author: zenghw
 * @Date: 2026-05-23 16:47:42
 * @Description: 
 * @LastEditTime: 2026-05-23 17:23:02
 */

#include "common.h"
#include "doubly_list.h"


void dlist_init(doubly_list_t *head)
{
    head->next = head;
    head->prev = head;
}

void dlist_insert_head(doubly_list_t *head, doubly_list_t *node)
{
    node->next = head->next;
    node->prev = head;

    head->next->prev = node;
    head->next = node;
}

void dlist_insert_tail(doubly_list_t *head, doubly_list_t *node)
{
    node->next = head;
    node->prev = head->prev;

    head->prev->next = node;
    head->prev = node;
}

void dlist_remove(doubly_list_t *node)
{
    if (NULL == node) {
        log_err("node is null");
        return;
    }
    node->prev->next = node->next;
    node->next->prev = node->prev;

    node->next = NULL;
    node->prev = NULL;
}
