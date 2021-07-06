# USB4GC

## What is this
USB4GC is an adapter which translates between a GameCube controller port and a USB Game controller. You can use it to play gamecube games on nintendo hardware with an xbox or playstation controller, or even a keyboard. It is relatively easy to build your own using a Seeeduino Xiao microcontroller dev board and a couple passive components.

Supported Controllers:
 - Xbox 360 Wired
 - Xbox One via USB micro B port
 - DualShock 3 (playstation 3 controller)
 - DualShock 4 (playstation 4 controller)
 - Keyboard (fixed bindings)

Tested on GameCube, Wii, and PC through Mayflash Adapter.


## How to:

You will need:
 - 1 Seeduino Xiao. Most of these will need soldering to attach the header pins, if you do not want to solder make sure the Xiao you are purchasing has header pins already attached.
 - 1 solderless breadboard. The instructions will show a 1/2 size standard breadboard for clarity but everything can pretty easily fit on a 17 row mini breadboard.
 - 1 NPN transistor
 - 1 GameCube controller cable to sacrifice. I recommend using a cheap GCC extension cable to avoid killing a controller, but dead or 3rd party controllers will work too, just make sure the cable wasn't the point of failure.
 - Breadboard pluggable wires
