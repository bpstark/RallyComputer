#pragma once
#include <Arduino.h>

class Config { 
    public:
        struct Data {
            int16_t time_zone;
            uint8_t dst;
            uint8_t dist_units;
            uint8_t speed_units;
        };
        Data* data() {return &data_;}
        static Config* get();
        void save();
    private:
        Config();
        Data data_;
        bool loaded_;

};

