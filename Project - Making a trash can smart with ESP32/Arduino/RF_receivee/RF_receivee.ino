#include <RH_ASK.h>
#ifdef RH_HAVE_HARDWARE_SPI
#include <SPI.h> // Not actually used but needed to compile
#endif

const int rxPin = A0;
const int txPin = 2;
const int led = 13;
const int pttPin = 10;

RH_ASK driver(2000, rxPin, txPin, pttPin, true);

void setup()
{
  #ifdef RH_HAVE_SERIAL
    Serial.begin(9600);   // Debugging only
  #endif
      if (!driver.init())
  #ifdef RH_HAVE_SERIAL
           Serial.println("init failed");
  #else
          ;
  #endif 
}
 
void loop()
{
  uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];
    uint8_t buflen = sizeof(buf);
 
    if (driver.recv(buf, &buflen)) // Non-blocking
    {
        int i;
 
        // Message with a good checksum received, dump it.
        driver.printBuffer("Got:", buf, buflen);
    }
}
