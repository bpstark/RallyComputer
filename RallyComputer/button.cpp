#include "button.h"

Button::Button(uint8_t pin, long delay):
    pin_(pin)
    , pressed_(false)
    , delay_(delay)
    , timeout_(0)
    , read_(false)
{
    pinMode(pin_, INPUT_PULLUP);
}

void Button::loop() {
    if(millis() > timeout_) {
        auto state = digitalRead(pin_);
        if(state == LOW) {
            pressed_ = true;
            read_ = false;
            timeout_ = millis() + delay_;
            if (timeout_ < millis()) {
                //TODO
            }
        }
        else {
            pressed_ = false;
            timeout_ = 0;
        }
    }
}

bool Button::pressed(bool readOnly) {
    if (pressed_ && !read_) {
        if(!readOnly) {
            read_ = true;
        }
        return true;
    }
    else {
        return false;
    }
}
