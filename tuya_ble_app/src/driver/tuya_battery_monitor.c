/*
 * @Author: zzw
 * @email: huanling.zhang@tuya.com
 * @LastEditors: zzw
 * @file name: tuya_battery_monitor.c
 * @Description: 锟斤拷氐锟斤拷锟斤拷锟斤拷
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2021-07-1
 * @LastEditTime: 2021-07-30
 *
 */
#include "tuya_battery_monitor.h"
#include "adc.h"
#include "drivers.h"
#include "tuya_spoon_system.h"

uint8_t power_alarm_buf[] = {0x65, 0x01, 0x01, 0x00};
unsigned long battery_time_tick = 0;

#define DP_BUF_LEN	(sizeof(power_alarm_buf) / sizeof(power_alarm_buf[0]))

// ADC锟斤拷锟斤拷频锟斤拷14位锟斤拷锟轿匡拷锟斤拷压1.2V
static unsigned int get_adc_value(void)
{
	adc_init();
	adc_base_init(BATTERY_ADC_PORT);
	adc_power_on_sar_adc(1);
	return adc_sample_and_get_result();
}

// 锟斤拷氐锟窖癸拷杉锟�
unsigned short get_battery_value(void)
{
	if (!clock_time_exceed(battery_time_tick, TIME_MS*10000)) {
			return 0;
	}
	battery_time_tick = clock_time();
	
	unsigned int battery_adc_value = 0;

	battery_adc_value = get_adc_value() * 2;	// 锟斤拷路锟斤拷压锟斤拷实锟绞碉拷氐锟窖� = 2*battery_adc_value
	if (battery_adc_value <= BATTERY_ADC_LOWER_LIMIT) {	//锟斤拷锟斤拷剩锟斤拷10%APP锟斤拷锟斤拷
		power_alarm_buf[3] = ALARM;
		tuya_ble_dp_data_report(power_alarm_buf, DP_BUF_LEN); //锟较憋拷锟斤拷锟斤拷状态
	} else {
		power_alarm_buf[3] = NORMAL;
		tuya_ble_dp_data_report(power_alarm_buf, DP_BUF_LEN);
	}
	TUYA_APP_LOG_INFO("battery_val = %dmv", battery_adc_value);

    return battery_adc_value;
}
