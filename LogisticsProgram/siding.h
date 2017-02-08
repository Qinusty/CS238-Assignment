//
// Created by qinusty on 15/11/16.
//

#ifndef SERVER_SIDING_H
#define SERVER_SIDING_H


#include "vehicle.h"

class siding {
    friend std::basic_ostream<char>& operator<<(
            std::basic_ostream<char>&, const siding&);
private:
    size_t length;
    std::vector<vehicle*> wagons;

public:
    siding(boost::tokenizer<boost::char_separator<char>>::iterator &tok, std::vector<vehicle*> storedWagons);
    virtual ~siding();
    int addWagon(vehicle* v);
    bool isEmpty() { return wagons.empty(); }
    bool isFull() { return wagons.size() >= length; }
    int getWagonCount() { return wagons.size(); }
    int getMaxLength() const;
    std::string prettyString(bool headshunt=false);
    vehicle* pop();
    virtual std::string streamHelper() const;
};


#endif //SERVER_SIDING_H
