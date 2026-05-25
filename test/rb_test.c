/*
 * @Author: zenghw
 * @Date: 2026-05-25 21:08:00
 * @Description: 
 * @LastEditTime: 2026-05-25 21:37:11
 */
#include <assert.h>
#include "common.h"
#include "ringbuffer.h"

#define RB_SIZE 16
static void rb_test_basic(void)
{
    uint8_t buf[RB_SIZE];
    struct rt_ringbuffer rb;
    uint8_t mem[RB_SIZE];

    rt_ringbuffer_init(&rb, mem, RB_SIZE);

    rt_ringbuffer_put(&rb, (uint8_t[]){1,2,3,4}, 4);
    rt_ringbuffer_get(&rb, buf, 4);

    assert(buf[0] == 1 && buf[3] == 4);
    log_info("basic test pass");

    rt_ringbuffer_destroy(&rb);
}

static void rb_test_try(void)
{
    uint8_t buf[RB_SIZE];
    struct rt_ringbuffer rb;
    uint8_t mem[RB_SIZE];

    rt_ringbuffer_init(&rb, mem, RB_SIZE);

    rt_ringbuffer_try_put(&rb, (uint8_t[]){10,11,12}, 3);
    rt_ringbuffer_try_get(&rb, buf, 3);

    assert(buf[0] == 10 && buf[2] == 12);
    log_info("try test pass");

    rt_ringbuffer_destroy(&rb);
}

static void rb_test_char(void)
{
    uint8_t ch;
    struct rt_ringbuffer rb;
    uint8_t mem[RB_SIZE];

    rt_ringbuffer_init(&rb, mem, RB_SIZE);

    rt_ringbuffer_putchar(&rb, 'A');
    rt_ringbuffer_putchar(&rb, 'B');

    rt_ringbuffer_getchar(&rb, &ch);
    assert(ch == 'A');

    rt_ringbuffer_getchar(&rb, &ch);
    assert(ch == 'B');

    log_info("char test pass");

    rt_ringbuffer_destroy(&rb);
}

static void rb_test_wrap(void)
{
    uint8_t buf[RB_SIZE + 4];
    struct rt_ringbuffer rb;
    uint8_t mem[RB_SIZE];

    rt_ringbuffer_init(&rb, mem, RB_SIZE);

    rt_ringbuffer_put(&rb, (uint8_t[]){0,1,2,3}, 4);
    rt_ringbuffer_get(&rb, buf, 4);

    rt_ringbuffer_put(&rb, (uint8_t[]){10,11,12,13,14,15}, 6);
    rt_ringbuffer_get(&rb, buf, 6);

    assert(buf[0] == 10 && buf[5] == 15);
    log_info("wrap test pass");

    rt_ringbuffer_destroy(&rb);
}

static void rb_test_null_input(void)
{
    uint8_t buf[8] = {0};
    struct rt_ringbuffer rb;
    uint8_t mem[RB_SIZE];

    rt_ringbuffer_init(&rb, mem, RB_SIZE);
    assert(rt_ringbuffer_put(NULL, buf, 1) == 0);
    assert(rt_ringbuffer_put(&rb, NULL, 1) == 0);
    assert(rt_ringbuffer_put(&rb, buf, 0) == 0);
    assert(rt_ringbuffer_get(NULL, buf, 1) == 0);
    assert(rt_ringbuffer_get(&rb, NULL, 1) == 0);
    assert(rt_ringbuffer_get(&rb, buf, 0) == 0);

    assert(rt_ringbuffer_try_put(NULL, buf, 1) == 0);
    assert(rt_ringbuffer_try_put(&rb, NULL, 1) == 0);
    assert(rt_ringbuffer_try_put(&rb, buf, 0) == 0);
    assert(rt_ringbuffer_try_get(NULL, buf, 1) == 0);
    assert(rt_ringbuffer_try_get(&rb, NULL, 1) == 0);
    assert(rt_ringbuffer_try_get(&rb, buf, 0) == 0);

    assert(rt_ringbuffer_putchar(NULL, 'A') == -1);
    assert(rt_ringbuffer_getchar(NULL, buf) == -1);

    log_info("input invalid test pass");
    rt_ringbuffer_destroy(&rb);
}


static void rb_test_size_one(void)
{
    uint8_t mem[1];
    struct rt_ringbuffer rb;
    uint8_t ch;

    rt_ringbuffer_init(&rb, mem, 1);

    assert(rt_ringbuffer_putchar(&rb, 'X') == 0);
    assert(rt_ringbuffer_putchar(&rb, 'Y') == -1); // full

    assert(rt_ringbuffer_getchar(&rb, &ch) == 0);
    assert(ch == 'X');

    assert(rt_ringbuffer_getchar(&rb, &ch) == -1); // empty

    log_info("size one test pass");
    rt_ringbuffer_destroy(&rb);
}

void ringbuffer_test(void)
{
    log_warn("=== ringbuffer test start ===");

    rb_test_null_input();
    rb_test_size_one();
    rb_test_basic();
    rb_test_try();
    rb_test_char();
    rb_test_wrap();

    log_warn("==== all ringbuffer tests passed ====");
}
