#include "dewpoint.h"
#include <math.h>

/* Compute dew point (°C) using the Magnus formula.
 * T: temperature in °C, RH: relative humidity in %
 */
double compute_dew_point(double T, double RH) {
    const double a = 17.27;
    const double b = 237.7;
    double alpha = log(RH / 100.0) + (a * T) / (b + T);
    return (b * alpha) / (a - alpha);
}
