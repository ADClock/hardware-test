#include "Calibration.h"

Calibration::Calibration(Motor &m, size_t hall_pin) : motor(m), hall_pin(hall_pin)
{
  pinMode(hall_pin, INPUT_PULLUP);
}

/**
 * @brief Starts a new calibration.
 * 
 */
void Calibration::start_calibration()
{
  this->steps = 0;
  this->state = FINDMAGNET;
#ifdef DEBUG
  Serial.println("Calibration " + String(hall_pin) + " >> Start calibration ...");
#endif
}

/**
 * @brief Calibrates the motor/hand to 12:00 o'clock.
 * 
 * Calibration steps:
 * 1. Rotate motor forward until magnet is inside hall sensor field. (FINDMAGNET)
 *    Count steps made until field is found.
 * 2. Check if motor was moved at least MIN_STEPS_OUTSIDE_FIELD steps. This is required for correct centering action.
 *    If not:
 *      2.1 Rotate motor backwards and force magnet to leave hall sensor field. (LEAVEMAGNET)
 *      2.2 Rotate motor foreward until magnet is inside hall sensor field. (FINDMAGNET)
 * 3. Rotate motor forward until magnet leaves hall sensor field. (INFIELD)
 *    Count steps made inside magnet field.
 * 4. Rotate motor backwards by steps_inside_field/2 steps. This should center the hand perfectly on 12:00 o'clock. (CENTERING)
 * 
 * @return true, if calibration was successfull
 * @return false, otherwise
 */
bool Calibration::calibrate()
{
#ifdef SKIPCALIBRATION
#ifdef DEBUG
  Serial.println("Calibration " + String(hall_pin) + " >> Calibration skipped because of config 'SKIPCALIBRATION'.");
#endif
  return true;
#endif

  if (this->state == CALIBRATED)
    return true;

  bool in_field = is_in_field();

  switch (this->state)
  {
  case FINDMAGNET:
    if (in_field && this->steps <= MIN_STEPS_OUTSIDE_FIELD)
    {
      this->steps = MIN_STEPS_OUTSIDE_FIELD + 1;
      this->state = LEAVEMAGNET;
#ifdef DEBUG
      Serial.println("Calibration " + String(hall_pin) + " >> Magnet found. Motor moved less then " + String(MIN_STEPS_OUTSIDE_FIELD) + " steps. Rotate backwards and leave magnet field...");
#endif
    }
    else if (in_field && this->steps > MIN_STEPS_OUTSIDE_FIELD)
    {
      this->steps = 0;
      this->state = INFIELD;
#ifdef DEBUG
      Serial.println("Calibration " + String(hall_pin) + " >> Magnet found in more then " + String(MIN_STEPS_OUTSIDE_FIELD) + " steps. Crossing magnet field...");
#endif
    }
    else
    {
      this->motor.step_forward();
      this->steps++;
    }
    break;

  case LEAVEMAGNET:
    if (this->steps == 0)
    {
      this->state = FINDMAGNET;
#ifdef DEBUG
      Serial.println("Calibration " + String(hall_pin) + " >> Motor moved enough backwards. Search for magnet again...");
#endif
    }
    else
    {
      this->motor.step_backward();
      this->steps--;
    }
    break;

  case INFIELD:
    if (!in_field)
    {
      this->state = CENTERING;
      this->steps = this->steps / 2;
#ifdef DEBUG
      Serial.println("Calibration " + String(hall_pin) + " >> Magnet field crossed. Centering...");
#endif
    }
    else
    {
      this->motor.step_forward();
      this->steps++;
    }
    break;

  case CENTERING:
    if (this->steps == 0)
    {
      this->state = CALIBRATED;
      this->motor.reset();

#ifdef DEBUG
      Serial.println("Calibration " + String(hall_pin) + " >> Motor/Hand centered. Calibration complete.");
#endif
    }
    else
    {
      this->steps--;
      this->motor.step_backward();
    }
    break;

  default:
    break;
  }

  return false;
}

/**
 * @brief Checks if the hall sensor detects a magnet.
 * 
 * @return true, if hall sensor detects magnet.
 * @return false, if hall sensor detects nothing.
 */
bool Calibration::is_in_field()
{
  return analogRead(this->hall_pin) < 100;
}