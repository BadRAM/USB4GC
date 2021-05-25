#include <Arduino.h>
#include <XBOXUSB.h>
#include <XBOXONE.h>
#include <PS3USB.h>
#include <PS4USB.h>
#include <KeyboardController.h>
#include <FlashStorage.h>

struct BindingsMap
{
  ButtonEnum A = A;
  ButtonEnum B = B;
  ButtonEnum X = X;
  ButtonEnum Y = Y;

  ButtonEnum Z = L1;

  ButtonEnum START = START;

  ButtonEnum ANALOG_L = L2;
  ButtonEnum ANALOG_R = R2;
  uint8_t DIGITAL_L = 250; // The digital L/R values serve as thresholds on controllers without dual stage triggers, and as button addresses on those with.
  uint8_t DIGITAL_R = 250;
    
  ButtonEnum DPADUP = UP;
  ButtonEnum DPADDOWN = DOWN;
  ButtonEnum DPADLEFT = LEFT;
  ButtonEnum DPADRIGHT = RIGHT;

  AnalogHatEnum CONTROLSTICK_X = LeftHatX;
  AnalogHatEnum CONTROLSTICK_Y = LeftHatY;
  AnalogHatEnum CSTICK_X = RightHatX;
  AnalogHatEnum CSTICK_Y = RightHatY; 
};

struct keebBindingsMap
{
  uint8_t A = 16;
  uint8_t B = 18;
  uint8_t X = 19;
  uint8_t Y = 39;

  uint8_t Z = 47;

  uint8_t START = 36;

  uint8_t L = 20;
  uint8_t R = 38;
  uint8_t LS1 = 45; // The digital L/R values serve as thresholds on controllers without dual stage triggers, and as button addresses on those with.
  uint8_t LS2 = 46;
  uint8_t LS1STRENGTH = 25;
  uint8_t LS2STRENGTH = 50;
    
  uint8_t DPADUP = 82;
  uint8_t DPADDOWN = 81;
  uint8_t DPADLEFT = 80;
  uint8_t DPADRIGHT = 79;

  uint8_t CONTROLSTICK_UP = 48;
  uint8_t CONTROLSTICK_DOWN = 32;
  uint8_t CONTROLSTICK_LEFT = 31;
  uint8_t CONTROLSTICK_RIGHT = 33;
  uint8_t CSTICK_UP = 14;
  uint8_t CSTICK_DOWN = 44; 
  uint8_t CSTICK_LEFT = 17; 
  uint8_t CSTICK_RIGHT = 54; 

  uint8_t MODX = 25; 
  uint8_t MODY = 5; 
  uint8_t MODXSTRENGTH = 80; 
  uint8_t MODYSTRENGTH = 160; 
};

byte SI6To2(byte six)
{
  switch (six)
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
    return 0b00000000; //this is the stop bit.
  }
  return 0b00000000;
}

byte SI2To6(byte two)
{
  // generate an SI message (6 bits) from the last two bits of the input byte.
  switch (two & 0b00000011)
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