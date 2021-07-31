/*
 * @Author: zzw
 * @email: huanling.zhang@tuya.com
 * @LastEditors: zzw
 * @file name: tuya_spoon_flash.c
 * @Description: 上下电配网，断电记忆
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2021-07-1
 * @LastEditTime: 2021-07-30
 *
 */
#include "tuya_spoon_flash.h"
#include "stack/ble/ble.h"
#include "tuya_spoon_system.h"

#define FLASH_ADDR      0x040000	//0x040000~0x066000为flash未使用区域
#define FLASH_BUFF_LEN  1
#define POWER_COUNT		2

volatile unsigned char Flash_Read_Buff[FLASH_BUFF_LEN] = {0};
volatile unsigned char Flash_Write_Buff[FLASH_BUFF_LEN] = {0};

// 清除flash中断电配网标志位
static int clear_ble_flag(void)
{
	Flash_Write_Buff[0] = 0;
	flash_erase_sector(FLASH_ADDR);
	flash_write_page(FLASH_ADDR, FLASH_BUFF_LEN, (unsigned char *)Flash_Write_Buff);
	blt_soft_timer_delete(&clear_ble_flag);
	return 0;
}

//通断电三次配网
uint8_t switch_setup_network(void)
{
	blt_soft_timer_add(&clear_ble_flag, 5000*TIME_MS);	// 设备正常使用5s后清除flash中断电配网标志位
	flash_read_page(FLASH_ADDR, FLASH_BUFF_LEN, (unsigned char *)Flash_Read_Buff);
	Flash_Write_Buff[0] = Flash_Read_Buff[0];
	TUYA_APP_LOG_INFO("Flash_Read_Buff[1] = %d", Flash_Read_Buff[0]);

	// 上下电三次重新配网
	if (Flash_Read_Buff[0] < POWER_COUNT) {
		Flash_Write_Buff[0]++;
		flash_erase_sector(FLASH_ADDR);
		flash_write_page(FLASH_ADDR, FLASH_BUFF_LEN, (unsigned char *)Flash_Write_Buff);
	} else {
		TUYA_APP_LOG_INFO("start connect new device");
		tuya_ble_device_factory_reset();	// 断开与当前设备的连接(会清除已绑定设备的信息)，进入配网模式
	}

	return Flash_Write_Buff[0];
}

