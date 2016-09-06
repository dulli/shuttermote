#ifndef IO_IO_HANDLER_H
#define IO_IO_HANDLER_H

#include "../definitions.h"
#include "../command_manager.h"
#include "wifi_io.h"
#include "serial_io.h"
#include "led_o.h"
#include "rotary_i.h"

class IOHandler{
public:
  static const int MAX_LOG_LENGTH = 256;

  IOHandler(int buffer_size);
  bool  setup_wifi(IP ip, IP gateway, IP subnet, unsigned int port);
  bool  setup_led(int pin);
  bool  setup_rotary(int pin_up, int pin_down, int pin_out, int interval);
  bool  setup_serial(int baud);
  bool  add_commander(CommandManager* cmd);
  bool  get();
  char* read();
  void  log(const char* fmt, ...);
  void  clean_up(char* input);
  void  heartbeat(unsigned long elapsed);
  WifiIO*   wifi_;
  SerialIO* serial_;
  LedO*     led_;
  RotaryI*  rotary_;

private:
  int       buffer_size_;
  char*     buffer_;
};

#endif
