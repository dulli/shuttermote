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
  bool get_position();
  std::vector<Motor*> get_motors();

private:
  std::vector<Motor*> motors_;
  int                 state_;
};

#endif
