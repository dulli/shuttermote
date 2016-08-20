#ifndef MODULES_MOTOR_H
#define MODULES_MOTOR_H

#include "../definitions.h"

#define CMD_MOTOR_UP      "up"
#define CMD_MOTOR_DOWN    "down"
#define CMD_MOTOR_HOLD    "hold"
#define CMD_MOTOR_OPEN    "open"
#define CMD_MOTOR_CLOSE   "close"

class Motor : public Module {
public:
  static const int RELAY_ON      = LOW;
  static const int RELAY_OFF     = HIGH;

  static const int STATE_UP      = 0x1;
  static const int STATE_DOWN    = 0x2;
  static const int STATE_STOPPED = 0x3;

  Motor(const char* name, int up, int down, int height, int close_time, int open_time);
  ~Motor();
  bool  fire(Command cmd) override;
  bool up(Arguments args);
  bool up(unsigned long duration);
  bool down(Arguments args);
  bool down(unsigned long duration);
  bool hold();
  bool open();
  bool close();
  char* get_id();

private:
  const char* name_;
  char*       id_;
  int         height_;
  int         close_time_;
  int         open_time_;
  int         pin_up_;
  int         pin_down_;
  int         state_;
};

#endif
