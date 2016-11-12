#include <Wire.h>
#include "DHT.h"

#define DHTPIN 2     // what digital pin we're connected to
#define DHTTYPE DHT11   // DHT 11
#define SLAVE_ADDRESS 0x04

#pragma pack(push, 1)

DHT dht(DHTPIN, DHTTYPE);

  struct package {

    int32_t data1;
    int32_t data2;
    int32_t data3;

    char data4;
    char data5;
    char data6;
    char data7;
    char data8;
    char data9;
    char data10;

  };

  package dataOut;

  float t, h, f;

  bool state = false;
 
void setup() {
 pinMode(13, OUTPUT);
 
 // initialize i2c as slave
 Wire.begin(SLAVE_ADDRESS);
 
 // define callbacks for i2c communication
 Wire.onReceive(receiveData);
 Wire.onRequest(sendData);
 
  dht.begin();
    Serial.begin(9600);
  Serial.println("Slave 1 has booted!");
}
 
void loop() {
 delay(2000);
 //temp = GetTemp();

   t = dht.readTemperature();
   h = dht.readHumidity();
   f = dht.readTemperature(true);
}
 
// callback for received data
void receiveData(int byteCount){

  char r[20];

 int i = 0;
  
 while(Wire.available()) {
   r[i] = Wire.read();
   
   i++;
 }

  package dataIn;
  memcpy(&dataIn, r, sizeof(dataIn));

  handler(dataIn);
 
}
 
// callback for sending data
void sendData(){
 char b[sizeof(dataOut)];
 memcpy(b, &dataOut, sizeof(dataOut));
 Wire.write(b, sizeof(dataOut));
}
 
// Get the internal temperature of the arduino
double GetTemp(void)
{
 unsigned int wADC;
 double t;
 ADMUX = (_BV(REFS1) | _BV(REFS0) | _BV(MUX3));
 ADCSRA |= _BV(ADEN); // enable the ADC
 delay(20); // wait for voltages to become stable.
 ADCSRA |= _BV(ADSC); // Start the ADC
 while (bit_is_set(ADCSRA,ADSC));
 wADC = ADCW;
 t = (wADC - 324.31 ) / 1.22;
 return (t);
}

void handler(package& data) {

  clear();

  Serial.print("Function Code: ");
  Serial.println(data.data1);

  if(data.data1 == 1) {
    dataOut.data1 = h;
  }

  if(data.data1 == 2) {
    dataOut.data2 = t;
  }

  if(data.data1 == 3) {
    if(state == false) {
      dataOut.data3 = 1;
      digitalWrite(13, HIGH);
      state = true;
    } else {
      dataOut.data3 = 0;
      digitalWrite(13, LOW);
      state = false;
    }
  }

  if(data.data1 == 4) {
    dataOut.data1 = h;
    dataOut.data2 = t;
    dataOut.data3 = (int)GetTemp();
  }

  demonstrate();
}

void demonstrate() {

Serial.println(dataOut.data1);
Serial.println(dataOut.data2);
Serial.println(dataOut.data3);
Serial.println(dataOut.data4);
Serial.println(dataOut.data5);
Serial.println(dataOut.data6);
Serial.println(dataOut.data7);
Serial.println(dataOut.data8);
Serial.println(dataOut.data9);
Serial.println(dataOut.data10);

}

void clear() {

  dataOut = {};
  
}



