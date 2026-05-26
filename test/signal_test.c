/*
 * @Author: zenghw
 * @Date: 2026-05-27 00:03:57
 * @Description: 信号槽测试
 * @LastEditTime: 2026-05-27 00:33:35
 */

#include "common.h"
#include "signal.h"

DEFINE_SIGNAL(temperature, float); /* 定义温度信号类型 */
temperature_signal_t temp_sig = { .sig_name = "temperature", .slot_count = 0};

void on_temp_change(float temp) 
{ 
	log_warn("温度: %.1f\n", temp);
}

/* ========== 信号定义 ========== */
/* 温度模块对外只暴露这一个信号 */
signal_t temp_updated = SIGNAL_INIT(temp_updated);

/* ========== 槽函数实现（各模块各自实现） ========== */

/* LCD显示模块 */
void lcd_show_temp(void *arg)
{
	float temp = *(float *)arg;
	log_warn("[LCD] 当前温度: %.1f°C", temp);
}

/* 报警模块 */
void alarm_check(void *arg)
{
	float temp = *(float *)arg;
	if(temp > 85.0f) {
		log_err("[ALARM] 温度过高! %.1f°C 超过阈值!", temp);
		/* 这里可以驱动蜂鸣器、发送告警等 */
	}
}

/* 数据记录模块 */
void logger_record(void *arg)
{
	float temp = *(float *)arg;
	log_info("[LOG] 记录温度: %.1f°C", temp);
	/* 实际项目中写入Flash或SD卡 */
}

/* ========== 系统初始化 ========== */
void signal_test_init(void)
{
	/* 各模块自行注册关心的信号 */
	signal_connect(&temp_updated, lcd_show_temp);
	signal_connect(&temp_updated, alarm_check);
	signal_connect(&temp_updated, logger_record);
}

/* ========== 温度采集任务 ========== */
void signal_test_task(void)
{
	float current_temp = 90.0f; //read_sensor(); /* 读取传感器 */
	signal_emit(&temp_updated, &current_temp); /* 发出信号，完事 */
}
