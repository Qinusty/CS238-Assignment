//
// Created by qinusty on 28/11/16.
//
#include <stdlib.h>
#include <limits.h>
#include <errno.h>

#include "Utils.h"

/**
 * Duplicates an array of ints.
 * @param src The int array to duplicate
 * @param len The length of the array to duplicate.
 * @return The new pointer to the start of the array.
 */
int * intdup(int const * src, size_t len)
{
    int * p = malloc(len * sizeof(int));
    memcpy(p, src, len * sizeof(int));
    return p;
}

/**
 * CREDIT: https://stackoverflow.com/questions/8871711/atoi-how-to-identify-the-difference-between-zero-and-error
 * 
 * WILL FAIL IF PASSED NEGATIVE INT
 * @param in_str
 * @return -1 if anything goes wrong. Otherwise the positive integer 
 * representation is returned.
 */
int str_to_int(char* in_str) {
    long lnum;
    char *end;

    errno = 0;

    lnum = strtol(in_str, &end, 10);        //10 specifies base-10
    if (end == in_str)     //if no characters were converted these pointers are equal
        return -1;

//If sizeof(int) == sizeof(long), we have to explicitly check for overflows
    if ((lnum == LONG_MAX || lnum == LONG_MIN) && errno == ERANGE)
        return -1;

//Because strtol produces a long, check for overflow
    if ( (lnum > INT_MAX) || (lnum < INT_MIN) )
        return -1;

//Finally convert the result to a plain int (if that's what you want)
    return (int)lnum;
}

/**
 * Counts the occurrences of c within cs
 * @param c The character to search for.
 * @param str The string to search
 * @return The number of times c is within str
 */
int count_occurances(char c, char* str) {
    int x = 0;
    while (*str != '\0')
        if (*(str++) == c)
            x++;
    return x;
}