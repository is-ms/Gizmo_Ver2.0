/*

*/

#define RAD_TO_DEG  57.2957795131
#define DEG_TO_RAD  0.01745329252
#define g_acc       9.80665

float VBatt;
//Global variables for "Main"
bool start = 0;
uint32_t loop_timer = 0;
bool previous_state = 0;
uint8_t loop_delay_counter = 0;
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

//Global variables for"VL53L0X_Interface"
uint16_t VL53L0X_altitude = 0;
uint8_t VHV_CONFIG_PAD_SCL_SDA__EXTSUP_HV = 0;
uint8_t SYSTEM_INTERRUPT_CONFIG_GPIO = 0;
uint8_t VL53L0X_stop_variable = 0;
uint8_t VL53L0X_int = 0x01;
uint8_t VL53L0X_test_value = 0;
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
int16_t gyro_axis_dps[3];
float mpu6050_temperature;
bool gyro_calibration_enabled = true;
float acc_scale_corr[3][3] = {{0.993368, 0.000021, -0.002501}, {0.000021, 0.993433, 0.000434}, { -0.002501, 0.000434, 0.986603}};
float acc_bias_corr[3] = {94.623890, -64.502734, -42.677866};
float acc_axis_calibrated[3];
float acc_axis_lpf[3];
int32_t gyro_axis_cal[3];
float gyro_axis_lpf[3];
int32_t acc_total_vector;
int16_t counter = 0;
float gyro_roll, gyro_pitch, gyro_yaw;
float roll_angle, pitch_angle, yaw_angle;
float roll_angle_acc, pitch_angle_acc, yaw_angle_acc, roll_angle_acc_startup, pitch_angle_acc_startup;
float roll_angle_acc_trim = -1.688;
float pitch_angle_acc_trim = 0.966;

//Global variables for "IMU"
float acc_axis_rotated[3];

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
uint8_t rc_throttle_input_counter = 0;
float altitude_setpoint, altitude_error, altitude_i_error, altitude_last_error;
float altitude_pid_p_gain = 2;
float altitude_pid_i_gain = 0.1;
float altitude_pid_d_gain = 0;
uint8_t altitude_pid_output = 0;

//pitch_PID
int8_t rc_pitch_input = 0;
float pitch_setpoint, pitch_error, pitch_i_error, pitch_last_error;
float pitch_pid_p_gain = 0.42;
float pitch_pid_i_gain = 0.001;
float pitch_pid_d_gain = 2.2;
int16_t pitch_pid_output = 0;

//roll_PID
int8_t rc_roll_input = 0;
float roll_setpoint, roll_error, roll_i_error, roll_last_error;
float roll_pid_p_gain = 0.42;
float roll_pid_i_gain = 0.001;
float roll_pid_d_gain = 2.2;
int16_t roll_pid_output = 0;

//yaw_PID
int8_t rc_yaw_input = 0;
float yaw_setpoint, yaw_error, yaw_i_error, yaw_last_error;
float yaw_pid_p_gain = 0.4;
float yaw_pid_i_gain = 0.001;
float yaw_pid_d_gain = 8.2;
int16_t yaw_pid_output = 0;
