#include "motor.h"

Motor::Motor(const char* name, int up, int down, int height, int close_time){
  name_ = name;
  height_ = height;
  close_time_ = close_time * 1000;
  pin_up_ = up;
  pin_down_ = down;
  state_ = STATE_STOPPED;
  virtual_time_ = 0;

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
  else if(strcmp(cmd.codeword, CMD_MOTOR_POS) == 0) success = get_position();
  else if(strcmp(cmd.codeword, CMD_MOTOR_RESET) == 0) success = reset();
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
  //TODO compensate the slower motor acceleration when moving upwards
  if(state_ == STATE_STOPPED){
    state_ = STATE_UP;
    last_time_ = millis();
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
    last_time_ = millis();
    digitalWrite(pin_up_, RELAY_OFF);
    digitalWrite(pin_down_, RELAY_ON);
    if(duration > 0) cmd->delay(std::bind(&Motor::hold, this), duration);
    return true;
  } else return false;
}

bool Motor::hold(){
  if(state_ != STATE_STOPPED) update_time();
  state_ = STATE_STOPPED;
  digitalWrite(pin_up_, RELAY_OFF);
  digitalWrite(pin_down_, RELAY_OFF);
  return true;
}

void Motor::update_time(){
  // Keeps track of the time that would have elapsed to get into the current
  // position by adding up all "down" movements and substracting "up" movements
  unsigned long now = millis();
  unsigned long elapsed = now - last_time_;
  if(state_ == STATE_UP){
    if(virtual_time_ < elapsed) virtual_time_ = 0;
    else virtual_time_ -= elapsed;
  }
  else if(state_ == STATE_DOWN){
    virtual_time_ += elapsed;
    if(virtual_time_ > close_time_) virtual_time_ = close_time_;
  }
  last_time_ = now;
}

bool Motor::open(){
  return up(virtual_time_);
}

bool Motor::close(){
  return down(close_time_ - virtual_time_);
}

bool Motor::reset(){
  return up(close_time_);
}

bool Motor::get_position(){
  //TODO: output the actual position
  update_time();
  Serial.print("   ");
  Serial.print(name_);
  Serial.print(":\t");
  Serial.println(virtual_time_, DEC);
  return true;
}

char* Motor::get_id(){
  return id_;
}
