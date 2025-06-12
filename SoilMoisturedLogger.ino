/* File: SoilMoistureLogger.ino
   Soil Moisture Logger with DS3231 & SD Card
*/

#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <RTClib.h>

// ===== User Config =====
const int   SOIL_PIN       = A0;      // Analog sensor input
const int   BUTTON_PIN     = 2;       // Manual log trigger
const int   LED_PIN        = 8;       // Status LED (optional)
const int   SD_CS_PIN      = 10;      // SD card CS pin

const unsigned long LOG_INTERVAL = 600000UL; // 10 minutes in ms
const unsigned long DEBOUNCE_TIME = 50;      // Button debounce ms

// ===== Globals =====
RTC_DS3231 rtc;
unsigned long  lastLogTime   = 0;
unsigned long  lastButtonTime= 0;
bool           buttonState   = LOW;
bool           lastButton    = LOW;

// ===== Setup =====
void setup() {
  Serial.begin(9600);
  pinMode(SOIL_PIN,    INPUT);
  pinMode(BUTTON_PIN,  INPUT);
  pinMode(LED_PIN,     OUTPUT);

  // Initialize RTC
  if (!rtc.begin()) {
    Serial.println("RTC not found!");
    while (1);
  }
  // Uncomment to set RTC to compile time once:
  // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

  // Initialize SD card
  if (!SD.begin(SD_CS_PIN)) {
    Serial.println("SD init failed!");
    while (1);
  }

  // Create log file with header if not exists
  if (!SD.exists("soil_log.csv")) {
    File f = SD.open("soil_log.csv", FILE_WRITE);
    if (f) {
      f.println("timestamp,moisture");
      f.close();
    }
  }

  lastLogTime = millis();
  Serial.println("Logger ready");
}

// ===== Main Loop =====
void loop() {
  unsigned long now = millis();

  // Check periodic logging
  if (now - lastLogTime >= LOG_INTERVAL) {
    logSoil();
    lastLogTime = now;
  }

  // Check manual button (with debounce)
  bool reading = digitalRead(BUTTON_PIN);
  if (reading != lastButton && (now - lastButtonTime) > DEBOUNCE_TIME) {
    lastButtonTime = now;
    if (reading == HIGH) {
      logSoil();
    }
  }
  lastButton = reading;
}

// ===== Log Function =====
void logSoil() {
  // Read sensor
  int moisture = analogRead(SOIL_PIN);

  // Get timestamp
  DateTime t = rtc.now();
  char buf[20];
  snprintf(buf, sizeof(buf), "%04u-%02u-%02u %02u:%02u:%02u",
           t.year(), t.month(), t.day(),
           t.hour(), t.minute(), t.second());

  // Append to SD
  File f = SD.open("soil_log.csv", FILE_WRITE);
  if (f) {
    f.print(buf);
    f.print(',');
    f.println(moisture);
    f.close();
    Serial.print("Logged: ");
    Serial.print(buf);
    Serial.print(" -> ");
    Serial.println(moisture);
    blinkLED();
  } else {
    Serial.println("Error opening soil_log.csv");
  }
}

// ===== LED Feedback =====
void blinkLED() {
  digitalWrite(LED_PIN, HIGH);
  delay(100);
  digitalWrite(LED_PIN, LOW);
}
