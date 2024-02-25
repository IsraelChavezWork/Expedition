/*
 * 
 * led            OUTPUT
 * buzzer         OUTPUT
 * oled           OUTPUT
 * 
 * button         INPUT
 * light sensor   INPUT
 * air pressure   INPUT
 * temp and humi  INPUT
 * color          INPUT
 * air quality    IPNUT
 * 
 * 
 * 
 * libraries to look for:
 *    light sensor
 *    air pressure
 *    temp and humi
 *    color
 *    air qulaity
 *    oled 
 */

#include "DHT.h"
#include <Arduino.h>
#include <String.h>
#include <U8x8lib.h>
#include "Adafruit_CCS811.h"

//plotdelay is the time (ms) for waiting periods in data ploting and collection
#define PLOTDELAY 100
 
#define DHTPIN 3
#define DHTTYPE DHT11

#define pinLight A6
#define pinSound A2
//#define pinAirQuality write_the_pin
//#define pinColor write_the_pin
#define pinPot A0


//#define pinButton 6
#define pinLED 4
//#define pinBuzzer 5

//How can we define the I2C for oled and air presuure?

Adafruit_CCS811 ccs;

DHT dht(DHTPIN, DHTTYPE);
U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);
//U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(/* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);


void clearS(){
  for(int i = 0; i< 8; i++){
    u8x8.setCursor(0, i);
    u8x8.print("                           ");
    }
}



int getLight_pct(){
    int light = analogRead(pinLight);
    int light_pct = map(light, 0, 750, 0, 100);
    return light_pct;
  }


int getSound_pct(){
  long sound = 0;

  for(int i = 0; i<32; i++){
    sound += analogRead(pinSound);
    delay(10);
  }

  sound >>= 5;
  //Serial.print(sound);
  int sound_pct = map(sound, 0, 1000, 0, 100);
  return sound_pct;
  }

float getTemp_f(){
  float temp = dht.readTemperature();
  return ((temp * (9/5)) + 32);
  }

  
//this function will gather the sensors data and send it to serial, that way it can be plotted
void getAndPlotData(){ 
  float humi = dht.readHumidity();
  int sound_pct = getSound_pct();
  int light_pct = getLight_pct();
  float celciusToF = getTemp_f();
  
  delay(50);

   Serial.print(sound_pct);    
   Serial.print(",");
   Serial.print(light_pct);
   Serial.print(",");
   Serial.print(celciusToF);    
   Serial.print(",");
   Serial.println(humi);
  }




void setup() {
  Serial.begin(9600);

  u8x8.begin();
  u8x8.setFlipMode(1);
  u8x8.setFont(u8x8_font_chroma48medium8_r);

  ccs.begin();
  
  //title of graph
  Serial.println("Sound_%, Light_%, Temp_F, Humidity_%"); // just write down the order of the sensors seperated by a ", "

  pinMode(pinSound, INPUT);
  pinMode(pinLight, INPUT);
  pinMode(pinPot, INPUT);
  
  //pinMode(pinButton, INPUT_PULLUP);

  //pinMode(pinBuzzer, OUTPUT);
  pinMode(pinLED, OUTPUT);
  
}

void loop() {
  //tone(buzzerPin, 1000); // this is for the buzz sound (buzzer)

  int sensorValue = analogRead(A0);
  
  if (sensorValue >= 0 && sensorValue < 150){
    digitalWrite(pinLED, HIGH);
    clearS();
    getAndPlotData();
      u8x8.setCursor(0, 0);
      u8x8.print("Plotting: ON");

      u8x8.setCursor(0, 7);
      u8x8.print("1 out of 5");

    }
  else if (sensorValue >= 150 && sensorValue < 300){
    //do a clear screen
    //keep plotting
    digitalWrite(pinLED, HIGH);
    clearS();
    getAndPlotData();
    
    u8x8.setCursor(0, 0);
    u8x8.print("Sound%: "); u8x8.print(getSound_pct());

    u8x8.setCursor(0, 2);
    u8x8.print("Light%: "); u8x8.print(getLight_pct());

    u8x8.setCursor(0, 4);
    u8x8.print("TempF%: "); u8x8.print(getTemp_f());

    u8x8.setCursor(0, 6);
    u8x8.print("Humidity%: "); u8x8.print(dht.readHumidity());

    u8x8.setCursor(0, 7);
      u8x8.print("2 out of 5");
    }
  else if (sensorValue >= 300 && sensorValue < 450){
    //tvoc, ppm, uv
    //do a clear screen
    //keep plotting
    digitalWrite(pinLED, HIGH);
    clearS();
    getAndPlotData();
    u8x8.setCursor(0, 0);
    u8x8.print("TVOC: "); u8x8.print("pend.");

    u8x8.setCursor(0, 2);
    u8x8.print("PPM%: "); u8x8.print("pend.");

    u8x8.setCursor(0, 4);
    u8x8.print("UV index: "); u8x8.print("pend.");

    u8x8.setCursor(0, 7);
      u8x8.print("3 out of 5");

      
    }
  else if (sensorValue >= 600 && sensorValue < 750){
    //r, g, b, c
    //do a clear screen
    //keep plotting
    digitalWrite(pinLED, HIGH);
    clearS();
    getAndPlotData();
    u8x8.setCursor(0, 0);
    u8x8.print("Red: "); u8x8.print("pend.");

    u8x8.setCursor(0, 2);
    u8x8.print("Green: "); u8x8.print("pend.");

    u8x8.setCursor(0, 4);
    u8x8.print("Blue: "); u8x8.print("pend.");

    u8x8.setCursor(0, 6);
    u8x8.print("Clear: "); u8x8.print("pend.");

    u8x8.setCursor(0, 7);
      u8x8.print("4 out of 5");
    }
  else if (sensorValue >= 750){

    digitalWrite(pinLED, LOW);
    clearS();
    u8x8.setCursor(0, 0);
    u8x8.print("Plotting: OFF");
    u8x8.setCursor(0, 7);
     u8x8.print("5 out of 5");
    //not plotting text
    }

  delay(600); // Delay for 


}
