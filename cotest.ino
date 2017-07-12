#include <ArduinoJson.h>
#include <SoftwareSerial.h>  
#include <TimerOne.h>
#define DebugSerial Serial
#define GPRS900ASerail Serial1//GPRS模块
#define PMS5003Serial Serial2
#define DSSerial Serial3//ch2o传感器

#define Success 1U
#define Failure 0U

void setup(){
	
  DebugSerial.begin(9600);
  delay(2000);

}


void loop(){
  //硬件初始化
  
  float n = analogRead(0) * 0.01444;
  DebugSerial.println(n);

  delay(1000);
}
