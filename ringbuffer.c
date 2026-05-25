/*
 * @Author: zenghw
 * @Date: 2026-05-25 18:19:57
 * @Description: 环形缓冲区接口实现
 * @LastEditTime: 2026-05-25 21:29:18
 */

#include "common.h"
#include <string.h>
#include "ringbuffer.h"

// 初始化 ringbuffer
int rt_ringbuffer_init(rt_ringbuffer_t *rb, uint8_t *pool, int16_t size) 
{
    if (!rb || !pool || size <= 0) return -1;

    rb->buffer_ptr = pool;
    rb->buffer_size = size;
    rb->read_index = 0;
    rb->write_index = 0;
    rb->read_mirror = 0;
    rb->write_mirror = 0;

    // 初始化锁和条件变量
    pthread_mutex_init(&rb->lock, NULL);
    pthread_cond_init(&rb->not_empty, NULL);
    pthread_cond_init(&rb->not_full, NULL);
    return 0;
}

// 销毁 ringbuffer
void rt_ringbuffer_destroy(rt_ringbuffer_t *rb)
{
    if (!rb) return;
    pthread_mutex_destroy(&rb->lock);
    pthread_cond_destroy(&rb->not_empty);
    pthread_cond_destroy(&rb->not_full);

    rb->read_index = 0;
    rb->write_index = 0;
    rb->read_mirror = 0;
    rb->write_mirror = 0;
}

// 获取当前状态
static rt_ringbuffer_state rt_ringbuffer_status(struct rt_ringbuffer *rb)
{
    if (rb->read_index == rb->write_index) {
        if (rb->read_mirror == rb->write_mirror)
            return RT_RINGBUFFER_EMPTY;
        else
            return RT_RINGBUFFER_FULL;
    }
    return RT_RINGBUFFER_HALFFULL;
}

// 获取数据长度
static uint16_t rt_ringbuffer_data_len(struct rt_ringbuffer *rb) 
{
    switch (rt_ringbuffer_status(rb)) {
    case RT_RINGBUFFER_EMPTY:
        return 0;
    case RT_RINGBUFFER_FULL:
        return rb->buffer_size;
    case RT_RINGBUFFER_HALFFULL:
    default:
        if (rb->write_index > rb->read_index)
            return rb->write_index - rb->read_index;
        else
            return rb->buffer_size - (rb->read_index - rb->write_index);
    }
}

#define RB_EMPTY(rb)  (rt_ringbuffer_status(rb) == RT_RINGBUFFER_EMPTY)
#define RB_FULL(rb)   (rt_ringbuffer_status(rb) == RT_RINGBUFFER_FULL)

size_t rt_ringbuffer_put(struct rt_ringbuffer *rb, const uint8_t *ptr, uint16_t length)
{
    size_t size;

    if (!rb || !ptr || length == 0)
        return 0;
    pthread_mutex_lock(&rb->lock);

    // 如果缓冲区满，则阻塞等待
    while (RB_FULL(rb)) {
        pthread_cond_wait(&rb->not_full, &rb->lock);
    }

    size = length > (rb->buffer_size - rt_ringbuffer_data_len(rb)) 
           ? (rb->buffer_size - rt_ringbuffer_data_len(rb)) 
           : length;

    if (size == 0) {
        pthread_mutex_unlock(&rb->lock);
        return 0;
    }

    if (rb->buffer_size - rb->write_index > size) {
        memcpy(&rb->buffer_ptr[rb->write_index], ptr, size);
        rb->write_index += size;
    } else {
        memcpy(&rb->buffer_ptr[rb->write_index], ptr, rb->buffer_size - rb->write_index);
        memcpy(&rb->buffer_ptr[0], &ptr[rb->buffer_size - rb->write_index], 
               size - (rb->buffer_size - rb->write_index));
        rb->write_mirror = ~rb->write_mirror;
        rb->write_index = size - (rb->buffer_size - rb->write_index);
    }

    // 写入完成，唤醒可能正在等待数据的消费者
    pthread_cond_signal(&rb->not_empty);
    pthread_mutex_unlock(&rb->lock);
    return size;
}

size_t rt_ringbuffer_get(struct rt_ringbuffer *rb, uint8_t *ptr, uint16_t length)
{
    size_t size;
    if (!rb || !ptr || length == 0)
        return 0;

    pthread_mutex_lock(&rb->lock);

    // 如果缓冲区空，则阻塞等待
    while (RB_EMPTY(rb)) {
        pthread_cond_wait(&rb->not_empty, &rb->lock);
    }

    size = rt_ringbuffer_data_len(rb);
    if (size == 0) {
        pthread_mutex_unlock(&rb->lock);
        return 0;
    }
    if (size > length) size = length;

    if (rb->buffer_size - rb->read_index > size) {
        memcpy(ptr, &rb->buffer_ptr[rb->read_index], size);
        rb->read_index += size;
    } else {
        memcpy(&ptr[0], &rb->buffer_ptr[rb->read_index], rb->buffer_size - rb->read_index);
        memcpy(&ptr[rb->buffer_size - rb->read_index], &rb->buffer_ptr[0], 
               size - (rb->buffer_size - rb->read_index));
        rb->read_mirror = ~rb->read_mirror;
        rb->read_index = size - (rb->buffer_size - rb->read_index);
    }

    // 读取完成，唤醒可能正在等待空闲空间的生产者
    pthread_cond_signal(&rb->not_full);
    pthread_mutex_unlock(&rb->lock);
    return size;
}

size_t rt_ringbuffer_try_put(struct rt_ringbuffer *rb,
                            const uint8_t *ptr,
                            uint16_t length)
{
    size_t size;

    if (!rb || !ptr || length == 0)
        return 0;

    pthread_mutex_lock(&rb->lock);

    /* 缓冲区满，立即失败 */
    if (RB_FULL(rb)) {
        pthread_mutex_unlock(&rb->lock);
        return 0;
    }

    /* 计算可写入长度 */
    size = length > (rb->buffer_size - rt_ringbuffer_data_len(rb))
           ? (rb->buffer_size - rt_ringbuffer_data_len(rb))
           : length;

    if (size == 0) {
        pthread_mutex_unlock(&rb->lock);
        return 0;
    }

    /* 写数据（与 put 完全一致） */
    if (rb->buffer_size - rb->write_index > size) {
        memcpy(&rb->buffer_ptr[rb->write_index], ptr, size);
        rb->write_index += size;
    } else {
        memcpy(&rb->buffer_ptr[rb->write_index], ptr,
               rb->buffer_size - rb->write_index);
        memcpy(&rb->buffer_ptr[0],
               &ptr[rb->buffer_size - rb->write_index],
               size - (rb->buffer_size - rb->write_index));

        rb->write_mirror = ~rb->write_mirror;
        rb->write_index = size - (rb->buffer_size - rb->write_index);
    }

    /*
     * 注意：
     * try 接口通常不 signal cond，
     * 因为调用方不期望阻塞
     * 如果你希望“顺便唤醒阻塞的消费者”，也可以保留
     */
    pthread_cond_signal(&rb->not_empty);

    pthread_mutex_unlock(&rb->lock);
    return size;
}

size_t rt_ringbuffer_try_get(struct rt_ringbuffer *rb,
                            uint8_t *ptr,
                            uint16_t length)
{
    size_t size;

    if (!rb || !ptr || length == 0)
        return 0;

    pthread_mutex_lock(&rb->lock);

    /* 缓冲区空，立即失败 */
    if (RB_EMPTY(rb)) {
        pthread_mutex_unlock(&rb->lock);
        return 0;
    }

    size = rt_ringbuffer_data_len(rb);
    if (size == 0) {
        pthread_mutex_unlock(&rb->lock);
        return 0;
    }

    if (size > length)
        size = length;

    /* 读数据 */
    if (rb->buffer_size - rb->read_index > size) {
        memcpy(ptr, &rb->buffer_ptr[rb->read_index], size);
        rb->read_index += size;
    } else {
        memcpy(&ptr[0], &rb->buffer_ptr[rb->read_index],
               rb->buffer_size - rb->read_index);
        memcpy(&ptr[rb->buffer_size - rb->read_index],
               &rb->buffer_ptr[0],
               size - (rb->buffer_size - rb->read_index));

        rb->read_mirror = ~rb->read_mirror;
        rb->read_index = size - (rb->buffer_size - rb->read_index);
    }

    /*
     * 可选：唤醒阻塞的生产者
     * 如果系统中同时存在阻塞 put，则建议保留
     */
    pthread_cond_signal(&rb->not_full);

    pthread_mutex_unlock(&rb->lock);
    return size;
}

int rt_ringbuffer_putchar(struct rt_ringbuffer *rb, uint8_t ch)
{
    if (!rb)
        return -1;

    pthread_mutex_lock(&rb->lock);

    if (RB_FULL(rb)) {
        pthread_mutex_unlock(&rb->lock);
        return -1;
    }

    rb->buffer_ptr[rb->write_index] = ch;

    if (rb->write_index + 1 >= rb->buffer_size) {
        rb->write_mirror = ~rb->write_mirror;
        rb->write_index = 0;
    } else {
        rb->write_index++;
    }

    pthread_cond_signal(&rb->not_empty);
    pthread_mutex_unlock(&rb->lock);
    return 0;
}

int rt_ringbuffer_putchar_blocking(struct rt_ringbuffer *rb, uint8_t ch)
{
    if (!rb)
        return -1;

    pthread_mutex_lock(&rb->lock);

    if (RB_FULL(rb)) {
        pthread_cond_wait(&rb->not_full, &rb->lock);
    }

    rb->buffer_ptr[rb->write_index] = ch;

    if (rb->write_index + 1 >= rb->buffer_size) {
        rb->write_mirror = ~rb->write_mirror;
        rb->write_index = 0;
    } else {
        rb->write_index++;
    }

    pthread_cond_signal(&rb->not_empty);
    pthread_mutex_unlock(&rb->lock);
    return 0;
}


int rt_ringbuffer_getchar(struct rt_ringbuffer *rb, uint8_t *ch)
{
    if (!rb || !ch)
        return -1;

    pthread_mutex_lock(&rb->lock);

    if (RB_EMPTY(rb)) {
        pthread_mutex_unlock(&rb->lock);
        return -1;
    }

    *ch = rb->buffer_ptr[rb->read_index];

    if (rb->read_index + 1 >= rb->buffer_size) {
        rb->read_mirror = ~rb->read_mirror;
        rb->read_index = 0;
    } else {
        rb->read_index++;
    }

    pthread_cond_signal(&rb->not_full);
    pthread_mutex_unlock(&rb->lock);
    return 0;
}


int rt_ringbuffer_getchar_blocking(struct rt_ringbuffer *rb, uint8_t *ch)
{
    if (!rb || !ch)
        return -1;

    pthread_mutex_lock(&rb->lock);

    while (RB_EMPTY(rb)) {
        pthread_cond_wait(&rb->not_empty, &rb->lock);
    }

    *ch = rb->buffer_ptr[rb->read_index];

    if (rb->read_index + 1 >= rb->buffer_size) {
        rb->read_mirror = ~rb->read_mirror;
        rb->read_index = 0;
    } else {
        rb->read_index++;
    }

    pthread_cond_signal(&rb->not_full);
    pthread_mutex_unlock(&rb->lock);
    return 0;
}
