#include "shutters.h"

Shutters::Shutters(){
  state_ = -1;
}

bool Shutters::fire(Command cmd){
  bool success = true;
  if(!cmd.codeword) success = false;
  else if(strcmp(cmd.codeword, CMD_SHUTTERS_HOLD) == 0) success = hold();
  else if(strcmp(cmd.codeword, CMD_SHUTTERS_UP) == 0) success = up(cmd.args);
  else if(strcmp(cmd.codeword, CMD_SHUTTERS_DOWN) == 0) success = down(cmd.args);
  else if(strcmp(cmd.codeword, CMD_SHUTTERS_OPEN) == 0) success = open();
  else if(strcmp(cmd.codeword, CMD_SHUTTERS_CLOSE) == 0) success = close();
  else if(strcmp(cmd.codeword, CMD_SHUTTERS_POS) == 0) success = get_position();
  else if(strcmp(cmd.codeword, CMD_SHUTTERS_RESET) == 0) success = reset();
  else success = false;
  return success;
}

bool Shutters::setup_motor(const char* name, int up, int down, int height, int close_time){
  Motor* motor = new Motor(name, up, down, height, close_time);
  motors_.push_back(motor);
  return true;
}

bool Shutters::up(Arguments args){
  bool success = true;
  for(Motor* motor : motors_) success &= motor->up(args);
  return success;
}

bool Shutters::down(Arguments args){
  bool success = true;
  for(Motor* motor : motors_) success &=  motor->down(args);
  return success;
}

bool Shutters::hold(){
  bool success = true;
  for(Motor* motor : motors_) success &=  motor->hold();
  return success;
}

bool Shutters::open(){
  bool success = true;
  for(Motor* motor : motors_) success &=  motor->open();
  return success;
}

bool Shutters::close(){
  bool success = true;
  for(Motor* motor : motors_) success &=  motor->close();
  return success;
}

bool Shutters::reset(){
  bool success = true;
  for(Motor* motor : motors_) success &=  motor->reset();
  return success;
}

bool Shutters::get_position(){
  bool success = true;
  for(Motor* motor : motors_) success &=  motor->get_position();
  return success;
}

std::vector<Motor*> Shutters::get_motors(){
  return motors_;
}
