#include "tuya_sw_iic.h"
#include "timer.h"
#include "tuya_ble_log.h"

void IIC_Init(void)
{
	gpio_set_func(TMP75_SDA, AS_GPIO);
	gpio_set_func(TMP75_SCL, AS_GPIO);

	IIC_SCL_OUT;
	IIC_SDA_OUT;
}

static void IIC_Start(void)	//产生IIC起始信号
{
	IIC_SDA_OUT;     //sda线输出
	IIC_SDA_SET;	 //IIC_SDA=1;
	IIC_SCL_SET;	 //IIC_SCL=1;
	sleep_us(5);
	IIC_SDA_RESET;	 //IIC_SDA=0;//START:when CLK is high,DATA change form high to low
	sleep_us(5);
	IIC_SCL_RESET;	 //IIC_SCL=0;//钳住I2C总线，准备发送或接收数据
}

static void IIC_Stop(void)		//产生IIC停止信号
{
	IIC_SDA_OUT;
	IIC_SCL_RESET;
	IIC_SDA_RESET;		//IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
	sleep_us(5);
	IIC_SCL_SET;
	sleep_us(5);
	IIC_SDA_SET;		//IIC_SDA=1;//发送I2C总线结束信号
}

static void IIC_Ack(void)		//产生ACK应
{
	IIC_SCL_RESET;
	IIC_SDA_OUT;
	IIC_SDA_RESET;
	sleep_us(5);
	IIC_SCL_SET;
	sleep_us(5);
	IIC_SCL_RESET;
}

static void IIC_NAck(void)		//不产生ACK应答
{
	IIC_SCL_RESET;
	IIC_SDA_OUT;
	IIC_SDA_SET;
	sleep_us(5);
	IIC_SCL_SET;
	sleep_us(5);
	IIC_SCL_RESET;
}

static uint8_t IIC_Wait_Ack(void)		//等待应答信号到来:1,接收应答失败;0,接收应答成功
{
	uint8_t ucErrTime = 0;
	IIC_SDA_IN;
	IIC_SCL_SET;
	sleep_us(5);
	while (IIC_SDA_State) {		//检测SDA是否仍为高电平
		ucErrTime++;
		if (ucErrTime > 250) {
			IIC_Stop();
			return 1;
		}
	}

	IIC_SCL_RESET;

	return 0;
}

static void IIC_Send_Byte(uint8_t txd)	////IIC发送一个字节; 先发送高位
{
	uint8_t t;
	IIC_SDA_OUT;
	IIC_SCL_RESET;	//IIC_SCL=0;//鎷変綆鏃堕挓寮�濮嬫暟鎹紶杈�

	for (t=0; t<8; t++) {
		if ((txd&0x80) >> 7) {
			IIC_SDA_SET;	//IIC_SDA=1;
		} else {
			IIC_SDA_RESET;	//IIC_SDA=0;
		}

		txd <<= 1;
		sleep_us(5);    //拉低时钟开始数据传
		IIC_SCL_SET;
		sleep_us(5);
		IIC_SCL_RESET;
		sleep_us(5);
	}
}

static uint8_t IIC_Read_Byte(unsigned char ack)//读一个字节，可加是否应答位,1加ack，0不加ack 从高位开始读
{
	unsigned char i, receive = 0;
	IIC_SDA_IN;		//SDA设置为输
	for (i=0; i<8; i++) {
		IIC_SCL_RESET;
		sleep_us(5);
		IIC_SCL_SET;
		receive <<= 1;
		if (IIC_SDA_State) {
			receive++;
			sleep_us(5);
		}
	}
	if (ack) {
		IIC_Ack(); //发送ACK
	} else {
		IIC_NAck();//发送nACK
	}

	return receive;
}

static uint8_t IIC_Write_Byte(uint8_t DrvAddr, uint16_t WriteAddr, uint8_t data)//直接写一个字
{
	uint8_t ret = 0;
	IIC_Start();

	IIC_Send_Byte(DrvAddr);	   //发送写命令
	ret |= IIC_Wait_Ack();
	IIC_Send_Byte(WriteAddr);//发送地址
	ret |= IIC_Wait_Ack();
	IIC_Send_Byte(data);      //发送字节
	ret |= IIC_Wait_Ack();

	IIC_Stop();
	sleep_us(10);
	return ret;
}

uint8_t IIC_ReadMulByte(uint8_t DrvAddr, uint16_t ReadAddr, uint8_t *pBuffer, uint16_t NumToRead)//璇诲瓧鑺�
{
	uint8_t ret = 0;

	IIC_Start();
	IIC_Send_Byte(DrvAddr);
	ret |= IIC_Wait_Ack();
	IIC_Send_Byte(ReadAddr);
	ret |= IIC_Wait_Ack();

	IIC_Start();
	IIC_Send_Byte(DrvAddr+1);           //进入接收模式
	ret |= IIC_Wait_Ack();
	while (NumToRead) {
		if (NumToRead == 1) {
			*pBuffer = IIC_Read_Byte(0);
		} else {
			*pBuffer = IIC_Read_Byte(1);
		}
		pBuffer++;
		NumToRead--;
	}
	IIC_Stop();//产生一个停止条件
	return ret;
}

uint8_t IIC_WriteMulByte(uint8_t DrvAddr, uint16_t WriteAddr, uint8_t *pBuffer, uint16_t NumToWrite)//可一次写多个字节
{
	uint8_t ret = 0;
	IIC_Start();

	IIC_Send_Byte(DrvAddr);	    //发送写命令
	ret |= IIC_Wait_Ack();
	IIC_Send_Byte(WriteAddr);	//发送地址
	ret |= IIC_Wait_Ack();

	while (NumToWrite--) {
		IIC_Send_Byte(*pBuffer);     //发送字节
		ret |= IIC_Wait_Ack();
		pBuffer++;
	}
	IIC_Stop();
	sleep_us(10);
	return ret;
}
