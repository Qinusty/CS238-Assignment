//
// Created by qinusty on 15/11/16.
//

#include <fstream>
#include <iostream>
#include <cstring>
#include <stack>
#include "YardManager.h"

//#define DEBUG
#define NORMAL false
#define REVERSE true

YardManager::YardManager(std::string sidingConfigPath, std::string wagonConfigPath) : sidings() {
    auto wagons = std::vector<vehicle*>();
    loadWagons(wagonConfigPath, wagons);
    loadSidings(sidingConfigPath, wagons);
    // initialise n-2 points to normal
    loadPoints();
}



YardManager::~YardManager() {
    for(const auto& i : sidings)
        delete(i);
    delete(&points);
}

/*
 * https://stackoverflow.com/questions/53849/how-do-i-tokenize-a-string-in-c
 */
void YardManager::loadWagons(std::string wagonConfigPath, std::vector<vehicle*>& wagons) {
    using namespace boost;
    std::ifstream fin(wagonConfigPath);
    std::string line;
    while(std::getline(fin, line)) {
        tokenizer<escaped_list_separator<char>> parts(line);
        tokenizer<escaped_list_separator<char>>::iterator iter = parts.begin();
        vehicle* v = new vehicle(iter);
        wagons.push_back(v);
#ifdef DEBUG
        std::cout << "New wagon added: " << *v << std::endl;
#endif
    }
    fin.close();
}


void YardManager::loadPoints() {
    // prevent from attempting to remove a chunk of memory not
    // aquired from the operating system
    if (points != NULL) {
        delete(points);
        points = NULL;
    }
    // assign points to a new chunk of memory with suitable space for the current amount
    // of sidings.
    points = new bool[sidings.size()-2];
    for (size_t x = 0; x < sidings.size()-2; x++)
        points[x] = NORMAL;
}

/*!
 *
 * @param sidingConfigPath
 * @param wagons
 */
void YardManager::loadSidings(std::string sidingConfigPath, std::vector<vehicle*>& wagons) {
    using namespace boost;
    std::ifstream fin(sidingConfigPath);
    std::string line;
    while (std::getline(fin,line)) {
        char_separator<char> toks(" \t");
        tokenizer<char_separator<char>> parts(line, toks);
        auto iter = parts.begin();
        // create a new siding with the iterator and lookup wagons
        auto s = new siding(iter, wagons);
        sidings.push_back(s);
#ifdef DEBUG
        std::cout << "New Siding created: " << *s << std::endl;
#endif
    }
    fin.close();
}

/*!
 * Iterates through the sidings and configures the string to apropriately represent the sizes
 * of the sidings.
 * @return A configuration string to be sent to the server, this will sync the two configurations.
 */
std::string YardManager::getConfigCommand() {
    // config <num of sides> <siding 0 capacity> ... <siding n capacity>
    std::string configCommand("config ");
    configCommand.append(std::to_string(sidings.size()));
    auto iter = sidings.begin();
    while (iter != sidings.end()) {
        configCommand += " " + std::to_string((*iter++)->getMaxLength());
    }
    return configCommand;
}


std::string YardManager::getLoadCommand() { // TODO: Perhaps fix for headshunt (also in SBox)
    std::string loadCommand("load");
    auto iter = sidings.begin();
    iter++;
    for (size_t i = 1; i < sidings.size(); i++) {
        loadCommand += " " + std::to_string(sidings[i]->getWagonCount());
    }
    return loadCommand;
}

std::ostream &operator<<(std::ostream &os, const YardManager &yardManager) {
    // TODO: Comment this
    using namespace std;
    int padLength = yardManager.sidings[0]->getMaxLength() *6 + 1;
    string padding(padLength, ' ');
    os << yardManager.sidings[0]->prettyString(true) << "\\" << endl;
    for (size_t i=1; i < yardManager.sidings.size(); i++) {
        os << padding;
        if (i < yardManager.sidings.size()-1)
            if (yardManager.points[i-1] == NORMAL)
                os << '|';
            else
                os << "\\";
        else
            os << "\\";

        os << yardManager.sidings[i]->prettyString() << endl;
        if (i < yardManager.sidings.size()-1)
            os << padding << "|" << endl;
    }
    return os;
}

void YardManager::put(int sidingNo, int num) {
    for (int i = 0; i < num; i++) {
        sidings[sidingNo]->addWagon(sidings[0]->pop());
    }
}

void YardManager::take(int sidingNo, int num) {
    for (int i = 0; i < num; i++) {
        sidings[0]->addWagon(sidings[sidingNo]->pop());
    }
}

/*!
 * Sets the points apppropriately based on a standard response from the server.
 * @param response The response from the server.
 */
void YardManager::updatePoints(std::string &response) {
    using namespace boost;
    char_separator<char> sep(" ");
    tokenizer<char_separator<char>> respTok(response, sep);
    auto token = respTok.begin();
    token++; // skip Status
    token++; // skip response code

    // going into this the first token is a point
    for (size_t i = 0; i < sidings.size()-2; i++) {
        std::string curStr = *(token++);
        if (curStr.compare("NORMAL") == 0)
            points[i] = NORMAL;
        else if (curStr.compare("REVERSE") == 0)
            points[i] = REVERSE;
        else
            std::cout << "Something is wrong with updatePoints when parsing > "
                      << response << std::endl;
    }
}


