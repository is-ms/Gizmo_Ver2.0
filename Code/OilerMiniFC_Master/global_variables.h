/*

*/

float VBatt;
//Global variables for "Main"
bool start = 0;
int32_t loop_timer = 0;
bool previous_state = 0;
uint16_t loop_delay_counter = 0;
bool loop_delay_return = 0;
uint16_t disconnected_rc_loop_counter = 0;
uint8_t xx = 0;

//Global variables for "Receiver_Interface"
/*
   BIT 0 >  Trottle   UP
   BIT 1 >  Throttle  DOWN
   BIT 2 >  Yaw       RIGHT
   BIT 3 >  Yaw       LEFT
   BIT 4 >  Pitch     FORWARD
   BIT 5 >  Pitch     BACKWARD
   BIT 6 >  Roll      RIGHT
   BIT 7 >  Roll      LEFT
*/
RF24 radio(A1, A0);                                                    //CE, CSN
const byte rxAddr[6] = "00001";
uint8_t packet = B11111111;
bool RC_connected = 0;
bool RC_armed = 0;
uint16_t RC_arm_counter = 0;
int16_t throttle, pitch, roll, yaw;
uint8_t throttle_loop_counter = 0;

//Global variables for"BME280_Interface"
int16_t bmp280_ID = 0;
uint16_t dig_T1;                                                        //Factory calibration parameters
int16_t dig_T2, dig_T3;
int32_t bmp280_temperature_raw;
int32_t bmp280_temperature;
uint16_t dig_P1;
int16_t dig_P2, dig_P3, dig_P4, dig_P5, dig_P6, dig_P7, dig_P8, dig_P9;
int32_t bmp280_pressure_raw;
int64_t bmp280_pressure_calibrated;
float bmp280_pressure, bmp280_ground_altitude, bmp280_altitude;

//Global variables for "MPU6050_Interface"
int32_t acc_axis[3];
int16_t mpu6050_temperature_raw;
int16_t gyro_axis[3];
float mpu6050_temperature;
bool gyro_calibration_enabled = true;
int32_t gyro_axis_cal[3];
int32_t acc_total_vector;
int16_t counter = 0;
float gyro_roll, gyro_pitch, gyro_yaw;
float roll_angle, pitch_angle, yaw_angle;
float roll_angle_acc, pitch_angle_acc, yaw_angle_acc, roll_angle_acc_startup, pitch_angle_acc_startup;

//Global variables for "Motors_Interface"
int16_t M1_output = 0;
int16_t M2_output = 0;
int16_t M3_output = 0;
int16_t M4_output = 0;
int32_t pulse_rise, pulse_fall;

//Global variables for "PWR_LEDs"
uint16_t PWR_LED_RED_output = 0;
uint16_t PWR_LED_GRN_output = 0;
uint16_t PWR_LED_BLU_output = 0;
uint16_t PWR_LEDs_counter = 0;
uint16_t PWR_LEDs_local_off_delay, PWR_LEDs_local_on_delay;

//Global variables for "PID"
//altitude_PID
bool altitude_PID_enabled = false;
int8_t rc_throttle_input = 0;
float altitude_setpoint, altitude_error, altitude_i_error, altitude_last_error;
float altitude_pid_p_gain = 2;
float altitude_pid_i_gain = 0.1;
float altitude_pid_d_gain = 0;
int16_t altitude_pid_output = 0;

//pitch_PID
int8_t rc_pitch_input = 0;
float pitch_setpoint, pitch_error, pitch_i_error, pitch_last_error;
float pitch_pid_p_gain = 1;
float pitch_pid_i_gain = 0.0;
float pitch_pid_d_gain = 0;
int16_t pitch_pid_output = 0;

//roll_PID
int8_t rc_roll_input = 0;
float roll_setpoint, roll_error, roll_i_error, roll_last_error;
float roll_pid_p_gain = 1;
float roll_pid_i_gain = 0.0;
float roll_pid_d_gain = 0;
int16_t roll_pid_output = 0;

//yaw_PID
int8_t rc_yaw_input = 0;
float yaw_setpoint, yaw_error, yaw_i_error, yaw_last_error;
float yaw_pid_p_gain = 1;
float yaw_pid_i_gain = 0.0;
float yaw_pid_d_gain = 0;
int16_t yaw_pid_output = 0;
