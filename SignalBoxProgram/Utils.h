//
// Created by qinusty on 28/11/16.
//

#ifndef SIGNALBOXPROGRAM_UTILS_H
#define SIGNALBOXPROGRAM_UTILS_H

#include <glob.h>
#include <malloc.h>
#include <string.h>

int * intdup(int const * src, size_t len);
int str_to_int(char* in_str);
int count_occurances(char c, char* cs);
#endif //SIGNALBOXPROGRAM_UTILS_H
