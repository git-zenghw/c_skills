/*
 * @Author: zenghw
 * @Date: 2026-05-23 15:07:59
 * @LastEditTime: 2026-05-23 17:17:11
 */

#include "list.h"
#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

// 循环链表
void list_init(list_node_t *head)
{
    head->next = head;
}

void list_insert_head(list_node_t *head_note, list_node_t *new_node)
{
    new_node->next = head_note->next;
    head_note->next = new_node;
}

#ifdef __cplusplus
}
#endif
