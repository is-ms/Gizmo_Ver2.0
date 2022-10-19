/*
  SCL, SDA pins MUST be remapped to PB8, PB9 in the file below
  C:\Program Files\arduino-1.8.7\hardware\Arduino_STM32-master\STM32F1\cores\maple\libmaple\i2c_f1.c
*/

#include <Wire.h>

void VL53L0X_setup()
{
  Wire.setClock(400000);
  Wire.begin();
  delay(200);
  set_VL53L0X_registers();
  
}

void VL53L0X_read()
{
  Wire.beginTransmission(0x29);                         //Start communication with the MPU6050 default I2C address
  Wire.write(0x48); 
  Wire.endTransmission();
  Wire.requestFrom(0x29, 1);
  VL53L0X_test_value = Wire.read();
}

void set_VL53L0X_registers()
{
  Wire.beginTransmission(0x29);                         //Start communication with the VL53L0X default I2C address
  Wire.write(0x00);                                     //Set the register bits as 00000010 to start ranging in back to back operation mode
  Wire.write(0x02); 
  Wire.endTransmission();
}
