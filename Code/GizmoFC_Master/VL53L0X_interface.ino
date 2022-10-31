/*
  SCL, SDA pins MUST be remapped to PB8, PB9 in the file below
  C:\Program Files\arduino-1.8.7\hardware\Arduino_STM32-master\STM32F1\cores\maple\libmaple\i2c_f1.c
*/
VL53L0X sensor;

void VL53L0X_setup()
{
  sensor.setTimeout(500);
  sensor.init();
  //sensor.setMeasurementTimingBudget(20000);
  sensor.startContinuous();
}

void VL53L0X_read()
{
  //sensor.readRangeSingleMillimeters();
  VL53L0X_altitude = sensor.readRangeContinuousMillimeters();
}
/*
  void VL53L0X_setup()
  {
  Wire.setClock(400000);
  Wire.begin();
  delay(200);
  VL53L0X_data_init();
  VL53L0X_static_init();
  VL53L0X_configure_interrupt();
  VL53L0X_calibrate();
  VL53L0X_configure_sequence_steps();
  }

  void VL53L0X_read()
  {
  Wire.beginTransmission(0x29);                         //Stop any ongoing measurements
  Wire.write(0x80);
  Wire.write(0x01);
  Wire.endTransmission();
  Wire.beginTransmission(0x29);
  Wire.write(0xFF);
  Wire.write(0x01);
  Wire.endTransmission();
  Wire.beginTransmission(0x29);
  Wire.write(0x00);
  Wire.write(0x00);
  Wire.endTransmission();
  Wire.beginTransmission(0x29);
  Wire.write(0x91);
  Wire.write(VL53L0X_stop_variable);
  Wire.endTransmission();
  Wire.beginTransmission(0x29);
  Wire.write(0x00);
  Wire.write(0x01);
  Wire.endTransmission();
  Wire.beginTransmission(0x29);
  Wire.write(0xFF);
  Wire.write(0x00);
  Wire.endTransmission();
  Wire.beginTransmission(0x29);
  Wire.write(0x80);
  Wire.write(0x00);
  Wire.endTransmission();

  Wire.beginTransmission(0x29);                         //Start a new range measurement
  Wire.write(0x00);
  Wire.write(0x01);
  Wire.endTransmission();

  VL53L0X_int = 0x01;                                   //Poll result interrupt status
  while (VL53L0X_int != 0x00)
  {
    Wire.beginTransmission(0x29);
    Wire.write(0x13);
    Wire.endTransmission();
    Wire.requestFrom(0x29, 1);
    VL53L0X_int = Wire.read() & 0x07;
  }

  Wire.beginTransmission(0x29);                         //Read range in millimeters
  Wire.write(0x1E);
  Wire.endTransmission();
  Wire.requestFrom(0x29, 2);
  VL53L0X_altitude = Wire.read() << 8 | Wire.read();

  Wire.beginTransmission(0x29);                         //Clear result interrupt
  Wire.write(0x0B);
  Wire.write(0x01);
  Wire.endTransmission();
  }*/

void VL53L0X_data_init()
{
  //Set I2C voltage to 2.8V
  Wire.beginTransmission(0x29);                         //Start communication with the VL53L0X default I2C address
  Wire.write(0x89);
  Wire.endTransmission();
  Wire.requestFrom(0x29, 1);
  VHV_CONFIG_PAD_SCL_SDA__EXTSUP_HV = Wire.read();
  VHV_CONFIG_PAD_SCL_SDA__EXTSUP_HV |= 0x01;
  Wire.beginTransmission(0x29);
  Wire.write(0x89);
  Wire.write(VHV_CONFIG_PAD_SCL_SDA__EXTSUP_HV);
  Wire.endTransmission();

  //Set I2C standard mode
  Wire.beginTransmission(0x29);
  Wire.write(0x88);
  Wire.write(0x00);
  Wire.endTransmission();
  Wire.beginTransmission(0x29);
  Wire.write(0x80);
  Wire.write(0x01);
  Wire.endTransmission();
  Wire.beginTransmission(0x29);
  Wire.write(0xFF);
  Wire.write(0x01);
  Wire.endTransmission();
  Wire.beginTransmission(0x29);
  Wire.write(0x00);
  Wire.write(0x00);
  Wire.endTransmission();

  Wire.beginTransmission(0x29);
  Wire.write(0x91);
  Wire.endTransmission();
  Wire.requestFrom(0x29, 1);
  VL53L0X_stop_variable = Wire.read();

  Wire.beginTransmission(0x29);
  Wire.write(0x00);
  Wire.write(0x01);
  Wire.endTransmission();
  Wire.beginTransmission(0x29);
  Wire.write(0xFF);
  Wire.write(0x00);
  Wire.endTransmission();
  Wire.beginTransmission(0x29);
  Wire.write(0x80);
  Wire.write(0x00);
  Wire.endTransmission();
}

void VL53L0X_static_init()
{

  Wire.beginTransmission(0x29);
  Wire.write(0xFF);
  Wire.write(0x01);
  Wire.endTransmission();
  Wire.beginTransmission(0x29);
  Wire.write(0x00);
  Wire.write(0x00);
  Wire.endTransmission();
  Wire.beginTransmission(0x29);
  Wire.write(0xFF);
  Wire.write(0x00);
  Wire.endTransmission();
  Wire.beginTransmission(0x29);
  Wire.write(0x09);
  Wire.write(0x00);
  Wire.endTransmission();
  Wire.beginTransmission(0x29);
  Wire.write(0x10);
  Wire.write(0x00);
  Wire.endTransmission();
  Wire.beginTransmission(0x29);
  Wire.write(0x11);
  Wire.write(0x00);
  Wire.endTransmission();
  Wire.beginTransmission(0x29);
  Wire.write(0x24);
  Wire.write(0x01);
  Wire.endTransmission();
  Wire.beginTransmission(0x29);
  Wire.write(0x25);
  Wire.write(0xFF);
  Wire.endTransmission();
  Wire.beginTransmission(0x29);
  Wire.write(0x75);
  Wire.write(0x00);
  Wire.endTransmission();
  Wire.beginTransmission(0x29);
  Wire.write(0xFF);
  Wire.write(0x01);
  Wire.endTransmission();
  Wire.beginTransmission(0x29);
  Wire.write(0x4E);
  Wire.write(0x2C);
  Wire.endTransmission();
  Wire.beginTransmission(0x29);
  Wire.write(0x48);
  Wire.write(0x00);
  Wire.endTransmission();
  Wire.beginTransmission(0x29);
  Wire.write(0x30);
  Wire.write(0x20);
  Wire.endTransmission();
  Wire.beginTransmission(0x29);
  Wire.write(0xFF);
  Wire.write(0x00);
  Wire.endTransmission();
  Wire.beginTransmission(0x29);
  Wire.write(0x30);
  Wire.write(0x09);
  Wire.endTransmission();
  Wire.beginTransmission(0x29);
  Wire.write(0x54);
  Wire.write(0x00);
  Wire.endTransmission();
  Wire.beginTransmission(0x29);
  Wire.write(0x31);
  Wire.write(0x04);
  Wire.endTransmission();
  Wire.beginTransmission(0x29);
  Wire.write(0x32);
  Wire.write(0x03);
  Wire.endTransmission();
  Wire.beginTransmission(0x29);
  Wire.write(0x40);
  Wire.write(0x83);
  Wire.endTransmission();
  Wire.beginTransmission(0x29);
  Wire.write(0x46);
  Wire.write(0x25);
  Wire.endTransmission();
  Wire.beginTransmission(0x29);
  Wire.write(0x60);
  Wire.write(0x00);
  Wire.endTransmission();
  Wire.beginTransmission(0x29);
  Wire.write(0x27);
  Wire.write(0x00);
  Wire.endTransmission();
  Wire.beginTransmission(0x29);
  Wire.write(0x50);
  Wire.write(0x06);
  Wire.endTransmission();
  Wire.beginTransmission(0x29);
  Wire.write(0x51);
  Wire.write(0x00);
  Wire.endTransmission();
  Wire.beginTransmission(0x29);
  Wire.write(0x52);
  Wire.write(0x96);
  Wire.endTransmission();
  Wire.beginTransmission(0x29);
  Wire.write(0x56);
  Wire.write(0x08);
  Wire.endTransmission();
  Wire.beginTransmission(0x29);
  Wire.write(0x57);
  Wire.write(0x30);
  Wire.endTransmission();
  Wire.beginTransmission(0x29);
  Wire.write(0x61);
  Wire.write(0x00);
  Wire.endTransmission();
  Wire.beginTransmission(0x29);
  Wire.write(0x62);
  Wire.write(0x00);
  Wire.endTransmission();
  Wire.beginTransmission(0x29);
  Wire.write(0x64);
  Wire.write(0x00);
  Wire.endTransmission();
  Wire.beginTransmission(0x29);
  Wire.write(0x65);
  Wire.write(0x00);
  Wire.endTransmission();
  Wire.beginTransmission(0x29);
  Wire.write(0x66);
  Wire.write(0xA0);
  Wire.endTransmission();
  Wire.beginTransmission(0x29);
  Wire.write(0xFF);
  Wire.write(0x01);
  Wire.endTransmission();
  Wire.beginTransmission(0x29);
  Wire.write(0x22);
  Wire.write(0x32);
  Wire.endTransmission();
  Wire.beginTransmission(0x29);
  Wire.write(0x47);
  Wire.write(0x14);
  Wire.endTransmission();
  Wire.beginTransmission(0x29);
  Wire.write(0x49);
  Wire.write(0xFF);
  Wire.endTransmission();
  Wire.beginTransmission(0x29);
  Wire.write(0x4A);
  Wire.write(0x00);
  Wire.endTransmission();
  Wire.beginTransmission(0x29);
  Wire.write(0xFF);
  Wire.write(0x00);
  Wire.endTransmission();
  Wire.beginTransmission(0x29);
  Wire.write(0x7A);
  Wire.write(0x0A);
  Wire.endTransmission();
  Wire.beginTransmission(0x29);
  Wire.write(0x7B);
  Wire.write(0x00);
  Wire.endTransmission();
  Wire.beginTransmission(0x29);
  Wire.write(0x78);
  Wire.write(0x21);
  Wire.endTransmission();
  Wire.beginTransmission(0x29);
  Wire.write(0xFF);
  Wire.write(0x01);
  Wire.endTransmission();
  Wire.beginTransmission(0x29);
  Wire.write(0x23);
  Wire.write(0x34);
  Wire.endTransmission();
  Wire.beginTransmission(0x29);
  Wire.write(0x42);
  Wire.write(0x00);
  Wire.endTransmission();
  Wire.beginTransmission(0x29);
  Wire.write(0x44);
  Wire.write(0xFF);
  Wire.endTransmission();
  Wire.beginTransmission(0x29);
  Wire.write(0x45);
  Wire.write(0x26);
  Wire.endTransmission();
  Wire.beginTransmission(0x29);
  Wire.write(0x46);
  Wire.write(0x05);
  Wire.endTransmission();
  Wire.beginTransmission(0x29);
  Wire.write(0x40);
  Wire.write(0x40);
  Wire.endTransmission();
  Wire.beginTransmission(0x29);
  Wire.write(0x0E);
  Wire.write(0x06);
  Wire.endTransmission();
  Wire.beginTransmission(0x29);
  Wire.write(0x20);
  Wire.write(0x1A);
  Wire.endTransmission();
  Wire.beginTransmission(0x29);
  Wire.write(0x43);
  Wire.write(0x40);
  Wire.endTransmission();
  Wire.beginTransmission(0x29);
  Wire.write(0xFF);
  Wire.write(0x00);
  Wire.endTransmission();
  Wire.beginTransmission(0x29);
  Wire.write(0x34);
  Wire.write(0x03);
  Wire.endTransmission();
  Wire.beginTransmission(0x29);
  Wire.write(0x35);
  Wire.write(0x44);
  Wire.endTransmission();
  Wire.beginTransmission(0x29);
  Wire.write(0xFF);
  Wire.write(0x01);
  Wire.endTransmission();
  Wire.beginTransmission(0x29);
  Wire.write(0x31);
  Wire.write(0x04);
  Wire.endTransmission();
  Wire.beginTransmission(0x29);
  Wire.write(0x4B);
  Wire.write(0x09);
  Wire.endTransmission();
  Wire.beginTransmission(0x29);
  Wire.write(0x4C);
  Wire.write(0x05);
  Wire.endTransmission();
  Wire.beginTransmission(0x29);
  Wire.write(0x4D);
  Wire.write(0x04);
  Wire.endTransmission();
  Wire.beginTransmission(0x29);
  Wire.write(0xFF);
  Wire.write(0x00);
  Wire.endTransmission();
  Wire.beginTransmission(0x29);
  Wire.write(0x44);
  Wire.write(0x00);
  Wire.endTransmission();
  Wire.beginTransmission(0x29);
  Wire.write(0x45);
  Wire.write(0x20);
  Wire.endTransmission();
  Wire.beginTransmission(0x29);
  Wire.write(0x47);
  Wire.write(0x08);
  Wire.endTransmission();
  Wire.beginTransmission(0x29);
  Wire.write(0x48);
  Wire.write(0x28);
  Wire.endTransmission();
  Wire.beginTransmission(0x29);
  Wire.write(0x67);
  Wire.write(0x00);
  Wire.endTransmission();
  Wire.beginTransmission(0x29);
  Wire.write(0x70);
  Wire.write(0x04);
  Wire.endTransmission();
  Wire.beginTransmission(0x29);
  Wire.write(0x71);
  Wire.write(0x01);
  Wire.endTransmission();
  Wire.beginTransmission(0x29);
  Wire.write(0x72);
  Wire.write(0xFE);
  Wire.endTransmission();
  Wire.beginTransmission(0x29);
  Wire.write(0x76);
  Wire.write(0x00);
  Wire.endTransmission();
  Wire.beginTransmission(0x29);
  Wire.write(0x77);
  Wire.write(0x00);
  Wire.endTransmission();
  Wire.beginTransmission(0x29);
  Wire.write(0xFF);
  Wire.write(0x01);
  Wire.endTransmission();
  Wire.beginTransmission(0x29);
  Wire.write(0x0D);
  Wire.write(0x01);
  Wire.endTransmission();
  Wire.beginTransmission(0x29);
  Wire.write(0xFF);
  Wire.write(0x00);
  Wire.endTransmission();
  Wire.beginTransmission(0x29);
  Wire.write(0x80);
  Wire.write(0x01);
  Wire.endTransmission();
  Wire.beginTransmission(0x29);
  Wire.write(0x01);
  Wire.write(0xF8);
  Wire.endTransmission();
  Wire.beginTransmission(0x29);
  Wire.write(0xFF);
  Wire.write(0x01);
  Wire.endTransmission();
  Wire.beginTransmission(0x29);
  Wire.write(0x8E);
  Wire.write(0x01);
  Wire.endTransmission();
  Wire.beginTransmission(0x29);
  Wire.write(0x00);
  Wire.write(0x01);
  Wire.endTransmission();
  Wire.beginTransmission(0x29);
  Wire.write(0xFF);
  Wire.write(0x00);
  Wire.endTransmission();
  Wire.beginTransmission(0x29);
  Wire.write(0x80);
  Wire.write(0x00);
  Wire.endTransmission();
}

void VL53L0X_configure_interrupt()
{
  Wire.beginTransmission(0x29);     //Enable Interrupt GPIO
  Wire.write(0x0A);
  Wire.write(0x04);
  Wire.endTransmission();

  Wire.beginTransmission(0x29);
  Wire.write(0x84);
  Wire.endTransmission();
  Wire.requestFrom(0x29, 1);
  SYSTEM_INTERRUPT_CONFIG_GPIO = Wire.read();
  SYSTEM_INTERRUPT_CONFIG_GPIO &= ~0x10;

  Wire.beginTransmission(0x29);     //Configure Interrup GPIO as Active LOW
  Wire.write(0x84);
  Wire.write(SYSTEM_INTERRUPT_CONFIG_GPIO);
  Wire.endTransmission();

  Wire.beginTransmission(0x29);     //Clear any system interrupts
  Wire.write(0x0B);
  Wire.write(0x01);
  Wire.endTransmission();
}

void VL53L0X_calibrate()
{
  //Start VHV calibration
  Wire.beginTransmission(0x29);
  Wire.write(0x01);
  Wire.write(0x01);
  Wire.endTransmission();
  Wire.beginTransmission(0x29);
  Wire.write(0x00);
  Wire.write(0x41);
  Wire.endTransmission();

  VL53L0X_int = 0x01;                                   //Poll result interrupt status
  while (VL53L0X_int != 0x00)
  {
    Wire.beginTransmission(0x29);
    Wire.write(0x13);
    Wire.endTransmission();
    Wire.requestFrom(0x29, 1);
    VL53L0X_int = Wire.read() & 0x07;
  }

  //Start Phase calibration
  Wire.beginTransmission(0x29);
  Wire.write(0x01);
  Wire.write(0x02);
  Wire.endTransmission();
  Wire.beginTransmission(0x29);
  Wire.write(0x00);
  Wire.write(0x01);
  Wire.endTransmission();

  VL53L0X_int = 0x01;                                   //Poll result interrupt status
  while (VL53L0X_int != 0x00)
  {
    Wire.beginTransmission(0x29);
    Wire.write(0x13);
    Wire.endTransmission();
    Wire.requestFrom(0x29, 1);
    VL53L0X_int = Wire.read() & 0x07;
  }

  Wire.beginTransmission(0x29);                         //Clear result interrupt
  Wire.write(0x0B);
  Wire.write(0x01);
  Wire.endTransmission();
  Wire.beginTransmission(0x29);
  Wire.write(0x00);
  Wire.write(0x00);
  Wire.endTransmission();
}

void VL53L0X_configure_sequence_steps()
{
  Wire.beginTransmission(0x29);
  Wire.write(0x01);
  Wire.write(0xE8);
  Wire.endTransmission();
}
