/*
 * @Author: zenghw
 * @Date: 2026-05-27 00:03:57
 * @Description: 信号槽测试
 * @LastEditTime: 2026-05-27 02:45:50
 */

#include "common.h"
#include "signal.h"
#include "signal_slot.h"

typedef struct {
    signal_base_t base;
} signal_temp_t;

signal_temp_t temp_updated = {0};

/* LCD显示模块 */
void lcd_show_temp(float temp)
{
	log_warn("[LCD] 当前温度: %.1f°C", temp);
}

/* 报警模块 */
void alarm_check(float temp)
{
	if(temp > 85.0f) {
		log_err("[ALARM] 温度过高! %.1f°C 超过阈值!", temp);
		/* 这里可以驱动蜂鸣器、发送告警等 */
	}
}

/* 数据记录模块 */
void logger_record(float temp)
{
	log_info("[LOG] 记录温度: %.1f°C", temp);
	/* 实际项目中写入Flash或SD卡 */
}

void signal_test_task(void)
{
	DEFINE_SIGNAL(temp, float);
	temp_signal_t temp_sig;
	SIGNAL_INIT(temp, &temp_sig);
	SIGNAL_CONNECT(&temp_sig, lcd_show_temp);
	SIGNAL_CONNECT(&temp_sig, alarm_check);
	SIGNAL_CONNECT(&temp_sig, logger_record);

	float temp_value = 90.0f; //read_sensor();
	TYPED_EMIT(&temp_sig, temp_value);
}
