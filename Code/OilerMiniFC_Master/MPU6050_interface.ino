/*
  SCL, SDA pins MUST be remapped to PB8, PB9 in the file below
  C:\Program Files\arduino-1.8.7\hardware\Arduino_STM32-master\STM32F1\cores\maple\libmaple\i2c_f1.c
*/

#include <Wire.h>

void mpu6050_setup()
{
  Wire.setClock(400000);
  Wire.begin();
  delay(200);
  set_mpu6050_registers();
  delay(40);
  if (gyro_calibration_enabled)
  {
    for (int i = 0; i <= 1999; i++) {                       //Gyro static bias filter.
      if (i % 40 == 0) digitalWrite(IND_LED_RED, !digitalRead(IND_LED_RED));
      Wire.beginTransmission(0x68);                         //Start communication with the gyro.
      Wire.write(0x3B);                                     //Start reading @ register 3Bh and auto increment with every read.
      Wire.endTransmission();                               //End the transmission.
      Wire.requestFrom(0x68, 14);                           //Request 14 bytes from the gyro.
      acc_axis[0] = Wire.read() << 8 | Wire.read();
      acc_axis[1] = Wire.read() << 8 | Wire.read();
      acc_axis[2] = Wire.read() << 8 | Wire.read();
      mpu6050_temperature_raw = Wire.read() << 8 | Wire.read();
      gyro_axis[0] = Wire.read() << 8 | Wire.read();
      gyro_axis[1] = Wire.read() << 8 | Wire.read();
      gyro_axis[2] = Wire.read() << 8 | Wire.read();
      acc_total_vector = sqrt((acc_axis[0] * acc_axis[0]) + (acc_axis[1] * acc_axis[1]) + (acc_axis[2] * acc_axis[2]));    //Calculate the total accelerometer vector
      if ((acc_total_vector >= 3786) && (acc_total_vector <= 4406))       //Condition to ensure that sensor is stationary on initialization
      {
        gyro_axis_cal[0] += gyro_axis[0];
        gyro_axis_cal[1] += gyro_axis[1];
        gyro_axis_cal[2] += gyro_axis[2];
      }
      else 
      {
        i--;
        if (i <= 0) i = 0;
      }
    }
    digitalWrite(IND_LED_RED, HIGH);
    gyro_axis_cal[0] /= 2000;
    gyro_axis_cal[1] /= 2000;
    gyro_axis_cal[2] /= 2000;
  }
  else
  {
    digitalWrite(IND_LED_RED, HIGH);
    gyro_axis_cal[0] = -437;
    gyro_axis_cal[1] = 163;
    gyro_axis_cal[2] = 32;
  }
  digitalWrite(IND_LED_RED, LOW);
  delay(2000);
  for (int i = 0; i < 3; i++)
  {
    mpu6050_read();
    if ((acc_total_vector >= 3786) && (acc_total_vector <= 4406)) //Condition to ensure that sensor is stationary on initialization
    {
      roll_angle_acc_startup += roll_angle_acc;
      pitch_angle_acc_startup += pitch_angle_acc;
    }      
    else 
    {
      i--;
      if (i <= 0) i = 0;
    }
  }
  roll_angle = (roll_angle_acc_startup / 3);
  pitch_angle = (pitch_angle_acc_startup / 3);
  digitalWrite(IND_LED_RED, HIGH);
}

void mpu6050_read()
{
  //>>>>>>>>>>>>  READ RAW DATA  <<<<<<<<<<<<

  Wire.beginTransmission(0x68);                         //Start communication with the gyro.
  Wire.write(0x3B);                                     //Start reading @ register 3Bh and auto increment with every read.
  Wire.endTransmission();                               //End the transmission.
  Wire.requestFrom(0x68, 14);                           //Request 14 bytes from the gyro.
  acc_axis[0] = Wire.read() << 8 | Wire.read();
  acc_axis[1] = Wire.read() << 8 | Wire.read();
  acc_axis[2] = Wire.read() << 8 | Wire.read();
  mpu6050_temperature_raw = Wire.read() << 8 | Wire.read();
  gyro_axis[0] = Wire.read() << 8 | Wire.read();
  gyro_axis[1] = Wire.read() << 8 | Wire.read();
  gyro_axis[2] = Wire.read() << 8 | Wire.read();
  gyro_axis[0] -= gyro_axis_cal[0];                     //-436  X-axis
  gyro_axis[1] -= gyro_axis_cal[1];                     //163   Y-axis
  gyro_axis[2] -= gyro_axis_cal[2];                     //32    Z-axis
  //acc_axis[0] *= -1;                                  //Invert the direction of the acc roll axis ref. to the drone geometry
  acc_axis[1] *= -1;                                    //Invert the direction of the acc pitch axis ref. to the drone geometry
  //acc_axis[2] *= -1;                                  //Invert the direction of the acc Z axis ref. to the drone geometry
  //gyro_axis[0] *= -1;                                 //Invert the direction of the gyro pitch axis ref. to the drone geometry
  gyro_axis[1] *= -1;                                   //Invert the direction of the gyro roll axis ref. to the drone geometry
  //gyro_axis[2] *= -1;                                 //Invert the direction of the gyro yaw axis ref. to the drone geometry
  mpu6050_temperature = ((float)mpu6050_temperature_raw / 340) + 36.53;

  //>>>>>>>>>>>  GYRO ANGLE CALC  <<<<<<<<<<<

  roll_angle += (float)gyro_axis[1] / (250 * 65.5);             //Y-axis  ROLL
  pitch_angle += (float)gyro_axis[0] / (250 * 65.5);            //X-axis  PITCH
  yaw_angle += (float)gyro_axis[2] / (250 * 65.5);              //Z-axis  YAW
  pitch_angle -= roll_angle * sin(gyro_axis[2] * 0.000001066);  //If the IMU has yawed transfer the roll angle to the pitch angel.
  roll_angle += pitch_angle * sin(gyro_axis[2] * 0.000001066);  //If the IMU has yawed transfer the pitch angle to the roll angel.

  //>>>>>>>>>>>  ACCL ANGLE CALC  <<<<<<<<<<<

  acc_total_vector = sqrt((acc_axis[0] * acc_axis[0]) + (acc_axis[1] * acc_axis[1]) + (acc_axis[2] * acc_axis[2]));    //Calculate the total accelerometer vector
  if (abs(acc_axis[0]) < acc_total_vector) roll_angle_acc = asin((float)acc_axis[0] / acc_total_vector) * 57.296;
  if (abs(acc_axis[1]) < acc_total_vector) pitch_angle_acc = asin((float)acc_axis[1] / acc_total_vector) * -57.296;
  roll_angle_acc -= 0.85;
  pitch_angle_acc += 1.85;

  //>>>>>>>>>>>  FINAL ANGLE CALC  <<<<<<<<<<

  roll_angle = (0.9996 * roll_angle) + (0.0004 * roll_angle_acc);
  pitch_angle = (0.9996 * pitch_angle) + (0.0004 * pitch_angle_acc);
}

void set_mpu6050_registers()
{
  Wire.beginTransmission(0x68);                         //Start communication with the MPU6050 default I2C address
  Wire.write(0x6B);                                     //We want to write to the PWR_MGMT_1 register (6B hex)
  Wire.write(0x01);                                     //Set the register bits as 00000001 to activate the gyro & set clock source to X-Gyroscope
  Wire.endTransmission();
  Wire.beginTransmission(0x68);
  Wire.write(0x1B);                                     //We want to write to the GYRO_CONFIG register (1B hex).
  Wire.write(0x08);                                     //Set the register bits as 00001000 (500dps full scale).
  Wire.endTransmission();
  Wire.beginTransmission(0x68);
  Wire.write(0x1C);                                     //We want to write to the ACCEL_CONFIG register (1A hex).
  Wire.write(0x10);                                     //Set the register bits as 00010000 (+/- 8g full scale range).
  Wire.endTransmission();
  Wire.beginTransmission(0x68);
  Wire.write(0x1B);
  Wire.endTransmission();
  Wire.requestFrom(0x68, 1);                            //Quick check that MPU6050 is running & receiving the correct configurations
  while (Wire.available() < 1);
  if (Wire.read() != 0x08) {
    digitalWrite(IND_LED_RED, LOW);
    while(1) delay(10);
  }
  Wire.beginTransmission(0x68);
  Wire.write(0x1A);                                     //We want to write to the CONFIG register (1A hex).
  Wire.write(0x03);                                     //Set the register bits as 00000011 (Set Digital Low Pass Filter to ~43Hz).
  Wire.endTransmission();
}

void mpu6050_display_angles()
{
  /*Serial.print("ACC X = ");
  Serial.print(acc_axis[0]);
  Serial.print("  ACC Y = ");
  Serial.print(acc_axis[1]);
  Serial.print("  ACC Z = ");
  Serial.print(acc_axis[2]);
  Serial.print("  Total Vect = ");
  Serial.println(acc_total_vector);*/
  /*Serial.print("  Temperature = ");
  Serial.println(mpu6050_temperature);*/
  /*Serial.print("Gyro X = ");
  Serial.print(gyro_axis[0]);
  Serial.print("    Gyro Y = ");
  Serial.print(gyro_axis[1]);
  Serial.print("    Gyro Z = ");
  Serial.println(gyro_axis[2]);*/
  Serial.print("roll_angle = ");
  Serial.print(roll_angle);
  Serial.print("    pitch_angle = ");
  Serial.print(pitch_angle);
  Serial.print("    yaw_angle = ");
  Serial.println(yaw_angle);
}
