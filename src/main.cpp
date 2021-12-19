#include <Arduino.h>
#include "Config.h"
#include "MotorManager.h"
#include "HardwareTest.h"
Motor motor1(MOTOR_1_PIN_1, MOTOR_1_PIN_2, MOTOR_1_PIN_3, MOTOR_1_PIN_4);
Motor motor2(MOTOR_2_PIN_1, MOTOR_2_PIN_2, MOTOR_2_PIN_3, MOTOR_2_PIN_4);

Calibration calibration1(motor1, HALL_DATA_PIN_1);
Calibration calibration2(motor2, HALL_DATA_PIN_2);

MotorManager moma(motor1, motor2, calibration1, calibration2);

HardwareTest test(moma, motor1, motor2, calibration1, calibration2);

bool debug_active = false;
#ifdef DEBUG
debug_active = true;
// Wir steppen im Kreis und zählen wie viele Schritte es von Hall High bis Hall High ist.
void rotateUntilTomorrow()
{
  bool state = false; // Ist der Pin High or Low?
  bool lastState = false;
  size_t steps = 0;
  while (1)
  {
    motor2.step();
    steps++;
    lastState = state;
    state = analogRead(HALL_DATA_PIN_2) < 100;
    if (lastState && !state)
    {
      Serial.println("main >> Round took " + String(steps) + " steps.");
      steps = 0;
    }
    delay(4);
  }
}

#endif

void setup()
{

  Serial.begin(115200);
  Serial.println("main >> Setup...");
  delay(1000);
  Serial.println("main >> Setup done! Debug mode: " + String(debug_active));
}

void loop()
{
#ifdef DEBUG
  Serial.println("main >> Looping...");
#endif
  test.test_calibration();
  test.test_single_rotation();
  test.test_short_animation();
  test.test_magnet_field();
  delay(10000);
}
