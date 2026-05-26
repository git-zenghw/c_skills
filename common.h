/*
 * @Author: zenghw
 * @Date: 2026-05-23 16:08:20
 * @Description: 公共接口头文件，提供错误码、调试接口等
 * @LastEditTime: 2026-05-25 21:46:02
 */

#ifndef COMMON_H
#define COMMON_H

#ifdef __cplusplus
extern "C" {
#endif
#include <stdio.h>
#include <stdint.h>

#define OBJECT_HEAD_ADDR(type, num, ptr) \
    (type*)((char*)(ptr) - offsetof(type, num))

// ==================== 颜色定义 ANSI 转义序列 ====================
// 终端颜色控制，Linux / macOS 通用
#define COLOR_NONE          "\033[0m"       // 重置颜色
#define COLOR_RED           "\033[31m"      // 红色
#define COLOR_GREEN         "\033[32m"      // 绿色
#define COLOR_YELLOW        "\033[33m"      // 黄色

// ==================== 日志宏开关配置 ====================
// 注释掉下面一行可以关闭所有日志输出
#define ENABLE_LOG
// 注释掉下面一行可以关闭颜色（适合重定向到文件）
#define ENABLE_COLOR

// ==================== 核心日志宏定义 ====================
#ifdef ENABLE_LOG
// 正常信息（绿色，输出到标准输出 stdout）
#define log_info(fmt, ...) \
    do { \
        printf(COLOR_GREEN "[INFO] "); \
        printf("%s:%d-%s | ", __FILE__, __LINE__, __func__); \
        printf(fmt, ##__VA_ARGS__); \
        printf(COLOR_NONE "\n"); \
    } while (0)

// 警告信息（黄色，输出到标准错误 stderr）
#define log_warn(fmt, ...) \
    do { \
        fprintf(stderr, COLOR_YELLOW "[WARN] "); \
        fprintf(stderr, "%s:%d-%s | ", __FILE__, __LINE__, __func__); \
        fprintf(stderr, fmt, ##__VA_ARGS__); \
        fprintf(stderr, COLOR_NONE "\n"); \
    } while (0)

// 错误信息（红色，输出到标准错误 stderr）
#define log_err(fmt, ...) \
    do { \
        fprintf(stderr, COLOR_RED "[ERROR] "); \
        fprintf(stderr, "%s:%d-%s | ", __FILE__, __LINE__, __func__); \
        fprintf(stderr, fmt, ##__VA_ARGS__); \
        fprintf(stderr, COLOR_NONE "\n"); \
    } while (0)

#else
// 关闭日志时，宏为空
#define log_info(fmt, ...)
#define log_warn(fmt, ...)
#define log_error(fmt, ...)
#endif


/* 
    错误码定义 
    1、共32bit，高16bit表示默认类型，低16bit表示模块具体错误类型
    2、0表示成功
    3、接口的错误码通过返回值提供，需要返回有效数据均通过参数返回
*/
#define NO_ERROR  (0)
#define SUCCESS  NO_ERROR 



#ifdef __cplusplus
}
#endif

#endif