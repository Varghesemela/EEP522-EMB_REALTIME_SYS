#include "fan_control.h"
#include <stdio.h>

/*
 * set_pwm_duty - Set the PWM duty cycle for a given PWM channel.
 *
 * This function opens the provided sysfs file (e.g., for the inlet or exhaust fan),
 * writes the desired duty cycle (in microseconds), and closes the file.
 */
int set_pwm_duty(const char *path, unsigned long duty)
{
    FILE *fp = fopen(path, "w");
    if (!fp) {
        perror(path);
        return -1;
    }
    /* Write the duty cycle value */
    fprintf(fp, "%lu", duty);
    fclose(fp);
    return 0;
}
