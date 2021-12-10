#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

FILE *x_ang_vel_fd;
FILE *y_ang_vel_fd;
FILE *z_ang_vel_fd;
char *x_delta_buff;
char *y_delta_buff;
char *z_delta_buff;

void cleanup()
{
    free(x_delta_buff);
    free(y_delta_buff);
    free(z_delta_buff);
    fclose(x_ang_vel_fd);
    fclose(y_ang_vel_fd);
    fclose(z_ang_vel_fd);
}

int exit_error(char *msg) {
    fprintf(stderr, "%s\n", msg);
    fprintf(stderr, "%d: %s\n", errno, strerror(errno));
    cleanup();
    return EXIT_FAILURE;
}

int main() {
    struct timespec sleep_time = {0, 50000000};
    long x_delta, y_delta , z_delta = 0;

    x_delta_buff = malloc(256);
    y_delta_buff = malloc(256);
    z_delta_buff = malloc(256);

    // TODO: Find a way to dynamically look this path up so that it doesn't break when 
    // udev assignes the device to a different file.
    if ((x_ang_vel_fd = fopen("/sys/bus/iio/devices/iio:device2/in_anglvel_x_raw", "r")) == 0) {
        return exit_error("Error accessing internal x gyro.");
    }
    if ((y_ang_vel_fd = fopen("/sys/bus/iio/devices/iio:device2/in_anglvel_y_raw", "r")) == 0) {
        return exit_error("Error accessing internal y gyro.");
    }
    if ((z_ang_vel_fd = fopen("/sys/bus/iio/devices/iio:device2/in_anglvel_z_raw", "r")) == 0) {
        return exit_error("Error accessing internal z gyro.");
    }

    while (true) {
        if (fread(x_delta_buff, 1, 256, x_ang_vel_fd) == 0) {
            return exit_error("Error reading internal gyro x position.");
        }
        x_delta = strtol(x_delta_buff, NULL, 10);

        if (fread(y_delta_buff, 1, 256, y_ang_vel_fd) == 0) {
            return exit_error("Error reading internal gyro y position.");
        }
        y_delta = strtol(y_delta_buff, NULL, 10);

        if (fread(z_delta_buff, 1, 256, z_ang_vel_fd) == 0) {
            return exit_error("Error reading internal gyro z position.");
        }
        z_delta = strtol(z_delta_buff, NULL, 10);

        fseek(x_ang_vel_fd, 0, SEEK_SET);
        fseek(y_ang_vel_fd, 0, SEEK_SET);
        fseek(z_ang_vel_fd, 0, SEEK_SET);

        printf("%ld\t%ld\t%ld\n", x_delta, y_delta, z_delta);

        nanosleep(&sleep_time, NULL);
    }

    cleanup();
    return EXIT_SUCCESS;
}
