void getVoltage()
{
  Voltage = getVPP( voltageIn );
  //Voltage = Voltage - 0.4;
  //if (Voltage<0) { Voltage = -1*Voltage; }
  rVRMS = (Voltage / 2.0) * 0.707 * 393;
  Serial.print("Voltage "); Serial.print(rVRMS);    Serial.print(", "); Serial.print(Voltage); Serial.print("\t");
}

void getCurrent()
{
  Voltage = getVPP( currentIn );
  //Voltage = Voltage - 0.08;
  //if (Voltage<0) { Voltage = -1*Voltage; }
  sVRMS   = (Voltage / 2.0) * 0.707;
  AmpsRMS = ( (sVRMS * 1000) / 185 )*1.2;
  Serial.print("Current "); Serial.print(AmpsRMS);  Serial.print(", "); Serial.print(Voltage); Serial.print("\t");
}

float getVPP(const unsigned char sensorIn)
{
  float result  = 0;
  int readValue = 0;
  int maxValue  = 0;
  int minValue  = 4096;

  uint32_t start_time = millis();
  while ((millis() - start_time) < 1000)
  {
    readValue = analogRead(2);
    Serial.println(readValue);
    delay(100);                         //solved messy readings, idk how, lel. Probably caused due to Semaphores and Mutexes
    if (readValue > maxValue)
    { maxValue = readValue; }
    if (readValue < minValue)
    { minValue = readValue; }
  }
  result = ((maxValue - minValue) * 5.0) / 4096.0;
  return result;
}
