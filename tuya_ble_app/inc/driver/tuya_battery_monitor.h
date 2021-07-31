/*
 * @Author: zzw
 * @email: huanling.zhang@tuya.com
 * @LastEditors: zzw
 * @file name: tuya_battery_monitor.c
 * @Description: ��ص������
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2021-07-1
 * @LastEditTime: 2021-07-30
 *
 */
#ifndef __TUYA_BATTERY_MONITOR_
#define __TUYA_BATTERY_MONITOR_

#include "gpio_8258.h"

#define BATTERY_ADC_UPPER_LIMIT 	   4200		//��������ѹֵ
#define BATTERY_ADC_LOWER_LIMIT        3570		//���������ѹֵ

unsigned short get_battery_value(void);

#endif
