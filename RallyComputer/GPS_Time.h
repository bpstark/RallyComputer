#pragma once
struct GPS_Time {
    uint8_t hour;          ///< GMT hours
    uint8_t minute;        ///< GMT minutes
    uint8_t seconds;       ///< GMT seconds
    uint16_t milliseconds; ///< GMT milliseconds
    uint8_t year;          ///< GMT year
    uint8_t month;         ///< GMT month
    uint8_t day;           ///< GMT day
};
