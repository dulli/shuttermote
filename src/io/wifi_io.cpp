#include "wifi_io.h"

WifiIO::WifiIO(IP ip, IP gateway, IP subnet, unsigned int port, int buffer_size){
  WiFi.config(ip, gateway, subnet);
  reply_ = "OK";
  buffer_size_ = buffer_size;
  buffer_ = (char*) malloc(buffer_size);
  ip_ = ip;
  port_ = port;
  udp_ = new WiFiUDP();
  udp_->begin(port);
}

WifiIO::~WifiIO(){
  free(buffer_);
}

bool connect(char* ssid, char* pass){
  return (WiFi.begin(ssid, pass) == WL_CONNECTED);
}

bool WifiIO::get(){
  if(udp_->parsePacket()){
    int len = udp_->read(buffer_, buffer_size_);
    if (len > 0) buffer_[len] = 0;
    udp_->beginPacket(udp_->remoteIP(), udp_->remotePort());
    udp_->write(reply_);
    udp_->endPacket();

    Serial.print(buffer_);       // Relay the received command to Serial output
    Serial.println(" (WIFI)");
    return true;
  }
  return false;
}

char* WifiIO::read(){
  return buffer_;
}

void WifiIO::log(char* line){}
