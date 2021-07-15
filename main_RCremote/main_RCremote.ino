#include <SPI.h>            //Serial Peripheral Interface library
#include <nRF24L01.h>       //https://github.com/nRF24/RF24
#include <RF24.h>           //to access the RF24 class


#define Xpin A0
#define Ypin A1
#define Spin 2

#define CE_pin 7    //define Chip Enable (CE) and Chip Select Not (CSN) pins here
#define CSN_pin 8   

RF24 radio(CE_pin, CSN_pin);  //RF24 object 
const byte address[6] = "00000";
int Xval,Yval,Sval;
int vals[3];
void setup() 
{
  pinMode(Xpin, INPUT);
  pinMode(Ypin, INPUT);
  pinMode(Spin, INPUT);
  digitalWrite(Spin, HIGH);

  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
}

void loop() 
{
  Xval = analogRead(Xpin);
  Yval = analogRead(Ypin);
  Sval = digitalRead(Spin);

  vals[0] = Xval;
  vals[1] = Yval;
  vals[2] = Sval;
  radio.write(&vals, sizeof(vals));    //Transmit the array containing joystick input information to receiver
  delay(50);
}
