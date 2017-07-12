#include <ArduinoJson.h>
#include <SoftwareSerial.h>  
#include <TimerOne.h>
#define DebugSerial Serial
#define GPRS900ASerail Serial1//GPRS模块
#define PMS5003Serial Serial2

#define Success 1U
#define Failure 0U

struct SensorValuesBar {
  float temp;
  float humid;
  float pm25;
  float pm10;
  float ch2o;
};

const unsigned int sim900aRxBufferLength = 800;
char sim900aRxBuffer[sim900aRxBufferLength];

struct SensorValuesBar PMS5003Value;
// 传感器数值定义
long pmcf10, pmcf25, pmcf100, pmat10, pmat25, pmat100;
long sch2o,stemp,shumid;

unsigned int ii = 0;
unsigned long  timerCount = 0;    //定时器计数器

void setup(){
  //GPRS900ASerail.begin(9600);
  PMS5003Serial.begin(9600);
  DebugSerial.begin(9600);
  /* * * * * * * * * *
   * 开始计时      *
   * 每隔一秒      *
   * 计时器加 1      *
   * 等同于一个秒表  *
   * * * * * * * * * */
   
  //Timer1.initialize(1000);
  //Timer1.attachInterrupt(Timer1_handler);
  
  delay(2000);

}


void loop(){
  //硬件初始化

  PMS5003Value = getPMS5003();


  DebugSerial.print("pm10:");
  DebugSerial.print(PMS5003Value.pm10);
  DebugSerial.print("\tpm25:");
  DebugSerial.println(PMS5003Value.pm25);
  DebugSerial.print("\tch2o:");
  DebugSerial.println(PMS5003Value.ch2o);
  DebugSerial.print("\ttemp:");
  DebugSerial.println(PMS5003Value.temp);
  DebugSerial.print("\thumid:");
  DebugSerial.println(PMS5003Value.humid);
  delay(1000);
}

struct SensorValuesBar getPMS5003(){
  struct SensorValuesBar result;
  int count = 0;
  unsigned char readBuff;
  unsigned char highBits;
  while (PMS5003Serial.available()) {
    readBuff = PMS5003Serial.read();
	
    if((count==0 && readBuff!=0x42) || (count==1 && readBuff!=0x4d)){
       break;
    }
    if(count > 36){
       break;  // have read all data, read ends
    }
    else if(count == 4 || count == 6 || count == 8 || count == 10 || count == 12 || count == 14) highBits = readBuff;
    else if(count == 5){
      pmcf10 = 256*highBits + readBuff; // pm1.0
    }
    else if(count == 7){
      pmcf25 = 256*highBits + readBuff;
      result.pm25 = pmcf25; // pm2.5
      //DebugSerial.print("CF=1, PM2.5=");DebugSerial.print(pmcf25);DebugSerial.println(" ug/m3");
    }
    else if(count == 9){
      pmcf100 = 256*highBits + readBuff;
      result.pm10 = pmcf100;  // pm10.0
      //DebugSerial.print("CF=1, PM10=");DebugSerial.print(pmcf100);DebugSerial.println(" ug/m3");
    }
    else if(count == 11){
      pmat10 = 256*highBits + readBuff;
      //DebugSerial.print("atmosphere, PM1.0=");DebugSerial.print(pmat10);DebugSerial.println(" ug/m3");
    }
    else if(count == 13){
      pmat25 = 256*highBits + readBuff;
      //DebugSerial.print("atmosphere, PM2.5=");DebugSerial.print(pmat25);DebugSerial.println(" ug/m3");
    }
    else if(count == 15){
      pmat100 = 256*highBits + readBuff;
      //DebugSerial.print("atmosphere, PM10=");DebugSerial.print(pmat100);DebugSerial.println(" ug/m3");
    }
	else if(count == 29){
      sch2o = 256*highBits + readBuff;
	  result.ch2o = sch2o/1000.0;
      //DebugSerial.print("atmosphere, CH2O=");DebugSerial.print(ch2o);DebugSerial.println(" mg/m3");
    }
	else if(count == 31){
      stemp = 256*highBits + readBuff;
	  result.temp = stemp/10.0;
      //DebugSerial.print("atmosphere, TEMP=");DebugSerial.print(temp);DebugSerial.println(" `C");
    }
	else if(count == 33){
      shumid = 256*highBits + readBuff;
	  result.humid = shumid/10.0;
      //DebugSerial.print("atmosphere, HUMID=");DebugSerial.print(humid);DebugSerial.println(" %");
    }
    count++; 
  }
  DebugSerial.println();
  while(PMS5003Serial.available()) 
	  PMS5003Serial.read();
  return result;
}
