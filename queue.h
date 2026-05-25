/*
 * @Author: git-zenghw zhw15159517501@163.com
 * @Date: 2026-04-26 11:01:45
 * @LastEditTime: 2026-04-26 11:17:11
 * @Description: 提供队列接口
 */

#ifndef QUEUE_H
#define QUEUE_H
#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    void *buffer;      // 队列缓冲区
    int capacity;      // 队列容量（元素数量）
    int elem_size;     // 每个元素的大小
    int head;          // 队头索引
    int tail;          // 队尾索引
    int count;         // 当前队列中的元素数量
} queue_t;

int queue_init(queue_t *q, void *buffer, int capacity, int elem_size);
int queue_enqueue(queue_t *q, const void *elem);
int queue_dequeue(queue_t *q, void *elem);
int queue_is_empty(const queue_t *q);
int queue_is_full(const queue_t *q);
int queue_count(const queue_t *q);
void queue_clear(queue_t *q);

#ifdef __cplusplus
}
#endif
#endif
