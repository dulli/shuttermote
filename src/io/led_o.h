#ifndef IO_LED_O_H
#define IO_LED_O_H

#include "../definitions.h"

class LedO : public IO{
public:
  const static int LED_ON  = HIGH;
  const static int LED_OFF = LOW;

  LedO(int pin);
  void on();
  void off();
  void flash(int mode, unsigned long duration);
  void heartbeat(unsigned long elapsed);

private:
  int pin_;
  int state_;
};

#endif
