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
  //Data Initialization
  Wire.beginTransmission(0x29);                         //Start communication with the VL53L0X default I2C address
  Wire.write(0x89);                                     //Set I2C voltage to 2.8V
  Wire.endTransmission();
  Wire.requestFrom(0x29, 1);
  VHV_CONFIG_PAD_SCL_SDA__EXTSUP_HV = Wire.read();
  VHV_CONFIG_PAD_SCL_SDA__EXTSUP_HV |= 0x01;
  Wire.beginTransmission(0x29);
  Wire.write(0x89);
  Wire.write(VHV_CONFIG_PAD_SCL_SDA__EXTSUP_HV);
  Wire.endTransmission();
  
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
