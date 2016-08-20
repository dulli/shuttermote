#include "led_o.h"

LedO::LedO(int pin){
  pin_ = pin;
  pinMode(pin, OUTPUT);
  on();
}

void LedO::on(){
  state_ = LED_ON;
  digitalWrite(pin_, LED_ON);
}

void LedO::off(){
  state_ = LED_OFF;
  digitalWrite(pin_, LED_OFF);
}

void LedO::flash(int mode, unsigned long duration){
  if(mode == LED_ON){
    off();
    cmd->delay(std::bind(&LedO::on, this), duration);
  } else if(mode == LED_OFF){
    on();
    cmd->delay(std::bind(&LedO::off, this), duration);
  }
}

void LedO::heartbeat(unsigned long elapsed){
  flash(state_, elapsed * 0.1);
}
