#ifndef MODULES_SHUTTERS_H
#define MODULES_SHUTTERS_H

#include "../definitions.h"
#include "motor.h"

#define CMD_SHUTTERS_UP      "up"
#define CMD_SHUTTERS_DOWN    "down"
#define CMD_SHUTTERS_HOLD    "hold"
#define CMD_SHUTTERS_OPEN    "open"
#define CMD_SHUTTERS_CLOSE   "close"
#define CMD_SHUTTERS_POS     "position"
#define CMD_SHUTTERS_RESET   "reset"

class Shutters : public Module {
public:
  Shutters();
  bool fire(Command cmd) override;
  bool setup_motor(const char* name, int up, int down, int height, int close_time);
  bool up(Arguments args);
  bool down(Arguments args);
  bool hold();
  bool open();
  bool close();
  bool reset();
  bool position(Arguments args);
  std::vector<Motor*> get_motors();
  // TODO implement a method to sync multiple motors to either the lowest/highest
  //      position or somewhere in between

private:
  std::vector<Motor*> motors_;
  int                 state_;
};

#endif
