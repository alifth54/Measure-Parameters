#include <Adafruit_INA219.h>
#include <Adafruit_BMP085.h>
#include <RTClib.h>
#include <Time.h>
#include <Wire.h>
#include <DHT.h>
#include <SD.h>
#include <SPI.h>
//---------------------------------------
  void measureLongTerm();
  void longTerm();
  void saveData();
//---------------------------
const int dhtPin = 8;
int Tmp;
unsigned long Now;
float Temperature,current,HUM,motion,pressure,voltage;
float time_now,pDue= 30,lastTime = 0,Time;     //by default priode duration is 30 minute
String fileName,Date,nowTime;
//-----------------------------------
Adafruit_BMP085 bmp;
DHT dht(dhtPin,DHT11,15);
Adafruit_INA219 ina219;
File myFile;
RTC_DS3231 rtc;
DateTime NOW = rtc.now();
//-----------------------------------
void setup() {
  Serial.begin(9600);
  SD.begin(10);
  rtc.begin();
  
  Serial.println("please set duration of a period in minut");
  while(!Serial.available()){
    delay(1);
    }
  pDue = Serial.parseFloat();
  Serial.print("duration of a priode is:\t");
  Serial.println(pDue);
  Date = String(NOW.year());
  Date += "/";
  Date += String(NOW.month());
  Date += "/";
  Date += String(NOW.day());
  nowTime = String(NOW.hour());
  nowTime += ":"; 
  nowTime += String(NOW.minute());
  nowTime += ":";
  nowTime += String(NOW.second());
  fileName = Date;
  Now = NOW.second();
}

void loop() {  

  if((Now- NOW.second())/60 >= pDue){
    measureLongTerm();
    saveData();
    Serial.println(Date);
    Serial.println(nowTime);
    Now = NOW.second();
  }
}

void measureLongTerm(){
//*******************************************
//pressure
  pressure = bmp.readPressure();
  Serial.print("perussure is:\t");
  Serial.print(pressure);
  Serial.println("\tpa");
//*******************************************
//humidity
  HUM = dht.readHumidity();
  Serial.print("humidity is:\t");
  Serial.print(HUM);
  Serial.println(" %");
//**************************************
// Temperature
  Temperature = dht.readTemperature();
  Serial.print("Temperature is:\t");
  Serial.print(Temperature);
  Serial.println(" *C");
//******************************************* 
//current 
  current = ina219.getCurrent_mA();
  Serial.print("current is:\t");
  Serial.print(current);
  Serial.println("\tmA");
//*******************************************
//motion
  Serial.print("motion:\t");
  motion = 100.0 - (analogRead(A0)/1023.0)*100.0;
  Serial.print(motion);
  Serial.println("\t%");
//*******************************************
}
void saveData(){
    Date = String(NOW.year());
    Date += "/";
    Date += String(NOW.month());
    Date += "/";
    Date += String(NOW.day());
    nowTime = String(NOW.hour());
    nowTime += ":"; 
    nowTime += String(NOW.minute());
    nowTime += ":";
    nowTime += String(NOW.second());
    fileName = Date;
    if(Time >= 24){
          fileName = Date + ".txt"; //new file name
          myFile.close();
          delay(20);
          Serial.print("Creating ");
          Serial.print(fileName);
          Serial.println(" ...");
          myFile = SD.open(fileName,FILE_WRITE);
          Time = 0;     
        }
    
    //------------------------------------------------
    //save to SD
      if (myFile) {
        Serial.println("Saving data...");
        myFile = SD.open(fileName,FILE_WRITE);
        myFile.println("---------------------------------");
        myFile.print(" Date:\t");
        myFile.println(Date);
        myFile.print(" Time:\t");
        myFile.println(nowTime);
        myFile.print("pressure is:\t");
        myFile.print(pressure);
        myFile.println("\tpa");
        myFile.print("humidity of area:\t");
        myFile.print(HUM);
        myFile.println(" %");
        myFile.print("current:\t");
        myFile.print(current);
        myFile.println(" mA");
        if(motion != 0){
          myFile.print("panel has vibration( ");
          myFile.print(motion);
          myFile.println(" % )");
          }
        myFile.flush();
        myFile.close();
        Serial.println("Done");
      } else {
        // if the file didn't open, print an error
        Serial.print("error opening");
        Serial.print(fileName);
        Serial.println("...!!!");
      }
  }
