#include <stdio.h>
#include <stdlib.h>
#include <string.h>
<<<<<<< Updated upstream
=======
#include <unistd.h>
#include <stdbool.h>
>>>>>>> Stashed changes
#include <math.h>
#include "dewpoint.h"
#include "shtc3_parser.h"
#include "fan_control.h"


/*
 * Simple control algorithm:
 *  - If dome temperature is at or below the dew point, increase inlet fan speed (to warm the dome)
 *    and decrease exhaust fan speed.
 *  - If dome temperature is well above the dew point, decrease inlet fan speed and increase exhaust fan speed.
 *  - For intermediate conditions, linearly interpolate the fan speeds.
 */
void adjust_fans(double dome_temp, double dew_point) {
    double delta = dome_temp - dew_point;
    unsigned long inlet_duty, exhaust_duty;

    if (delta <= 0.0) {
        /* Dome is below or at dew point: maximize warming */
        inlet_duty   = (unsigned long)(PWM_PERIOD * 0.9);  // 90% duty cycle
        exhaust_duty = (unsigned long)(PWM_PERIOD * 0.2);  // 20% duty cycle
    } else if (delta >= 3.0) {
        /* Dome is well above dew point: minimal warming, maximum exhaust */
        inlet_duty   = (unsigned long)(PWM_PERIOD * 0.2);  // 20% duty cycle
        exhaust_duty = (unsigned long)(PWM_PERIOD * 0.8);  // 80% duty cycle
    } else {
        /* Linearly interpolate between the two extremes */
        double factor = delta / 3.0; // 0 <= factor <= 1
        inlet_duty   = (unsigned long)(PWM_PERIOD * (0.9 - 0.7 * factor));
        exhaust_duty = (unsigned long)(PWM_PERIOD * (0.2 + 0.6 * factor));
    }

    printf("Adjusting fans based on dome_temp=%.2f°C, dew_point=%.2f°C, delta=%.2f°C\n",
           dome_temp, dew_point, delta);
    printf("Setting inlet fan duty cycle to %lu (%.1f%%)\n", inlet_duty, inlet_duty * 100.0 / PWM_PERIOD);
    printf("Setting exhaust fan duty cycle to %lu (%.1f%%)\n", exhaust_duty, exhaust_duty * 100.0 / PWM_PERIOD);

<<<<<<< Updated upstream
    if (set_pwm_duty(INLET_PWM_PATH, inlet_duty) < 0) {
        fprintf(stderr, "Failed to set inlet fan PWM duty cycle.\n");
    }
    if (set_pwm_duty(EXHAUST_PWM_PATH, exhaust_duty) < 0) {
        fprintf(stderr, "Failed to set exhaust fan PWM duty cycle.\n");
    }
=======
    // if (set_pwm_duty(INLET_PWM_PATH, inlet_duty) < 0) {
    //     fprintf(stderr, "Failed to set inlet fan PWM duty cycle.\n");
    // }
    // if (set_pwm_duty(EXHAUST_PWM_PATH, exhaust_duty) < 0) {
    //     fprintf(stderr, "Failed to set exhaust fan PWM duty cycle.\n");
    // }
>>>>>>> Stashed changes
}

int main(void) {
    int ambient_temp_raw, ambient_hum_raw;
    int dome_temp_raw;
    double ambient_temp, ambient_hum;
    double dome_temp, dew_point;
<<<<<<< Updated upstream

    /* Read ambient sensor data from /dev/shtc3 */
    if (read_sensor("/dev/shtc3", &ambient_temp_raw, &ambient_hum_raw) < 0) {
        fprintf(stderr, "Error reading ambient sensor data.\n");
        return EXIT_FAILURE;
    }
    /* Convert raw values to physical units */
    ambient_temp = ambient_temp_raw / 100.0;
    ambient_hum  = ambient_hum_raw / 100.0;

    /* Compute dew point from ambient conditions */
    dew_point = compute_dew_point(ambient_temp, ambient_hum);

    /* Read dome sensor temperature from /dev/shtc3_dome.
     * For the dome sensor, assume only temperature is needed.
     */
/*    if (read_sensor("/dev/shtc3_dome", &dome_temp_raw, NULL) < 0) {
        fprintf(stderr, "Error reading dome sensor data.\n");
        return EXIT_FAILURE;
    }
    dome_temp = dome_temp_raw / 100.0;
*/
    /* Print the measured values */
    printf("Ambient Temperature: %.2f°C\n", ambient_temp);
    printf("Ambient Humidity: %.2f%%\n", ambient_hum);
    printf("Calculated Dew Point: %.2f°C\n", dew_point);
    printf("Dome Temperature: %.2f°C\n", dome_temp);

    /* Compare dome temperature with dew point.
     * If the dome temperature is at or below the dew point,
     * condensation on the dome lens is possible.
     */
    if (dome_temp <= dew_point) {
        printf("Warning: Dome lens temperature is at or below the dew point. Condensation may occur.\n");
    } else {
        printf("Dome lens temperature is above the dew point. No condensation expected.\n");
    }

   /* Adjust fan speeds based on sensor measurements */
    adjust_fans(dome_temp, dew_point);

=======
    while(true) {
        /* Clear screen for a refreshed display */
        printf("\033[H\033[J");
        /* Read ambient sensor data from /dev/shtc3 */
        if (read_sensor("/dev/shtc3", &ambient_temp_raw, &ambient_hum_raw) < 0) {
            fprintf(stderr, "Error reading ambient sensor data.\n");
            return EXIT_FAILURE;
        }
        /* Convert raw values to physical units */
        ambient_temp = ambient_temp_raw / 100.0;
        ambient_hum  = ambient_hum_raw / 100.0;

        /* Compute dew point from ambient conditions */
        dew_point = compute_dew_point(ambient_temp, ambient_hum);

        /* Read dome sensor temperature from /dev/shtc3_dome.
        * For the dome sensor, assume only temperature is needed.
        */
        if (read_sensor("/dev/shtc3_dome", &dome_temp_raw, NULL) < 0) {
            fprintf(stderr, "Error reading dome sensor data.\n");
            return EXIT_FAILURE;
        }
        dome_temp = dome_temp_raw / 100.0;

        /* Print the measured values */
        printf("Ambient Temperature: %.2f°C\n", ambient_temp);
        printf("Ambient Humidity: %.2f%%\n", ambient_hum);
        printf("Calculated Dew Point: %.2f°C\n", dew_point);
        printf("Dome Temperature: %.2f°C\n", dome_temp);

        /* Compare dome temperature with dew point.
        * If the dome temperature is at or below the dew point,
        * condensation on the dome lens is possible.
        */
        if (dome_temp <= dew_point) {
            printf("Warning: Dome lens temperature is at or below the dew point. Condensation may occur.\n");
        } else {
            printf("Dome lens temperature is above the dew point. No condensation expected.\n");
        }

        /* Adjust fan speeds based on sensor measurements */
        adjust_fans(dome_temp, dew_point);

        /* Sleep for a desired interval (e.g., 5 seconds) before repeating the loop */
        sleep(5);
    }
>>>>>>> Stashed changes
    return EXIT_SUCCESS;
}
