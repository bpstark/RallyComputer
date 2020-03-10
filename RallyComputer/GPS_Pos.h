#pragma once
struct GPS_Pos {
    float lat;
    float lon;
    float altitude;
};

inline float deg2rad(float deg) {
     return (deg * 71.0) / 4068.0;
}
