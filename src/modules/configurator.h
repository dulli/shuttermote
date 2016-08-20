#ifndef MODULES_CONFIGURATOR_H
#define MODULES_CONFIGURATOR_H

#include "../definitions.h"
#include "../io/wifi_io.h"

#define CMD_CONNECT      "connect"
#define CMD_STATUS       "status"

class Configurator : public Module {
public:
  Configurator(WifiIO* wifi);
  bool fire(Command cmd) override;
  bool connect(Arguments args);
  bool get_status();

private:
  WifiIO* wifi_;
};

#endif
