/*
 * @Author: zenghw
 * @Date: 2026-05-26 23:54:49
 * @Description: 
 * @LastEditTime: 2026-05-27 00:48:35
 */

#include "common.h"
#include "signal.h"

static inline int signal_connect_base(
    signal_base_t *sig,
    void (*fn)(void *, void *),
    void *ctx)
{
    if (!sig || sig->count >= MAX_SLOTS)
        return -1;

    sig->slots[sig->count].func = fn;
    sig->slots[sig->count].ctx = ctx;
    sig->count++;

    return 0;
}

/**
 *@brief  将槽函数连接到信号
 *@param  sig信号指针
 *@param  slot槽函数指针
 *@retval 0-成功,-1-失败
 */
int signal_connect(signal_t *sig, slot_func_t slot)
{
    if(sig == NULL || slot == NULL) {
        return -1;
    }

    /*防止重复连接同一个槽*/
    for (uint8_t i=0; i < sig->slot_count; i++) {
        if(sig->slots[i] == slot) {
            return 0;/*已经连过了，直接返回*/
        }
    }

    /*槽位满了*/
    if (sig->slot_count >= MAX_SLOTS) {
        return -1;
    }

    sig->slots[sig->slot_count] = slot;
    sig->slot_count++;

    return 0;
}

/**
* @brief 断开槽函数与信号的连接
* @param sig  信号指针
* @param slot 要断开的槽函数
* @retval 0-成功, -1-未找到
*/
int signal_disconnect(signal_t *sig, slot_func_t slot)
{
    if(sig == NULL|| slot == NULL) {
        return -1;
    }

    for(uint8_t i = 0; i < sig->slot_count; i++) {
        if(sig->slots[i] == slot) {
            /* 用最后一个元素填补空位，避免数组移动 */
            sig->slot_count--;
            sig->slots[i] = sig->slots[sig->slot_count];
            sig->slots[sig->slot_count] = NULL;
            return 0;
        }
    }
    return -1; /* 没找到这个槽 */
}

/**
* @brief 触发信号，依次调用所有已连接的槽函数
* @param sig 信号指针
* @param arg 传递给槽函数的参数
*/
void signal_emit(signal_t* sig, void *arg)
{
    if(sig ==NULL) {
        return;
    }

    for(uint8_t i =0; i < sig->slot_count; i++) {
        if(sig->slots[i] != NULL) {
            sig->slots[i](arg); /* 逐个调用槽函数 */
        }
    }
}