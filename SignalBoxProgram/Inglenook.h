//
// Created by qinusty on 28/11/16.
//

#ifndef SIGNALBOXPROGRAM_INGLENOOK_H
#define SIGNALBOXPROGRAM_INGLENOOK_H


#define BUFFER_SIZE 255
#define NUM_POINTS num_sidings-2

typedef struct ingle_struct {
    /* Number of sidings WITH the head shunt */
    int num_sidings;
    int* siding_sizes; // The maximum size of each siding
    int* siding_contains; // The current number of wagons in each siding.
    // An array of booleans representing the state of points (switches) which should have the size of num_siding - 2
    int* points; // 1 is reverse // 0 is normal
} siding_config;

void handleCommand(char* command, char* response, siding_config** config);
siding_config* init_siding(int n, int* sizes);
#endif //SIGNALBOXPROGRAM_INGLENOOK_H
