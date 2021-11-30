#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>


clock_t start;
FILE *angular_z_angular_velocity_file;
char *z_delta_buff;

void cleanup()
{
    fclose(angular_z_angular_velocity_file);
    free(z_delta_buff);
}

int main(int argc, char **argv) {
    start = clock();
    if ((angular_z_angular_velocity_file = fopen("/sys/bus/iio/devices/iio:device3/in_anglvel_z_raw", "r")) == 0) {
        fprintf(stderr, "Error accessing internal gyro.");
        return EXIT_FAILURE;
    }
    z_delta_buff = malloc(256);
    long z_delta = 0;

    while (true) {
        if (fread(z_delta_buff, 1, 256, angular_z_angular_velocity_file) == 0) {
            fprintf(stderr, "Error reading internal gyro position.");
            cleanup();
            return EXIT_FAILURE;
        }
        z_delta = strtol(z_delta_buff, NULL, 10);
        printf("%ld\t%ld\n", (clock() - start) / 1000, z_delta);
        sleep(.8);
        // TODO: Use memset(z_delta_buff, 0, 256) if needed
        z_delta_buff[0] = '\0';
        // Seek back to the start of the file since the file is not continuous.
        // It emits a new value on each read.
        fseek(angular_z_angular_velocity_file, 0, SEEK_SET);
    }

    cleanup();
    return EXIT_SUCCESS;
}

