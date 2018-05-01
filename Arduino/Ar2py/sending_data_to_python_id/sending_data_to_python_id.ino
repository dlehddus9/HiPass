#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>
#include <SPI.h>
#include "Timer.h"



int IR_sensor = A0;
int value;

char msg[10];
RF24 radio(7, 8);
const uint64_t pipe = 0x0a0c0a0c0aLL;
int lastmsg = 1;
String theMessage = "";

void setup(void)
{
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(1, pipe);
  radio.startListening();
  pinMode(IR_sensor, INPUT);
}
unsigned long prev_time = 0;
void loop() {

  long IR_value = analogRead(IR_sensor);
  long IR_range = gp2y0a21yk(IR_value);

  //Serial.println(IR_value);
  //Serial.print(IR_range);
  //Serial.println("cm");
  //Serial.println();

  //사람이 지나가면 value=1
  if (IR_range < 50)
  {
    value = 1;
    Serial.println(value);
  }

  else {
    value = 0;
    Serial.println(value);
  }
  delay(1000);

  //사람이 지나가면 이함수호출.
  if (value == 1)
  {
    unsigned long prev_time = millis();
    //while문 들어가기전에 시간측정 1초간 이함수를 실행시키기 위해서
    while (true)
    {
      unsigned long current_time = millis(); 
      //while문 들어온뒤에 시간측정 한뒤에 while문을 1초만 실행시키게하기위해
      //Serial.println(prev_time);
      //Serial.println(current_time-prev_time);
      if (radio.available()) {
        bool done = false;
        done = radio.read(msg, 1);
        char theChar = msg[0];
        if (msg[0] != 2) {
          theMessage.concat(theChar);
        }
        else {
          Serial.println(theMessage);
          theMessage = "";
        }
      }
      if (current_time - prev_time > 1000) {
        //loop에 들어온지 1초가넘으면 루프탈출.
        prev_time = current_time;
        break;
      }
    }

  }

}



long gp2y0a21yk(long IR_value)
{
  if (IR_value < 10)
  {
    IR_value = 10;
  }

  return ((67870.0 / (IR_value - 3.0)) - 40.0) / 10;
}



