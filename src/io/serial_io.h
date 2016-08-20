#ifndef IO_SERIAL_IO_H
#define IO_SERIAL_IO_H

#include "../definitions.h"

class SerialIO : public IO{
public:
  SerialIO(int baud, int buffer_size);
  ~SerialIO();
  bool  get();
  char* read();
  void  log(char *line);
  void  restart();

private:
  int   baud_;
  int   pos_;
  int   buffer_size_;
  char* buffer_;
};

#endif
