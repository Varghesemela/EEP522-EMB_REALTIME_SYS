#include "shtc3_parser.h"
#include <stdio.h>

/* Helper function to read a line from a device file and parse temperature and humidity.
 * The expected format is:
 *    "Temperature: <temp>, Humidity: <hum>"
 * where <temp> and <hum> are integer values.
 * If humidity is not present (for dome sensor) you may pass a NULL pointer for hum.
 */
int read_sensor(const char *dev_path, int *temp, int *hum) {
    FILE *fp;
    char buf[128];

    fp = fopen(dev_path, "r");
    if (!fp) {
        perror(dev_path);
        return -1;
    }

    if (!fgets(buf, sizeof(buf), fp)) {
        perror("fgets");
        fclose(fp);
        return -1;
    }
    fclose(fp);

    if (hum) {
        /* Parse both temperature and humidity */
        if (sscanf(buf, "Temperature: %d, Humidity: %d", temp, hum) != 2) {
            fprintf(stderr, "Failed to parse sensor data from %s\n", dev_path);
            return -1;
        }
    } else {
        /* Parse only temperature */
        if (sscanf(buf, "Temperature: %d", temp) != 1) {
            fprintf(stderr, "Failed to parse temperature from %s\n", dev_path);
            return -1;
        }
    }
    return 0;
}
