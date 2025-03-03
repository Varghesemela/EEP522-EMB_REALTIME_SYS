#ifndef DEWPOINT_H
#define DEWPOINT_H

/* 
 * Compute the dew point (in °C) from ambient temperature (°C) and relative humidity (%).
 * Uses the Magnus formula:
 *
 *    α = ln(RH/100) + (a * T)/(b + T)
 *    Dew point = (b * α)/(a - α)
 *
 * where:
 *    a = 17.27, b = 237.7°C.
 */
double compute_dew_point(double temperature, double humidity);

#endif // DEWPOINT_H
