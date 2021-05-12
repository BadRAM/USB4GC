#include <main.h>

USBHost UsbH;
XBOXUSB XboxUSB(&UsbH);
XBOXONE XboxONE(&UsbH);
PS3USB PS3(&UsbH);
PS4USB PS4(&UsbH);

USBHost keebH = UsbH;
KeyboardController keyboard(keebH);

keebBindingsMap keebmap;
BindingsMap Xbox360Map;
BindingsMap XboxOneMap;
BindingsMap PS3Map;
BindingsMap PS4Map;

// FlashStorage(keebconfig, keebBindingsMap);
// FlashStorage(Xbox360Config, BindingsMap);
// FlashStorage(XboxOneConfig, BindingsMap);
// FlashStorage(PS3Config, BindingsMap);
// FlashStorage(PS4Config, BindingsMap);


// Config Variables:

// this pin drives a debug LED
int LEDPin = 13;
// this pin can be logic analyzed for high speed status reporting
int DebugPin = 8;

// wait this many ms after the last SI poll command before polling the USB
uint16_t PollDelay = 5;

// // wait this many ms after USB polling to poll USB again, if there is no SI poll.
// int PollFreq = 50;

// --------

byte incomingByte = 0;
uint32_t recvBuffer = 0;
bool timeOfLastProbe = 0;
bool isTX = false;
bool keebMode = false; // This is true when the adapter is in keyboard mode.

bool control_up = false;
bool control_down = false;
bool control_left = false;
bool control_right = false;
bool c_up = false;
bool c_down = false;
bool c_left = false;
bool c_right = false;
bool modX = false;
bool modY = false;
bool ls1 = false;
bool ls2 = false;


unsigned long lastPoll = 0; // this is the millis() of the last time we polled the usb

byte data[8];
byte databuf[8];
byte keebdata[8] = {0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00};
//const byte neutral[8] = {0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00};

// This function intercepts key press
void keyPressed() {
  keebMode = true;
  uint8_t key = keyboard.getOemKey();

  if (key == keebmap.A)
  {
    keebdata[0] |= 0b00000001;
  }
  else if (key == keebmap.B)
  {
    keebdata[0] |= 0b00000010;
  }
  else if (key == keebmap.X)
  {
    keebdata[0] |= 0b00000100;
  }
  else if (key == keebmap.Y)
  {
    keebdata[0] |= 0b00001000;
  }
  else if (key == keebmap.START)
  {
    keebdata[0] |= 0b00000001;
  }
  else if (key == keebmap.Z)
  {
    keebdata[1] |= 0b00010000;
  }
  else if (key == keebmap.L)
  {
    keebdata[1] |= 0b01000000;
    keebdata[6] = 255;
  }
  else if (key == keebmap.R)
  {
    keebdata[1] |= 0b00100000;
    keebdata[7] = 255;
  }
  else if (key == keebmap.DPADUP)
  {
    keebdata[1] |= 0b00001000;
  }
  else if (key == keebmap.DPADDOWN)
  {
    keebdata[1] |= 0b00000100;
  }
  else if (key == keebmap.DPADRIGHT)
  {
    keebdata[1] |= 0b00000010;
  }
  else if (key == keebmap.DPADLEFT)
  {
    keebdata[1] |= 0b00000001;
  }
  else if (key == keebmap.CONTROLSTICK_UP)
  {
    if (modY && !modX)
    {
      keebdata[3] = 170;
    }
    else
    {
      keebdata[3] = 255;
    }
    
    control_up = true;
    control_down = false;
  }
  else if (key == keebmap.CONTROLSTICK_DOWN)
  {
    if (modY && !modX)
    {
      keebdata[3] = 86;
    }
    else
    {
      keebdata[3] = 0;
    }

    control_down = true;
    control_up = false;
  }
  else if (key == keebmap.CONTROLSTICK_LEFT)
  {
    if (modY && !modX)
    {
      keebdata[2] = 86;
    }
    else if (modX)
    {
      keebdata[2] = 44;
    }
    else
    {
      keebdata[2] = 0;
    }
    control_left = true;
    control_right = false;
  }
  else if (key == keebmap.CONTROLSTICK_RIGHT)
  {
    if (modY && !modX)
    {
      keebdata[2] = 170;
    }
    else if (modX)
    {
      keebdata[2] = 212;
    }
    else
    {
      keebdata[2] = 255;
    }
    control_right = true;
    control_left = false;
  }
  else if (key == keebmap.CSTICK_UP)
  {
    keebdata[5] = 255;
    c_up = true;
    c_down = false;
  }
  else if (key == keebmap.CSTICK_DOWN)
  {
    keebdata[5] = 0;
    c_down = true;
    c_up = false;
  }
  else if (key == keebmap.CSTICK_LEFT)
  {
    keebdata[4] = 0;
    c_left = true;
    c_right = false;
  }
  else if (key == keebmap.CSTICK_RIGHT)
  {
    keebdata[4] = 255;
    c_right = true;
    c_left = false;
  }
  else if (key == keebmap.MODX)
  {
    modX = true;
  }
  else if (key == keebmap.MODY)
  {
    modY = true;
  }
  else if (key == keebmap.LS1)
  {
    keebdata[6] = 60;
    ls1 = true;
  }
  else if (key == keebmap.LS2)
  {
    keebdata[6] = 180;
    ls2 = true;
  }
}

// This function intercepts key release
void keyReleased() {
  keebMode = true;
  uint8_t key = keyboard.getOemKey();

  if (key == keebmap.A)
  {
    keebdata[0] &= 0b11111110;
  }
  else if (key == keebmap.B)
  {
    keebdata[0] &= 0b11111101;
  }
  else if (key == keebmap.X)
  {
    keebdata[0] &= 0b11111011;
  }
  else if (key == keebmap.Y)
  {
    keebdata[0] &= 0b11110111;
  }
  else if (key == keebmap.START)
  {
    keebdata[0] &= 0b11111110;
  }
  else if (key == keebmap.Z)
  {
    keebdata[1] &= 0b11101111;
  }
  else if (key == keebmap.L)
  {
    keebdata[1] &= 0b10111111;
    keebdata[6] = 0;
  }
  else if (key == keebmap.R)
  {
    keebdata[1] &= 0b11011111;
    keebdata[7] = 0;
  }
  else if (key == keebmap.DPADUP)
  {
    keebdata[1] &= 0b11110111;
  }
  else if (key == keebmap.DPADDOWN)
  {
    keebdata[1] &= 0b11111011;
  }
  else if (key == keebmap.DPADRIGHT)
  {
    keebdata[1] &= 0b11111101;
  }
  else if (key == keebmap.DPADLEFT)
  {
    keebdata[1] &= 0b11111110;
  }
  else if (key == keebmap.CONTROLSTICK_UP)
  {
    if (!control_down)
    {
      keebdata[3] = 128;
    }
    control_up = false;
  }
  else if (key == keebmap.CONTROLSTICK_DOWN)
  {
    if (!control_up)
    {
      keebdata[3] = 128;
    }
    control_down = false;
  }
  else if (key == keebmap.CONTROLSTICK_LEFT)
  {
    if (!control_right)
    {
      keebdata[2] = 128;
    }
    control_left = false;
  }
  else if (key == keebmap.CONTROLSTICK_RIGHT)
  {
    if (!control_left)
    {
      keebdata[2] = 128;
    }
    control_right = false;
  }
  else if (key == keebmap.CSTICK_UP)
  {
    if (!c_down)
    {
      keebdata[5] = 128;
    }
    c_up = false;
  }
  else if (key == keebmap.CSTICK_DOWN)
  {
    if (!c_up)
    {
      keebdata[5] = 128;
    }
    c_down = false;
  }
  else if (key == keebmap.CSTICK_LEFT)
  {
    if (!c_right)
    {
      keebdata[4] = 128;
    }
    c_left = false;
  }
  else if (key == keebmap.CSTICK_RIGHT)
  {
    if (!c_left)
    {
      keebdata[4] = 128;
    }
    c_right = false;
  }
  else if (key == keebmap.MODX)
  {
    modX = false;
  }
  else if (key == keebmap.MODY)
  {
    modY = false;
  }
  else if (key == keebmap.LS1)
  {
    keebdata[2] = 0;
    ls1 = false;
  }
  else if (key == keebmap.LS2)
  {
    keebdata[2] = 0;
    ls2 = false;
  }
}

// Update data[] by polling the connected USB device.
// This function is too slow to run in between receiving the poll command and 
// sending the response, so it needs to be timed in between SI polls
void pollUSB() 
{
  digitalWrite(DebugPin, HIGH);

  UsbH.Task();
  keebH.Task();

  // If chain tests until it finds a match for the connected controller
  if (XboxUSB.Xbox360Connected)
  {
    keebMode = false;
    // This is the Xbox 360 Wired controller. The wireless controller is not supported.
    databuf[0] = 0x00;
    databuf[0] |= XboxUSB.getButtonPress(A);
    databuf[0] |= XboxUSB.getButtonPress(B) << 1;
    databuf[0] |= XboxUSB.getButtonPress(X) << 2;
    databuf[0] |= XboxUSB.getButtonPress(Y) << 3;
    databuf[0] |= XboxUSB.getButtonPress(START) << 4;
  
    databuf[1] = 0b10000000;
    databuf[1] |= XboxUSB.getButtonPress(LEFT);
    databuf[1] |= XboxUSB.getButtonPress(RIGHT) << 1;
    databuf[1] |= XboxUSB.getButtonPress(DOWN) << 2;
    databuf[1] |= XboxUSB.getButtonPress(UP) << 3;
    databuf[1] |= XboxUSB.getButtonPress(R1) << 4;
    // This controller doesn't have dual stage triggers, so we press L/R above 250/255
    databuf[1] |= (XboxUSB.getButtonPress(R2) > 250) << 5;
    databuf[1] |= (XboxUSB.getButtonPress(L2) > 250) << 6;

    // The library returns the stick position as a 16 bit signed int, centered at zero, so we must convert to byte
    databuf[2] = XboxUSB.getAnalogHat(LeftHatX) / 256 + 128;
    databuf[3] = XboxUSB.getAnalogHat(LeftHatY) / 256 + 128;
    databuf[4] = XboxUSB.getAnalogHat(RightHatX) / 256 + 128;
    databuf[5] = XboxUSB.getAnalogHat(RightHatY) / 256 + 128;

    // L2 and R2 are bytes for some reason.
    databuf[6] = XboxUSB.getButtonPress(L2);
    databuf[7] = XboxUSB.getButtonPress(R2);
  }
  else if (XboxONE.XboxOneConnected)
  {
    keebMode = false;
    // This is the Xbox One controller, connected via USB
    databuf[0] = 0x00;
    databuf[0] |= XboxONE.getButtonPress(A);
    databuf[0] |= XboxONE.getButtonPress(B) << 1;
    databuf[0] |= XboxONE.getButtonPress(X) << 2;
    databuf[0] |= XboxONE.getButtonPress(Y) << 3;
    databuf[0] |= XboxONE.getButtonPress(START) << 4;
  
    databuf[1] = 0b10000000;
    databuf[1] |= XboxONE.getButtonPress(LEFT);
    databuf[1] |= XboxONE.getButtonPress(RIGHT) << 1;
    databuf[1] |= XboxONE.getButtonPress(DOWN) << 2;
    databuf[1] |= XboxONE.getButtonPress(UP) << 3;
    databuf[1] |= XboxONE.getButtonPress(R1) << 4;
    databuf[1] |= (XboxONE.getButtonPress(R2) > 1000) << 5;
    databuf[1] |= (XboxONE.getButtonPress(L2) > 1000) << 6;
  
    // The library returns the stick position as a 16 bit signed int, centered at zero, so we must convert to byte
    databuf[2] = XboxONE.getAnalogHat(LeftHatX) / 256 + 128;
    databuf[3] = XboxONE.getAnalogHat(LeftHatY) / 256 + 128;
    databuf[4] = XboxONE.getAnalogHat(RightHatX) / 256 + 128;
    databuf[5] = XboxONE.getAnalogHat(RightHatY) / 256 + 128;
    // This controller's trigger values scale from 0-1024
    databuf[6] = XboxONE.getButtonPress(L2) / 4;
    databuf[7] = XboxONE.getButtonPress(R2) / 4;
  }
  else if (PS3.PS3Connected)
  {
    keebMode = false;
    // This is the Playstation 3 controller, connected via USB. Both the Dualshock and Sixaxis only versions work.
    databuf[0] = 0x00;
    databuf[0] |= PS3.getButtonPress(CROSS);
    databuf[0] |= PS3.getButtonPress(CIRCLE) << 1;
    databuf[0] |= PS3.getButtonPress(SQUARE) << 2;
    databuf[0] |= PS3.getButtonPress(TRIANGLE) << 3;
    databuf[0] |= PS3.getButtonPress(START) << 4;
  
    databuf[1] = 0b10000000;
    databuf[1] |= PS3.getButtonPress(LEFT);
    databuf[1] |= PS3.getButtonPress(RIGHT) << 1;
    databuf[1] |= PS3.getButtonPress(DOWN) << 2;
    databuf[1] |= PS3.getButtonPress(UP) << 3;
    databuf[1] |= PS3.getButtonPress(R1) << 4;
    databuf[1] |= (PS3.getAnalogButton(R2) > 250) << 5;
    databuf[1] |= (PS3.getAnalogButton(L2) > 250) << 6;
  
    // The Y Axes are inverted, must be unflipped.
    databuf[2] = PS3.getAnalogHat(LeftHatX);
    databuf[3] = 255 - PS3.getAnalogHat(LeftHatY);
    databuf[4] = PS3.getAnalogHat(RightHatX);
    databuf[5] = 255 - PS3.getAnalogHat(RightHatY);
    databuf[6] = PS3.getAnalogButton(L2);
    databuf[7] = PS3.getAnalogButton(R2);  
  }
  else if (PS4.connected())
  {
    keebMode = false;
    // This is the Playstation 4 controller, Connected via USB.
    databuf[0] = 0x00;
    databuf[0] |= PS4.getButtonPress(CROSS);
    databuf[0] |= PS4.getButtonPress(CIRCLE) << 1;
    databuf[0] |= PS4.getButtonPress(SQUARE) << 2;
    databuf[0] |= PS4.getButtonPress(TRIANGLE) << 3;
    databuf[0] |= PS4.getButtonPress(START) << 4;
  
    databuf[1] = 0b10000000;
    databuf[1] |= PS4.getButtonPress(LEFT);
    databuf[1] |= PS4.getButtonPress(RIGHT) << 1;
    databuf[1] |= PS4.getButtonPress(DOWN) << 2;
    databuf[1] |= PS4.getButtonPress(UP) << 3;
    databuf[1] |= PS4.getButtonPress(R1) << 4;
    databuf[1] |= (PS4.getAnalogButton(R2) > 250) << 5;
    databuf[1] |= (PS4.getAnalogButton(L2) > 250) << 6;
  
    // The Y Axes are inverted, must be unflipped.
    databuf[2] = PS4.getAnalogHat(LeftHatX);
    databuf[3] = 255 - PS4.getAnalogHat(LeftHatY);
    databuf[4] = PS4.getAnalogHat(RightHatX);
    databuf[5] = 255 - PS4.getAnalogHat(RightHatY);
    databuf[6] = PS4.getAnalogButton(L2);
    databuf[7] = PS4.getAnalogButton(R2);
  }
  else if (keebMode)
  {
    databuf[0] = keebdata[0];
    databuf[1] = keebdata[1];
    databuf[2] = keebdata[2];
    databuf[3] = keebdata[3];
    databuf[4] = keebdata[4];
    databuf[5] = keebdata[5];
    databuf[6] = keebdata[6];
    databuf[7] = keebdata[7];
  }
  else // set to neutral if no controller recognized
  {
    databuf[0] = 0x00;
    databuf[1] = 0x80;
    databuf[2] = 0x80;
    databuf[3] = 0x80;
    databuf[4] = 0x80;
    databuf[5] = 0x80;
    databuf[6] = 0x00;
    databuf[7] = 0x00;
  }

  // This layer of isolation is not necessary right now, but could be if interrupts or multithreading are used someday.
  data[0] = databuf[0];
  data[1] = databuf[1];
  data[2] = databuf[2];
  data[3] = databuf[3];
  data[4] = databuf[4];
  data[5] = databuf[5];
  data[6] = databuf[6];
  data[7] = databuf[7];

  if (data[2] == 0 ||
    data[2] == 255 ||
    data[3] == 0 ||
    data[3] == 255 ||
    data[4] == 0 ||
    data[4] == 255 ||
    data[5] == 0 ||
    data[5] == 255)
  {
    digitalWrite(LEDPin, HIGH);
  }
  else
  {
    digitalWrite(LEDPin, LOW);
  }

  lastPoll = millis();
  digitalWrite(DebugPin, LOW);
}

byte SI6To2(byte six)
{
  switch(six)
  {
  case 0b00000100:
    return 0b00000000;
  case 0b00000111:
    return 0b00000001;
  case 0b00110100:
    return 0b00000010;
  case 0b00110111:
    return 0b00000011;
  case 0b00111111:
    return 0b00000000;//this is the stop bit.
  }
  return 0b00000000;
}

byte SI2To6(byte two)
{
  // generate an SI message (6 bits) from the last two bits of the input byte.
  switch(two & 0b00000011)
  {
  case 0b00000000:
    return 0b00000100;
  case 0b00000001:
    return 0b00110100;
  case 0b00000010:
    return 0b00000111;
  case 0b00000011:
    return 0b00110111;
  }
  return 0b00000100;
}

void SISendByte(byte toSend)
{
  for(int i=6; i>=0; i-=2)
  {
    Serial1.write(SI2To6(toSend >> i));
  }
}

void pollResponse()
{
  SISendByte(data[0]); // 0, 0, 0, Start, Y, X, B, A
  SISendByte(data[1]); // 1, L, R, Z, D-up, D-down, D-right, D-left
  SISendByte(data[2]); // Joystick X value
  SISendByte(data[3]); // Joystick Y value
  SISendByte(data[4]); // C-Stick X value
  SISendByte(data[5]); // C-Stick Y value
  SISendByte(data[6]); // L analog value
  SISendByte(data[7]); // R analog value
  Serial1.write(0b111111); // Stop bit
}

void setup() {
  Serial1.begin(800000, SERIAL_6N1);

  pinMode(LEDPin, OUTPUT);    
  pinMode(8, OUTPUT);

  if (UsbH.Init()) 
    {
    while (true) //halt if UsbH.Init() fails
    {
      digitalWrite(LEDPin, LOW);
      delay(100);
      digitalWrite(LEDPin, HIGH);
      delay(400);
    }
  }

  digitalWrite(LEDPin, HIGH);
  
  pollUSB();
}

void loop() {  
  while (Serial1.available() > 0) 
  {
    // read the incoming byte:
    incomingByte = Serial1.read();

    // decode the input and add it to the recv buffer
    recvBuffer <<= 2;

    switch(incomingByte)
    {
    case 0b00000100:
      recvBuffer |= 0b00; // very important! :P
      break;
    case 0b00110100:
      recvBuffer |= 0b01;
      break;
    case 0b00000111:
      recvBuffer |= 0b10;
      break;
    case 0b00110111:
      recvBuffer |= 0b11;
      break;
    case 0b00111111:// Stop bit
      recvBuffer >>= 2;
      if (isTX) // check if this is the end of one of our messages, ignore if so.
      {
        isTX = false;
      }
      else if (recvBuffer == 0x00) // ID probe
      {
        SISendByte(0x09);
        SISendByte(0x00);
        SISendByte(0x03);
        isTX = true;
        Serial1.write(0b111111); // end bit
      }
      else if ((recvBuffer & 0xFF) == 0b01000001) // Calibration / origins check
      {
        SISendByte(0x00);
        SISendByte(0x80);
        SISendByte(0x80);
        SISendByte(0x80);
        SISendByte(0x80);
        SISendByte(0x80);
        SISendByte(0x00);
        SISendByte(0x00);
        SISendByte(0x00);
        SISendByte(0x00);
        isTX = true;
        Serial1.write(0b111111); // end bit
      }
      else if ((recvBuffer & 0x00FF0000) == 0x00400000) // input poll
      {
        if ((recvBuffer & 0x0000FF00) == 0x00000300) // standard format
        {
          pollResponse();
          isTX = true;
          lastPoll = millis();
        }
      }
      recvBuffer = 0;
    }
  }

  // Check if it's time to poll the USB
  if (millis() - lastPoll > PollDelay)
  {
    pollUSB();
  }
}