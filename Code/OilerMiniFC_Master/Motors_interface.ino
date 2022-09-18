/*

*/

void motors_setup()
{
  pinMode(M1, OUTPUT);
  pinMode(M2, OUTPUT);
  pinMode(M3, OUTPUT);
  pinMode(M4, OUTPUT);
  TCCR2A = _BV(COM2B1) | _BV(WGM21) | _BV(WGM20);
  TCCR2B = _BV(CS22);
  TCCR0A = _BV(COM2A1) | _BV(COM2B1) | _BV(WGM21) | _BV(WGM20);
  TCCR0B = _BV(CS00) | _BV(CS01);
  TCCR1A = _BV(COM2A1) | _BV(WGM21) | _BV(WGM20);
  TCCR1B = _BV(CS10) | _BV(CS11);
  motors_off();
}
  
void motors_output()
{
  //analogWrite(M1, M1_output);
  OCR0B = M1_output;
  //analogWrite(M2, M2_output);
  OCR1A = M2_output;
  //analogWrite(M3, M3_output);
  OCR0A = M3_output;
  //analogWrite(M4, M4_output);
  OCR2B = M4_output;
}

void motors_off()
{
  M1_output = 0;
  M2_output = 0;
  M3_output = 0;
  M4_output = 0;
  motors_output();
}
