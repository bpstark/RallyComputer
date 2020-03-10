#include "Config.h"
#include <EEPROM.h>

static const int CHKVAL = 1234;

Config* Config::get() {
    static Config cfg;
    return &cfg;
}

Config::Config() : loaded_(false) {
    int val = 0;
    EEPROM.get(0, val);
    if (val == CHKVAL) {
        EEPROM.get(sizeof(CHKVAL), data_);
        loaded_ = true;
    }
}

void Config::save() {
    EEPROM.put(0, CHKVAL);
    EEPROM.put(sizeof(CHKVAL), data_);
}
