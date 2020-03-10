#include "GPS_Reader.h"
#include "Distance.h"
#include "Screen.h"

// what's the name of the hardware serial port?
#define GPSSerial Serial1


// Connect to the GPS on the hardware port
GPS_Reader GPS(&GPSSerial);
Distance segment;
Distance total;
Screen *screen = nullptr;

void setup()
{
    Serial.begin(115200);
    GPS.init();
    GPS.addDistUpdater(&segment);
    GPS.addDistUpdater(&total);
    screen = new Screen(&segment, &total, &GPS, &GPS, Screen::Cfg());
    screen->init();

}

void loop() // run over and over again
{
    GPS.loop();
    screen->loop();
}
