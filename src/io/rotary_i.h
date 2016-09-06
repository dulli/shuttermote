#ifndef IO_ROTARY_I_H
#define IO_ROTARY_I_H

#include "../definitions.h"

class RotaryI : public IO{
public:
  const static int ACTIVE   = LOW;
  const static int INACTIVE = HIGH;

  RotaryI(int pin_up, int pin_down, int pin_ground, int interval);
  bool  get();
  char* read();

private:
  int interval_;
  int prev_tick_;
  int state_up_;
  int state_down_;
  int pin_in_up_;
  int pin_in_down_;
  int pin_out_;
  const char* buffer_;
};

#endif
