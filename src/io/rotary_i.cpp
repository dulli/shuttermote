#include "rotary_i.h"

RotaryI::RotaryI(int pin_up, int pin_down, int pin_out, int interval){
  pin_in_up_ = pin_up;
  digitalWrite(pin_in_up_, LOW);
  pinMode(pin_in_up_, INPUT_PULLUP);

  pin_in_down_ = pin_down;
  digitalWrite(pin_in_down_, LOW);
  pinMode(pin_in_down_, INPUT_PULLUP);

  // This pin is held constantly low and acts as another ground pin because I
  // didn't feel like soldering another wire to the actual ground pin
  pin_out_ = pin_out;
  pinMode(pin_out_, OUTPUT);
  digitalWrite(pin_out_, LOW);

  state_up_   = INACTIVE;
  state_down_ = INACTIVE;
  prev_tick_  = millis();
  interval_   = interval;
}

bool RotaryI::get(){
  // If the current switch status is read too often it can jump around a bit
  // while changing, throttling it to a saner rate prevents rapid changes
  // of the motor power and direction
  int now = millis();
  if(now - prev_tick_ < interval_) return false;
  prev_tick_ = now;

  // Remind the output pin that it should stay at its value, otherwise the switch
  // stops working after a few minutes
  digitalWrite(pin_out_, LOW);

  int down = digitalRead(pin_in_down_);
  int up   = digitalRead(pin_in_up_);
  bool rotary_changed = (state_down_ != down || state_up_ != up);
  state_up_ = up;
  state_down_ = down;
  if(rotary_changed){
    if(up == ACTIVE) buffer_ = "shutters up";
    else if(down == ACTIVE) buffer_ = "shutters down";
    else buffer_ = "shutters hold";
    return true;
  }
  return false;
}

char* RotaryI::read(){
  Serial.print(buffer_);
  Serial.println(" (ROTARY)");
  return (char*) buffer_;
}
