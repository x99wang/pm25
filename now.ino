#include <ArduinoJson.h>
#include <SoftwareSerial.h>  
#include <TimerOne.h>
#include <Wire.h>
#include <SparkFunHTU21D.h>

#define DebugSerial Serial
#define GPRS900ASerail Serial1//GPRS模块
#define PMS5003Serial Serial2//pm传感器

#define Success 1U
#define Failure 0U

struct SensorValuesBar {
  float temp;
  float humid;
  float pm25;
  float pm10;
  float ch2o;
};

struct SensorValuesBar PMS5003Value;
struct SensorValuesBar DSValue;
// 传感器数值定义
long pmcf10, pmcf25, pmcf100, pmat10, pmat25, pmat100;
long sch2o,stemp,shumid;

//Create an instance of the object
HTU21D HTSensor;

const unsigned int sim900aRxBufferLength = 800;
char sim900aRxBuffer[sim900aRxBufferLength];

unsigned int ii = 0;
unsigned long  timerCount = 0;    //定时器计数器

void setup(){
  GPRS900ASerail.begin(9600);
  DebugSerial.begin(9600);
  PMS5003Serial.begin(9600);
  
  /* * * * * * * * * *
   * 开始计时      *
   * 每隔一秒      *
   * 计时器加 1      *
   * 等同于一个秒表  *
   * * * * * * * * * */
   
  Timer1.initialize(1000);
  Timer1.attachInterrupt(Timer1_handler);
  
  delay(2000);
  //GPRS初始化
  initSim900a();

}


void loop(){
  //硬件初始化
  delay(5000);
  PMS5003Value = getPMS5003();
  delay(1000);
  openSapbr();
  doHttp();
  closeSapbr();

  delay(5000);
}

//GPRS初始化
void initSim900a(){
  
  if (sendCommand("AT\r\n", "OK", 10000, 10) != Success)  
    SIM900A_ERROR(0);
  clrSim900aRxBuffer();
  
  if (sendCommand("AT+CPIN?\r\n", "OK", 10000, 10) != Success)  
    SIM900A_ERROR(1);
  clrSim900aRxBuffer();

  if (sendCommand("AT+CFUN=1\r\n", "OK", 3000, 10) != Success)
    SIM900A_ERROR(2);
  clrSim900aRxBuffer();
}

//制造重启命令
void(* resetFunc)(void) = 0;

//出现错误情况
void SIM900A_ERROR(int num){
  //如果出现错误
  //重启
  while (1)
  {
    /* 
      digitalWrite(LED, HIGH);
      delay(300);
      digitalWrite(LED, LOW);
      delay(300); 
    */
    
    DebugSerial.print("SIM900A ERROR Num:");
    DebugSerial.println(num);
    
    delay(3000);
    if (sendCommand("AT\r\n", "OK", 100, 10) == Success)
    {
      //ReBoot
      resetFunc();
    }
  }
}

//指令
unsigned int sendCommand(char *Command, char *Response, unsigned long Timeout, int Retry){
  //发送数据前先进行缓存清空
  clrSim900aRxBuffer();

  for (int n = 0; n < Retry; n++)
  {
    DebugSerial.print("~~Sent Command: ");
    DebugSerial.println(Command);
    //发送AT指令
    GPRS900ASerail.write(Command);
    //计时器归零
    timerCount = 0;
    unsigned long cnt= 1;
    while (timerCount < Timeout)
    {
      //如果没有超时
      //读数据
      sim900aReadBuffer();
      if (strstr(sim900aRxBuffer, Response) != NULL)
      { 
        //如果存在数据
        //输出接收到的信息
        DebugSerial.println("\r\n============");
        DebugSerial.println("*Received Command Buffer: [Success]");
        DebugSerial.println(sim900aRxBuffer);
        DebugSerial.println("============");
        return Success;
      }
      if(timerCount%1000==0 && timerCount!=cnt)
      {
        cnt = timerCount;
        DebugSerial.print(cnt/1000);
        DebugSerial.print("s.  ");
        //GPRS900ASerail.print("0\r\n");
      }
    }
    DebugSerial.print(n);
    DebugSerial.println("time");
    //计时器归零
    timerCount = 0;
  }
  DebugSerial.println("============");
  DebugSerial.println("*Received Command Buffer: [Time out Failure]");
  DebugSerial.println(sim900aRxBuffer);
  DebugSerial.println("============");
  return Failure;
}

//HttpPost
void doHttp(){
  
  /*
   * AT+HTTPINIT
   * AT+HTTPPARA="CID","1"
   * AT+HTTPPARA="URL","http://139.199.153.220/pmJson/update"
   * AT+HTTPPARA="CONTENT","application/json"
   * AT+HTTPDATA=200,5000
   * 
   * AT+HTTPACTION=1
   * AT+HTTPREAD=0,799
   * AT+HTTPTERM
   */

  //初始化HTTP服务
  if (sendCommand("AT+HTTPTERM\r\n", "ERROR", 2000, 3) == Success)
    ;
  clrSim900aRxBuffer();
  
  if (sendCommand("AT+HTTPINIT\r\n", "OK", 10000, 5) != Success)
    SIM900A_ERROR(5);
  clrSim900aRxBuffer();
  //设置承载上下文标识，每次登陆网页前都需要进行一次，不然网页缓存会造车频繁更新的数据无法通过url传值到web后台  
  if (sendCommand("AT+HTTPPARA=\"CID\",\"1\"\r\n", "OK", 10000, 3) != Success)
    SIM900A_ERROR(6);
  clrSim900aRxBuffer();

  // 设置要访问的url，假设需要传tem字段，值为12，通过如下URL传值可以实现，当然web端要与之对应 
  // AT+HTTPPARA="URL","http://www.13903936592.cn/pmJson/update"
  // String sendDaata = "AT+HTTPPARA=\"URL\",\"http://120.9.9.129/xxx/update?tem=12\"";  
  // char urll[]="http://www.13903936592.cn/pmJson/update";
  

  if (sendCommand("AT+HTTPPARA=\"URL\",\"http://139.199.153.220/AirDevice/putAir\"\r\n", "OK", 10000, 3) != Success)
    SIM900A_ERROR(7);
  clrSim900aRxBuffer();

  
  if (sendCommand("AT+HTTPPARA=\"CONTENT\",\"application/json\"\r\n", "OK", 10000, 3) != Success)
    SIM900A_ERROR(8);
  clrSim900aRxBuffer();

  
  if (sendCommand("AT+HTTPDATA=200,5000\r\n", "DOWNLOAD", 5000, 3) != Success)
    SIM900A_ERROR(9);
  clrSim900aRxBuffer();
  
  //GPRS900ASerail.write("{\"id\":10004,\"ch2o\":12.6,\"co\":2.6,\"pm25\":26.6,\"pm10\":13.6,\"temp\":27.6,\"humid\":80.6,\"lng\":118.046271801,\"lat\":24.3706416091}\r\n");
 
  if (sendCommand(getJson(), "OK", 20000, 1) != Success)
    SIM900A_ERROR(10);
  clrSim900aRxBuffer();

  delay(1000);
  //激活HTTP请求，后面参数表示Http请求方式，参数0表示Get方式，1表示Post方式，2表示head方式
  if (sendCommand("AT+HTTPACTION=1\r\n", "+HTTPACTION", 30000, 3) != Success)
    SIM900A_ERROR(11);
  clrSim900aRxBuffer();
  
  delay(5000);
  
  if (sendCommand("AT+HTTPREAD=0,799\r\n", "+HTTPREAD", 30000, 3) != Success)
    SIM900A_ERROR(11);
  clrSim900aRxBuffer();
  
  // 读取返回信息读取数据
//  if (sendCommand("AT+HTTPREAD\r\n", "+HTTPREAD:", 10000, 3) != Success)
//    SIM900A_ERROR(12);
//  clrSim900aRxBuffer();
  
  //GPRS900ASerail.write("AT+HTTPREAD=1,799\r\n");
  //sim900aReadBuffer();
  //GPRS900ASerail.println(sim900aRxBuffer);
  //clrSim900aRxBuffer();
  
  //终止HTTP服务
  if (sendCommand("AT+HTTPTERM\r\n", "OK", 5000, 3) != Success)
    SIM900A_ERROR(13);
  clrSim900aRxBuffer();
}

//SAPBR
void openSapbr(){
  /* 
    Serial.println("AT+SAPBR=?");
    delay(5000);
    printLine();
    Serial.println("AT+SAPBR=3,1,\"APN\",\"cmnet\"");
    delay(5000);
    printLine();
    Serial.println("AT+SAPBR=4,1");
    delay(5000);
    printLine();
    //查看网络连接状态以及ip地址
    Serial.println("AT+SAPBR=2,1");
    delay(5000);
    printLine(); 
  */
  //打开承载，打开网络通道
  while (sendCommand("AT+SAPBR=2,1\r\n", "0.0.0.0", 1000, 1) == Success)
  {
    if(sendCommand("AT+SAPBR=1,1\r\n", "OK", 2000, 1) == Success)
      break;
  }
  
  /*
  while (timerCount/1000<=10)
  {
    //
    if(timerCount%1000==0)
    {
      GPRS900ASerail.write("AT+SAPBR=1,1\r\n");
      sim900aReadBuffer();
      if (strstr(sim900aRxBuffer, "OK") != NULL)
        break;
      clrSim900aRxBuffer();
    }
  }
  if(timerCount/1000>10)
    SIM900A_ERROR(100);
  */
  
  clrSim900aRxBuffer();
  
  //查看网络连接状态以及ip地址
  /* 
  if (sendCommand("AT+SAPBR=2,1\r\n", "OK", 3000, 5) != Success)
    SIM900A_ERROR(200);
  clrSim900aRxBuffer(); 
  */
  
  timerCount=0;
}

//Close Sapbr
void closeSapbr(){
  if (sendCommand("AT+SAPBR=0,1\r\n", "OK", 5000, 5) != Success)
    SIM900A_ERROR(300);
  clrSim900aRxBuffer();
}

//计时器
void Timer1_handler(void){
  timerCount++;
}

//读数据
void sim900aReadBuffer(void){
  //类似于输入流
  while (GPRS900ASerail.available())
  { 
    //读到sim900aRxBuffer中
    sim900aRxBuffer[ii++] = GPRS900ASerail.read();
    //如果读到 buffer 结尾 则清空buffer
    if (ii == sim900aRxBufferLength)
      clrSim900aRxBuffer();
  }
}

//清空缓存
void clrSim900aRxBuffer(void){
  memset(sim900aRxBuffer, 0, sim900aRxBufferLength);
  ii = 0;
}

//拿pm数据
struct SensorValuesBar getPMS5003()
{
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
    else if(count == 4 || count == 6 || count == 8 || count == 10 || count == 12 || count == 14|| count == 16|| count == 18|| count == 20|| count == 22|| count == 24|| count == 26|| count == 28|| count == 30|| count == 32) highBits = readBuff;
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
  while(PMS5003Serial.available()) 
	  PMS5003Serial.read();
  return result;
}

char* getJson()
{
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root["id"] = 10001;
  root["pm25"] = PMS5003Value.pm25;
  root["pm10"] = PMS5003Value.pm10;
  root["ch2o"] = PMS5003Value.ch2o;
  root["temp"] = PMS5003Value.temp;
  root["co"] = analogRead(0) * 0.01444;
  root["humid"] = PMS5003Value.humid;
  root["lng"] = 118.046271801;
  root["lat"] = 24.3706416091;
  

  char str[200];
  int num = root.printTo(str, sizeof(str));
  return str;
  //strcpy(json.p , str);
}