//
// Created by qinusty on 15/11/16.
//

#include <boost/algorithm/string/trim.hpp>
#include <iostream>
#include "siding.h"

siding::siding(boost::tokenizer<boost::char_separator<char>>::iterator &tok,
               std::vector<vehicle*> storedWagons) : wagons() {
    length = std::stoi(*tok); // get the max length of siding
    tok++;
    // Amount of serial numbers coming up.
    int counter = std::stoi(*tok);
    tok++;
    while (counter > 0) {
        int serialNo = std::stoi(*tok);
        for (auto& v: storedWagons) { // find the wagon with this serial number in storedWagons
            if (v->getSerialNumber() == serialNo) {
                addWagon(v); // add it to the wagons vector.
                break;
            }
        }
        counter--; tok++;
    }
}
siding::~siding() {
    for (auto v : wagons) {
        delete(v);
    }
}

int siding::addWagon(vehicle* v) {
    if (wagons.size() < length) {
        wagons.push_back(v);
        return 1;
    } else
        return 0;
}

/*!
 * \brief helper function used to create polymorphic behaviour for stream insertion.
 *
 * \return the string representation of this siding
 */
std::string siding::streamHelper() const {

    std::stringstream os;
    os << "Siding of capacity: " << length << " Number of wagons: " << wagons.size()
       << std::endl;

    for (auto v : wagons)
        os << *v << std::endl;
    return os.str();
}

std::basic_ostream<char>& operator<<(
        std::basic_ostream<char>& os, const siding& s) {

    return os << s.streamHelper();
}

std::string siding::prettyString(bool headshunt) {
    std::string pretty;
    for (size_t i = length-1; i >= 0; i--) {
        pretty.append("=");
        if (i < wagons.size()) {
            if (headshunt) // if its a headshunt, print the wagons right to left
                pretty.append(std::to_string(wagons[wagons.size()-1-i]->getSerialNumber()));
            else          // else print left to right
                pretty.append(std::to_string(wagons[i]->getSerialNumber()));

        } else {
            pretty.append("=====");
        }
        if (i==0) // this prevents i from overflowing and therefore causing infinite loop.
            break;
    }
    return pretty;
}

int siding::getMaxLength() const {
    return length;
}

vehicle *siding::pop() {
    auto wag = wagons.back(); // take back
    wagons.pop_back(); // remove back
    return wag;
}



