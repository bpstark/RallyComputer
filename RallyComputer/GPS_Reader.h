#pragma once
#include <Adafruit_GPS.h>
#include <Arduino.h>
#include "GPS_Pos.h"
#include "Node.h"
#include "Units.h"
#include "Provider.h"


class DistUpdate {
    public:
        virtual void update(double dist) = 0;
};

class GPS_Reader : public Adafruit_GPS, public VelocityProvider, public TimeProvider{
    public:
#if (defined(__AVR__) || defined(ESP8266)) && defined(USE_SW_SERIAL)
        GPS_Reader(SoftwareSerial *ser); // Constructor when using SoftwareSerial
#endif
        GPS_Reader(HardwareSerial *ser); // Constructor when using HardwareSerial

        void init();
        void loop();
        void addDistUpdater(DistUpdate* updater);
        float velocity(Units units = METRIC) const override;
        void getTime(GPS_Time* time) const override;
    private:
        void end();
        void handle_dist();
        bool dist_init;
        GPS_Pos prev_pos;

        using DistUpdateList = List<DistUpdate*>;
        DistUpdateList dist_update_list_;
};
