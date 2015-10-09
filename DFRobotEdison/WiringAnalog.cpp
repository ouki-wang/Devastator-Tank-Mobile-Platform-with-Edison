/*
 *Copyright (c) 2014, �ɶ���Ȥ�Ƽ����޹�˾
 *All rights reserved
 *
 *�ļ����ƣ�WiringAnalog.c
 *�ļ���ʶ��
 *ժ    Ҫ�� ��ȡIO�ڵ�ģ�����Ϳ��Ƶ��
 *
 *��ǰ�汾��V1.1
 *��    �ߣ�������(15196617738@163.com)
 *���ʱ�䣺2015.1.7
 *
 *ȡ���汾��V1.0
 *ԭ����  : ������(15196617738@163.com)
 *���ʱ�䣺2014.12.18
 */

#include "DFRobot.h"

DFrobotEdison dFrobotEdison = DFrobotEdison();

/*
 *brief: ���캯��
 *param: @No 
 *retval: @No
 */
DFrobotEdison::DFrobotEdison()
{
	
}

/*
 *brief: ��ȡA0-A5�˿ڵ�ģ����
 *param: @(input) ulPin:A0, A1, A2, A3, A4, A5
 *retval: @��ȡ������ģ��ֵ
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
 *brief: �����ʼ��
 *param: @(input) numMotor:M1��M2
 *retval: NO
 */
void DFrobotEdison::begin( const uint32_t Motor)
{
	numMotor = Motor;

	stop();
}

/*
 *brief: �������
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
 *brief: ֹͣ���
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
 *brief: ���Ƶ��
 *param: @(input) ulValue:����ٶ�ֵ
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
 *brief: ���Ƶ��ת������
 *param: @(input) direction:����ֵ
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
 *brief: д����ȥ�ӻ�
 *param: @(input) cmd:д��������
         @(input) value: д����
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
 *brief: ������ȥ�ӻ�
 *param: @(output) outBuf:������ģ��ֵ
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
