/*
* Name: clock and temp project
* Author: Victor Huke
* Date: 2024-10-10
* Description: This project uses a ds3231 to measure time and displays the time to an 1306 oled display,
* Further, it measures temprature with a analog temprature module and displays a mapped value to a 9g-servo-motor
*/

// Include Libraries
#include <RTClib.h>
#include <Wire.h>
#include "U8glib.h"
#include <Servo.h>

// Init constants
const int servoPin = 9;
const int tempPin = A0;
const int button = 7;

// construct objects
RTC_DS3231 rtc;
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NO_ACK);
Servo myServo;

void setup() {
  // init communication
  Serial.begin(9600);
  Wire.begin();
  rtc.begin();
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  
  // Init Hardware
  u8g.setFont(u8g_font_fub17);  
  myServo.attach(servoPin);
  pinMode(tempPin, INPUT);  
  pinMode(button, INPUT_PULLUP);

}


void loop() {
  Serial.println("The time is: " + getTime());
  oledWrite(getTime());
  servoWrite(getTemp());
  Serial.println(getTemp());
  Serial.println(digitalRead(button));
  delay(1000);
}


/*
*This function reads time from an ds3231 module and package the time as a String
*Parameters: Void
*Returns: time in hh:mm:ss as String
*/
String getTime() {
  DateTime now = rtc.now();
  String currenttime = (String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second()));
  return currenttime;
}

/*
* This function reads an analog pin connected to an analog temprature sensor and calculates the corresponding temp
*Parameters: Void
*Returns: temprature as float
*/
float getTemp() {
  int Vo;
  float R1 = 10000; // value of R1 on board
  float logR2, R2, T;
  float c1 = 0.001129148, c2 = 0.000234125, c3 = 0.0000000876741; //steinhart-hart coeficients for thermistor
  Vo = analogRead(tempPin);
  R2 = R1 * (1023.0 / (float)Vo - 1.0); //calculate resistance on thermistor
  logR2 = log(R2);
  T = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2)); // temperature in Kelvin
  float temp = T - 273.15; //convert Kelvin to Celcius
  return temp;
}

/*
* This function takes a string and draws it to an oled display
*Parameters: - text: String to write to display
*Returns: void
*/
void oledWrite(String text) {
  u8g.firstPage();

  do {

    u8g.drawStr(23, 30, text.c_str());
  
  } while (u8g.nextPage());
}

/*
* takes a temprature value and maps it to corresppnding degree on a servo
*Parameters: - value: temprature
*Returns: void
*/
void servoWrite(float value) {
  value = map(value, 10, 30, 0, 180);
  myServo.write(value);
}