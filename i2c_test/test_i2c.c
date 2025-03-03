#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <stdint.h>  // For uint16_t
#include <string.h>  // For strerror()

#define I2C_DEV "/dev/i2c-1"   // Use I2C bus 1
#define SHTC3_ADDR 0x70        // SHTC3 I2C address (Check with `i2cdetect -y 1`)
#define CMD_WAKEUP   0x3517
#define CMD_MEASURE  0x7866
#define CMD_SLEEP    0xB098
volatile int recording = 1;  // 1 = Start recording, 0 = Stop
#define RECORD_DURATION 60   // Recording duration in seconds

char* get_current_timestamp() {
    static char buffer[20];
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d_%H-%M-%S", t);
    return buffer;
}


void main() {
    int fd;
    char buf[6];

    if ((fd = open(I2C_DEV, O_RDWR)) < 0) {
        fprintf(stderr, "Failed to open the i2c bus: %s\n", strerror(errno));
        fflush(stderr);
        fflush(stdout);
        return;
    }

    if (ioctl(fd, I2C_SLAVE, SHTC3_ADDR) < 0) {
        fprintf(stderr, "Failed to acquire bus access and/or talk to slave: %s\n", strerror(errno));
        fflush(stderr);
        fflush(stdout);
        close(fd);
        return;
    }
    
    printf("%s: 1 Started SHTC3 Recording \n", get_current_timestamp());
    fflush(stderr);
    fflush(stdout);
    // freopen(shtc3_log_path, "a", stdout);
    // freopen(shtc3_log_path, "a", stderr);

    while (recording) {

        char *timestamp = get_current_timestamp();

        time_t start_time = time(NULL);
        while (recording && (time(NULL) - start_time) < RECORD_DURATION) { // Collect data for 1 minute
            // Wake up sensor
            buf[0] = CMD_WAKEUP >> 8;
            buf[1] = CMD_WAKEUP & 0xFF;
            if (write(fd, buf, 2) != 2) {
                fprintf(stderr, "Failed to wake up sensor: %s\n", strerror(errno));
                close(fd);
                fflush(stderr);
                fflush(stdout);
                return;
            }
            usleep(1000);

            // Measure
            buf[0] = CMD_MEASURE >> 8;
            buf[1] = CMD_MEASURE & 0xFF;
            if (write(fd, buf, 2) != 2) {
                fprintf(stderr, "Failed to start measurement: %s\n", strerror(errno));
                close(fd);
                fflush(stderr);
                fflush(stdout);
                return;
            }
            usleep(50000);

            // Read measurement
            if (read(fd, buf, 6) != 6) {
                fprintf(stderr, "Failed to read measurement: %s\n", strerror(errno));
                close(fd);
                fflush(stderr);
                fflush(stdout);
                return;
            }

            uint16_t temp_raw = (buf[0] << 8) | buf[1];
            uint16_t humidity_raw = (buf[3] << 8) | buf[4];

            float temperature = -45.0 + 175.0 * ((float)temp_raw / 65535.0);
            float humidity = 100.0 * ((float)humidity_raw / 65535.0);

            printf("%s,%.2f,%.2f\n", get_current_timestamp(), temperature, humidity);

            // Sleep sensor
            buf[0] = CMD_SLEEP >> 8;
            buf[1] = CMD_SLEEP & 0xFF;
            if (write(fd, buf, 2) != 2) {
                fprintf(stderr, "Failed to put sensor to sleep: %s\n", strerror(errno));
                close(fd);
                fflush(stderr);
                fflush(stdout);
                return;
            }

            usleep(1000000); // Sleep for 1 seconds
        }

    }
    printf("%s: Stopped SHTC3 Recording \n", get_current_timestamp());
    // freopen(shtc3_log_path, "a", stdout);
    // freopen(shtc3_log_path, "a", stderr);
}
