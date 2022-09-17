
/*Decision Making MCU           //             \\
                               (4)             (1)
                               //  *         *  \\
  Motors orientation                  *   *
                                        +
                                      *   *
                               \\  *         *  //
                               (3)             (2)
                                \\             //

*/
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
//#include <TimerOne.h>

RF24 radio(A1, A0);                      //CE, CSN
const byte rxAddr[6] = "00001";
uint8_t packet = B11111111;
uint16_t pulse_rise = 0;
int16_t pulse_fall = 0;
int16_t throttle, roll, pitch, yaw;

void setup()
{
  pinMode(A5, OUTPUT);                  //Buzzer

  Serial.begin(115200);
  radio.begin();
  radio.setChannel(13);
  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_2MBPS);
  radio.openReadingPipe(0, rxAddr);
  radio.setAutoAck(0);
  radio.startListening();
  digitalWrite(A2, HIGH);
  delay(50);
  digitalWrite(A2, LOW);
  delay(50);
  digitalWrite(A2, HIGH);
  delay(50);
  digitalWrite(A2, LOW);
  delay(100);                           //Wait for the radio to initialize
  //Timer1.initialize(16000);             //Set 62.5Hz refresh rate (16000 uS period) for the output channels by Timer 1
  //Timer1.attachInterrupt(channels_out);
}

void loop()
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
    digitalWrite(A2, LOW);
  }
  else
  {
    throttle = 200;
    pitch = 3000;
    roll = 3000;
    yaw = 3000;
    Serial.println("No radio connection...");
    digitalWrite(A2, HIGH);
  }
  delay(13);
}

void channels_out() 
{
  PORTD |= B11110000;
  pulse_rise = TCNT1;
  while ((PORTD >> 4) != 0) 
  {
    pulse_fall = (TCNT1 - pulse_rise);
    //if (pulse_fall < 0) pulse_fall += 0xFFFF;
    if (pulse_fall >= yaw) PORTD &= B01111111;
    if (pulse_fall >= roll) PORTD &= B10111111;
    if (pulse_fall >= pitch) PORTD &= B11011111;
    if (pulse_fall >= throttle) PORTD &= B11101111;
  }
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
