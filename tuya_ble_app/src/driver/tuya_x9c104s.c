/*
 * @Author: zzw
 * @email: huanling.zhang@tuya.com
 * @LastEditors: zzw
 * @file name: tuya_x9c104s.c
 * @Description: x9c104s阻值变换
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2021-07-1
 * @LastEditTime: 2021-07-30
 *
 */
#include "tuya_x9c104s.h"
#include "tuya_spoon_system.h"
#include "gpio_8258.h"

void x9c104s_pins_init(void)
{
	gpio_set_func(R_CS_PIN, AS_GPIO);
	gpio_set_func(R_INC_PIN, AS_GPIO);
	gpio_set_func(R_UD_PIN, AS_GPIO);

	gpio_set_output_en(R_CS_PIN, 1);
	gpio_set_output_en(R_INC_PIN, 1);
	gpio_set_output_en(R_UD_PIN, 1);
}

static void x9c104s_reset(void)
{
	uint8_t i;
	gpio_write(R_UD_PIN, 0);
	sleep_us(5);

	for(i=0; i<100; i++) {	//该芯片为100抽头
		gpio_write(R_INC_PIN, 0);
		sleep_us(2);
		gpio_write(R_INC_PIN, 1);
		sleep_us(2);
	}
	gpio_write(R_UD_PIN, 1);
	sleep_us(5);
}

void x9c104s_set(uint8_t num)
{
	uint8_t i;

	x9c104s_reset();
	gpio_write(R_UD_PIN, 1);
	sleep_us(5);

	for (i=0; i<num; i++) {
		gpio_write(R_INC_PIN, 0);
		sleep_us(2);
		gpio_write(R_INC_PIN, 1);
		sleep_us(2);
	}

	gpio_write(R_UD_PIN, 1);
	sleep_us(5);
}


