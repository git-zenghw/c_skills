/*
 * @Author: zenghw
 * @Date: 2026-05-26 23:54:57
 * @Description: 信号-槽机制实现
 * @LastEditTime: 2026-05-27 00:45:21
 */
#ifndef SIGNAL_H
#define SIGNAL_H

#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>

#define MAX_SLOTS 6

typedef struct {
    void (*func)(void *, void *);
    void *ctx;
} slot_entry_t;

typedef struct {
    slot_entry_t slots[MAX_SLOTS];
    uint8_t count;
} signal_base_t;

/* 定义带类型信息的信号 */
#define DEFINE_SIGNAL(name, param_type) \
    typedef void (*name##_slot_t)(param_type); \
    typedef struct { \
        const char *sig_name; \
        name##_slot_t slots[MAX_SLOTS]; \
        uint8_t slot_count;  \
    } name##_signal_t

/* 类型安全的emit */
#define TYPED_EMIT(sig, arg) \
    do { \
        for (uint8_t i = 0; i < (sig)->slot_count; i++) { \
            if ((sig)->slots[i]) \
                (sig)->slots[i](arg);\
        }\
    } while(0)

/* 槽函数类型：接受一个通用参数指针 */
typedef void(*slot_func_t)(void*arg);

/* 信号结构体 */
typedef struct {
    const char *name;        /* 信号名称，方便调试 */
    slot_func_t slots[MAX_SLOTS];  /* 槽函数数组 */
    uint8_t   slot_count;     /* 当前已连接的槽数量 */
}signal_t;

/* 信号初始化宏，用起来更方便 */
#define SIGNAL_INIT(sig_name) { .name =#sig_name, .slot_count = 0 }
int signal_connect(signal_t *sig, slot_func_t slot);
int signal_disconnect(signal_t *sig, slot_func_t slot);
void signal_emit(signal_t* sig, void *arg);

#ifdef __cplusplus
}
#endif

#endif
