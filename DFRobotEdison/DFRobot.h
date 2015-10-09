/*
 *Copyright (c) 2014, 成都极趣科技有限公司
 *All rights reserved
 *
 *文件名称：DFRobot.h
 *文件标识：
 *摘    要： dfrobot 库文件
 *
 *当前版本：V1.1
 *作    者：邓卫兵(15196617738@163.com)
 *完成时间：2015.1.7
 *
 *取代版本：V1.0
 *原作者  : 邓卫兵(15196617738@163.com)
 *完成时间：2014.12.18
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
#define M1 10 //点击控制M1
#define M2 9  //点击控制M2 
#define CLOCKWISE    1 //电机顺时针旋转
#define ANTICLOCKWISE  0  //电机逆时针旋转
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