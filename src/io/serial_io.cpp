#include "serial_io.h"

SerialIO::SerialIO(int baud, int buffer_size){
  baud_ = baud;
  pos_ = 0;
  buffer_size_ = buffer_size;
  buffer_ = (char*) malloc(buffer_size);
  Serial.begin(baud_);
  Serial.print("\r\n");
}

SerialIO::~SerialIO(){
  free(buffer_);
}

bool SerialIO::get(){
  if(Serial.available()){
    char byte = Serial.read();
    //TODO handle arrow keys when using PuTTY
    if(pos_ == 0 && byte == '\r'){
      Serial.print("\r\n");
      restart();
      return false;
    } else if(byte == '\n' || (pos_ == 0 && (byte == ' ' || byte == '\b' || byte == '\x7F'))){
      return false;           // Filter out backspaces (and DEL for PuTTY) and empty inputs
    } else if(byte != '\r'){
      Serial.print(byte);     // Relay the typed character back to the terminal
    }

    if(byte == '\r' || pos_ == buffer_size_ - 1){
      if(pos_ == buffer_size_ - 1) Serial.print("\b ");
      Serial.print("\r\n");
      buffer_[pos_] = 0;      // Add 0 terminator after the completed command
      pos_ = 0;               // Reset buffer position to overwrite it with new data
      return true;
    } else if((byte == '\b' || byte == '\x7F') && pos_ > 0){
      Serial.print(" \b");    // Print destructive backspace
      pos_--;                 // Discard the last character
      return false;
    } else {
      buffer_[pos_] = byte;
      pos_++;
      return false;
    }
  }
  return false;
}

char* SerialIO::read(){
  return buffer_;
}

void SerialIO::log(char* line){
  Serial.print("   ");
  Serial.print(line);
}

void SerialIO::restart(){
  Serial.print(" > ");
}
