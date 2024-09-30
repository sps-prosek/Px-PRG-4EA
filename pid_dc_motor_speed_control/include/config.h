#ifndef CONFIG_H
#define CONFIG_H

// Version
#define VERSION "0.0.1"

// Motor pins
#define MOTOR_PWM_PIN 13
#define MOTOR_DIR1_PIN 14
#define MOTOR_DIR2_PIN 15

// Encoder pins and resolution
#define ENCODER_A_PIN 10
#define ENCODER_B_PIN 11
#define ENCODER_RESOLUTION 150.0f

// PID constants
#define Kp 0.02f
#define Ki 0.055f
#define Kd 4.0f

// Macros for conversions and cropping
#define STEPS_TO_DEGREES(x) (float)x * 360.0f / ENCODER_RESOLUTION
#define STEPS_PER_SECOND_TO_RPM(x) (float)x * 60.0f / ENCODER_RESOLUTION

#endif // CONFIG_H