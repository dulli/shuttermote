#include "configurator.h"

Configurator::Configurator(WifiIO* wifi){
  wifi_ = wifi;
}

bool Configurator::fire(Command cmd){
  bool success = true;
  if(!cmd.codeword) success = false;
  else if(strcmp(cmd.codeword, CMD_CONNECT) == 0) success = connect(cmd.args);
  else if(strcmp(cmd.codeword, CMD_STATUS) == 0) success = get_status();
  else success = false;
  return success;
}

bool Configurator::connect(Arguments args){
  // TODO handle SSIDs and passwords that contain spaces
  char* ssid = args[0];
  char* pass = args[1];
  return (WiFi.begin(ssid, pass) == WL_CONNECTED);
}

bool Configurator::get_status(){
  // TODO use the logging module instead of prints
  Serial.print("   Status:\t");
  switch(WiFi.status()){
    case WL_CONNECTED: Serial.println("WL_CONNECTED"); break;
    case WL_CONNECT_FAILED: Serial.println("WL_CONNECT_FAILED"); break;
    case WL_DISCONNECTED: Serial.println("WL_DISCONNECTED"); break;
    case WL_NO_SSID_AVAIL: Serial.println("WL_NO_SSID_AVAIL"); break;
    case WL_IDLE_STATUS: Serial.println("WL_IDLE_STATUS"); break;
    default: Serial.println(WiFi.status());
  }
  Serial.print("   IP:  \t");
  Serial.println(WiFi.localIP());
  Serial.print("   SSID:\t");
  Serial.println(WiFi.SSID());
  Serial.print("   Stärke:\t");
  Serial.print(WiFi.RSSI());
  Serial.println(" dBm");
  Serial.print("   MAC:  \t");
  Serial.println(WiFi.macAddress());
  return true;
}
