#include <Arduino.h>

#define voltageIn 36
#define currentIn 04
float Voltage = 0;
float sVRMS = 0;
float rVRMS = 0;
float AmpsRMS = 0;
float power = 0;

#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

char auth[] = "9e9b0c9e899e49c5a3c41f9166dbe060";
char ssid[] = "WiFi Name";
char pass[] = "WiFi Password";

WidgetBridge bridge1(V1);
BlynkTimer timer;

TaskHandle_t trackBlynk;

float getVPP(const unsigned char sensorIn)
{
	float result = 0;
	int readValue = 0;
	int maxValue = 0;
	int minValue = 4096;

	uint32_t start_time = millis();
	while ((millis() - start_time) < 1000)
	{
		readValue = analogRead(2);
		Serial.println(readValue);
		delay(100); //solved messy readings, idk how, lel. Probably caused due to Semaphores and Mutexes
		if (readValue > maxValue)
		{
			maxValue = readValue;
		}
		if (readValue < minValue)
		{
			minValue = readValue;
		}
	}
	result = ((maxValue - minValue) * 5.0) / 4096.0;
	return result;
}

void getVoltage()
{
	Voltage = getVPP(voltageIn);
	//Voltage = Voltage - 0.4;
	//if (Voltage<0) { Voltage = -1*Voltage; }
	rVRMS = (Voltage / 2.0) * 0.707 * 393;
	Serial.print("Voltage ");
	Serial.print(rVRMS);
	Serial.print(", ");
	Serial.print(Voltage);
	Serial.print("\t");
}

void getCurrent()
{
	Voltage = getVPP(currentIn);
	//Voltage = Voltage - 0.08;
	//if (Voltage<0) { Voltage = -1*Voltage; }
	sVRMS = (Voltage / 2.0) * 0.707;
	AmpsRMS = ((sVRMS * 1000) / 185) * 1.2;
	Serial.print("Current ");
	Serial.print(AmpsRMS);
	Serial.print(", ");
	Serial.print(Voltage);
	Serial.print("\t");
}

void send2blynk()
{
	Blynk.virtualWrite(V2, power);
	if (analogRead(13) >= 1000)
	{
		Blynk.virtualWrite(V6, 1023);
	}
	else
	{
		Blynk.virtualWrite(V6, 0);
	}
}

// Send to slave
BLYNK_CONNECTED()
{
	bridge1.setAuthToken("457b126136f04389a7b3dddb2924a119"); //slave unit
}
int pinData = 0;
BLYNK_WRITE(V1)
{
	pinData = param.asInt();
}
void send2slave()
{
	if (pinData)
	{
		bridge1.digitalWrite(4, HIGH);
		bridge1.virtualWrite(V5, 1);
	}
	else
	{
		bridge1.digitalWrite(4, LOW);
		bridge1.virtualWrite(V5, 0);
	}
}

void core0task(void *parameter)
{
	pinMode(currentIn, INPUT);
	while (1)
	{
		Serial.println();
		//getVoltage();
		getCurrent();
		delay(500);
		power = AmpsRMS * 220; // * rVRMS;
		Serial.print("Power   ");
		Serial.print(power);
		vTaskDelay(1 / portTICK_PERIOD_MS);
	}
}

void setup()
{
	Serial.begin(9600);
	Blynk.begin(auth, ssid, pass);
	timer.setInterval(1000L, send2slave);
	timer.setInterval(500L, send2blynk);
	xTaskCreatePinnedToCore(core0task, "core0task", 1000, NULL, 1, &trackBlynk, 0);
}
void loop()
{
	Blynk.run();
	timer.run();
}