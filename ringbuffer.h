/*
 * @Author: zenghw
 * @Date: 2026-05-25 20:14:52
 * @Description: 环形缓冲区
 * @LastEditTime: 2026-05-25 20:55:03
 */

#ifndef RINGBUFFER_H
#define RINGBUFFER_H

#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>
#include <pthread.h>

// 状态枚举，参照 RT-Thread 定义
typedef enum {
    RT_RINGBUFFER_EMPTY,
    RT_RINGBUFFER_FULL,
    RT_RINGBUFFER_HALFFULL
} rt_ringbuffer_state;

/*
 *          mirror = 0                    mirror = 1
     * +---+---+---+---+---+---+---+|+~~~+~~~+~~~+~~~+~~~+~~~+~~~+
     * | 0 | 1 | 2 | 3 | 4 | 5 | 6 ||| 0 | 1 | 2 | 3 | 4 | 5 | 6 | Full
     * +---+---+---+---+---+---+---+|+~~~+~~~+~~~+~~~+~~~+~~~+~~~+
     *  read_idx-^                   write_idx-^
     *
     * +---+---+---+---+---+---+---+|+~~~+~~~+~~~+~~~+~~~+~~~+~~~+
     * | 0 | 1 | 2 | 3 | 4 | 5 | 6 ||| 0 | 1 | 2 | 3 | 4 | 5 | 6 | Empty
     * +---+---+---+---+---+---+---+|+~~~+~~~+~~~+~~~+~~~+~~~+~~~+
     * read_idx-^ ^-write_idx
 */
typedef struct rt_ringbuffer {
    uint8_t *buffer_ptr;      // 缓冲区指针
    pthread_mutex_t lock;      // 互斥锁，保证线程安全
    pthread_cond_t  not_empty;// 条件变量：缓冲区非空（唤醒消费者）
    pthread_cond_t  not_full;  // 条件变量：缓冲区非满（唤醒生产者）

    uint16_t read_mirror : 1;  // 读镜像位
    uint16_t read_index : 15;  // 读指针
    uint16_t write_mirror : 1; // 写镜像位
    uint16_t write_index : 15; // 写指针
    int16_t buffer_size;       // 缓冲区大小
}rt_ringbuffer_t;

/* 外部传入缓存区，非malloc */
int rt_ringbuffer_init(rt_ringbuffer_t *rb, uint8_t *pool, int16_t size);
void rt_ringbuffer_destroy(rt_ringbuffer_t *rb);
size_t rt_ringbuffer_put(struct rt_ringbuffer *rb, const uint8_t *ptr, uint16_t length);
size_t rt_ringbuffer_get(struct rt_ringbuffer *rb, uint8_t *ptr, uint16_t length);
size_t rt_ringbuffer_try_put(struct rt_ringbuffer *rb,
                            const uint8_t *ptr,
                            uint16_t length);
size_t rt_ringbuffer_try_get(struct rt_ringbuffer *rb,
                            uint8_t *ptr,
                            uint16_t length);
int rt_ringbuffer_putchar(struct rt_ringbuffer *rb, uint8_t ch);
int rt_ringbuffer_putchar_blocking(struct rt_ringbuffer *rb, uint8_t ch);
int rt_ringbuffer_getchar(struct rt_ringbuffer *rb, uint8_t *ch);
int rt_ringbuffer_getchar_blocking(struct rt_ringbuffer *rb, uint8_t *ch);

#ifdef __cplusplus
}
#endif
#endif
