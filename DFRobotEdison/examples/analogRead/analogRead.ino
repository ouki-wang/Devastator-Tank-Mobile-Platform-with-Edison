/*
 *Copyright (c) 2014, Chengdu Geeker Technology Co., Ltd.
 *All rights reserved
 *
 *File Name : analogRead.ino
 *File Tag  :
 *Summary   : A0-A5 port Analog reading example
 *
 *Version   : V1.0
 *Author    : weibing.deng(15196617738@163.com)
 *Time      : 2014.12.22
 */
#include <DFRobot.h>
#include <IIC1.h>

unsigned int value = 0;

void setup() {
 Serial.begin(9600);
}

void loop() {
  value = analogRead(A0);
  Serial.print("A0=");Serial.println((int)value);
  value = analogRead(A1);
  Serial.print("A1=");Serial.println((int)value);
  value = analogRead(A2);
  Serial.print("A2=");Serial.println((int)value);
  value = analogRead(A3);
  Serial.print("A3=");Serial.println((int)value);
  value = analogRead(A4);
  Serial.print("A4=");Serial.println((int)value);
  value = analogRead(A5);
  Serial.print("A5=");Serial.println((int)value);
  
  delay(2000);
}
