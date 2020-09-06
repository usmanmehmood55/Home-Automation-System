void core0task( void * parameter )
{
  pinMode(currentIn, INPUT);
  while (1)
  {
    Serial.println();
    //getVoltage();
    getCurrent();
    delay(500);
    power = AmpsRMS*220;// * rVRMS;
    Serial.print("Power   "); Serial.print(power);
    vTaskDelay(1 / portTICK_PERIOD_MS);
  }
}
