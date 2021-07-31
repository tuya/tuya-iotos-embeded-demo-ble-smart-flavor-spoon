/*
 * @Author: zzw
 * @email: huanling.zhang@tuya.com
 * @LastEditors: zzw
 * @file name: tuya_tmp75.h
 * @Description:tmp75温度传感器功能代码
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2021-07-1
 * @LastEditTime: 2021-07-30
 *
 */
#ifndef __TUYA_TMP75_H_
#define __TUYA_TMP75_H_
#include "tuya_ble_type.h"

#define     TMP75_ADDRESS 	  0x90
#define     TemperatureRegister     0x00
#define     ConfigurationRegister   0x01

unsigned int TMP75_getTemperature(void);
void TMP75_Init(void);
void TASK_TMP75(void);

#endif
