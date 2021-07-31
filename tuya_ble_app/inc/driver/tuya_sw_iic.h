#ifndef __TUYA_SW_IIC_H_
#define __TUYA_SW_IIC_H_

#include "tuya_ble_type.h"
#include "gpio_8258.h"
#include "tuya_spoon_system.h"

#define IIC_SDA_OUT 	{gpio_set_input_en(TMP75_SDA, 0); gpio_set_output_en(TMP75_SDA, 1);}
#define IIC_SDA_IN  	{gpio_set_output_en(TMP75_SDA, 0); gpio_set_input_en(TMP75_SDA, 1);}
#define IIC_SDA_SET 	gpio_write(TMP75_SDA, 1)
#define IIC_SDA_RESET   gpio_write(TMP75_SDA, 0)
#define IIC_SDA_State 	gpio_read(TMP75_SDA)

#define IIC_SCL_OUT 	gpio_set_output_en(TMP75_SCL, 1)
#define IIC_SCL_IN  	gpio_set_input_en(TMP75_SCL, 1)
#define IIC_SCL_SET 	gpio_write(TMP75_SCL, 1)
#define IIC_SCL_RESET   gpio_write(TMP75_SCL, 0)
#define IIC_SCL_State 	gpio_read(TMP75_SCL)


void IIC_Init(void);
uint8_t IIC_ReadMulByte(uint8_t DrvAddr,uint16_t ReadAddr,uint8_t *pBuffer,uint16_t NumToRead);
uint8_t IIC_WriteMulByte(uint8_t DrvAddr,uint16_t WriteAddr,uint8_t *pBuffer,uint16_t NumToWrite);


#endif
