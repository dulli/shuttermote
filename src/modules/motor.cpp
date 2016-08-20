#include "motor.h"

Motor::Motor(const char* name, int up, int down, int height, int close_time, int open_time){
  name_ = name;
  height_ = height;
  close_time_ = close_time * 1000;
  open_time_ = open_time * 1000;
  pin_up_ = up;
  pin_down_ = down;
  state_ = STATE_STOPPED;

  const char* prefix = "motor-";
  char* id = (char*) calloc(1, strlen(prefix) + strlen(name) + 1);
  for(int i=0; prefix[i]; i++) id[i] = prefix[i];
  for(int i=0; name[i]; i++) id[i + strlen(prefix)] = tolower(name[i]);
  id_ = id;

  pinMode(pin_up_, OUTPUT);
  pinMode(pin_down_, OUTPUT);
  digitalWrite(pin_up_, RELAY_OFF);
  digitalWrite(pin_down_, RELAY_OFF);
}

Motor::~Motor(){
  free(id_);
}

bool Motor::fire(Command cmd){
  bool success = true;
  if(!cmd.codeword) success = false;
  else if(strcmp(cmd.codeword, CMD_MOTOR_HOLD) == 0) success = hold();
  else if(strcmp(cmd.codeword, CMD_MOTOR_UP) == 0) success = up(cmd.args);
  else if(strcmp(cmd.codeword, CMD_MOTOR_DOWN) == 0) success = down(cmd.args);
  else if(strcmp(cmd.codeword, CMD_MOTOR_OPEN) == 0) success = open();
  else if(strcmp(cmd.codeword, CMD_MOTOR_CLOSE) == 0) success = close();
  else success = false;
  return success;
}

bool Motor::up(Arguments args){
  // TODO check wether the argument can be parsed to a long
  unsigned long duration;
  if(args.empty()) duration = 0;
  else duration = strtoul(args[0], NULL, 10) * 1000; // in s
  return up(duration);
}

bool Motor::up(unsigned long duration){
  if(state_ == STATE_STOPPED){
    state_ = STATE_UP;
    digitalWrite(pin_down_, RELAY_OFF);
    digitalWrite(pin_up_, RELAY_ON);
    if(duration > 0) cmd->delay(std::bind(&Motor::hold, this), duration);
    return true;
  } else return false;
}

bool Motor::down(Arguments args){
  // TODO check wether the argument can be parsed to a long
  unsigned long duration;
  if(args.empty()) duration = 0;
  else duration = strtoul(args[0], NULL, 10) * 1000; // in s
  return down(duration);
}

bool Motor::down(unsigned long duration){
  if(state_ == STATE_STOPPED){
    state_ = STATE_DOWN;
    digitalWrite(pin_up_, RELAY_OFF);
    digitalWrite(pin_down_, RELAY_ON);
    if(duration > 0) cmd->delay(std::bind(&Motor::hold, this), duration);
    return true;
  } else return false;
}

bool Motor::hold(){
  state_ = STATE_STOPPED;
  digitalWrite(pin_up_, RELAY_OFF);
  digitalWrite(pin_down_, RELAY_OFF);
  return true;
}

bool Motor::open(){
  return up(open_time_);
}

bool Motor::close(){
  return down(close_time_);
}

char* Motor::get_id(){
  return id_;
}
