/*
 *Copyright (c) 2014, �ɶ���Ȥ�Ƽ����޹�˾
 *All rights reserved
 *
 *�ļ����ƣ�DFRobot.h
 *�ļ���ʶ��
 *ժ    Ҫ�� dfrobot ���ļ�
 *
 *��ǰ�汾��V1.1
 *��    �ߣ�������(15196617738@163.com)
 *���ʱ�䣺2015.1.7
 *
 *ȡ���汾��V1.0
 *ԭ����  : ������(15196617738@163.com)
 *���ʱ�䣺2014.12.18
 */
 
#ifndef DFROBOT_H
#define DFROBOT_H

#include "IIC1.h"
#include "Arduino.h"
#include "pins_arduino.h"
#include "trace.h"
#include "Servo1.h"

#define MUX_ANALOG_INPUTS 19
#define MIN_ANALOG_INPUTS  14
#define M1 10 //�������M1
#define M2 9  //�������M2 
#define CLOCKWISE    1 //���˳ʱ����ת
#define ANTICLOCKWISE  0  //�����ʱ����ת
#define MY_TRACE_PREFIX "wiring_analog"
#define analogRead  (dFrobotEdison.readAnalog)
#define controlMotor (dFrobotEdison.motorControl)
 
class DFrobotEdison {
	public:  
		DFrobotEdison();
		uint32_t readAnalog(uint32_t ulPin);  
		void begin( const uint32_t Motor);
		void move(void);
		void setDirection(const uint8_t direction);
		void setSpeed( const uint32_t whatSpeed );
		void stop(void); 
		
	private:
		void writeToSlv(const uint32_t cmd, const uint32_t value );
		void writebyte(const uint32_t value );
		void readFromSlv(uint32_t *outBuf);

		uint32_t numMotor;
};

extern DFrobotEdison dFrobotEdison ;

#endif