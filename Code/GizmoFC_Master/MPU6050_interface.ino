/*
  SCL, SDA pins MUST be remapped to PB8, PB9 in the file below
  C:\Program Files\arduino-1.8.7\hardware\Arduino_STM32-master\STM32F1\cores\maple\libmaple\i2c_f1.c
*/

void mpu6050_setup()
{
  delay(200);
  set_mpu6050_registers();
  delay(40);
  if (acc_calibration_enabled)
  {
    for (int i = 0; i < 500; i++) {                         //Accelerometer static bias filter.
      if (i % 10 == 0) {
        digitalWrite(IND_LED_RED, !digitalRead(IND_LED_RED));
        Serial.println("Calibrating Accelerometer..");
      }
      Wire.beginTransmission(0x68);                         //Start communication with the gyro.
      Wire.write(0x3B);                                     //Start reading @ register 3Bh and auto increment with every read.
      Wire.endTransmission();                               //End the transmission.
      Wire.requestFrom(0x68, 6);                            //Request 6 bytes from the gyro.
      acc_axis[0] = Wire.read() << 8 | Wire.read();
      acc_axis[1] = Wire.read() << 8 | Wire.read();
      acc_axis[2] = Wire.read() << 8 | Wire.read();
      acc_total_vector = sqrt((acc_axis[0] * acc_axis[0]) + (acc_axis[1] * acc_axis[1]) + (acc_axis[2] * acc_axis[2]));    //Calculate the total accelerometer vector
      if ((acc_total_vector >= 4280) && (acc_total_vector <= 4350))       //Condition to ensure that sensor is stationary on initialization
      {
        acc_axis_cal[0] += acc_axis[0];
        acc_axis_cal[1] += acc_axis[1];
        acc_axis_cal[2] += acc_axis[2];
      }
      else
      {
        i--;
        if (i <= 0) i = 0;
      }
      delay(2);
    }
    digitalWrite(IND_LED_RED, HIGH);
    acc_axis_cal[0] /= 500;
    acc_axis_cal[1] /= 500;
    acc_axis_cal[2] /= 500;
    acc_axis_cal[2] -= 4096;
  }
  else
  {
    digitalWrite(IND_LED_RED, HIGH);
    acc_axis_cal[0] = 0;
    acc_axis_cal[1] = 0;
    acc_axis_cal[2] = 0;
  }

  if (gyro_calibration_enabled)
  {
    for (int i = 0; i < 500; i++) {                        //Gyro static bias filter.
      if (i % 10 == 0) {
        digitalWrite(IND_LED_RED, !digitalRead(IND_LED_RED));
        Serial.println("Calibrating Gyroscope..");
      }
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
      //acc_axis[0] -= acc_axis_cal[0];
      //acc_axis[1] -= acc_axis_cal[1];
      //acc_axis[2] -= acc_axis_cal[2];
      //acc_total_vector = sqrt((acc_axis[0] * acc_axis[0]) + (acc_axis[1] * acc_axis[1]) + (acc_axis[2] * acc_axis[2]));    //Calculate the total accelerometer vector
      //if ((acc_total_vector >= 4080) && (acc_total_vector <= 4112))       //Condition to ensure that sensor is stationary on initialization
      //{
        gyro_axis_cal[0] += gyro_axis[0];
        gyro_axis_cal[1] += gyro_axis[1];
        gyro_axis_cal[2] += gyro_axis[2];
      //}
      //else
      //{
      //  i--;
      //  if (i <= 0) i = 0;
      //}
      delay(2);
    }
    digitalWrite(IND_LED_RED, HIGH);
    gyro_axis_cal[0] /= 500;
    gyro_axis_cal[1] /= 500;
    gyro_axis_cal[2] /= 500;
  }
  else
  {
    digitalWrite(IND_LED_RED, HIGH);
    gyro_axis_cal[0] = -326;
    gyro_axis_cal[1] = -33;
    gyro_axis_cal[2] = -254;
  }
  digitalWrite(IND_LED_RED, LOW);
  delay(2000);
  for (int i = 0; i < 15; i++)
  {
    mpu6050_read();
    //if ((acc_total_vector >= 4090) && (acc_total_vector <= 4102)) //Condition to ensure that sensor is stationary on initialization
    //{
      roll_angle_acc_startup += roll_angle_acc;
      pitch_angle_acc_startup += pitch_angle_acc;
    //}
    //else
    //{
    //  i--;
    //  if (i <= 0) i = 0;
    //}
  }
  roll_angle = (roll_angle_acc_startup / 15);
  pitch_angle = (pitch_angle_acc_startup / 15);
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
  //acc_axis[0] -= acc_axis_cal[0];
  //acc_axis[1] -= acc_axis_cal[1];
  //acc_axis[2] -= acc_axis_cal[2];
  gyro_axis[0] -= gyro_axis_cal[0];
  gyro_axis[1] -= gyro_axis_cal[1];
  gyro_axis[2] -= gyro_axis_cal[2];
  //acc_axis[0] *= -1;                                  //Invert the direction of the acc roll axis ref. to the drone geometry
  acc_axis[1] *= -1;                                    //Invert the direction of the acc pitch axis ref. to the drone geometry
  //acc_axis[2] *= -1;                                  //Invert the direction of the acc Z axis ref. to the drone geometry
  //gyro_axis[0] *= -1;                                 //Invert the direction of the gyro pitch axis ref. to the drone geometry
  gyro_axis[1] *= -1;                                   //Invert the direction of the gyro roll axis ref. to the drone geometry
  //gyro_axis[2] *= -1;                                 //Invert the direction of the gyro yaw axis ref. to the drone geometry
  //mpu6050_temperature = ((float)mpu6050_temperature_raw / 340) + 36.53;

  //>>>>>>>>>>>  GYRO ANGLE CALC  <<<<<<<<<<<

  roll_angle += (float)gyro_axis[1] / 16375;             //Y-axis  ROLL // 16375 =>(250 * 65.5)
  pitch_angle += (float)gyro_axis[0] / 16375;            //X-axis  PITCH
  yaw_angle += (float)gyro_axis[2] / 16375;              //Z-axis  YAW
  pitch_angle -= roll_angle * sin(gyro_axis[2] * 0.000001066);  //If the IMU has yawed transfer the roll angle to the pitch angel.
  roll_angle += pitch_angle * sin(gyro_axis[2] * 0.000001066);  //If the IMU has yawed transfer the pitch angle to the roll angel.

  //>>>>>>>>>>>  ACCL ANGLE CALC  <<<<<<<<<<<

  acc_total_vector = sqrt((acc_axis[0] * acc_axis[0]) + (acc_axis[1] * acc_axis[1]) + (acc_axis[2] * acc_axis[2]));    //Calculate the total accelerometer vector
  if (abs(acc_axis[0]) < acc_total_vector) roll_angle_acc = asin((float)acc_axis[0] / acc_total_vector) * 57.296;
  if (abs(acc_axis[1]) < acc_total_vector) pitch_angle_acc = asin((float)acc_axis[1] / acc_total_vector) * -57.296;
  roll_angle_acc -= 1.25;                                       //Sensor to frame mounting offset
  pitch_angle_acc += 0.91;

  //>>>>>>>>>>>  FINAL ANGLE CALC  <<<<<<<<<<

  roll_angle = (0.9994 * roll_angle) + (0.0006 * roll_angle_acc);
  pitch_angle = (0.9994 * pitch_angle) + (0.0006 * pitch_angle_acc);
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
    while (1) delay(10);
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
  /*Serial.print("Acc_cal X = ");
    Serial.print(acc_axis_cal[0]);
    Serial.print("    Acc_cal Y = ");
    Serial.print(acc_axis_cal[1]);
    Serial.print("    Acc_cal Z = ");
    Serial.println(acc_axis_cal[2]);*/
  /*Serial.print("Gyro_cal X = ");
    Serial.print(gyro_axis_cal[0]);
    Serial.print("    Gyro_cal Y = ");
    Serial.print(gyro_axis_cal[1]);
    Serial.print("    Gyro_cal Z = ");
    Serial.println(gyro_axis_cal[2]);*/
  /*Serial.print("roll_angle_acc = ");
    Serial.print(roll_angle_acc);
    Serial.print("    pitch_angle_acc = ");
    Serial.print(pitch_angle_acc);*/
  //Serial.print("roll_angle = ");
  Serial.print(roll_angle);
  //Serial.print("    pitch_angle = ");
  Serial.print(pitch_angle);
  //Serial.print("    yaw_angle = ");
  Serial.println(yaw_angle);
}
