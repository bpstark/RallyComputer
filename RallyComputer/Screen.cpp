#include "Screen.h"
#include "Config.h"
#include "GPS_Reader.h"

char* km = "KM";
char* m = " M";
#define ROWS 4
#define COLS 20
char g_buffer [ROWS][COLS+1];
char* ALL_SPACES = "                    ";

void rightFill(char* buffer, float val, char* units) {
    if (val >= 10000) {
        val -= 10000;
    }
    static char tmp[11];
    dtostrf(val, 4, 2, tmp);
    auto len = strlen(tmp);
    auto offset = 8 - len;
    for (auto i = 0; i < len; ++i) {
        buffer[i+ offset] = tmp[i];
    }
    buffer[8] = units[0];
    buffer[9] = units[1];
    buffer[10] = '\0';

}

void leftFillSpeed(char* buffer, int speed, Units units) {
    sprintf(buffer, "%-10d", speed);
    buffer[10] = ' ';
    int i = 0;
    if (speed > 99) {
        i = 4;
    }
    else if (speed > 9) {
        i = 3;
    }
    else {
        i = 2;
    }
    buffer[i++] = units == METRIC ? 'K' : 'M';
    buffer[i++] = 'P';
    buffer[i++] = 'H';
}

void fillTime(char* buffer, int hour, int min, int sec) {
    sprintf(buffer, "      %02d:%02d:%02d", hour, min, sec);
}

Screen::Screen(const Distance* segment,
                const Distance* total,
                const VelocityProvider* vel_provider,
                const TimeProvider* time_provider,
                const Cfg &config):
    lcd(0x27,COLS,ROWS)
    , menu_(true)
    , main_(true)
    , segment_(segment)
    , total_(total)
    , vel_provider_(vel_provider)
    , time_provider_(time_provider)
    , config_(config)
    , curr_menu_(nullptr)
    , curr_option_(nullptr)
{
    PopulateMenu(&menu_);
    for(size_t i = 0; i < BUTTON_POS_SIZE; ++i) {
        buttons_[i] = new Button(config_.button_pins[i]);
    }
    timer_ = millis();
}

void Screen::init() {
    lcd.init();  //initialize the lcd
    lcd.backlight();  //open the backlight 
    lcd.clear();
    resetBuffer();
}

void Screen::resetBuffer() {
    lcd.clear();
    for(auto i = 0; i < ROWS; ++i){
        for(auto j = 0; j < COLS; ++j){
            g_buffer[i][j] = ' ';
        }
        g_buffer[i][COLS] = '\0';
    }
}

bool Screen::button() {
    for(size_t i = 0; i < BUTTON_POS_SIZE; ++i) {
        if(buttons_[i]->pressed(true)) {
            return true;
        }
    }
    return false;
}

void Screen::loop() {
    buttonLoop();
    const static unsigned nextInterval = 200;
    if (timer_ > millis()) {
        timer_ = millis();
    }
    if (millis() - timer_ > nextInterval || button()) {
        timer_ = millis(); // reset the timer_
        if(main_) {
            handleMain();
        }
        else {
            handleMenu();
        }
    }
}

void Screen::handleMain() {
    if (buttons_[TOP_LEFT]->pressed()) {
        main_ = false;
        resetBuffer();
        handleMenu();
        return;
    }
    if(buttons_[TOP_RIGHT]->pressed()) {
        segment_->reset();
    }
    if(buttons_[BOTTOM_LEFT]->pressed()) {
        //Currently has no function in this mode
    }
    if(buttons_[BOTTOM_RIGHT]->pressed()) {
        total_->reset();
    }
    auto cfg = Config::get()->data();
    time_provider_->getTime(&time_);
    fillTime(g_buffer[0], time_.hour, time_.minute, time_.seconds);
    leftFillSpeed(g_buffer[2], vel_provider_->velocity(cfg->speed_units), cfg->speed_units);
    auto dist_units = cfg->dist_units;
    const char* unit_str = dist_units == METRIC ? km : m;
    rightFill(&g_buffer[2][10], segment_->distance(dist_units), unit_str);
    rightFill(&g_buffer[3][10], total_->distance(dist_units), unit_str);
    for (auto i=0; i < ROWS; ++i) {
        lcd.setCursor(0, i);
        lcd.print(g_buffer[i]);
    }
}

void Screen::handleMenu() {
    if (!curr_menu_) {
        curr_menu_ = menu_.head;
        writeRow(0, curr_menu_->val->name());
    }
    if(buttons_[TOP_LEFT]->pressed()) {
        if (!curr_option_) {
            main_ = true;
            curr_menu_ = nullptr;
            resetBuffer();
            handleMain();
        }
        else {
            curr_option_ = nullptr;
            clearRow(2);
        }
    }
    else if(buttons_[TOP_RIGHT]->pressed()) {
        if(curr_option_) {
            clearRow(2);
            curr_option_ = curr_option_->prev;
            writeRow(2, curr_option_->val->name());
        }
        else {
            clearRow(0);
            curr_menu_ = curr_menu_->prev;
            writeRow(0, curr_menu_->val->name());
        }
    }
    else if(buttons_[BOTTOM_RIGHT]->pressed()) {
        if(curr_option_) {
            clearRow(2);
            curr_option_ = curr_option_->next;
            writeRow(2, curr_option_->val->name());
        }
        else {
            clearRow(0);
            curr_menu_ = curr_menu_->next;
            writeRow(0, curr_menu_->val->name());
        }
    }
    else if(buttons_[BOTTOM_LEFT]->pressed()) {
        if(curr_option_) {
            clearRow(2);
            curr_option_->val->select();
            curr_option_ = nullptr;
        }
        else {
            clearRow(2);
            curr_option_ = curr_menu_->val->options.head;
            writeRow(2, curr_option_->val->name());
        }
    }
}

void Screen::clearRow(uint8_t row) {
    if (row >= ROWS) {
        return;
    }
    lcd.setCursor(0, row);
    lcd.print(ALL_SPACES);
}

void Screen::writeRow(uint8_t row, const char* str) {
    if (row >= ROWS) {
        return;
    }
    lcd.setCursor(0, row);
    lcd.print(str);
}

void Screen::buttonLoop() {
    for(size_t i = 0; i < BUTTON_POS_SIZE; ++i) {
        buttons_[i]->loop();
    }
}
