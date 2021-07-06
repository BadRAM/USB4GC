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


## How to

### Step 1: Tools and Materials

Tools | Comment
-|-
Wire stripper/cutter | knockoff GameCube cable wires are usually very thin, so a 20-30 AWG wire stripper is recommended.
Multimeter for continuity testing | knockoff GameCube cable wires are often correctly color coded, but the only way to know for sure is to test them. Assembling the wire is the hardest part of this build and a continuity tester will make this a lot easier.
USB-C - USB-A cable | for programming the Seeeduino Xiao
Soldering iron | it is possible to do without one, but you will need a JST crimping tool and a preassembled Seeeduino Xiao
JST-XH Crimping tool | it is possible to do without one, but you will need to do some difficult soldering and your cable will be much weaker.

Materials | Comment
-|-
Seeduino Xiao | Most of these will need soldering to attach the header pins, if you do not want to solder make sure the Xiao you are purchasing has header pins already attached.
USB-C to USB-A Adapter | 
solderless breadboard | The tutorial build uses a 1/2 size standard breadboard for clarity but everything can pretty easily fit on a 17 row mini breadboard.
PNP transistor | 
GameCube controller cable to sacrifice | I recommend using a cheap GCC extension cable to avoid killing a controller, but dead (or mad catz) controllers will work too, just make sure the cable wasn't the point of failure.
Breadboard pluggable wires | I use 22 gauge solid core wires in the instructions.
JST-XH 5 position socket, plug, and appropriate crimping tool | This is for attaching the GameCube controller cable to the breadboard. There are a few other ways to achieve this, detailed in step 2.

### Step 2: Cable Preparation

Let's get the hardest part out of the way first. The goal here is to be able to plug a breadboard into a GameCube controller port. If you can find a better way of doing this, I'd love to hear about it! If you do not have a JST-XH crimp tool and appropriate connectors, but do have a soldering iron, some perfboard, headers, and some patience, proceed to step 2b.1.

2.1: Decide how long you want your adapter's cable to be, then cut your GameCube controller cable to that length, plus about 2 centimeters.

2.2: Carefully cut away 1.5 cm of the rubber cable insulation, and then peel back the foil shielding around the wire bundle (if present). Check that none of the wires have been damaged or removed in the procedure. If any of them have, cut the entire cable off just above the damage and start again.

2.3: Strip the top 3mm of each wire. Once they are all stripped, test each one for continuity to each pin of the gamecube plug. If any of them do not make continuity, or make continuity at more than one pin, check the whole cable for damage. if the cable has been damaged, cut just below that and return to step 2.1. If the cable colors do not match the provided diagram, Draw your own with the correct colors and remember to use it in step 2.6. The two ground wires are interchangable, we are going to tie them together in part 3 anyway so don't sweat mixing them up. If your wire stripper is too big and strong for the pathetic wires they use in these cables, try the technique in fig. C, where you hold the wire against the back of the stripper with your thumb, and pull the wire out perpendicular.

2.4: If there is a wire connected to shield (this is often not insulated), cut it down to the edge of the cable insulation to get it out of the way.

2.5: Crimp JST-XH connectors to the ends of all the remaining wires. Refer to [this](https://iotexpert.com/jst-connector-crimping-insanity/) guide if you need help, but stop short of inserting the crimped wires into the plug housing.

2.6: Insert the crimped wires into the housing according to this diagram and the color chart from step 2.3.

2.7: perform one final continuity check and you are done! proceed to Step 3.


2b.1: Perform 2.1 through 2.4, but cut away much more cable insulation, and strip at least 5mm from the wires.

2b.2: break off a segment of perfboard


### Step 3: Breadboard Assembly

Now we just need to lay everything out on the breadboard. I color code and pre-strip all the wires for your convenience, you can achieve a similar look by following [this](https://www.youtube.com/watch?v=PE-_rJqvDhQ) guide from the legendary Ben Eater.

Here is the schematic if you want to arrange everything yourself.
![Schematic.png](https://octodex.github.com/images/yaktocat.png)

3.1: Place the JST socket, Seeeduino Xiao, and PNP transistor. Make sure the transistor's Flat side is facing outwards, towards the near edge of the breadboard.

3.2: Place all the green wires.

3.3: Place all the red wires.

3.4: Place all the yellow and blue wires.

3.5: Place all the black wires.


### Step 4: Programming

That Seeeduino Xiao doesn't know how to speak gamecube yet.

4.1: Go to this github repository's [releases](https://github.com/BadRAM/USB4GCsecret/releases) page and download the latest release's firmware binary. It will be named USB4GC_FW_BIN_xx.UF2.

4.2: Connect the Xiao to your computer via the USB-C cable.

4.2: Plug a jumper wire into ground (row 25 is easily accessible) and with the other end, tap the "RST" pad on the Xiao twice. The onboard LED should begin to pulsate, and the Seeeduino should appear as a removable storage device on your computer.

4.3: copy the USB4GC_FW_BIN_xx.UF2 file onto the Xiao. If this step is successful, the amber LED should begin to flash steadily.


### Step 5: Usage instructions

To use the adapter, plug the GameCube cable into the JST socket on the breadboard, then into the controller port on your console. Plug your controller into the Xiao using your USB-A to USB-C adapter, and you're done. If you want to use a keyboard, you will need to connect the keyboard once to switch the adapter into keyboard mode, then disconnect and reconnect it for it to work. once in keyboard mode, you will need to do the same with any controller to return to standard mode.


## Contributing

If there are any features you would like to implement, I'd be happy to help you get set up. feel free to contact me via Discord at BadRAM#6838
