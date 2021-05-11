#include <Arduino.h>
#include <XBOXUSB.h>
#include <XBOXONE.h>
#include <PS3USB.h>
#include <PS4USB.h>

USBHost UsbH;
XBOXUSB XboxUSB(&UsbH);
XBOXONE XboxONE(&UsbH);
PS3USB PS3(&UsbH);
PS4USB PS4(&UsbH);


// Config Variables:

// this pin drives a debug LED
int LEDPin = 13;

// wait this many ms after the last SI poll command before polling the USB
int PollDelay = 4;

// wait this many ms after USB polling to poll USB again, if there is no SI poll.
int PollFreq = 100;

// --------

byte incomingByte = 0;
uint32_t recvBuffer = 0;
bool timeOfLastProbe = 0;
bool isTX = false;
bool AButtonDown = false;

unsigned long lastPoll = 0; // this is the millis() of the last time the console polled for input data.

byte data[8];
byte databuf[8];

//enum SIQuery
//{
//  probe = 0b00000000
//}

// Update data[] by polling the connected USB device.
// This function is too slow to run in between receiving the poll command and 
// sending the response, so it needs to be timed in between SI polls
void pollUSB() 
{
  UsbH.Task();


  // If chain tests until it finds a match for the connected controller
  if (XboxUSB.Xbox360Connected)
  {
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

  // This layer of isolation is not necessary right now, but could be if interrupts are used someday.
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
  default:
    break;
  }
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

  if (UsbH.Init()) 
    {
    while (true) //halt
    {
      digitalWrite(LEDPin, true);
      delay(1000);
      digitalWrite(LEDPin, false);
      delay(1000);
    }
  }

  digitalWrite(LEDPin, LOW);
  
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
  if (lastPoll != 0 && millis() - lastPoll > PollDelay)
  {
    pollUSB();
    lastPoll = millis() + PollFreq - PollDelay;
  }
}