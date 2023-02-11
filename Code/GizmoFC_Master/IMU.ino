/*

*/

float M[3][3];

void IMU()
{
  acc_angles_calc();
  roll_pitch_kalman_filter();
  yaw_angle += (float)gyro_axis_dps[2] * 0.004;

  rotate_body_to_world_frame();
}

void acc_angles_calc()
{
  acc_total_vector = sqrt((acc_axis_lpf[0] * acc_axis_lpf[0]) + (acc_axis_lpf[1] * acc_axis_lpf[1]) + (acc_axis_lpf[2] * acc_axis_lpf[2]));    //Calculate the total accelerometer vector
  if (abs(acc_axis_lpf[0]) < acc_total_vector) roll_angle_acc = asin((float)acc_axis_lpf[0] / acc_total_vector) * RAD_TO_DEG;
  if (abs(acc_axis_lpf[1]) < acc_total_vector) pitch_angle_acc = asin((float)acc_axis_lpf[1] / acc_total_vector) * RAD_TO_DEG;
  roll_angle_acc -= roll_angle_acc_trim;
  pitch_angle_acc -= pitch_angle_acc_trim;
}

void rotate_body_to_world_frame()
{
  float cosx = cos(roll_angle * DEG_TO_RAD);
  float sinx = sin(roll_angle * DEG_TO_RAD);
  float cosy = cos(pitch_angle * DEG_TO_RAD);
  float siny = sin(pitch_angle * DEG_TO_RAD);
  float cosz = cos(yaw_angle * DEG_TO_RAD);
  float sinz = sin(yaw_angle * DEG_TO_RAD);

  float coszcosx = cosz * cosx;
  float sinzcosx = sinz * cosx;
  float coszsinx = sinx * cosz;
  float sinzsinx = sinx * sinz;

  M[0][0] = cosz * cosy;
  M[0][1] = -cosy * sinz;
  M[0][2] = siny;
  M[1][0] = sinzcosx + (coszsinx * siny);
  M[1][1] = coszcosx - (sinzsinx * siny);
  M[1][2] = -sinx * cosy;
  M[2][0] = (sinzsinx) - (coszcosx * siny);
  M[2][1] = (coszsinx) + (sinzcosx * siny);
  M[2][2] = cosy * cosx;

  acc_axis_rotated[0] = acc_axis_lpf[0] * M[0][0] + acc_axis_lpf[1] * M[0][1] + acc_axis_lpf[2] * M[0][2];
  acc_axis_rotated[1] = acc_axis_lpf[0] * M[1][0] + acc_axis_lpf[1] * M[1][1] + acc_axis_lpf[2] * M[1][2];
  acc_axis_rotated[2] = acc_axis_lpf[0] * M[2][0] + acc_axis_lpf[1] * M[2][1] + acc_axis_lpf[2] * M[2][2];
}

void IMU_display_angles()
{
  Serial.print("roll_angle = ");
  Serial.print(roll_angle);
  Serial.print("    pitch_angle = ");
  Serial.print(pitch_angle);
  Serial.print("    yaw_angle = ");
  Serial.println(yaw_angle);
}
