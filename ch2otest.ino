#include <ArduinoJson.h>
#include <SoftwareSerial.h>  
#include <TimerOne.h>
#define DebugSerial Serial
#define GPRS900ASerail Serial1//GPRS模块
#define PMS5003Serial Serial2
#define DSSerial Serial3//ch2o传感器

#define Success 1U
#define Failure 0U

struct SensorValuesBar {
  //float temp;
  //float humid;
  float pm25;
  float pm10;
};

const unsigned int sim900aRxBufferLength = 800;
char sim900aRxBuffer[sim900aRxBufferLength];

struct SensorValuesBar PMS5003Value;
// 传感器数值定义
long pmcf10, pmcf25, pmcf100, pmat10, pmat25, pmat100;

unsigned int ii = 0;
unsigned long  timerCount = 0;//定时器计数器

void setup(){
	
  DSSerial.begin(9600);
  DebugSerial.begin(9600);
  delay(2000);

}


void loop(){
  //硬件初始化
  
  //PMS5003Value = getPMS5003();
  float n = getHCHO();
  DebugSerial.println(n);

  delay(1000);
}

struct SensorValuesBar getPMS5003(){
  struct SensorValuesBar result;
  char command[7]={0x42,0x4d,0x01,0x00,0x00,0x00,0x90};
  DebugSerial.println(command);
  char readBuff;
  DSSerial.write(command);
  int count = 0;
  while (DSSerial.available())
  {
    readBuff = DSSerial.read();
    if(count >= 10){
      break;  // have read all data, read ends
    }
	else
	{
		DebugSerial.print("readbuffxxxxxxxxxxxxxxxxxxx=");
		DebugSerial.print(readBuff);
	}
	
    /* else if(count == 3 || count == 4 || count == 5 || count == 6){
      DebugSerial.print("readbuffxxxxxxxxxxxxxxxxxxx=");
      DebugSerial.print(readBuff+"\r\n");
    } */
    count++;
  }
	DebugSerial.print("\r\n");
  return result;
}

float getHCHO(void)
{
  unsigned char requestComm[7]={0x42,0x4d,0x01,0x00,0x00,0x00,0x90};
  float highBits,lowBits;
  DSSerial.write(requestComm,7);
  delay(2);
  
  int count = 0;
  float value;
  unsigned char readBuff;
  
  while (DSSerial.available()) {
    readBuff = DSSerial.read();
	//DebugSerial.print(readBuff);
    if((count==0 && readBuff!=0x42) || (count==1 && readBuff!=0x4d)){
       return 11111;
       break;
    }
    if(count > 9){
       break;
    }
    else if(count == 6){
      highBits = readBuff;
      //Serial.print("high number=");
      //Serial.println(highBits);
    }
    else if(count == 7){
      lowBits = readBuff;
      //Serial.print("low number=");
      //Serial.println(lowBits);
    }
    count++;   
  }
//DebugSerial.print("\r\n");
  value = lowBits/100;
  while(DSSerial.available()) 
	  DSSerial.read();
  
  return value;
}

