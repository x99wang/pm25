#include <ArduinoJson.h>
#include <SoftwareSerial.h>  
#include <TimerOne.h>
#define DebugSerial Serial
#define GPRS900ASerail Serial1//GPRS模块
#define PMS5003Serial Serial2

#define Success 1U
#define Failure 0U

struct jsonBag
{
  char p[200];
} json;

void setup(){
  //GPRS900ASerail.begin(9600);
  DebugSerial.begin(9600);

}


void loop(){
  //硬件初始化
  //struct jsonBag json;
  
  DebugSerial.println(getJson());
  delay(1000);
}
char* getJson()
{
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root["id"] = "2017";
  root["pm25"] = 1351824120;
  root["pm10"] = 10.5;

  char str[200];
  int num = root.printTo(str, sizeof(str));
  return str;
  //strcpy(json.p , str);
}