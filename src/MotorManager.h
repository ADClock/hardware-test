
#ifndef _MOTORMANAGER_H_
#define _MOTORMANAGER_H_
#include "Arduino.h"
#include "Calibration.h"

struct MotorData
{
  uint16_t position;
  uint16_t waitSteps;
  int8_t delay;
  bool direction;
};

class MotorManager
{
public:
  MotorManager(Motor &motor1, Motor &motor2, Calibration &calibration1, Calibration &calibration2);

  bool calibrate() const;
  void try_step() const;
  bool is_finished() const;

private:
  Motor &motor1;
  Motor &motor2;
  Calibration &calibration1;
  Calibration &calibration2;
};
#endif
