/*
 * @Author: zzw
 * @email: huanling.zhang@tuya.com
 * @LastEditors: zzw
 * @file name: tuya_spoon_system.h
 * @Description: DP数据上报下发
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2021-07-1
 * @LastEditTime: 2021-07-30
 *
 */

#ifndef __TUYA_SPOON_SYSTEM_H_
#define __TUYA_SPOON_SYSTEM_H_

#include <stdbool.h>
#include "tuya_ble_type.h"
#include "tuya_ble_log.h"
#include "blt_soft_timer.h"
#include "timer.h"
#include "tuya_ble_api.h"

#define ON      1
#define OFF     0
#define ALARM   1
#define NORMAL  0

#define TMP75_SCL		GPIO_PD3
#define TMP75_SDA		GPIO_PD7
#define HEAT_PIN		GPIO_PC0
#define TONGUE_PIN		GPIO_PA0
#define BLE_LED_PIN		GPIO_PB4
#define R_CS_PIN		GPIO_PD2
#define R_INC_PIN		GPIO_PB5
#define R_UD_PIN		GPIO_PC3
#define BATTERY_ADC_PORT 	GPIO_PB6

typedef bool BOOL;

#define TIME_MS 1000
extern unsigned long time_tick;
extern uint8_t status_flag;

typedef enum {
	sour = 0,
	sweet,
	bitter,
	salty,
	user_defined
}palate;

typedef enum{
	uA_20 = 0,
	uA_40,
	uA_60,
	uA_80,
	uA_100,
	uA_120,
	uA_140,
	uA_180,
	uA_200,

	_20uA = 0,
	_40uA = 18,
	_60uA = 45,
	_80uA = 59,
	_100uA = 67,
	_120uA = 72,
	_140uA = 76,
	_180uA = 82,
	_200uA = 83
}electricity;

typedef enum{
	hz_50 = 0,
	hz_100,
	hz_200,
	hz_400,
	hz_600,
	hz_800,
	hz_1000,
	hz_0,

	_hz50 = 20000,
	_hz100 = 10000,
	_hz200 = 5000,
	_hz400 = 2500,
	_hz600 = 1600,
	_hz800 = 1250,
	_hz1000 = 100
}frequency;

typedef enum {
	T_30 = 0,
	T_31,
	T_32,
	T_33,
	T_34,
	T_35,
	T_36,
	T_37,
	T_38,
	T_39,
	T_40,

	// tmp75温度检测范围 -40~125℃
	temp_30 = 260000,
	temp_31 = 270000,
	temp_32 = 280000,
	temp_33 = 290000,
	temp_34 = 300000,
	temp_35 = 310000,
	temp_36 = 320000,
	temp_37 = 330000,
	temp_38 = 340000,
	temp_39 = 350000,
	temp_40 = 360000,

	sweet_bitter_temp_H = 310000,
	sweet_bitter_temp_L = 280000,
	sour_temp = 278000,
	heat_off = -400000
}temperature;

typedef struct {
	BOOL power_alarm;
	palate kinds;
	electricity uA;
	frequency Hz;
	int temp;
}SPOON_STATE_T;

extern SPOON_STATE_T spoon_state;

void spoon_init(void);
int start_heat(void);
void led_connect_status(void);
void app_dp_handle(uint8_t *dp_data);
void user_defined_temp_contorl(unsigned int temp);

#endif
