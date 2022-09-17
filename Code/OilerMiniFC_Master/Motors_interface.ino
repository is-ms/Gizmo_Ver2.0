/*

*/

void motors_setup()
{
  pinMode(M1, OUTPUT);
  pinMode(M2, OUTPUT);
  pinMode(M3, OUTPUT);
  pinMode(M4, OUTPUT);
  //motors_off();
}
  
void motors_output()
{
  analogWrite(M1, M1_output);
  analogWrite(M2, M2_output);
  analogWrite(M3, M3_output);
  analogWrite(M4, M4_output);
}

void motors_off()
{
  M1_output = 0;
  M2_output = 0;
  M3_output = 0;
  M4_output = 0;
  motors_output();
}
