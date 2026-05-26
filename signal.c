/*
 * @Author: zenghw
 * @Date: 2026-05-26 23:54:49
 * @Description: 
 * @LastEditTime: 2026-05-27 01:16:20
 */

#include "common.h"
#include "signal.h"

int signal_connect_base(signal_base_t *sig, void (*fn)(void *, void *), void *ctx)
{
    if (!sig || sig->count >= MAX_SLOTS)
        return -1;

    /*防止重复连接同一个槽*/
    for (uint8_t i=0; i < sig->count; i++) {
        if (sig->slots[i].func == fn) {
            return 0;  /*已经连过了，直接返回*/
        }
    }
    sig->slots[sig->count].func = fn;
    sig->slots[sig->count].ctx = ctx;
    sig->count++;

    return 0;
}

int signal_disconnect_base(signal_base_t *sig, void (*fn)(void *, void *))
{
    if(sig == NULL|| fn == NULL) {
        return -1;
    }

    for(uint8_t i = 0; i < sig->count; i++) {
        if(sig->slots[i].func == fn) {
            /* 用最后一个元素填补空位，避免数组移动 */
            sig->count--;
            sig->slots[i].ctx = sig->slots[sig->count].ctx;
            sig->slots[i].func = sig->slots[sig->count].func;
            sig->slots[sig->count].func = NULL;
            sig->slots[sig->count].ctx = NULL;
            return 0;
        }
    }
    return -1; /* 没找到这个槽 */
}

void signal_emit_base(signal_base_t *sig, void *data)
{
    if (!sig) return;

    for (int i = 0; i < sig->count; i++) {
        if (sig->slots[i].func)
            sig->slots[i].func(sig->slots[i].ctx, data);
    }
}
