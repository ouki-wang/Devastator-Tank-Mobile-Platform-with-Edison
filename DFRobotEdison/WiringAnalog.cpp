/*
 *Copyright (c) 2014, 成都极趣科技有限公司
 *All rights reserved
 *
 *文件名称：WiringAnalog.c
 *文件标识：
 *摘    要： 读取IO口的模拟量和控制电机
 *
 *当前版本：V1.1
 *作    者：邓卫兵(15196617738@163.com)
 *完成时间：2015.1.7
 *
 *取代版本：V1.0
 *原作者  : 邓卫兵(15196617738@163.com)
 *完成时间：2014.12.18
 */

#include "DFRobot.h"

DFrobotEdison dFrobotEdison = DFrobotEdison();

/*
 *brief: 构造函数
 *param: @No 
 *retval: @No
 */
DFrobotEdison::DFrobotEdison()
{
	
}

/*
 *brief: 读取A0-A5端口的模拟量
 *param: @(input) ulPin:A0, A1, A2, A3, A4, A5
 *retval: @读取出来的模拟值
 */
uint32_t DFrobotEdison::readAnalog(uint32_t ulPin)
{
	uint32_t ulValue = 0; 
	if ((ulPin > MUX_ANALOG_INPUTS)  || (ulPin < MIN_ANALOG_INPUTS))
	{
		trace_error("Invalid analog input channel specified\n");
		return 0;
	}
	
	switch (ulPin)
	{
		case A0:
		{
			writeToSlv(0xA0, 1);
			readFromSlv(&ulValue);
			break;
		}
		case A1:
		{
			writeToSlv(0xA1, 1);
			readFromSlv(&ulValue);

			break;
		}
		case A2:
		{
			writeToSlv(0xA2, 1);
			readFromSlv(&ulValue);

			break;
		}
		case A3:
		{
			writeToSlv(0xA3, 1);
			readFromSlv(&ulValue);

			break;
		}
		case A4:
		{
			writeToSlv(0xA4, 1);
			readFromSlv(&ulValue);

			break;
		}
		case A5:
		{
			writeToSlv(0xA5, 1);
			readFromSlv(&ulValue);

			break;
		}
		default:
		{
			trace_error("error \n");
			break;
		}
	}

	return ulValue;
}

/*
 *brief: 电机初始化
 *param: @(input) numMotor:M1，M2
 *retval: NO
 */
void DFrobotEdison::begin( const uint32_t Motor)
{
	numMotor = Motor;

	stop();
}

/*
 *brief: 电机启动
 *param: void
 *retval: NO
 */
void DFrobotEdison::move(void)
{
	switch(numMotor)
	{
		case M1:
		{
			writeToSlv(0xC1, 0xff);
			break;
		}

		case M2:
		{
			writeToSlv(0xC2, 0xff);
			break;
		}

		default:
		{
			trace_error("error \n");
			break;
		}
	}
}

/*
 *brief: 停止电机
 *param: void
 *retval: NO
 */
void DFrobotEdison::stop(void)
{
	switch(numMotor)
	{
		case M1:
		{
			writeToSlv(0xC1, 0x0);
			break;
		}

		case M2:
		{
			writeToSlv(0xC2, 0x0);
			break;
		}

		default:
		{
			trace_error("error \n");
			break;
		}
	}
}

/*
 *brief: 控制电机
 *param: @(input) ulValue:电机速度值
 *retval: NO
 */
void DFrobotEdison::setSpeed( const uint32_t whatSpeed )
{
	switch (numMotor)
	{
		case M1:
		{
			writeToSlv(0xC1, whatSpeed);
			break;
		}

		case M2:
		{
			writeToSlv(0xC2, whatSpeed);
			break;
		}

		default:
		{
			trace_error("error \n");
			break;
		}
	}
}

/*
 *brief: 控制电机转动方向
 *param: @(input) direction:方向值
 *retval: NO
 */
 void DFrobotEdison::setDirection(const uint8_t direction)
{
	switch (numMotor)
	{
		case M1:
		{
			writeToSlv(0xB1, direction);break;
		}

		case M2:
		{
			writeToSlv(0xB2, direction);break;
		}

		default:
		{
			trace_error("error \n");
			break;
		}
	}
}

/*
 *brief: 写数据去从机
 *param: @(input) cmd:写命令数据
         @(input) value: 写数据
 *retval: NO
 */
 void DFrobotEdison::writeToSlv(uint32_t cmd, uint32_t value )
{
    Wire.begin();        
    Wire.beginTransmission(4);
	Wire.write(0x55);
	Wire.write(0xaa);
    Wire.write(cmd);        
    Wire.write(value);
	Wire.write((unsigned char)(0x55+0xaa+cmd+value));	
    Wire.endTransmission();    
}

/*
 *brief: 读数据去从机
 *param: @(output) outBuf:读出的模拟值
 *retval: NO
 */
 void DFrobotEdison::readFromSlv(uint32_t *outBuf)
{
	unsigned int value = 0;
	unsigned int count = 0;
	unsigned char num=0;
	unsigned char buf[5]={0};
	Wire.begin();
	int ret = Wire.requestFrom(4, 5);
	while(num<5){
		count = 0;
		while (Wire.available() == 0)
		{
			if(++count == 1000){
				value = -1;
				break;
			}
			delay(1);
		}
		if(count < 1000){
			buf[num++] = Wire.read();
		}else{
			break;
		}
	}
 
	
	if((count >= 1000)
		||(buf[0]!=0x55)
		||(buf[1]!=0xaa)
		||(buf[4]!=(unsigned char)(buf[0]+buf[1]+buf[2]+buf[3]))){
		*outBuf = -1;
	}else{
		*outBuf = buf[2]+(buf[3]<<8); 
	}
}
