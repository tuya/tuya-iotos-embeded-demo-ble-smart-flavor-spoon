/*
 * @Author: zzw
 * @email: huanling.zhang@tuya.com
 * @LastEditors: zzw
 * @file name: tuya_spoon_system.c
 * @Description: DP数据上报下发
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2021-07-1
 * @LastEditTime: 2021-07-30
 *
 */
#include "tuya_spoon_system.h"
#include "tuya_x9c104s.h"
#include "tuya_tmp75.h"
#include "tuya_sw_iic.h"

SPOON_STATE_T spoon_state = {
		.power_alarm = NORMAL,
		.kinds = sour,
		.uA = uA_20,
		.Hz = hz_50,
		.temp = 20
};

unsigned long time_tick = 0;
unsigned long temp_time_tick = 0;

uint8_t status_flag = 0;

static void BleLed_PinInit(void)		//配网指示灯初始化，低有效
{
	gpio_set_func(BLE_LED_PIN, AS_GPIO);
	gpio_set_output_en(BLE_LED_PIN, 1);
	gpio_write(BLE_LED_PIN, 1);
}


static void Heat_Tongue_pin_init(void)		//加热功能、频率引脚出初始化
{
	gpio_set_func(HEAT_PIN, AS_GPIO);
	gpio_set_func(TONGUE_PIN, AS_GPIO);

	gpio_set_output_en(HEAT_PIN, 1);
	gpio_set_output_en(TONGUE_PIN, 1);

	gpio_write(TONGUE_PIN, 0);
	gpio_write(HEAT_PIN, 0);
}

void spoon_init(void)
{
    IIC_Init();
    TMP75_Init();
    x9c104s_pins_init();
    Heat_Tongue_pin_init();
    BleLed_PinInit();
}

void led_connect_status(void)		//配网指示灯状态判断 loop中调用
{
	if (BONDING_CONN == status_flag) {
		gpio_write(BLE_LED_PIN, 0);
	} else {
		if (clock_time_exceed(time_tick, TIME_MS * 400)) {
			time_tick = clock_time();

			gpio_toggle(BLE_LED_PIN);
		}
	}
}

//static int sour_temp_contorl(void)		//咸味温度范围控制
//{
//	if (TMP75_getTemperature() > sour_temp) {
//		gpio_write(HEAT_PIN, 0);
//	}
//	return 0;
//}

static int sweet_temp_contorl(void)		//甜味温度控制
{
	if (TMP75_getTemperature() > sweet_bitter_temp_H) {
		gpio_write(HEAT_PIN, 0);
	} else if (TMP75_getTemperature() < sweet_bitter_temp_L) {
		gpio_write(HEAT_PIN, 1);
	}
	return 0;
}

static int bitter_temp_contorl(void)	//苦味温度控制
{
	if (TMP75_getTemperature() > sweet_bitter_temp_H) {
		gpio_write(HEAT_PIN, 0);
	} else if (TMP75_getTemperature() < sweet_bitter_temp_L) {
		gpio_write(HEAT_PIN, 1);
	}
	return 0;
}

void user_defined_temp_contorl(unsigned int temp)	//自定义模式下温度控制
{
	if (clock_time_exceed(temp_time_tick, TIME_MS*200)) {
		temp_time_tick = clock_time();

		if (heat_off == temp) {
			return;
		} else if (temp != heat_off && TMP75_getTemperature() > temp) {
			gpio_write(HEAT_PIN, 0);
		} else if (TMP75_getTemperature() < temp) {
			gpio_write(HEAT_PIN, 1);
		}
	}
}

static int set_tongue_Hz(void)	//设置频率，定时器回调
{
	// duty = 50%
	gpio_toggle(TONGUE_PIN);
	return 0;
}

/**
 * @brief     APP端DP数据下发处理函数
 * @param[in] dp_data - 存放下发数据  dp_data[] = {dp_id, dp_type, dp_len, dp_data}
 * @return    none
 */
void app_dp_handle(uint8_t *dp_data)
{
	TUYA_APP_LOG_INFO("dp_data:%d  %d  %d  %d", dp_data[0], dp_data[1], dp_data[2], dp_data[3]);

	switch (dp_data[0]) {
	// 味觉 酸、甜、苦、咸
	case 0x66:
		switch (dp_data[3]) {
		case sour:
			spoon_state.temp = sour_temp;
			blt_soft_timer_delete(&sweet_temp_contorl);
			blt_soft_timer_delete(&bitter_temp_contorl);
			gpio_write(TONGUE_PIN, 1);
			x9c104s_set(_140uA);
//			blt_soft_timer_add(&sour_temp_contorl, 200*TIME_MS);
			break;
		case sweet:
			spoon_state.temp = heat_off;
//			blt_soft_timer_delete(&sour_temp_contorl);
			blt_soft_timer_delete(&bitter_temp_contorl);
			gpio_write(TONGUE_PIN, 1);
			x9c104s_set(_80uA);
			blt_soft_timer_add(&sweet_temp_contorl, 200*TIME_MS);
			break;
		case bitter:
			spoon_state.temp = heat_off;
//			blt_soft_timer_delete(&sour_temp_contorl);
			blt_soft_timer_delete(&sweet_temp_contorl);
			gpio_write(TONGUE_PIN, 1);
			x9c104s_set(_100uA);
			blt_soft_timer_add(&bitter_temp_contorl, 200*TIME_MS);
			break;
		case salty:
			spoon_state.temp = heat_off;
//			blt_soft_timer_delete(&sour_temp_contorl);
			blt_soft_timer_delete(&sweet_temp_contorl);
			blt_soft_timer_delete(&bitter_temp_contorl);
			x9c104s_set(_60uA);
			blt_soft_timer_add(&set_tongue_Hz, _hz100);
			break;
		case user_defined:
//			blt_soft_timer_sour_tempdelete(&sour_temp_contorl);
			blt_soft_timer_delete(&sweet_temp_contorl);
			blt_soft_timer_delete(&bitter_temp_contorl);
			x9c104s_set(_60uA);
			blt_soft_timer_add(&set_tongue_Hz, _hz100);
			break;
		default:
			break;
		}
		break;

	// 电流调节 自定义模式下使用
	case 0x67:
		switch (dp_data[3]) {
		case uA_20:
			x9c104s_set(_20uA);
			break;
		case uA_40:
			x9c104s_set(_40uA);
			break;
		case uA_60:
			x9c104s_set(_60uA);
			break;
		case uA_80:
			x9c104s_set(_80uA);
			break;
		case uA_100:
			x9c104s_set(_100uA);
			break;
		case uA_120:
			x9c104s_set(_120uA);
			break;
		case uA_140:
			x9c104s_set(_140uA);
			break;
		case uA_180:
			x9c104s_set(_180uA);
			break;
		case uA_200:
			x9c104s_set(_200uA);
			break;
		default:
			break;
		}
		break;

	// 频率调节 自定义模式下使用
	case 0x68:
		switch  (dp_data[3]) {
		case hz_50:
			blt_soft_timer_add(&set_tongue_Hz, _hz50);
			break;
		case hz_100:
			blt_soft_timer_add(&set_tongue_Hz, _hz100);
			break;
		case hz_200:
			blt_soft_timer_add(&set_tongue_Hz, _hz200);
			break;
		case hz_400:
			blt_soft_timer_add(&set_tongue_Hz, _hz400);
			break;
		case hz_600:
			blt_soft_timer_add(&set_tongue_Hz, _hz600);
			break;
		case hz_800:
			blt_soft_timer_add(&set_tongue_Hz, _hz800);
			break;
		case hz_1000:
			blt_soft_timer_add(&set_tongue_Hz, _hz1000);
			break;
		case hz_0:
			blt_soft_timer_delete(&set_tongue_Hz);
			gpio_write(TONGUE_PIN, 1);
			break;
		default:
			break;
		}
		break;

	// 温度调节 自定义模式下使用
	case 0x69:
		switch (dp_data[3]) {
		case T_30:
			break;
		case T_31:
			spoon_state.temp = temp_31;
			break;
		case T_32:
			spoon_state.temp = temp_32;
			break;
		case T_33:
			spoon_state.temp = temp_33;
			break;
		case T_34:
			spoon_state.temp = temp_34;
			break;
		case T_35:
			spoon_state.temp = temp_35;
			break;
		case T_36:
			spoon_state.temp = temp_36;
			break;
		case T_37:
			spoon_state.temp = temp_37;
			break;
		case T_38:
			spoon_state.temp = temp_38;
			break;
		case T_39:
			spoon_state.temp = temp_39;
			break;
		case T_40:
			spoon_state.temp = temp_40;
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}

