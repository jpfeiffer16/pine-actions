#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

const char TORCH_ON = '1';

FILE *z_ang_vel_fd;
FILE *torch_fd;
char *z_delta_buff;
char *torch_stat_buff;

void cleanup()
{
    fclose(z_ang_vel_fd);
    fclose(torch_fd);
    free(z_delta_buff);
    free(torch_stat_buff);
}

int main(int argc, char **argv) {
    struct timespec sleep_time = {0, 50000000};
    /* clock_t start = clock(); */
    clock_t first_shake = 0;
    clock_t bottomed = 0;

    if ((z_ang_vel_fd = fopen("/sys/bus/iio/devices/iio:device2/in_anglvel_z_raw", "r")) == 0) {
        fprintf(stderr, "Error accessing internal gyro.");
        return EXIT_FAILURE;
    }

    if ((torch_fd = fopen("/sys/class/leds/white:flash/brightness", "r+")) == 0) {
        fprintf(stderr, "Error accessing internal torch led.");
        return EXIT_FAILURE;
    }
    
    z_delta_buff = malloc(256);
    torch_stat_buff = malloc(1);
    long z_delta = 0;

    while (true) {
        if (fread(z_delta_buff, 1, 256, z_ang_vel_fd) == 0) {
            fprintf(stderr, "Error reading internal gyro position.");
            cleanup();
            return EXIT_FAILURE;
        }
        z_delta = strtol(z_delta_buff, NULL, 10);
        /* printf("%ld\t%ld\n", (clock() - start) / 1000, z_delta); */
        fflush(stdout);
        if (first_shake) {
            clock_t current_time = clock();
            if (current_time - first_shake < 20000) {
                if (z_delta > 6000 && bottomed) {
                    first_shake = 0;
                    printf("%s", "Change");
                    if (fread(torch_stat_buff, 1, 1, torch_fd) == 0) {
                        fprintf(stderr, "Error reading internal torch state.");
                        cleanup();
                        return EXIT_FAILURE;
                    }
                    fseek(torch_fd, 0, SEEK_SET);
                    char torch_write_stat[2] = "1\n";
                    if (torch_stat_buff[0] == TORCH_ON) {
                        torch_write_stat[0] = '0';
                    }
                    printf(" %c\n", torch_write_stat[0]);
                    if (fwrite(torch_write_stat, 1, 2, torch_fd) == 0) {
                        // TODO: Put this all in a function
                        fprintf(stderr, "Error writing internal torch state.\n");
                        fprintf(stderr, "%d: %s\n", errno, strerror(errno));
                        cleanup();
                        return EXIT_FAILURE;
                    }
                    fseek(torch_fd, 0, SEEK_SET);
                }
            } else {
                first_shake = 0;
                bottomed = 0;
            }
        }
        if (z_delta > 15000 && !first_shake) {
            first_shake = clock();
        }
        if (z_delta < 1 && first_shake) {
            bottomed = clock();
        }
        nanosleep(&sleep_time, NULL);
        // TODO: Use memset(z_delta_buff, 0, 256) if needed
        z_delta_buff[0] = '\0';
        // Seek back to the start of the file since the file is not continuous.
        // It emits a new value on each read.
        fseek(z_ang_vel_fd, 0, SEEK_SET);
    }

    cleanup();
    return EXIT_SUCCESS;
}

