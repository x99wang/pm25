#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "printf.h"


RF24 radio (9, 10); //yours
unsigned int times;

struct hchoDataStruct
{
  float time;
  float value;
}hchoValue;

unsigned char ADDRESS0[5]  = 
{
  0xb2,0x43,0x88,0x99,0x45
}; // Define a static TX address
//just change b1 to b2 or b3 to send to other pip on resciever

void setup()
{
  radio.begin();
  Serial.begin(9600);
  printf_begin();
  radio.setRetries(15,15);
  radio.enableDynamicPayloads();
  radio.setDataRate(RF24_250KBPS);
  radio.openWritingPipe(ADDRESS0);
  radio.openReadingPipe(0,ADDRESS0);
  radio.stopListening();
}

void loop()
{
  hchoValue.value = getHCHO();
  hchoValue.time = times++;
  bool ok=radio.write(&hchoValue, sizeof(hchoValue));

  if(ok)
  {
    Serial.print("Pipe 2-");
    Serial.print("HCHO:");
    Serial.print(hchoValue.value);
    Serial.print(",times:");
    Serial.print(hchoValue.time);

  }else{
    Serial.println("it failed to send");
  }

  delay(1000);
   
}

float getHCHO(void)
{
  unsigned char requestComm[7]={0x42,0x4d,0x01,0x00,0x00,0x00,0x90};
  float highBits,lowBits;
  Serial.write(requestComm,7);
  delay(2);
  
  int count = 0;
  float value;
  unsigned char readBuff;
  
  while (Serial.available()) {
    readBuff = Serial.read();
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
  value = lowBits/100;
  //Serial.print("value=");
  //Serial.println(value);
  while(Serial.available()) Serial.read();
  //Serial.println();
  return value;
}


