#include "Arduino.h"
#include "Config.h"

#ifndef _MOTOR_H_
#define _MOTOR_H_

class Motor
{
public:
  Motor(uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t pin4);
  void step();
  void step_forward();
  void step_backward();
  void set_target_pos(size_t pos);
  void set_step_delay(uint8_t delay);
  void set_wait_steps(size_t waitSteps);
  void set_direction(bool direction);
  void try_step();
  void reset();
  bool is_finished() const;

private:
  void write_step();
  void all_pins_off();

  const uint8_t pin1;
  const uint8_t pin2;
  const uint8_t pin3;
  const uint8_t pin4;

  size_t current_pos;
  size_t target_pos;
  size_t wait_steps;
  bool direction;
  size_t step_delay;

  unsigned long last_step;
  size_t coil_state;
};

#endif