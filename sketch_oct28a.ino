#define BLYNK_TEMPLATE_ID "TMPL3q5K_lGNn"
#define BLYNK_TEMPLATE_NAME "Smart Irrigation"
#define BLYNK_AUTH_TOKEN "LZtBNG3WGsM3o_p1e3y1gY6iadwoCLq7"
#include <Blynk.h>
#include <ArduinoWiFiServer.h>
#include <BearSSLHelpers.h>
#include <CertStoreBearSSL.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiAP.h>
#include <ESP8266WiFiGeneric.h>
#include <ESP8266WiFiGratuito us.h>
#include <DHT.h>
#include <BlynkSimpleEsp8266.h>

#define BLYNK_PRINT Serial                  
//#define BLYNK_DEBUG

//#define APP_DEBUG
DHT dht(5, DHT11); 
char ssid[] = "A14";
char pass[] = "mynetworkk";
int moisture_threshold = 35;
int sensorPin = A0;
const int pumpPin = 4;
float sensorReading;
float moistureLevel;
float temperature;
float humidity;
bool alert_sent = false;
BlynkTimer timer;

void myTimer() {
  Blynk.virtualWrite(V4, moistureLevel);
  Blynk.virtualWrite(V7, temperature);
  Blynk.virtualWrite(V8, humidity);
}
BLYNK_WRITE(V5) {
  moisture_threshold = param.asInt();
  Serial.print("Threshold changed");
}
void setup() {
  // put your setup code here, to run once:
  Blynk.config(BLYNK_AUTH_TOKEN);
  Blynk.connectWiFi(ssid, pass);
  Serial.begin(115200);
  delay(100);
  pinMode(pumpPin, OUTPUT);
  dht.begin();
  timer.setInterval(1000L, myTimer);
}

void loop() {
  // put your main code here, to run repeatedly:
  Blynk.run();
  timer.run();
  humidity = dht.readHumidity();/* Get humidity value */
  temperature = dht.readTemperature();/* Get temperature value */
  sensorReading = analogRead(sensorPin);
  moistureLevel = ( 100 - ( (sensorReading/1023.00) * 100 ) );
  if (moistureLevel < moisture_threshold && alert_sent == false) {
    Blynk.logEvent("low_moisture_level", "Moisture level: " + String(moistureLevel));
    Serial.print("Low moisture level, Turn on Pump");
    Serial.print(moistureLevel);
    Serial.print("\n");  
    digitalWrite(pumpPin, HIGH);
    alert_sent = true;
  }
  else if (moistureLevel >= moisture_threshold) {
    Blynk.logEvent("high_moisture_level", "Moisture level: " + String(moistureLevel));
    Serial.print("High moisture level, Turn off Pump");
    Serial.print(moistureLevel);
    Serial.print("\n");  
    digitalWrite(pumpPin, LOW);
    alert_sent = false; 
  }
}
