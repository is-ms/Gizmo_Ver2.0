/*

*/

float P = 4;
float P_prev, K;

void roll_pitch_kalman_filter()
{
  //Predict
  roll_angle += (gyro_axis_dps[1] * 0.004);
  pitch_angle += (gyro_axis_dps[0] * 0.004);

  //Update error covariance (Increase)
  P = P_prev + (0.000256);
  P_prev = P;

  //Calculate kalman gain
  K = P / (P + 9);

  //Update
  roll_angle += K * (roll_angle_acc - roll_angle);
  pitch_angle += K * (pitch_angle_acc - pitch_angle);

  //Update error covariance (Decrease)
  P = (1 - K) * P;
}
