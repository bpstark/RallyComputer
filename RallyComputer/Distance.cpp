#include "Distance.h"


Distance::Distance():
    distance_(0.0)
{}

void Distance::reset() {
    distance_ = 0.0;
}
void Distance::update(double dist) {
    distance_ += dist;
}

double Distance::distance(Units units) const{
    if (units == METRIC) {
        return distance_/1000.0;
    }
    else {
        return distance_ * 3.28084 / 5280.0;
    }
}
