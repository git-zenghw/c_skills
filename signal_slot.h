/*
 * @Author: zenghw
 * @Date: 2026-05-27 02:21:15
 * @Description: 
 * @LastEditTime: 2026-05-27 02:42:23
 */

#ifndef SIGNAL_SLOT_H
#define SIGNAL_SLOT_H

#include "common.h"

#define MAX_SIG_SLOTS 8

// 1. 定义信号（带类型安全参数）
#define DEFINE_SIGNAL(name, param_type)          \
    typedef void (*name##_slot_t)(param_type);   \
    typedef struct {                             \
        const char      *sig_name;                \
        name##_slot_t    slots[MAX_SIG_SLOTS];        \
        uint8_t          slot_count;              \
    } name##_signal_t

#define SIGNAL_INIT(name, sig_ptr)       \
    do {                                 \
        (sig_ptr)->sig_name = #name;      \
        (sig_ptr)->slot_count = 0;        \
    } while(0)

#define SIGNAL_CONNECT(sig_ptr, slot_func)        \
    do {                                          \
        for (uint8_t i=0; i < (sig_ptr)->slot_count; i++) { \
            if ((sig_ptr)->slots[i] == slot_func) {       \
                return;                         \
            }                                     \
        }                                         \
        if ((sig_ptr)->slot_count < MAX_SIG_SLOTS) {  \
            (sig_ptr)->slots[(sig_ptr)->slot_count++] = slot_func; \
        }                                         \
    } while(0)

#define SIGNAL_DISCONNECT(sig_ptr, slot_func)        \
    do {                                          \
        uint8_t idx;                               \
        for (idx = 0; idx < sig_ptr->slot_count; idx ++) { \
            if ((sig_ptr)->slots[idx] == slot_func) { \
                (sig_ptr)->slot_count --; \
                (sig_ptr)->slots[idx] = (sig_ptr)->slots[sig->count]; \
                (sig_ptr)->slots[(sig_ptr)->count] = NULL; \
            }                                              \
        }                                                   \
    } while(0)

#define TYPED_EMIT(sig, arg)                   \
    do {                                       \
        for (uint8_t i = 0; i < (sig)->slot_count; i++) { \
            if ((sig)->slots[i]) {             \
                (sig)->slots[i](arg);          \
            }                                  \
        }                                      \
    } while(0)

#endif