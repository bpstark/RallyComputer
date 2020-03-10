#include "GPS_Reader.h"
#include "Config.h"

#define R 6371000.0

uint32_t timer = millis();

#if (defined(__AVR__) || defined(ESP8266)) && defined(USE_SW_SERIAL)
GPS_Reader::GPS_Reader(SoftwareSerial *ser):
    Adafruit_GPS(ser)
    , dist_init(false)
{}
#endif
GPS_Reader::GPS_Reader(HardwareSerial *ser):
    Adafruit_GPS(ser)
    , dist_init(false)
{}

void GPS_Reader::init() {
    // 9600 NMEA is the default baud rate for Adafruit MTK GPS's- some use 4800
    begin(9600);
    sendCommand(PMTK_SET_BAUD_57600);
    end();
    delay(1000);
    begin(57600);
    delay(1000);
    sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA); //send only RMC and GGA
    sendCommand(PMTK_SET_NMEA_UPDATE_5HZ);
    sendCommand(PMTK_API_SET_FIX_CTL_5HZ);

    // Request updates on antenna status, comment out to keep quiet
    //GPS.sendCommand(PGCMD_ANTENNA);
    delay(1000);

    // Ask for firmware version
    //GPSSerial.println(PMTK_Q_RELEASE);
    
    /*
    begin(9600);
    // uncomment this line to turn on RMC (recommended minimum) and GGA (fix data) including altitude
    sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
    // uncomment this line to turn on only the "minimum recommended" data
    //GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);
    // For parsing data, we don't suggest using anything but either RMC only or RMC+GGA since
    // the parser doesn't care about other sentences at this time
    // Set the update rate
    sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); // 1 Hz update rate
    // For the parsing code to work nicely and have time to sort thru the data, and
    // print it out we don't suggest using anything higher than 1 Hz

    // Request updates on antenna status, comment out to keep quiet
    sendCommand(PGCMD_ANTENNA);

    delay(1000);
    */
}

void GPS_Reader::loop() {
    char c = read();
    // if you want to debug, this is a good time to do it!
    if (newNMEAreceived()) {
        if (!parse(lastNMEA())) // this also sets the newNMEAreceived() flag to false
            return; // we can fail to parse a sentence in which case we should just wait for another
    }
    /*
    if (timer > millis()) timer = millis();

  // approximately every 2 seconds or so, print out the current stats
    if (millis() - timer > 2000) {
        timer = millis(); // reset the timer
        Serial.print("\nTime: ");
        if (hour < 10) { Serial.print('0'); }
        Serial.print(hour, DEC); Serial.print(':');
        if (minute < 10) { Serial.print('0'); }
        Serial.print(minute, DEC); Serial.print(':');
        if (seconds < 10) { Serial.print('0'); }
        Serial.print(seconds, DEC); Serial.print('.');
        if (milliseconds < 10) {
            Serial.print("00");
        } else if (milliseconds > 9 && milliseconds < 100) {
            Serial.print("0");
        }
        Serial.println(milliseconds);
        Serial.print("Date: ");
        Serial.print(day, DEC); Serial.print('/');
        Serial.print(month, DEC); Serial.print("/20");
        Serial.println(year, DEC);
        Serial.print("Fix: "); Serial.print((int)fix);
        Serial.print(" quality: "); Serial.println((int)fixquality);
        if (fix) {
            Serial.print("Location: ");
            Serial.print(latitude, 4); Serial.print(lat);
            Serial.print(", ");
            Serial.print(longitude, 4); Serial.println(lon);
            Serial.print("Speed (knots): "); Serial.println(speed);
            Serial.print("Angle: "); Serial.println(angle);
            Serial.print("Altitude: "); Serial.println(altitude);
            Serial.print("Satellites: "); Serial.println((int)satellites);
        }
    }
    */
    handle_dist();
}

void GPS_Reader::handle_dist() {
    if(!dist_init) {
        prev_pos.lat = latitudeDegrees;
        prev_pos.lon = longitudeDegrees;
        prev_pos.altitude = altitude;
        dist_init = true;
        return;
    }
    auto d_lat = deg2rad(latitudeDegrees - prev_pos.lat);
    auto d_lon = deg2rad(longitudeDegrees - prev_pos.lon);

    auto s_lat = sin(d_lat/2);
    auto s_lon = sin(d_lon/2);

    auto a = s_lat * s_lat + cos(deg2rad(latitudeDegrees)) * cos(deg2rad(prev_pos.lat)) * s_lon * s_lon;

    auto c = 2.0 * atan2(sqrt(a), sqrt(1.0-a));

    double distance = R * c;
    distance += abs(altitude - prev_pos.altitude);

    prev_pos.lat = latitudeDegrees;
    prev_pos.lon = longitudeDegrees;
    prev_pos.altitude = altitude;

    if (distance > 1000) {
        //This is an init issue
        return;
    }

    auto* dn = dist_update_list_.head;
    while(dn) {
        dn->val->update(distance);
        dn = dn->next;
    }
}

void GPS_Reader::end() {
#if (defined(__AVR__) || defined(ESP8266)) && defined(USE_SW_SERIAL)
    if (gpsSwSerial) {
        gpsSwSerial->end();
    }
#endif
    if (gpsHwSerial) {
        gpsHwSerial->end();
    }
}

void GPS_Reader::addDistUpdater(DistUpdate* updater) {
    dist_update_list_.append(updater);
}

float GPS_Reader::velocity(Units units) const {
    if (units == METRIC) {
        return speed * 1.852;
    }
    else if(units == SI) {
        return speed * 1.15078;
    }
    else {
        return speed;
    }
}

void GPS_Reader::getTime(GPS_Time* time) const{
    /*
    uint8_t hour;
    uint8_t minute;
    uint8_t seconds;
    uint16_t milliseconds;
    uint8_t year;
    uint8_t month;
    uint8_t day;
    */
    auto cfg = Config::get()->data();
    int16_t hr = hour;
    hr += cfg->time_zone;
    if (cfg->dst) {
        ++hr;
    }
    if(hr < 0) {
        hr += 24;
    }
    time->hour = hr;
    time->minute = minute;
    time->seconds = seconds;
}
