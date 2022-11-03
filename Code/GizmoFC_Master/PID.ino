/*

*/

void PID()
{
  if (altitude_PID_enabled) altitude_PID();
  else
  {
    if (rc_throttle_input_counter >= 12) {
      altitude_pid_output += rc_throttle_input;
      rc_throttle_input_counter = 0;
    }
    else rc_throttle_input_counter++;
    if (altitude_pid_output < 2) altitude_pid_output = 2;
    if (altitude_pid_output > 200) altitude_pid_output = 200;
  }
  pitch_PID();
  roll_PID();
  yaw_PID();
  M1_output = altitude_pid_output + pitch_pid_output + roll_pid_output + yaw_pid_output;// + 20;
  M2_output = altitude_pid_output - pitch_pid_output + roll_pid_output - yaw_pid_output;// + 20;
  M3_output = altitude_pid_output - pitch_pid_output - roll_pid_output + yaw_pid_output;// + 20;
  M4_output = altitude_pid_output + pitch_pid_output - roll_pid_output - yaw_pid_output;// + 20;

  if (M1_output <= 10) M1_output = 0;                                                     //Limit motors output (1080 ~ 1980uS)
  if (M2_output <= 10) M2_output = 0;
  if (M3_output <= 10) M3_output = 0;
  if (M4_output <= 10) M4_output = 0;
  if (M1_output > 250) M1_output = 250;
  if (M2_output > 250) M2_output = 250;
  if (M3_output > 250) M3_output = 250;
  if (M4_output > 250) M4_output = 250;
}

void altitude_PID()
{
  altitude_setpoint -= rc_throttle_input;                                                     //Update setpoint upon the user input
  altitude_error = bmp280_altitude - altitude_setpoint;
  altitude_i_error += altitude_pid_i_gain * altitude_error;
  altitude_pid_output = (altitude_pid_p_gain * altitude_error) + altitude_i_error + (altitude_pid_d_gain * (altitude_error - altitude_last_error));
  altitude_last_error = altitude_error;

  if (altitude_pid_output > 320) altitude_pid_output = 320;                                   //Limit PID output
  if (altitude_pid_output < -80) altitude_pid_output = -80;
}

void pitch_PID()
{
  pitch_error = pitch_angle - pitch_setpoint + (rc_pitch_input * 4);                          //Update setpoint upon the user input
  //pitch_error = (rc_pitch_input * 4);
  pitch_i_error += pitch_pid_i_gain * pitch_error;
  pitch_pid_output = (pitch_pid_p_gain * pitch_error) + pitch_i_error + (pitch_pid_d_gain * (pitch_error - pitch_last_error));
  pitch_last_error = pitch_error;

  if (pitch_pid_output > 120) pitch_pid_output = 120;                                         //Limit PID output
  if (pitch_pid_output < -120) pitch_pid_output = -120;
}

void roll_PID()
{
  roll_error = roll_angle - roll_setpoint + (rc_roll_input * 4);                              //Update setpoint upon the user input
  //roll_error = (rc_roll_input * 4);
  roll_i_error += roll_pid_i_gain * roll_error;
  roll_pid_output = (roll_pid_p_gain * roll_error) + roll_i_error + (roll_pid_d_gain * (roll_error - roll_last_error));
  roll_last_error = roll_error;

  if (roll_pid_output > 120) roll_pid_output = 120;                                           //Limit PID output
  if (roll_pid_output < -120) roll_pid_output = -120;
}

void yaw_PID()
{
  //yaw_error = yaw_angle - yaw_setpoint + (rc_yaw_input * 15);                                 //Update setpoint upon the user input
  yaw_error = (rc_yaw_input * 15);
  yaw_i_error += yaw_pid_i_gain * yaw_error;
  yaw_pid_output = (yaw_pid_p_gain * yaw_error) + yaw_i_error + (yaw_pid_d_gain * (yaw_error - yaw_last_error));
  yaw_last_error = yaw_error;

  if (yaw_pid_output > 40) yaw_pid_output = 40;                                             //Limit PID output
  if (yaw_pid_output < -40) yaw_pid_output = -40;
}
