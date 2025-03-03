#ifndef FAN_CONTROL_H
#define FAN_CONTROL_H

/* PWM period in microseconds (example value) */
#define PWM_PERIOD 1000000UL

/* Define PWM sysfs file paths for the two fans */
#define INLET_PWM_PATH   "/sys/class/pwm/pwmchip0/pwm0/duty_cycle"
#define EXHAUST_PWM_PATH "/sys/class/pwm/pwmchip0/pwm1/duty_cycle"

/* 
 * set_pwm_duty - Set the PWM duty cycle for a given PWM channel.
 *
 * @path:  The sysfs path to the PWM channel's duty_cycle file.
 * @duty:  The duty cycle in microseconds (0 <= duty <= PWM_PERIOD).
 *
 * Returns 0 on success, or -1 on failure.
 */
int set_pwm_duty(const char *path, unsigned long duty);

#endif /* FAN_CONTROL_H */
