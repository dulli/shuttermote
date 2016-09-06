ShutterMote
===========
Remotely control the roller shutters on your windows using an ESP8266 that controls
the shutter motors which are connected to a relay board.

...

Known Issues:
-------------
- Serial output sometimes freezes when the relays are switched while motors are connected
- Different commands from multiple source can conflict which each other, causing the shutters to hold earlier than expected
