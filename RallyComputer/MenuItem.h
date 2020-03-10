#pragma once
#include <Arduino.h>
#include "Node.h"

class MenuOption {
    public:
        MenuOption(const char* nm);
        virtual const char* name() {return name_;}
        virtual void select();
    protected:
        const char* name_;
};
class MenuItem {
    public:
        MenuItem();
        virtual const char* name();
        using OptionList = List<MenuOption*>;
        OptionList options;
};

void PopulateMenu(List<MenuItem*>* menu);
