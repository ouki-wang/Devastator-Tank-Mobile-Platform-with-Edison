/*
 *Copyright (c) 2014, 成都极趣科技有限公司
 *All rights reserved
 *
 *文件名称：analogRead.ino
 *文件标识：
 *摘    要：A0-A5端口模拟读例程  
 *
 *当前版本：V1.0
 *作    者：邓卫兵(15196617738@163.com)
 *完成时间：2014.12.22
 *
 *取代版本：无
 *原作者  : 无
 *完成时间：无
 */
#include <DFRobot.h>
#include <IIC1.h>

unsigned int value = 0;

void setup() {
 Serial.begin(9600);
}

void loop() {

  value = analogRead(A0);
 
  Serial.println((int)value);
  
  delay(2000);
}
