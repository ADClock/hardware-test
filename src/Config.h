#ifndef _CONFIG_H_
#define _CONFIG_H_

// Calibration
#define MIN_STEPS_OUTSIDE_FIELD 20
#define MAX_ROUNDS_FOR_CALIBRATION 5

// Motor
#define STEPS_PER_ROUND 1705
#define MIN_STEP_DELAY 4000 // us

// Pins
#define HALL_DATA_PIN_1 A1
#define HALL_DATA_PIN_2 A0

#define MOTOR_1_PIN_1 10
#define MOTOR_1_PIN_2 11
#define MOTOR_1_PIN_3 12
#define MOTOR_1_PIN_4 13

#define MOTOR_2_PIN_1 9
#define MOTOR_2_PIN_2 8
#define MOTOR_2_PIN_3 7
#define MOTOR_2_PIN_4 6

// Skip Calibration (for unfinished clock modules with no magnet)
// #define SKIPCALIBRATION
// Enable Debug messages for Serial
// #define DEBUG

#endif