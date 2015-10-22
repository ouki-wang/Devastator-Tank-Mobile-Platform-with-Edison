/*
 *Copyright (c) 2014, Chengdu Geeker Technology Co., Ltd.
 *All rights reserved
 *
 *FileName :DFRobot.h
 *File Tag :
 *Summary  : remeo for edison library
 *
 *Version  : V1.1
 *Author   : weibing.deng(15196617738@163.com)
 *time     : 2015.1.7
 *
 *Version  : V1.0
 *Author   : weibing.deng(15196617738@163.com)
 *Time     : 2014.12.18
 */
 
#ifndef DFROBOT_H
#define DFROBOT_H

#include "IIC1.h"
#include "Arduino.h"
#include "pins_arduino.h"
#include "trace.h"

#define MUX_ANALOG_INPUTS 19
#define MIN_ANALOG_INPUTS  14
#define M1 10 //Moter M1
#define M2 9  //Moter M2 
#define CLOCKWISE    1 
#define ANTICLOCKWISE  0  
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
	public:
		uint32_t numMotor;
};

extern DFrobotEdison dFrobotEdison ;

#endif