
/*
* Name: övningsprojekt - Final Version (med LDR)
* Author: Kevin
* Date: 2025-11-10
* Description: Displays time, temperature (from DS3231) and light level (from LDR)
*              on an SSD1306 OLED display, and moves a servo based on temperature.
*/

// Bibliotek
#include <Wire.h>
#include <RTClib.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Servo.h>

// OLED settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// RTC object
RTC_DS3231 rtc;

// Servo setup
Servo myServo;
#define SERVO_PIN 9

// LDR setup
#define LDR_PIN A0   

// Optional: set RTC once at compile time 
#define SET_TIME_ONCE true

void setup() {
  Serial.begin(9600);
  Wire.begin();

  // Init RTC
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (SET_TIME_ONCE) {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  // Init OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    while (1);
  }

// Inställningar för LED
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("Init...");
  display.display();
  delay(1000);

  // Attach servo
  myServo.attach(SERVO_PIN);

  Serial.println("System startat!");
}

void loop() {
  // Read time, temperature and LDR
  String timeString = getTime();
  float temp = getTemp();
  int ldrValue = analogRead(LDR_PIN);         
  int lightPercent = map(ldrValue, 0, 1023, 100, 0); 

  // Serial output 
  Serial.print(timeString);
  Serial.print("  Temp: ");
  Serial.print(temp);
  Serial.print(" C  Ljus: ");
  Serial.print(lightPercent);
  Serial.println("%");

  // OLED output
  oledWrite(timeString, temp, lightPercent);

  // Servo styrning baserat på temperatur 
  int servoAngle = map(temp * 10, 0, 500, 0, 180);
  servoAngle = constrain(servoAngle, 0, 180);
  myServo.write(servoAngle);

  delay(500);
}

// Läs tid från DS3231 
String getTime() {
  DateTime now = rtc.now();
  char buffer[9];
  snprintf(buffer, sizeof(buffer), "%02d:%02d:%02d", now.hour(), now.minute(), now.second());
  return String(buffer);
}

// Läs temperatur från DS3231
float getTemp() {
  return rtc.getTemperature();
}

// Skriv data till OLED
void oledWrite(String timeText, float temp, int lightPercent) {
  display.clearDisplay();

  // Tid
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 5);
  display.print(timeText);

  // Temperatur
  display.setTextSize(1);
  display.setCursor(10, 35);
  display.print("Temp: ");
  display.print(temp, 1);
  display.print((char)247);
  display.print("C");

  // Ljusnivå
  display.setCursor(10, 50);
  display.print("Ljus: ");
  display.print(lightPercent);
  display.print("%");

  display.display();
}
