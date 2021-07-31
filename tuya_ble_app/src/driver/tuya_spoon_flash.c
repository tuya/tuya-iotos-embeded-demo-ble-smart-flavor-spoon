/*
 * @Author: zzw
 * @email: huanling.zhang@tuya.com
 * @LastEditors: zzw
 * @file name: tuya_spoon_flash.c
 * @Description: ���µ��������ϵ����
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2021-07-1
 * @LastEditTime: 2021-07-30
 *
 */
#include "tuya_spoon_flash.h"
#include "stack/ble/ble.h"
#include "tuya_spoon_system.h"

#define FLASH_ADDR      0x040000	//0x040000~0x066000Ϊflashδʹ������
#define FLASH_BUFF_LEN  1
#define POWER_COUNT		2

volatile unsigned char Flash_Read_Buff[FLASH_BUFF_LEN] = {0};
volatile unsigned char Flash_Write_Buff[FLASH_BUFF_LEN] = {0};

// ���flash�жϵ�������־λ
static int clear_ble_flag(void)
{
	Flash_Write_Buff[0] = 0;
	flash_erase_sector(FLASH_ADDR);
	flash_write_page(FLASH_ADDR, FLASH_BUFF_LEN, (unsigned char *)Flash_Write_Buff);
	blt_soft_timer_delete(&clear_ble_flag);
	return 0;
}

//ͨ�ϵ���������
uint8_t switch_setup_network(void)
{
	blt_soft_timer_add(&clear_ble_flag, 5000*TIME_MS);	// �豸����ʹ��5s�����flash�жϵ�������־λ
	flash_read_page(FLASH_ADDR, FLASH_BUFF_LEN, (unsigned char *)Flash_Read_Buff);
	Flash_Write_Buff[0] = Flash_Read_Buff[0];
	TUYA_APP_LOG_INFO("Flash_Read_Buff[1] = %d", Flash_Read_Buff[0]);

	// ���µ�������������
	if (Flash_Read_Buff[0] < POWER_COUNT) {
		Flash_Write_Buff[0]++;
		flash_erase_sector(FLASH_ADDR);
		flash_write_page(FLASH_ADDR, FLASH_BUFF_LEN, (unsigned char *)Flash_Write_Buff);
	} else {
		TUYA_APP_LOG_INFO("start connect new device");
		tuya_ble_device_factory_reset();	// �Ͽ��뵱ǰ�豸������(������Ѱ��豸����Ϣ)����������ģʽ
	}

	return Flash_Write_Buff[0];
}

