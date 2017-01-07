#ifndef _DEFINITIONS_H
#define _DEFINITIONS_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#undef min  // Workaround needed to include Arduino and STL headers in the same
#undef max  // file, see: https://github.com/platformio/platformio/issues/646#issuecomment-216244875

#include <functional>
#include <map>
#include <vector>
#include <string.h>

using namespace std::placeholders;
typedef std::function<void(int a)> Callback;
typedef std::function<bool(Callback, unsigned long, int)> Scheduler;
typedef std::vector<char*> Arguments;
typedef IPAddress IP;

// Platform.io displays errors when the pins are not manually defined:
namespace Pin{
  static const uint8_t BUILTIN_LED = 2;

  static const uint8_t D0   = 16;
  static const uint8_t D1   = 5;
  static const uint8_t D2   = 4;
  static const uint8_t D3   = 0;
  static const uint8_t D4   = 2;
  static const uint8_t D5   = 14;
  static const uint8_t D6   = 12;
  static const uint8_t D7   = 13;
  static const uint8_t D8   = 15;
  static const uint8_t RX   = 3;
  static const uint8_t TX   = 1;
}

struct Command{
  char*  target;
  char*  codeword;
  Arguments args;
};

struct Schedule{
  Callback      func;
  unsigned long delay;
  unsigned long left;
  int           times;
  unsigned int  id;
};

struct Comparer{
  bool operator()(char const* a, char const* b){ return strcmp(a,b) < 0; }
};

class Manager{
public:
  Manager(){};
  virtual unsigned int add_scheduled(Callback func, unsigned long delay, int times) = 0;
  virtual unsigned int delay(Callback func, unsigned long delay) = 0;
  virtual unsigned int interval(Callback func, unsigned long period) = 0;
  virtual bool stop_scheduled(unsigned int id) = 0;
};

class Module{
public:
  Module(){};
  virtual bool fire(Command cmd) = 0;
  Manager* cmd;
};

class IO{
public:
  IO(){};
  Manager* cmd;
};

#endif
