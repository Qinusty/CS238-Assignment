//
// Created by qinusty on 28/11/16.
//

#include <malloc.h>
#include <stdlib.h>
#include "Inglenook.h"
#include "Utils.h"

#define DEBUG

void put(int to, int n, siding_config** config, char* response);
void take(int from, int n, siding_config** config, char* response);
void craft_response(char* resp, int resp_code, siding_config* config);
void set_points(int up_until, siding_config **config);


/**
 * The primary command for siding control, pass this function a command string and a response string, the response
 * string will be written over. A conig must be initialised and provided for the function to manipulate.
 * @param command the command for the function to use
 * @param response the response for a response string to be placed in to return it.
 * @param config A pointer pointer of a config object, must be initialised.
 */
void handleCommand(char* command, char* response, siding_config** config) {
    char buff[BUFFER_SIZE] = "";
#define DELIM " "
    strcpy (buff, command);
    int num_tokens = count_occurances(*DELIM, command) + 1;
    char* token = strtok(buff, DELIM);
    if (strcmp(token, "take") == 0 && num_tokens > 2) { // if a valid take command
        int siding_num = str_to_int(strtok(NULL, DELIM));
        int wagon_count = str_to_int(strtok(NULL, DELIM));
        if (siding_num == -1 || wagon_count == -1) { // check for errors from the str_to_int command
            craft_response(response, -4, *config); // something went wrong, send command not recognised.
            return;
        }
        take(siding_num, wagon_count, config, response); // perform the action if reached this far
    } else if (strcmp(token, "put") == 0 && num_tokens > 2) {
        int siding_num = str_to_int(strtok(NULL, DELIM));
        int wagon_count = str_to_int(strtok(NULL, DELIM));
        if (siding_num == -1 || wagon_count == -1) {
            craft_response(response, -4, *config); // something went wrong, send command not recognised.
            return;
        }
        put(siding_num, wagon_count, config, response);  // perform action if reaches this far.
    } else if (strcmp(token, "load") == 0 && num_tokens > 2) { // if valid load command
        int siding = 1;
        printf("Sidings reconfigured with Load command!\n");
        // loop through sidings reading tokens.
        while (token != NULL && siding < (*config)->num_sidings) {
            int convert = str_to_int(strtok(NULL, DELIM));
            if (convert == -1) {
                craft_response(response, -4, *config);
                return;
            }
            // set the number of sidings contained.
            (*config)->siding_contains[siding++] = convert;
        }
        craft_response(response, 1, *config);
    } else if (strcmp(token, "config") == 0 && num_tokens > 2) { // config <number of sidings> <Siding 0 size (headshunt)> ... <Siding n size>
        int num_sides = str_to_int(strtok(NULL, DELIM));
        printf("Config complete, sidings rearranged, we now have %d sidings\n", num_sides);
        int i = 0;
        int sizes[num_sides];
        for (i = 0; i < num_sides; i++) {
            int convert = str_to_int(strtok(NULL, DELIM));
            if (convert == -1 || num_sides == -1){
                craft_response(response, -4, *config);
                return;
            }
            sizes[i] = convert;
        }
        (*config) = init_siding(num_sides, sizes);
        craft_response(response, num_sides, *config);
//    } else if (strcmp(token, "HELP") == 0) {
//        char buf[20] = "[";
//        for (int x = 0; x < (*config)->num_sidings; x++) {
//            char tmp[3] = "";
//            sprintf(tmp, "%d/%d", (*config)->siding_contains[x], (*config)->siding_sizes[x]);
//            strcat(buf, tmp);
//            if (x < (*config)->num_sidings - 1)
//                strcat(buf, ", ");
//        }
//        strcat(buf, "]");
//        strcpy(response, buf);
    } else {
        craft_response(response, -4, *config);
    }
#ifdef DEBUG
    printf("[ ");
    for (int i = 0; i < (*config)->num_sidings; i++)
        printf("(%d/%d) ", (*config)->siding_contains[i],(*config)->siding_sizes[i]);
    printf("]\n");
#endif
}

/**
 * Checks whether or not the move is valid between two sidings represented by their indicies.
 * @param from take from siding
 * @param to put into siding
 * @param n number of wagons to move
 * @param config the siding config
 * @return n is returned if it is a valid move. Otherwise, it returns the error code relative to the issue.
 * -3 for trying to access unknown siding
 * -2 for trying to take more wagons than a siding holds.
 * -1
 */
int valid_move(int from, int to, int n, siding_config** config) {
    if (from > (*config)->num_sidings || to > ((*config)->num_sidings))
        return -3;
    if ((*config)->siding_contains[from] < n)
        return -2;
    if ((*config)->siding_contains[to] + n > (*config)->siding_sizes[to])
        return -1;
    return n;
}

/**
 *  Initialises sidings to have n sidings and copies the array of sizes.
 */
siding_config* init_siding(int n, int* sizes) {
    siding_config* config = (siding_config*)malloc(sizeof(siding_config));
    config->num_sidings = n;
    config->siding_sizes = intdup(sizes, n);
    config->siding_contains = (int*)calloc(n, sizeof(int));
    config->points = (int*)calloc(n-2, sizeof(int));
    return config;
}

/**
 * Moves n wagons between from and to if (from or to is 0)
 * Also, sets points when the move is valid.
 * @param from siding to take from
 * @param to siding to put into
 * @param n number of wagons to move if successful
 * @param config siding config to altere
 * @param response response to be returned
 */
void move_to(int from, int to, int n, siding_config** config, char* response) {
    if (from*to != 0) printf("Something is going wrong, you're working at too high level (move_to)\n");
    int resp_code = valid_move(from, to, n, config);
    if (resp_code > 0){ // if valid
        if (from == 0)
            set_points(to, config);
        else
            set_points(from, config);
        (*config)->siding_contains[to] += n;
        (*config)->siding_contains[from] -= n;
        printf("%d -> %d [ %d Wagons moved ]\n", from, to, n);
        craft_response(response, resp_code, *config);
    } else {
        craft_response(response, resp_code, *config);
    }
}

/**
 * tries to takes n wagons from the siding with index <from>
 * @param from the index of the siding to take from
 * @param n the number of wagons to take
 * @param config the config to alter
 * @param response the response to return.
 */
void take(int from, int n, siding_config **config, char *response) {
    move_to(from, 0, n, config, response);
}

/**
 * tries to put n wagons into the siding with index <to>
 * @param to the index of the siding to put into
 * @param n the number of wagons to take
 * @param config the config to alter
 * @param response the response to return.
 */
void put(int to, int n, siding_config **config, char *response) {
    move_to(0, to, n, config, response);
}

/**
 * Takes the respcode and current config and creates an appropriate response message to be returned into the string resp
 * @param resp The string to return
 * @param resp_code The response code you wish to use
 * @param config The config to get points from.
 */
void craft_response(char *resp, int resp_code, siding_config* config) {
    int i = 0;
    printf("Creating response with code %d\n", resp_code);
    sprintf(resp, "Status %d ", resp_code);
    for (i = 0; i < config->NUM_POINTS; i++){
        if (config->points[i]==1)
            strcat(resp, "REVERSE ");
        else
            strcat(resp, "NORMAL ");
    }
    //strcat(resp, "\n");

}

/**
 * Set the points between 0 and up_until appropriately.
 * @param up_until the siding index you are taking from or putting into.
 * @param config the config to modify.
 */
void set_points(int up_until, siding_config **config) {
    int end_siding = (*config)->num_sidings-1;
    // handling points
    if (up_until == 1)
        (*config)->points[0] = 1; // special case for getting to 1st siding
    else {
        (*config)->points[0] = 0; // otherwise traverse towards to final siding

        for (int i = 1; i <= up_until - 2; i++)
            (*config)->points[i] = 0;

        if (up_until < end_siding)
            (*config)->points[up_until-1] = 1;
    }
}

/**
 * Free's all the memory allocated with init_siding
 * @param conf config to free
 */
void free_config(siding_config* conf) {
    free(conf->siding_sizes);
    free(conf->siding_contains);
    free(conf);
}
