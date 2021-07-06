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

### Step 1: Tools and Materials

Tools | Comment
-|-
Wire stripper/cutter | knockoff GameCube cable wires are usually very thin, so a 20-30 AWG wire stripper is recommended.
Multimeter for continuity testing | knockoff GameCube cable wires are often correctly color coded, but the only way to know for sure is to test them. Assembling the wire is the hardest part of this build and a continuity tester will make this a lot easier.
USB-C cable | for programming the Seeeduino Xiao
Soldering iron | it is possible to do without one, but you will need a JST crimping tool and a preassembled Seeeduino Xiao
JST-XH Crimping tool | it is possible to do without one, but you will need to do some difficult soldering and your cable will be much weaker.

Materials | Comment
-|-
1 Seeduino Xiao | Most of these will need soldering to attach the header pins, if you do not want to solder make sure the Xiao you are purchasing has header pins already attached.
1 USB-C to USB-A Adapter | 
1 solderless breadboard | The tutorial build uses a 1/2 size standard breadboard for clarity but everything can pretty easily fit on a 17 row mini breadboard.
1 NPN transistor | 
1 GameCube controller cable to sacrifice | I recommend using a cheap GCC extension cable to avoid killing a controller, but dead (or mad catz) controllers will work too, just make sure the cable wasn't the point of failure.
Breadboard pluggable wires | I use 22 gauge solid core wires in the instructions.
JST-XH 5 position socket, plug, and appropriate crimping tool | This is for attaching the GameCube controller cable to the breadboard. There are a few other ways to achieve this, detailed in step 2.

### Step 2: Cable Preparation

Let's get the hardest part out of the way first. The goal here is to be able to plug a breadboard into a GameCube controller port. If you can find a better way of doing this, I'd love to hear about it! If you do not have a JST-XH crimp tool and appropriate connectors, but do have a soldering iron and some patience, proceed to step 2b.1.

2.1: Decide how long you want your adapter's cable to be, then cut your GameCube controller cable to that length, plus about 2 centimeters.

2.2: Carefully cut away 1.5 cm of the rubber cable insulation, and then peel back the foil shielding around the wire bundle (if present). Check that none of the wires have been damaged or removed in the procedure. If any of them have, cut the entire cable off just above the damage and start again.

2.3: Strip the top 5mm of each wire. Once they are all stripped, test each one for continuity to each pin of the gamecube plug. If any of them do not make continuity, or make continuity at more than one pin, check the whole cable for damage. if the cable has been damaged, cut just below that and return to step 2.1. If the cable colors do not match the provided diagram, Draw your own with the correct colors and remember to use it in step 2.6. The two ground wires are interchangable, we are going to tie them together in part 3 anyway so don't sweat mixing them up. If your wire stripper is too big and strong for the pathetic wires they use in these cables, try the technique in fig. C, where you hold the wire against the back of the stripper with your thumb, and pull the wire out perpendicular.

2.4: If there is a wire connected to shield (this is often not insulated), cut it down to the edge of the cable insulation to get it out of the way.

2.5: Crimp JST-XH connectors to the ends of all the remaining wires. Refer to [this](https://iotexpert.com/jst-connector-crimping-insanity/) guide if you need help, but stop short of inserting the crimped wires into the plug housing.

2.6: Insert the crimped wires into the housing according to this diagram and the color chart from step 2.3.

2.7: perform one final continuity check and you are done! proceed to Step 3.


### Step 3: Breadboard Assembly

Now we just need to lay everything out on the breadboard. Here is the schematic if you want to arrange everything yourself. I color code and pre-strip all the wires for your convenience, you can achieve a similar look by following [this](https://www.youtube.com/watch?v=PE-_rJqvDhQ) guide from the legendary Ben Eater.

3.1: Place the JST plug, Seeeduino Xiao, and NPN transistor. Make sure the transistor's Flat side is facing outwards, towards the near edge of the breadboard.

3.2: Place all the green wires.

3.3: Place all the red wires.

3.4: Place all the yellow and blue wires.

3.5: Place all the black wires.


### Step 4: Programming

That Seeeduino Xiao doesn't know how to speak gamecube yet.

4.1: Follow Seeed studio's getting started guide for the xiao: https://wiki.seeedstudio.com/Seeeduino-XIAO/#getting-started

4.2: Go to this github repository's [releases]() page and download the latest release's firmware binary. It will be named USB4GC_FW_BIN_xx.bin.

4.3: If you still have the arduino IDE popup open from uploading the blink sketch 

4.3: Plug a jumper cable into ground (row 25 is accessible) and with the other end, tap the "RST" pad on the Xiao twice. The onboard LED should begin to pulsate.

4.4: 




