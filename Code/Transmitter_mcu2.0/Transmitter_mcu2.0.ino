/*Transmitter side MCU          //             \\
                           CW  (4)      ^      (1)  CCW
                               //  *    ^    *  \\
  Motors orientation                  * ^ *
                                        +
                                      *   *
                               \\  *         *  //
                          CCW  (3)             (2)  CW
                                \\             //

*/
#include <SPI.h>
#include <TimerOne.h>
#include <nRF24L01.h>
#include <RF24.h>
#define Buzz_Out      3                                 //D3, PD3
#define Throttle_Up   (PIND & B00100000)                //D5, PD5
#define Throttle_Dwn  (PINB & B00000010)                //D9, PB1
#define Yaw_Left      (PIND & B01000000)                //D6, PD6
#define Yaw_Right     (PIND & B00010000)                //D4, PD4
#define Pitch_Bck     (PINC & B00000001)                //A0, PC0
#define Roll_Right    (PINC & B00000010)                //A1, PC1
#define Pitch_Fwd     (PINC & B00000100)                //A2, PC2
#define Roll_Left     (PINC & B00001000)                //A3, PC3
RF24 radio(7, 8);                                       //(CE,CSN)  D7,D8
const byte rxAddr[6] = "00001";
uint8_t packet = B11111111;
float vbatt = 0;
bool batt_low = 0;
int i = 0;
int j = 0;
/*
   BIT 0 >  Trottle   UP
   BIT 1 >  Throttle  DOWN
   BIT 2 >  Yaw       RIGHT
   BIT 3 >  Yaw       LEFT
   BIT 4 >  Pitch     FORWARD
   BIT 5 >  Pitch     BACKWARD
   BIT 6 >  Roll      RIGHT
   BIT 7 >  Roll      LEFT
*/

void setup()
{
  Serial.begin(115200);
  radio.begin();
  radio.setRetries(10, 10);
  radio.setChannel(13);
  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_2MBPS);
  radio.setAutoAck(0);
  radio.openWritingPipe(rxAddr);
  radio.stopListening();
  DDRB  &= B11111101;                                   //All switches are in N/O, active LOW configuration
  DDRC  &= B00000000;
  DDRD  &= B10001111;
  PORTB |= B00000010;
  PORTC |= B00001111;
  PORTD |= B01110000;
  pinMode(Buzz_Out, OUTPUT);                            //Buzzer O/P
  PORTD |= B00001000;                                   //Buzzer startup sequence
  delay(50);
  PORTD &= B11110111;
  delay(50);
  PORTD |= B00001000;
  delay(50);
  PORTD &= B11110111;
  delay(50);
  PORTD |= B00001000;
  delay(100);
  PORTD &= B11110111;
  Timer1.initialize(1000000);             //Set 62.5Hz refresh rate (16000 uS period) for the output channels by Timer 1
  Timer1.attachInterrupt(Display_Out);
}

void loop()
{
  vbatt = (float)(analogRead(A7)) / 210 * 1.941;
  (vbatt <= 6.5) ? batt_low = 1 : batt_low = 0;
  (batt_low == 1) ? alarm_beep(150, 2000) : PORTD &= B11110111;      //Controller battery low warning

  (Throttle_Up  == 0) ? packet &= B11111110  :  packet |= B00000001; //Throttle Up
  (Throttle_Dwn == 0) ? packet &= B11111101  :  packet |= B00000010; //Throttle Down
  (Yaw_Right    == 0) ? packet &= B11111011  :  packet |= B00000100; //Yaw Right
  (Yaw_Left     == 0) ? packet &= B11110111  :  packet |= B00001000; //Yaw Left
  (Pitch_Fwd    == 0) ? packet &= B11011111  :  packet |= B00100000; //Pitch Forward
  (Pitch_Bck    == 0) ? packet &= B11101111  :  packet |= B00010000; //Pitch Backward
  (Roll_Right   == 0) ? packet &= B10111111  :  packet |= B01000000; //Roll Right
  (Roll_Left    == 0) ? packet &= B01111111  :  packet |= B10000000; //Roll Left

  uint8_t rc_data[1] = {packet};
  radio.write(&rc_data, sizeof(rc_data));
}

int alarm_beep(int beep_on, int beep_off)
{
  i++;
  if (i >= beep_off)
  {
    PORTD |= B00001000;
    j++;
    if (j >= beep_on)
    {
      PORTD &= B11110111;
      i = 0;
      j = 0;
    }
  }
}

void Display_Out()
{
  Serial.write(packet);
}
