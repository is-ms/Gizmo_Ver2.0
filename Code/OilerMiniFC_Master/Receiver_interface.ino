
/*
 
*/



void receiver_setup()
{
  radio.begin();
  radio.setChannel(13);
  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_2MBPS);
  radio.openReadingPipe(0, rxAddr);
  radio.setAutoAck(0);
  radio.startListening();
}

void receiver_read()
{
  if (radio.available())
  {
    radio.read(&packet, sizeof(packet));

    if (((packet & B00000001) == 0) && ((packet & B00000010) == 0))         throttle = 3000;          //Throttle
    else if ((packet & B00000001) == 0)                                     throttle = 4000;
    else if ((packet & B00000010) == 0)                                     throttle = 2000;
    else                                                                    throttle = 3000;

    if (((packet & B00100000) == 0) && ((packet & B00010000) == 0))         pitch = 3000;             //Pitch
    else if ((packet & B00100000) == 0)                                     pitch = 4000;
    else if ((packet & B00010000) == 0)                                     pitch = 2000;
    else                                                                    pitch = 3000;

    if (((packet & B10000000) == 0) && ((packet & B01000000) == 0))         roll = 3000;              //Roll
    else if ((packet & B10000000) == 0)                                     roll = 4000;
    else if ((packet & B01000000) == 0)                                     roll = 2000;
    else                                                                    roll = 3000;

    if (((packet & B00001000) == 0) && ((packet & B00000100) == 0))         yaw = 3000;               //Yaw
    else if ((packet & B00001000) == 0)                                     yaw = 4000;
    else if ((packet & B00000100) == 0)                                     yaw = 2000;
    else                                                                    yaw = 3000;
    Serial.println(packet);
    //print_result();
    digitalWrite(IND_LED_RED, LOW);
  }
  else
  {
    throttle = 200;
    pitch = 3000;
    roll = 3000;
    yaw = 3000;
    Serial.println("No radio connection...");
    digitalWrite(IND_LED_RED, HIGH);
  }
  //delay(13);
}

void print_result()
{
  if ((packet & B00000001) == 0)          //Throttle
  {
    Serial.print("Throttle: ^^^ ");
  }
  else if ((packet & B00000010) == 0)
  {
    Serial.print("Throttle: vvv ");
  }
  else {
    Serial.print("Throttle: --- ");
  }

  if ((packet & B10000000) == 0)          //Roll
  {
    Serial.print("Roll: <<< ");
  }
  else if ((packet & B01000000) == 0)
  {
    Serial.print("Roll: >>> ");
  }
  else {
    Serial.print("Roll: --- ");
  }

  if ((packet & B00100000) == 0)          //Pitch
  {
    Serial.print("Pitch: ^^^ ");
  }
  else if ((packet & B00010000) == 0)
  {
    Serial.print("Pitch: vvv ");
  }
  else {
    Serial.print("Pitch: --- ");
  }

  if ((packet & B00001000) == 0)          //Yaw
  {
    Serial.println("Yaw: <<< ");
  }
  else if ((packet & B00000100) == 0)
  {
    Serial.println("Yaw: >>> ");
  }
  else {
    Serial.println("Yaw: --- ");
  }

}
