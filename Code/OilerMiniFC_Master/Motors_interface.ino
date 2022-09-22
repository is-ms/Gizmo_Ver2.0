/*

*/

void motors_setup()
{
  pinMode(M1, OUTPUT);
  pinMode(M2, OUTPUT);
  pinMode(M3, OUTPUT);
  pinMode(M4, OUTPUT);
  TCCR2A = _BV(COM2B1) | _BV(WGM21) | _BV(WGM20);                   //FAST PWM Mode
  TCCR2B = _BV(CS21);                                               //Pre-scaler = 8
  TCCR0A = _BV(COM0A1) | _BV(COM0B1) | _BV(WGM01) | _BV(WGM00);     //FAST PWM Mode
  TCCR0B = _BV(CS01);                                               //Pre-scaler = 8 (Note: for micros() to work PSC needs to be = 64)
  TCCR1A = _BV(COM1A1) | _BV(WGM12) | _BV(WGM10);                   //FAST PWM Mode, Top Value = 0x00FF
  TCCR1B = _BV(CS11);                                               //Pre-scaler = 8  
  motors_off();
}

void motors_output()
{
  OCR0B = uint8_t(M1_output);
  OCR1A = uint8_t(M2_output);
  OCR0A = uint8_t(M3_output);
  OCR2B = uint8_t(M4_output);
}

void motors_off()
{
  M1_output = 0;
  M2_output = 0;
  M3_output = 0;
  M4_output = 0;
  motors_output();
}
