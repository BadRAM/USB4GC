/*
 *******************************************************************************
 * Legacy Serial MIDI and USB Host bidirectional converter
 * Copyright (C) 2013-2017 Yuuichi Akagawa
 *
 * for use with Arduino MIDI library
 * https://github.com/FortySevenEffects/arduino_midi_library/
 *
 * Note:
 * - If you want use with Leonardo, you must choose Arduino MIDI library v4.0 or higher.
 * - This is sample program. Do not expect perfect behavior.
 *******************************************************************************
 */

#include <MIDI.h>
#include <usbh_midi.h>
#include <usbhub.h>

#if defined(ARDUINO_SAM_DUE) || defined(ARDUINO_ARCH_SAMD)
    struct MySettings : public midi::DefaultSettings
    {
      static const unsigned SysExMaxSize = 1024; // Accept SysEx messages up to 1024 bytes long.
    };
    #define _MIDI_SERIAL_PORT Serial1
    MIDI_CREATE_CUSTOM_INSTANCE(HardwareSerial, _MIDI_SERIAL_PORT, MIDI, MySettings);
#else
//Arduino MIDI library v4.2 compatibility
#define _MIDI_SERIAL_PORT Serial
#ifdef MIDI_CREATE_DEFAULT_INSTANCE
MIDI_CREATE_DEFAULT_INSTANCE();
#endif
#endif

//////////////////////////
// MIDI Pin assign
// 2 : GND
// 4 : +5V(Vcc) with 220ohm
// 5 : TX
//////////////////////////

USBHost UsbH;
USBH_MIDI Midi(&UsbH);

void MidiUsb_poll();
void doDelay(uint32_t t1, uint32_t t2, uint32_t delayTime);

//If you want handle System Exclusive message, enable this #define otherwise comment out it.
#define USBH_MIDI_SYSEX_ENABLE

#ifdef USBH_MIDI_SYSEX_ENABLE
//SysEx:
void handle_sysex( byte* sysexmsg, unsigned sizeofsysex) {
  Midi.SendSysEx(sysexmsg, sizeofsysex);
}
#endif

void setup()
{
  MIDI.turnThruOff();
  MIDI.begin(MIDI_CHANNEL_OMNI);
#ifdef USBH_MIDI_SYSEX_ENABLE
  MIDI.setHandleSystemExclusive(handle_sysex);
#endif
  if (UsbH.Init()) {
    while (1); //halt
  }
  delay( 200 );
}

void loop()
{
  uint8_t msg[4];

  UsbH.Task();
  if ( Midi ) {
    MidiUsb_poll();
    if (MIDI.read()) {
      msg[0] = MIDI.getType();
      switch (msg[0]) {
        case midi::ActiveSensing :
          break;
        case midi::SystemExclusive :
          //SysEx is handled by event.
          break;
        default :
          msg[1] = MIDI.getData1();
          msg[2] = MIDI.getData2();
          Midi.SendData(msg, 0);
          break;
      }
    }
  }
  //delay(1ms)
  //doDelay(t1, (uint32_t)micros(), 1000);
}

// Poll USB MIDI Controler and send to serial MIDI
void MidiUsb_poll()
{
  uint8_t size;
#ifdef USBH_MIDI_SYSEX_ENABLE
  uint8_t recvBuf[MIDI_EVENT_PACKET_SIZE];
  uint8_t rcode = 0;     //return code
  uint16_t  rcvd;
  uint8_t   readPtr = 0;

  rcode = Midi.RecvData( &rcvd, recvBuf);

  //data check
  if (rcode != 0) return;
  if ( recvBuf[0] == 0 && recvBuf[1] == 0 && recvBuf[2] == 0 && recvBuf[3] == 0 ) {
    return ;
  }

  uint8_t *p = recvBuf;
  while (readPtr < MIDI_EVENT_PACKET_SIZE)  {
    if (*p == 0 && *(p + 1) == 0) break; //data end

    uint8_t outbuf[3];
    uint8_t rc = Midi.extractSysExData(p, outbuf);
    if ( rc == 0 ) {
      p++;
      size = Midi.lookupMsgSize(*p);
      _MIDI_SERIAL_PORT.write(p, size);
      p += 3;
    } else {
      _MIDI_SERIAL_PORT.write(outbuf, rc);
      p += 4;
    }
    readPtr += 4;
  }
#else
  uint8_t outBuf[3];
  do {
    if ( (size = Midi.RecvData(outBuf)) > 0 ) {
      //MIDI Output
      _MIDI_SERIAL_PORT.write(outBuf, size);
    }
  } while (size > 0);
#endif
}
