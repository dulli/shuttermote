#include "definitions.h"
#include "command_manager.h"
#include "io/io_handler.h"
#include "modules/configurator.h"
#include "modules/shutters.h"

Shutters*       shutters;
IOHandler*      io;
CommandManager* commander;
Configurator*   configurator;

void setup(){
  static const bool WIFI_ENABLED   = true;
  static const bool LED_ENABLED    = true;
  static const bool ROTARY_ENABLED = true;
  static const bool SERIAL_ENABLED = true;

  shutters     = new Shutters();
  io           = new IOHandler(/*input_buffer_size=*/ 256);
  commander    = new CommandManager();
  configurator = new Configurator(io->wifi_);

  bool shutter_success = true;
  shutter_success &= shutters->setup_motor("Tür", Pin::D0, Pin::D5, 197 /*cm*/,
    27 /*s*/);
  shutter_success &= shutters->setup_motor("Fenster", Pin::D7, Pin::D6, 117 /*cm*/,
    20 /*s*/);

  bool io_success = true;
  if(SERIAL_ENABLED) io_success &= io->setup_serial(9600 /*bps*/);
  if(LED_ENABLED)    io_success &= io->setup_led(Pin::BUILTIN_LED);
  if(ROTARY_ENABLED) io_success &= io->setup_rotary(Pin::D1, Pin::D2, Pin::D4,
                        100 /*ms*/);
  if(WIFI_ENABLED)   io_success &= io->setup_wifi(IP(192,168,188,100),
                        IP(192,168,188,1), IP(255,255,255,0), 20201);

  bool cmd_success = true;
  cmd_success &= commander->setup_tickrate(10 /*ticks/s*/);
  cmd_success &= commander->setup_module("esp", configurator);
  cmd_success &= commander->setup_module("shutters", shutters);
  for(Motor* motor : shutters->get_motors())
    cmd_success &= commander->setup_module(motor->get_id(), motor);
  cmd_success &= io->add_commander(commander);

  io->log("[SETUP] ESP%s%s%s\r\n",
          ((shutter_success) ? ", Shutters" : ""),
          ((io_success) ? ", I/O" : ""),
          ((cmd_success) ? ", Commander" : ""));

  Callback heartbeat = std::bind(&IOHandler::heartbeat, io, _1);
  commander->interval(heartbeat, 30000 /*ms*/);
  // TODO fix the logging module to actually be useful
  // TODO keep track of the current shutter position
}

void loop(){
  if(io->get()){
    char* input = io->read();
    if(input){
      Command cmd = commander->parse(input);
      Module* target = commander->get_module(cmd);
      if(target){
        if(target->fire(cmd)) io->log("[INFO]  %s -> %s\r\n", cmd.target, cmd.codeword);
        else io->log("[ERROR] Invalid call '%s' in '%s'\r\n", cmd.codeword, cmd.target);
      } else io->log("[ERROR] Call to undefined module '%s'\r\n", cmd.target);
      io->clean_up(input);
    }
  }
  if(commander->tick()) commander->fire_scheduled();
  yield();
}
