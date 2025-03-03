#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "dewpoint.h"
#include "shtc3_parser.h"

int main(void) {
    int ambient_temp_raw, ambient_hum_raw;
    int dome_temp_raw;
    double ambient_temp, ambient_hum;
    double dome_temp, dew_point;

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

    return EXIT_SUCCESS;
}
