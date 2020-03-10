#pragma once
#include <Arduino.h>

class Button {
    public:
        Button(uint8_t pin, long delay = 250);
        void loop();
        bool pressed(bool readOnly=false);
    private:
        uint8_t pin_;
        bool pressed_;
        long delay_;
        long timeout_;
        bool read_;

};
