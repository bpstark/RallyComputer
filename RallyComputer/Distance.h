#pragma once
#include <Arduino.h>
#include "GPS_Pos.h"
#include "Units.h"
#include "GPS_Reader.h"

class Distance : public DistUpdate{
    public:
        Distance();
        void update(double dist) override;
        void reset();
        double distance(Units units=METRIC) const;
    private:
        double distance_;
};

