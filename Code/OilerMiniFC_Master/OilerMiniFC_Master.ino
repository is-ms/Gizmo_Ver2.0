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
  digitalWrite(IND_LED_RED, LOW);
  loop_timer = micros();
}

void loop()
{
  //VBatt_read();                                  //Read main battery voltage
  //process_Rx_inputs();                           //Read remote controller inputs
  mpu6050_read();
  if (xx >= 4) {
    xx = 0;
    receiver_read();
  }
  else xx++;
  PID();
  //motors_output();
  /*if (RC_connected == 1) {                       //Check remote controller status and change drone states accordingly
    if (RC_armed == 1) {
    if (start != true) start = true;           //Don't re-write if already set to TRUE
    PWR_LEDs_mode(BLU, 30, 2000);
    }
    else {
    start = false;
    motors_off();
    check_RC_arm();
    PWR_LEDs_mode(RED, 9999, 0);
    }
    }
    else {                                         //Connection lost to remote controller
    if (RC_armed == 1) {
    if (altitude_PID_enabled) rc_throttle_input = 0;  //Let drone hover at last registered altitude setpoint
    else                                              //Decrease throttle input gradually till drone lands
    {
      disconnected_rc_loop_counter++;
      rc_throttle_input = 0;
      if (disconnected_rc_loop_counter >= 100)
      {
        rc_throttle_input = -1;
        disconnected_rc_loop_counter = 0;
      }
    }
    rc_pitch_input = 0;
    rc_roll_input = 0;
    rc_yaw_input = 0;
    PWR_LEDs_mode(GRN, 100, 100);
    }
    else {
    start = false;
    motors_off();
    PWR_LEDs_mode(RED, 450, 450);
    }
    }
    if (start == true) {                           //Remote controller initialization done, ready to fly!
    if (previous_state != 1) {                   //Idle motors only on startup, i.e. on toggling start from 0 to 1
    M1_output = 1080;
    M2_output = 1080;
    M3_output = 1080;
    M4_output = 1080;motors_output();
    }
    check_RC_arm();                              //Check if remote controller is disarmed
    //mpu6050_read();
    //bmp280_read();
    PID();
    motors_output();
    }
    previous_state = start;
  */
  //display_rc_inputs();
  //mpu6050_display_angles();
  //Serial.println(loop_timer);
  /*Serial.print(M1_output);
    Serial.print("        ");
    Serial.print(M2_output);
    Serial.print("        ");
    Serial.print(M3_output);
    Serial.print("        ");
    Serial.println(M4_output);*/


  if ((micros() - loop_timer) > 32032) PORTC |= 0b00000100;   //32000 counts = 4000 microseconds
  //else PORTC &= 0b11111011;  
  while ((micros() - loop_timer) < 31952);
  loop_timer = micros();
}

bool loop_delay(uint16_t counter) {
  loop_delay_counter++;
  if (loop_delay_counter >= (counter / 4)) {
    loop_delay_counter = 0;
    return true;
  }
  else return false;
}
