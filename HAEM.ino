#define voltageIn      36
#define currentIn      04
float Voltage = 0;
float sVRMS   = 0;
float rVRMS   = 0;
float AmpsRMS = 0;
float power   = 0;

#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

char auth[] = "9e9b0c9e899e49c5a3c41f9166dbe060";
char ssid[] = "artologics";
char pass[] = "11458900";

WidgetBridge bridge1(V1);
BlynkTimer timer;

TaskHandle_t trackBlynk;

void setup()
{
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(1000L, send2slave);
  timer.setInterval(500L, send2blynk);
  xTaskCreatePinnedToCore( core0task, "core0task", 1000, NULL, 1, &trackBlynk, 0);
}
void loop()
{
  Blynk.run();
  timer.run();
}
