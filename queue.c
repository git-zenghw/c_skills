/*
 * @Author: git-zenghw zhw15159517501@163.com
 * @Date: 2026-04-26 11:01:33
 * @LastEditTime: 2026-04-26 20:18:58
 * @Description: 实现队列功能
 */

#include "queue.h"
#include <stdio.h>
#include <string.h>

/**
 * @brief 初始化队列
 * @param q       队列对象指针
 * @param buffer  外部缓冲区
 * @param capacity 队列容量（最多存放元素个数）
 * @param elem_size 每个元素大小（字节）
 * @return 成功0，失败-1
 */
int queue_init(queue_t *q, void *buffer, int capacity, int elem_size)
{
    if (!q || !buffer || capacity <= 0 || elem_size <= 0) {
        return -1;
    }

    q->buffer    = buffer;
    q->capacity  = capacity;
    q->elem_size = elem_size;
    q->head      = 0;
    q->tail      = 0;
    q->count     = 0;

    return 0;
}

/**
 * @brief 入队
 * @param q    队列对象指针
 * @param elem 待写入元素的指针
 * @return 成功0，失败(满/无效)-1
 */
int queue_enqueue(queue_t *q, const void *elem)
{
    if (!q || !elem) {
        return -1;
    }

    if (q->count >= q->capacity) {
        fprintf(stderr, "Queue is full. Cannot enqueue element.\n");
        return -1; /* 队列已满 */
    }

    /* 计算目标位置并拷贝 */
    unsigned char *dst = (unsigned char *)q->buffer + (size_t)q->tail * (size_t)q->elem_size;
    memcpy(dst, elem, (size_t)q->elem_size);

    /* 环形后移 */
    q->tail = (q->tail + 1) % q->capacity;
    q->count++;

    return 0;
}

/**
 * @brief 出队
 * @param q    队列对象指针
 * @param elem 用于接收出队元素的缓冲区
 * @return 成功0，失败(空/无效)-1
 */
int queue_dequeue(queue_t *q, void *elem)
{
    if (!q || !elem) {
        return -1;
    }

    if (q->count <= 0) {
        return -1; /* 队列为空 */
    }

    /* 从 head 位置读取 */
    unsigned char *src = (unsigned char *)q->buffer + (size_t)q->head * (size_t)q->elem_size;
    memcpy(elem, src, (size_t)q->elem_size);

    /* 环形后移 */
    q->head = (q->head + 1) % q->capacity;
    q->count--;

    return 0;
}

/**
 * @brief 判断队列是否为空
 * @return 空返回1，非空返回0，参数无效返回-1
 */
int queue_is_empty(const queue_t *q)
{
    if (!q) {
        return -1;
    }
    return (q->count == 0) ? 1 : 0;
}

/**
 * @brief 判断队列是否已满
 * @return 满返回1，未满返回0，参数无效返回-1
 */
int queue_is_full(const queue_t *q)
{
    if (!q) {
        return -1;
    }
    return (q->count >= q->capacity) ? 1 : 0;
}

/**
 * @brief 获取队列中当前元素个数
 * @return 元素数量，参数无效返回-1
 */
int queue_count(const queue_t *q)
{
    if (!q) {
        return -1;
    }
    return q->count;
}

/**
 * @brief 清空队列
 */
void queue_clear(queue_t *q)
{
    if (!q) {
        return;
    }
    q->head  = 0;
    q->tail  = 0;
    q->count = 0;
}
