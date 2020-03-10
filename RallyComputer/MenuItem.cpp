#include "MenuItem.h"
#include "Config.h"
#include "Units.h"

MenuItem::MenuItem():
    options(true)
{}

MenuOption::MenuOption(const char* nm):
    name_(nm)
{}

class SpeedOption : public MenuOption {
    public:
        SpeedOption(const char* nm, uint8_t val):
            MenuOption(nm)
            , val_(val)
        {}
        void select() override {
            Config::get()->data()->speed_units = val_;
            Config::get()->save();
        }
    private:
        uint8_t val_;

};

class DistOption : public MenuOption {
    public:
        DistOption(const char* nm, uint8_t val):
            MenuOption(nm)
            , val_(val)
        {}
        void select() override {
            Config::get()->data()->dist_units = val_;
            Config::get()->save();
        }
    private:
        uint8_t val_;
};
class DSTOption : public MenuOption {
    public:
        DSTOption(const char* nm, uint8_t val):
            MenuOption(nm)
            , val_(val)
        {}
        void select() override {
            Config::get()->data()->dst = val_;
            Config::get()->save();
        }
    private:
        uint8_t val_;
};
class TimeZoneOption : public MenuOption {
    public:
        TimeZoneOption(const char* nm, int16_t val):
            MenuOption(nm)
            , val_(val)
        {}
        void select() override {
            Config::get()->data()->time_zone = val_;
            Config::get()->save();
        }
    private:
        int16_t val_;
};

class SpeedUnitsItem : public MenuItem {
    public:
        const char* name() override {
            return "Speed Units";
        }
        SpeedUnitsItem() {
            options.append(new SpeedOption("METRIC", METRIC));
            options.append(new SpeedOption("SI", SI));
        }
};

class DistUnitsItem : public MenuItem {
    public:
        const char* name() override {
            return "Dist Units";
        }
        DistUnitsItem() {
            options.append(new DistOption("METRIC", METRIC));
            options.append(new DistOption("SI", SI));
        }
};

class DSTItem : public MenuItem {
    public:
        const char* name() override {
            return "DST";
        }
        DSTItem() {
            options.append(new DSTOption("ON", 1));
            options.append(new DSTOption("OFF", 0));
        }
};
class TimeZoneItem : public MenuItem {
    public:
        const char* name() override {
            return "Time Zone";
        }
        TimeZoneItem() {
            options.append(new TimeZoneOption("UTC", 0));
            options.append(new TimeZoneOption("Eastern", -5));
            options.append(new TimeZoneOption("Central", -6));
            options.append(new TimeZoneOption("Mountain", -7));
            options.append(new TimeZoneOption("Pacific", -8));
        }
};

void PopulateMenu(List<MenuItem*>* menu) {
    menu->append(new TimeZoneItem());
    menu->append(new DSTItem());
    menu->append(new SpeedUnitsItem());
    menu->append(new DistUnitsItem());
}
