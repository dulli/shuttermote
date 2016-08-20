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

bool IOHandler::setup_rotary(int pin_up, int pin_down){
  return false;
}

bool IOHandler::setup_serial(int baud){
  serial_ = new SerialIO(baud, buffer_size_);
  return true;
}

bool IOHandler::add_commander(CommandManager* cmd){
  if(led_)    led_->cmd = cmd;
  if(serial_) serial_->cmd = cmd;
  return true;
}

bool IOHandler::get(){
  if(serial_->get())    buffer_ = serial_->read();
  else if(wifi_->get()) buffer_ = wifi_->read();
  else return false;
  return true;
}

char* IOHandler::read(){
  return buffer_;
}

void IOHandler::log(const char* format, ...){
  char line[MAX_LOG_LENGTH];

  va_list args;
  va_start(args, format);
  vsnprintf(line, MAX_LOG_LENGTH, format, args);
  va_end(args);

  if(serial_) serial_->log(line);
}

void IOHandler::restart(){
  if(serial_) serial_->restart();
}

void IOHandler::heartbeat(unsigned long elapsed){
  if(led_) led_->heartbeat(elapsed);
}
