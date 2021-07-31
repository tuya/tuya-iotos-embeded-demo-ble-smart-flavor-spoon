/*
 * @Author: zzw
 * @email: huanling.zhang@tuya.com
 * @LastEditors: zzw
 * @file name: tuya_battery_monitor.c
 * @Description: 电池电量检测
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2021-07-1
 * @LastEditTime: 2021-07-30
 *
 */
#ifndef __TUYA_BATTERY_MONITOR_
#define __TUYA_BATTERY_MONITOR_

#include "gpio_8258.h"

#define BATTERY_ADC_UPPER_LIMIT 	   4200		//满电量电压值
#define BATTERY_ADC_LOWER_LIMIT        3570		//电量不足电压值

unsigned short get_battery_value(void);

#endif
