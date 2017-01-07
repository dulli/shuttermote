#include "command_manager.h"

CommandManager::CommandManager(){
  scheduled_id_ = 0;
}

Command CommandManager::parse(char* buffer){
  char* target = strtok(buffer, " ");
  char* cmd = strtok(NULL, " ");
  std::vector<char*> args;
  if(cmd) while(char* arg = strtok(NULL, " ")) args.push_back(arg);
  return Command{target, cmd, args};
}

Module* CommandManager::get_module(Command cmd){
  if(modules_.find(cmd.target) == modules_.end()) return NULL;
  else return modules_[cmd.target];
}

bool CommandManager::setup_module(const char* key, Module* mod){
  mod->cmd = this;
  modules_[key] = mod;
  return true;
}

bool CommandManager::setup_tickrate(unsigned long tickrate){
  prev_tick_ = millis();
  ms_per_tick_ = 1000 / tickrate;
  return true;
}

bool CommandManager::fire_scheduled(){
  unsigned long current_tick = millis();
  unsigned long elapsed = current_tick - prev_tick_;
  std::vector<Schedule> next_schedule;
  if(to_schedule_.size()) refresh_scheduled();
  for(Schedule cb : scheduled_){
    if(elapsed > cb.left) cb.left = 0;
    else cb.left -= elapsed;
    if(cb.left > 0){                // If the callback is not due yet re-add it to the schedule and move on
      next_schedule.push_back(cb);
      continue;
    }
    if(cb.times > 0) cb.times--;    // Decrement number of times the callback is fired again if its not periodical
    cb.func(cb.delay - cb.left);    // Fire the callback with the number of ms it has been delayed
    if(cb.times){                   // Check if the callback has to be fired again
      cb.left = cb.delay;           // Reset elapsed time
      next_schedule.push_back(cb);  // Re-add the callback to the schedule
    }
  }
  scheduled_ = next_schedule;
  prev_tick_ = current_tick;
  return true;
}

unsigned int CommandManager::add_scheduled(Callback func, unsigned long delay, int times){
  to_schedule_.push_back(Schedule{func, delay, delay, times, scheduled_id_});
  return scheduled_id_++;
}

unsigned int CommandManager::delay(Callback func, unsigned long delay){
  return add_scheduled(func, delay, 1);
}

unsigned int CommandManager::interval(Callback func, unsigned long period){
  return add_scheduled(func, period, -1);
}

void CommandManager::refresh_scheduled(){
  for(Schedule cb : to_schedule_) scheduled_.push_back(cb);
  to_schedule_.clear();
}

bool CommandManager::stop_scheduled(unsigned int id){
  Schedule to_remove;
  for(auto it = scheduled_.begin(); it != scheduled_.end(); ++it){
    if((*it).id != id) continue;
    scheduled_.erase(it);
    return true;
  }
  return false;
}

bool CommandManager::tick(){
  unsigned long now = millis();
  unsigned long elapsed = now - prev_tick_;
  return (elapsed >= ms_per_tick_);
}
