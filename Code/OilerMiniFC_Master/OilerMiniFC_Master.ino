/*
       OILER FLIGHT CONTROLLER       //             \\
                                 CC (4)             (1) CCW
                                    //  *    ^    *  \\
       Motors orientation                  * ^ *
                                             +
                                           *   *
                                    \\  *         *  //
                                CCW (3)             (2) CC
                                     \\             //

       Written by Islam S. 09/09/2022
       Target MCU ATMEGA328p
  _________________________________________________________
*/
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "global_variables.h"
#include "GPIOs_Map.h"

void setup()
{
  pinMode(IND_LED_RED, OUTPUT);
  digitalWrite(IND_LED_RED, HIGH);
  Serial.begin(115200);
  receiver_setup();
  mpu6050_setup();
  motors_setup();
  loop_timer = micros();
}

void loop()
{

}
