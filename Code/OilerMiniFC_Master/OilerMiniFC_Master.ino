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
  //VBatt_read();                                  //Read main battery voltage
  //process_Rx_inputs();                           //Read remote controller inputs
  //mpu6050_read();
  //PID();
  //motors_output();
  if (xx >= 3) {
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
  /*delay(1000);
    M1_output = 0;
    M2_output = 0;
    M3_output = 0;
    M4_output = 0;
    motors_output();
    delay(2500);
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
  //Serial.println(VBatt);
  /*Serial.print(M1_output);
    Serial.print("        ");
    Serial.print(M2_output);
    Serial.print("        ");
    Serial.print(M3_output);
    Serial.print("        ");
    Serial.println(M4_output);*/
  /*Serial.print(gyro_axis_cal[0]);
    Serial.print("        ");
    Serial.print(gyro_axis_cal[1]);
    Serial.print("        ");
    Serial.print(gyro_axis_cal[2]);
    Serial.print("        ");
    Serial.println(mpu6050_temperature);
    /*if (packet != 255)
    {
    (0b1 & (packet >> 1)) ?  digitalWrite(BUZZER_PIN, LOW) : digitalWrite(BUZZER_PIN, HIGH);
    (0b1 & (packet >> 4)) ?  PWR_LED_RED_output = 1000 : PWR_LED_RED_output = 0;
    (0b1 & (packet >> 5)) ?  PWR_LED_GRN_output = 1000 : PWR_LED_GRN_output = 0;
    (0b1 & (packet >> 6)) ?  PWR_LED_BLU_output = 1000 : PWR_LED_BLU_output = 0;
    (0b1 & (packet >> 3)) ?  digitalWrite(IND_LED_RED, HIGH) : digitalWrite(IND_LED_RED, LOW);
    (0b1 & (packet >> 7)) ?  digitalWrite(IND_LED_RED, HIGH) : digitalWrite(IND_LED_RED, LOW);

    if (0b1 & (packet >> 0))
    {
      M1_output = 1000;
      M2_output = 1060;
      M3_output = 1000;
      M4_output = 1060;
    }
    else
    {
      M1_output = 1000;
      M2_output = 1000;
      M3_output = 1000;
      M4_output = 1000;
    }
    }
    else
    {
    digitalWrite(IND_LED_RED, HIGH);
    }*/
  /*digitalWrite(M1, HIGH);
    digitalWrite(M2, HIGH);
    digitalWrite(M3, HIGH);
    digitalWrite(M4, HIGH);
    pulse_rise = micros();
    while (digitalRead(M1) || digitalRead(M2) || digitalRead(M3) || digitalRead(M4)) {
    pulse_fall = micros() - pulse_rise;
    if (pulse_fall >= M1_output) digitalWrite(M1, LOW);
    if (pulse_fall >= M2_output) digitalWrite(M2, LOW);
    if (pulse_fall >= M3_output) digitalWrite(M3, LOW);
    if (pulse_fall >= M4_output) digitalWrite(M4, LOW);
    }*/

  if ((micros() - loop_timer) > 4000) digitalWrite(IND_LED_RED, LOW);
  else digitalWrite(IND_LED_RED, HIGH);
  while ((micros() - loop_timer) < 4000);
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
