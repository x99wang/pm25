#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "printf.h"

RF24 radio (9, 10); //yours
unsigned int times;


struct CODataStruct{
  float value;
  float time;
}COValue;

unsigned char ADDRESS0[5]  = 
{
  0xb4,0x43,0x88,0x99,0x45
}; // Define a static TX address
//just change b1 to b2 or b3 to send to other pip on resciever

void setup()
{
  Serial.begin(9600);
  radio.begin();
  printf_begin();
  radio.setRetries(15,15);
  radio.enableDynamicPayloads();
  radio.setDataRate(RF24_250KBPS);
  radio.openWritingPipe(ADDRESS0);
  radio.openReadingPipe(0,ADDRESS0);
  radio.stopListening();
  radio.printDetails();
}

void loop()
{
  COValue.value = analogRead(3) * 0.01444;
  COValue.time = times++;
  bool ok=radio.write(&COValue, sizeof(COValue));

  if(ok)
  {
    Serial.print("Pipe 4-");
    Serial.print("temp:");
    Serial.print(COValue.value);
    Serial.print(",times:");
    Serial.println(COValue.time);

  }else{
    Serial.println("it failed to send");
  }

  delay(1000);
   
}

