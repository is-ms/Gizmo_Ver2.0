/*
  SCL, SDA pins MUST be remapped to PB8, PB9 in the file below
  C:\Program Files\arduino-1.8.7\hardware\Arduino_STM32-master\STM32F1\cores\maple\libmaple\i2c_f1.c
*/
VL53L0X sensor;

void VL53L0X_setup()
{
  sensor.setTimeout(500);
  sensor.init();
  sensor.startContinuous();
}

void VL53L0X_read()
{
  Wire.beginTransmission(0x29);
  Wire.write(0x13);
  Wire.endTransmission();
  Wire.requestFrom(0x29, 1);
  VL53L0X_int = Wire.read() & 0x07;

  if (VL53L0X_int != 0)
  {
    Wire.beginTransmission(0x29);
    Wire.write(0x14 + 10);
    Wire.endTransmission();
    Wire.requestFrom(0x29, 2);
    VL53L0X_altitude = Wire.read() << 8 | Wire.read();

    Wire.beginTransmission(0x29);                         //Clear result interrupt
    Wire.write(0x0B);
    Wire.write(0x01);
    Wire.endTransmission();
  }
}
