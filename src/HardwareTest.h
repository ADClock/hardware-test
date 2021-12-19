#ifndef _HARDWARE_TEST_H_
#define _HARDWARE_TEST_H_
#include "Arduino.h"
#include "MotorManager.h"

class HardwareTest
{
public:
  HardwareTest(MotorManager &moma, Motor &motor1, Motor &motor2, Calibration &calibration1, Calibration &calibration2);
  void test_calibration() const;
  void test_single_rotation() const;
  void test_magnet_field() const;
  void test_short_animation() const;

private:
  void run_until_finished_or_timeout() const;
  void prepare_test(String name) const;
  MotorManager &moma;
  Motor &motor1;
  Motor &motor2;
  Calibration &calibration1;
  Calibration &calibration2;
};

#endif