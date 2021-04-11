/* USB Host to PL2303-based USB GPS unit interface */
/* Navibee GM720 receiver - Sirf Star III */
/* USB support */
#include <usbhub.h>
/* CDC support */
#include <cdcacm.h>
#include <cdcprolific.h>

// On SAMD boards where the native USB port is also the serial console, use
// Serial1 for the serial console. This applies to all SAMD boards except for
// Arduino Zero and M0 boards.
#if (USB_VID==0x2341 && defined(ARDUINO_SAMD_ZERO)) || (USB_VID==0x2a03 && defined(ARDUINO_SAM_ZERO))
#define SerialDebug SERIAL_PORT_MONITOR
#else
#define SerialDebug Serial1
#endif

class PLAsyncOper : public CDCAsyncOper {
public:
        uint8_t OnInit(ACM *pacm);
};

uint8_t PLAsyncOper::OnInit(ACM *pacm) {
        uint8_t rcode;

        // Set DTR = 1
        rcode = pacm->SetControlLineState(1);

        if(rcode) {
                ErrorMessage<uint8_t>(PSTR("SetControlLineState"), rcode);
                return rcode;
        }

        LINE_CODING lc;
        lc.dwDTERate = 4800; //default serial speed of GPS unit
        lc.bCharFormat = 0;
        lc.bParityType = 0;
        lc.bDataBits = 8;

        rcode = pacm->SetLineCoding(&lc);

        if(rcode)
                ErrorMessage<uint8_t>(PSTR("SetLineCoding"), rcode);

        return rcode;
}

USBHost UsbH;
USBHub Hub(&UsbH);
PLAsyncOper AsyncOper;
PL2303 Pl(&UsbH, &AsyncOper);
uint32_t read_delay;
#define READ_DELAY 100

void setup() {
        SerialDebug.begin(115200);
        SerialDebug.println("Start");

        if(UsbH.Init())
                SerialDebug.println("USB host did not start");

        delay(200);
}

void loop() {
        uint32_t rcode;
        uint8_t buf[64]; //serial buffer equals Max.packet size of bulk-IN endpoint
        uint16_t rcvd = 64;

        UsbH.Task();

        if(Pl.isReady()) {
                /* reading the GPS */
                if((int32_t)((uint32_t)millis() - read_delay) >= 0L) {
                        read_delay += READ_DELAY;
                        rcode = Pl.RcvData(&rcvd, buf);
                        if(rcode && rcode != USB_ERRORFLOW)
                                ErrorMessage<uint8_t>(PSTR("Ret"), rcode);
                        if(rcvd) { //more than zero bytes received
                                for(uint16_t i = 0; i < rcvd; i++) {
                                        SerialDebug.print((char)buf[i]); //printing on the screen
                                }//for( uint16_t i=0; i < rcvd; i++...
                        }//if( rcvd
                }//if( read_delay > millis()...
        }//if( UsbH.getUsbTaskState() == USB_STATE_RUNNING..
}


