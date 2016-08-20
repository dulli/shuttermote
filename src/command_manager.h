#ifndef _COMMAND_MANAGER_H
#define _COMMAND_MANAGER_H

#include "definitions.h"

class CommandManager : public Manager{
public:
  CommandManager();
  bool         setup_module(const char* key, Module* mod);
  bool         setup_tickrate(unsigned long tickrate);
  bool         fire_scheduled();
  bool         tick();
  unsigned int add_scheduled(Callback func, unsigned long delay, int times);
  unsigned int delay(Callback func, unsigned long delay);
  unsigned int interval(Callback func, unsigned long period);
  void         refresh_scheduled();
  bool         stop_scheduled(unsigned int id);
  Command parse(char* buffer);
  Module* get_module(Command cmd);

private:
  unsigned long                            prev_tick_;
  unsigned long                            ms_per_tick_;
  unsigned int                             scheduled_id_;
  std::vector<Schedule>                    to_schedule_;
  std::vector<Schedule>                    scheduled_;
  std::map<const char*, Module*, Comparer> modules_;
};

#endif
