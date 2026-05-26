/*
 * @Author: zenghw
 * @Date: 2026-05-26 23:54:57
 * @Description: 信号-槽机制实现
 * @LastEditTime: 2026-05-27 02:11:56
 */
#ifndef SIGNAL_H
#define SIGNAL_H

#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>

#define MAX_SLOTS 6

typedef struct slot_entry {
    void (*func)(void *, void *);
    void *ctx;
} slot_entry_t;

typedef struct signal_base{
    slot_entry_t slots[MAX_SLOTS];
    uint8_t count;
} signal_base_t;

int signal_connect_base(signal_base_t *sig, void (*fn)(void *, void *), void *ctx);
int signal_disconnect_base(signal_base_t *sig, void (*fn)(void *, void *));
void signal_emit_base(signal_base_t *sig, void *data);

#define CONNECT(sig, slot_fn, ctx_ptr)              \
            do {                                    \
                signal_connect_base(&(sig).base,       \
                    (void (*)(void*, void*))(slot_fn),  \
                    (ctx_ptr));                         \
            }while (0)

#define EMIT(sig, value)                                 \
    do {                                                \
        signal_base_t *base = &(sig).base;              \
        signal_emit_base(base, (void *)value);                 \
    } while (0)

#ifdef __cplusplus
}
#endif

#endif
