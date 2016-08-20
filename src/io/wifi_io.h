#ifndef IO_WIFI_IO_H
#define IO_WIFI_IO_H

#include "../definitions.h"

class WifiIO : public IO{
public:
  WifiIO(IP ip, IP gateway, IP subnet, unsigned int port, int buffer_size);
  ~WifiIO();
  bool  connect(char* ssid, char* pass);
  bool  get();
  char* read();
  void  log(char *line);

private:
  IP           ip_;
  unsigned int port_;
  WiFiUDP*     udp_;
  int          buffer_size_;
  char*        buffer_;
  const char*  reply_;
};

#endif
