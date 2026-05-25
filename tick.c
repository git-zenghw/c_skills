/*
 * @Author: zenghw
 * @Date: 2026-05-25 15:25:52
 * @Description: 
 * @LastEditTime: 2026-05-25 15:56:09
 */

#include "common.h"
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <errno.h>

#define TICK_MS    1       // 1ms 一个tick（可修改）
static volatile uint32_t gs_sys_tick = 0;  // 全局系统tick（volatile禁止编译器优化）
static pthread_mutex_t tick_mutex = PTHREAD_MUTEX_INITIALIZER;

/* 获取当前系统tick */ 
uint32_t get_sys_tick(void)
{
    uint32_t tick;
    pthread_mutex_lock(&tick_mutex);
    tick = gs_sys_tick;
    pthread_mutex_unlock(&tick_mutex);
    return tick;
}

/* 计算前后时间差 */
uint32_t tick_diff(uint32_t old_tick)
{
    uint32_t now = get_sys_tick();
    return now - old_tick;
}

/* 精准延时（ms）*/
void delay_ms(uint32_t ms)
{
    uint32_t start = get_sys_tick();
    while (tick_diff(start) < ms);
}

static void* gs_sys_tick_thread(void* arg)
{
    (void)arg;
    struct timespec ts;

    clock_gettime(CLOCK_MONOTONIC, &ts);
    while (1)
    {
        // 【关键】每次增加 1ms，绝对时间定时，无漂移
        ts.tv_nsec += TICK_MS * 1000000;

        // 纳秒进位到秒
        while (ts.tv_nsec >= 1000000000)
        {
            ts.tv_sec++;
            ts.tv_nsec -= 1000000000;
        }

        // 绝对时间等待（不会漂移！）
        clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &ts, NULL);

        // tick +1（线程安全）
        pthread_mutex_lock(&tick_mutex);
        gs_sys_tick++;
        pthread_mutex_unlock(&tick_mutex);
    }
    return NULL;
}

int sys_tick_init(void)
{
    pthread_t tid;
    pthread_attr_t attr;

    pthread_attr_init(&attr);
    // 设置线程为分离态（自动释放资源）
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    if (pthread_create(&tid, &attr, gs_sys_tick_thread, NULL) != 0)
    {
        log_err("pthread_create failed");
        return -1;
    }

    pthread_attr_destroy(&attr);
    log_info("System Tick thread started (%dms)", TICK_MS);
    return 0;
}

void dump_date(void)
{
    struct timeval tv;
    struct tm *local;

    // 获取高精度时间（含微秒）
    gettimeofday(&tv, NULL);
    
    // 转本地时间
    local = localtime(&tv.tv_sec);

    // 输出：年-月-日 时:分:秒
    log_warn("date: %04d-%02d-%02d %02d:%02d:%02d.%03ld",
           local->tm_year + 1900,
           local->tm_mon + 1,
           local->tm_mday,
           local->tm_hour,
           local->tm_min,
           local->tm_sec,
           tv.tv_usec / 1000);  // 微秒转毫秒

}



