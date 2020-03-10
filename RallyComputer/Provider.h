#pragma once
#include "Arduino.h"
#include "Units.h"
#include "GPS_Time.h"

class VelocityProvider {
    public:
        virtual float velocity(Units units) const;
};

class TimeProvider {
    public:
        virtual void getTime(GPS_Time* time) const;
};
