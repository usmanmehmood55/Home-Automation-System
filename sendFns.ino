void send2blynk()
{
  Blynk.virtualWrite(V2, power);
  if ( analogRead(13) >= 1000)
  {
    Blynk.virtualWrite(V6, 1023);
  }
  else
  {
    Blynk.virtualWrite(V6, 0);
  }
}

// Send to slave
BLYNK_CONNECTED() {
  bridge1.setAuthToken("457b126136f04389a7b3dddb2924a119"); //slave unit
}
int pinData = 0;
BLYNK_WRITE(V1)
{
  pinData = param.asInt();
}
void send2slave()
{
  if (pinData) {
    bridge1.digitalWrite(4, HIGH);
    bridge1.virtualWrite(V5, 1);
  } else {
    bridge1.digitalWrite(4, LOW);
    bridge1.virtualWrite(V5, 0);
  }
}
