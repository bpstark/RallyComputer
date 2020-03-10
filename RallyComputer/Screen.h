#pragma once
#include <Arduino.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include "Node.h"
#include "Distance.h"
#include "MenuItem.h"
#include "Provider.h"
#include "Button.h"

class Screen {
    public:
        enum BUTTON_POS: size_t {
            TOP_LEFT = 0,
            TOP_RIGHT,
            BOTTOM_LEFT,
            BOTTOM_RIGHT,
            BUTTON_POS_SIZE //Must remain as last element
        };

        struct Cfg {
            uint8_t button_pins[BUTTON_POS_SIZE] = {4, 5, 6, 7};
        };
        Screen(const Distance* segment,
                const Distance* total,
                const VelocityProvider* vel_provider,
                const TimeProvider* time_provider,
                const Cfg &config);
        void init();
        void loop();
        void resetBuffer();
        void clearRow(uint8_t row);
        void writeRow(uint8_t row, const char* str);
        void buttonLoop();
    private:
        void handleMain();
        void handleMenu();
        bool button();
        LiquidCrystal_I2C lcd;
        List<MenuItem*> menu_;
        Button* buttons_ [BUTTON_POS_SIZE];
        bool main_;
        GPS_Time time_;
        const Distance* segment_;
        const Distance* total_;
        const VelocityProvider* vel_provider_;
        const TimeProvider* time_provider_;
        const Cfg config_;
        uint32_t timer_;
        Node<MenuItem*>* curr_menu_;
        Node<MenuOption*>* curr_option_;

};
