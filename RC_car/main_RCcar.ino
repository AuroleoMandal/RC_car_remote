#include <Motor.h>
#include <SPI.h>            //Serial Peripheral Interface library
#include <nRF24L01.h>       //https://github.com/nRF24/RF24
#include <RF24.h> 
#include <Arduino.h>

#define mot00 2             //Pins connected to driver
#define mot01 3
#define mot10 4.
#define mot11 5
#define mot0e 9
#define mot1e 10

#define CE_pin 7    //define Chip Enable (CE) and Chip Select Not (CSN) pins here
#define CSN_pin 8  

Motor motor1(mot00, mot01, mot0e);  //Motor class object
Motor motor2(mot10, mot11, mot1e);

RF24 radio(CE_pin, CSN_pin);  //RF24 object 
const byte address[6] = "00000";
int vals[3];

int Xval;
int Yval;
int Sval;

bool reading=false;
int deadZone=100;
float Yspeed;
float l_dirScale, r_dirScale;
int l_Yadjspeed, r_Yadjspeed;

void setup() 
{
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
}

void loop() 
{
  if (radio.available()) 
  { 
    radio.read(&vals, sizeof(vals));
    Serial.print("received");
  }

  Xval = vals[0]-512;
  Yval = vals[1]-512;
  Sval = vals[2];
  
  if(Xval>0)
  {
    l_dirScale = 1 - (abs(Xval)/2.)/256.;
    r_dirScale = 1;
  }
  if(Xval<0)
  {
    l_dirScale = 1;
    r_dirScale = 1 - (abs(Xval)/2.)/256;
  }
  
  Yspeed = (abs(Yval)/2.)-1;

  l_Yadjspeed = Yspeed * l_dirScale;
  r_Yadjspeed = Yspeed * r_dirScale;
  
  if(sqrt(pow(Xval,2)+pow(Yval,2))<deadZone)
  {
    reading=false;
  }
  else
    reading=true;

  if(reading==true && Yval>0)
  {
    motor1.fwd(l_Yadjspeed);
    motor2.fwd(r_Yadjspeed);
    Serial.print("reversing");
  }
  else if(reading==true && Yval<0)
  {
    motor1.rev(l_Yadjspeed);
    motor2.rev(r_Yadjspeed);
    Serial.print("onwards!!");
  }
  else
  {
    motor1.off();
    motor2.off();
  }

  stats();
  delay(50);

}

void stats()
{
  Serial.print(Xval);
  Serial.print(" ");
  Serial.print(Yval);
  Serial.print(" ");
  Serial.print(Yspeed);
  Serial.print(" ");
  Serial.print(l_Yadjspeed);
  Serial.print(" ");
  Serial.println(r_Yadjspeed);
}
