#include "HardwareTest.h"

HardwareTest::HardwareTest(MotorManager &moma, Motor &motor1, Motor &motor2, Calibration &calibration1, Calibration &calibration2)
    : moma(moma), motor1(motor1), motor2(motor2), calibration1(calibration1), calibration2(calibration2) {}

/**
 * @brief Tests the calibration for both motors.
 * After the calibration is completed for both motors the clock will sleep for 15 seconds allowing adjustment for minute and hour hand.
 * 
 */
void HardwareTest::test_calibration() const
{
#ifdef DEBUG
  Serial.println("HardwareTest >> Testing calibration...");
#endif
  moma.calibrate();
  delay(15000);
}

/**
 * @brief Tests one single rotation.
 * 
 * Motor1 is rotating one full round forwards
 * Motor2 is rotating one full round backwards
 * 
 */
void HardwareTest::test_single_rotation() const
{
  prepare_test("test_single_rotation");
  motor1.set_target_pos(STEPS_PER_ROUND - 1);
  motor1.set_direction(true); // rotate forwards

  motor2.set_target_pos(1);
  motor2.set_direction(false); // totate backwards

  run_until_finished_or_timeout();
}

/**
 * @brief Rotate both motors forward and slow them down when they are inside the magnet field.
 * This test runs for exactly 60 seconds.
 * 
 */
void HardwareTest::test_magnet_field() const
{
  prepare_test("test_magnet_field");
  unsigned long run_until = millis() + 60000;

  unsigned long last_step_motor1;
  unsigned long last_step_motor2;

  while (run_until > millis())
  {
    if ((millis() - last_step_motor1) > (calibration1.is_in_field() ? 20 : 5))
    {
      motor1.step_forward();
      last_step_motor1 = millis();
    }
    if ((millis() - last_step_motor2) > (calibration2.is_in_field() ? 20 : 5))
    {
      motor2.step_forward();
      last_step_motor2 = millis();
    }
  }
}

/**
 * @brief Testcase performing a simple animation to show off the functions of our clock.
 * 
 * Animation steps:
 * 1. Rotate motor1 90 degree forward and motor2 90 degree backward.
 * 2. Rotate motor2 90 degree forward, then rotate both motors 45 degree forward.
 * 3. Change position of motor1 and motor2 twice (creates waving effect)
 * 4. Rotate both hands to same position, but one hand with twice that speed of the other hand.
 * 5. Rotate both hands to the opposite site, but with different directions.
 * 
 */
void HardwareTest::test_short_animation() const
{
  prepare_test("test_short_animation");

  // image 1
  motor1.set_target_pos(STEPS_PER_ROUND / 4);     // 90 degree
  motor1.set_direction(true);                     // forward
  motor2.set_target_pos(STEPS_PER_ROUND / 4 * 3); // 270 degree
  motor2.set_direction(false);                    // backward
  run_until_finished_or_timeout();

  // image 2
  motor1.set_target_pos(STEPS_PER_ROUND / 8 * 3); // 135 degree
  motor1.set_wait_steps(STEPS_PER_ROUND / 4);     // wait 90 degree rotation before start
  motor2.set_target_pos(STEPS_PER_ROUND / 8);     // 45 degree
  motor2.set_direction(true);                     // forward
  run_until_finished_or_timeout();

  // image 3
  motor1.set_target_pos(STEPS_PER_ROUND / 8);     // 45 degree
  motor1.set_direction(false);                    // backward
  motor2.set_target_pos(STEPS_PER_ROUND / 8 * 3); // 135 degree
  motor2.set_direction(true);                     // forward
  run_until_finished_or_timeout();

  motor1.set_target_pos(STEPS_PER_ROUND / 8 * 3); // 135 degree
  motor1.set_direction(true);                     // forward
  motor2.set_target_pos(STEPS_PER_ROUND / 8);     // 45 degree
  motor2.set_direction(false);                    // backward
  run_until_finished_or_timeout();

  // image 4
  motor1.set_target_pos(STEPS_PER_ROUND / 8 * 7);       // 315 degree
  motor1.set_step_delay((uint8_t)(MIN_STEP_DELAY * 2)); // decrease speed by factor two
  motor2.set_target_pos(STEPS_PER_ROUND / 8 * 7);       // 315 degree
  motor2.set_direction(true);                           // forward
  run_until_finished_or_timeout();

  // image 5
  motor1.set_target_pos(STEPS_PER_ROUND / 8 * 3); // 135 degree
  motor1.set_direction(true);                     // forward
  motor1.set_step_delay((uint8_t)MIN_STEP_DELAY);
  motor2.set_target_pos(STEPS_PER_ROUND / 8 * 3); // 135 degree
  motor2.set_direction(false);                    // backward
  run_until_finished_or_timeout();
}

/**
 * @brief Try motor steps until the target position is reached or a timeout occurred.
 * 
 */
void HardwareTest::run_until_finished_or_timeout() const
{
  unsigned long run_until = millis() + 60000;
  while (!moma.is_finished() && run_until > millis())
    moma.try_step();

#ifdef DEBUG
  Serial.println("HardwareTest >> run_until_finished_or_timeout finished: " + String(moma.is_finished()));
#endif
}

/**
 * @brief Resets motors to defaults.
 * 
 * @param name Name of test case for debug messages
 */
void HardwareTest::prepare_test(String name) const
{
#ifdef DEBUG
  Serial.println("HardwareTest >> Preparing test case '" + name + "'.");
#endif
  motor1.reset();
  motor2.reset();
  run_until_finished_or_timeout();
  delay(2000); // wait two seconds before starting
}