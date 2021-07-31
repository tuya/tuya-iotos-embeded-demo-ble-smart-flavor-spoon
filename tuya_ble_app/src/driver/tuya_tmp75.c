/*
 * @Author: zzw
 * @email: huanling.zhang@tuya.com
 * @LastEditors: zzw
 * @file name: tuya_tmp75.c
 * @Description:温度传感器读取当前温度
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2021-07-1
 * @LastEditTime: 2021-07-30
 *
 */
#include "tuya_tmp75.h"
#include "tuya_spoon_system.h"
#include "gpio_8258.h"
#include "tuya_sw_iic.h"

static void TMP75_setConfiguration(int8_t TEMP)
{
	uint8_t buf;
	buf = 96;
	IIC_WriteMulByte(TMP75_ADDRESS, ConfigurationRegister, &buf, 1);
}

static int16_t TMP75_getConfiguration(void)
{
	uint8_t buf[1];
	IIC_ReadMulByte(TMP75_ADDRESS, ConfigurationRegister, buf, 1);

	return buf[0];
}

unsigned int TMP75_getTemperature(void)
{
	uint8_t buf[2];
	unsigned int temp;
	IIC_ReadMulByte(TMP75_ADDRESS, TemperatureRegister, buf, 2);
	temp = ((((buf[0] << 8) | buf[1]) >> 4) * 625);  // 温度放大 10000 倍, 当前分辨率12位，支持9~12位分辨率
	TUYA_APP_LOG_INFO("temp = %d", temp);

	return temp;
}

void TMP75_Init(void)
{
	TMP75_setConfiguration(0x60);		// 3<<5
	if (0x60 == TMP75_getConfiguration()) {
		TUYA_APP_LOG_INFO("TMP75_OK==mode3");
	} else {
		TUYA_APP_LOG_INFO("TMP75_MISS");
	}
}

