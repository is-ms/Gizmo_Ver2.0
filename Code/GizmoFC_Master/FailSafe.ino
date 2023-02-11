/*

*/

bool safety_flag = false;

bool FailSafe()
{
  if (((abs(roll_angle)) >= 40) || ((abs(pitch_angle)) >= 40)) {    //Trigger fail safe flag if drone is flipped
    safety_flag = true;
  }
  return safety_flag;
}
