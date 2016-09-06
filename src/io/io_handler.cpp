#include "io_handler.h"

IOHandler::IOHandler(int buffer_size){
  buffer_size_ = buffer_size;
  led_ = NULL;
  serial_ = NULL;
}

bool IOHandler::setup_wifi(IP ip, IP gateway, IP subnet, unsigned int port){
  wifi_ = new WifiIO(ip, gateway, subnet, port, buffer_size_);
  return true;
}

bool IOHandler::setup_led(int pin){
  led_ = new LedO(pin);
  return true;
}

bool IOHandler::setup_rotary(int pin_up, int pin_down, int pin_out, int interval){
  rotary_ = new RotaryI(pin_up, pin_down, pin_out, interval);
  return true;
}

bool IOHandler::setup_serial(int baud){
  serial_ = new SerialIO(baud, buffer_size_);
  return true;
}

bool IOHandler::add_commander(CommandManager* cmd){
  if(led_)    led_->cmd = cmd;
  if(serial_) serial_->cmd = cmd;
  if(wifi_)   wifi_->cmd = cmd;
  if(rotary_) rotary_->cmd = cmd;
  return true;
}

bool IOHandler::get(){
  if(rotary_->get())      buffer_ = rotary_->read();
  else if(serial_->get()) buffer_ = serial_->read();
  else if(wifi_->get())   buffer_ = wifi_->read();
  else return false;
  return true;
}

char* IOHandler::read(){
  char* input = (char*) malloc(strlen(buffer_) + 1);
  strcpy(input, buffer_);
  return input;
}

void IOHandler::clean_up(char* input){
  free(input);
  if(serial_) serial_->restart();
}

void IOHandler::log(const char* format, ...){
  char line[MAX_LOG_LENGTH];

  va_list args;
  va_start(args, format);
  vsnprintf(line, MAX_LOG_LENGTH, format, args);
  va_end(args);

  if(serial_) serial_->log(line);
}

void IOHandler::heartbeat(unsigned long elapsed){
  if(led_) led_->heartbeat(elapsed);
}
