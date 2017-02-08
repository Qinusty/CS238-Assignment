//
// Created by qinusty on 15/11/16.
//

#include <fstream>
#include "Utils.h"

char* readWholeFile(char* path, char* buffer) {
    std::ifstream fin(path);
    // get pointer to associated buffer object
    std::filebuf* pbuf = fin.rdbuf();
    // get file size using buffer's members
    std::size_t size = pbuf->pubseekoff (0,fin.end,fin.in);
    pbuf->pubseekpos (0,fin.in);
    // get file data
    pbuf->sgetn (buffer,size);
    fin.close();
    return buffer;
}

