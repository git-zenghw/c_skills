/*
 * @Author: zenghw
 * @Date: 2026-05-22 23:21:24
 * @Description: 
 * @LastEditTime: 2026-05-23 16:21:22
 * Copyright (c) 2023 by 汇创智控科技有限公司, All Rights Reserved
 */

#include <stdio.h>
#include <stdint.h>
#include "common.h"
#include "task.h"


#define TEST_TASK    1

int main(int argc, char *argv[])
{

    log_info("linux c skills demo");
#if TEST_TASK
    task_loop();
#endif
    return 0;
}