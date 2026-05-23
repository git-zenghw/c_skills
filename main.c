/*
 * @Author: zenghw
 * @Date: 2026-05-22 23:21:24
 * @Description: 
 * @LastEditTime: 2026-05-23 17:15:16
 * Copyright (c) 2023 by 汇创智控科技有限公司, All Rights Reserved
 */

#include "common.h"
#include "task.h"
#include "timer.h"


#define TEST_TASK    0
#define TEST_TIMER    1

int main(int argc, char *argv[])
{

    log_info("linux c skills demo");
#if TEST_TASK
    task_loop();
#endif

#if TEST_TIMER
   timer_loop();
#endif

    return 0;
}