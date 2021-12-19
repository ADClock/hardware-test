#include "MotorManager.h"
MotorManager::MotorManager(Motor &motor1, Motor &motor2, Calibration &calibration1, Calibration &calibration2) : motor1(motor1), motor2(motor2), calibration1(calibration1), calibration2(calibration2)
{
}

/**
 * @brief Calibrates both motors.
 * 
 * @return true, when calibration for both motors was successfull.
 * @return false, otherwise
 */
bool MotorManager::calibrate() const
{
  calibration1.start_calibration();
  calibration2.start_calibration();
  bool motor1_calibrated = false;
  bool motor2_calibrated = false;
  size_t stepcounter = 0;
  do
  {
    motor1_calibrated = calibration1.calibrate();
    motor2_calibrated = calibration2.calibrate();
    stepcounter++;
    delay(5);

    if (stepcounter > STEPS_PER_ROUND * MAX_ROUNDS_FOR_CALIBRATION)
    {
#ifdef DEBUG
      Serial.println("MoMa >> Calibration cancelled. Magnets not found within max rounds for calibration.");
#endif
      return false;
    }
  } while (!motor1_calibrated || !motor2_calibrated);

  return true;
}

/**
 * @brief Try next step for both motors.
 * 
 */
void MotorManager::try_step() const
{
  this->motor1.try_step();
  this->motor2.try_step();
}

/**
 * @brief Checks if both motors reached their target position.
 * 
 * @return true, if both motors are done.
 * @return false, otherwise
 */
bool MotorManager::is_finished() const
{
  return this->motor1.is_finished() && this->motor2.is_finished();
}