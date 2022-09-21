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
  //mpu6050_setup();
  //motors_setup();
  loop_timer = micros();
}

void loop()
{
  //VBatt_read();                                  //Read main battery voltage
  //process_Rx_inputs();                           //Read remote controller inputs
  //mpu6050_read();
  //PID();
  if (xx >= 7) {
    xx = 0;
    receiver_read();
  }
  else xx++;
  if ((packet & B00000001) == 0) {
    M1_output = 120;
    M2_output = 100;
    M3_output = 120;
    M4_output = 120;
  }
  else {
    M1_output = 0;
    M2_output = 0;
    M3_output = 0;
    M4_output = 0;
  }
  motors_output();
  //display_rc_inputs();
  //mpu6050_display_angles();
  //Serial.println(VBatt);

  if ((micros() - loop_timer) > 4000) digitalWrite(IND_LED_RED, LOW);
  else digitalWrite(IND_LED_RED, HIGH);
  while ((micros() - loop_timer) < 4000);
  loop_timer = micros();

}
