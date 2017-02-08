/* 
 * File:   vehicle.cpp
 * 
 * \brief implementation of a basic vehicle class.
 * 
 * \author Neal Snooke
 *
 * Created on 05 October 2016, 15:59
 */
#include <iostream>
#include <string>

#include "vehicle.h"

/*!
 * \brief tokenizer based constructor
 */
vehicle::vehicle(boost::tokenizer<boost::escaped_list_separator<char> >::iterator &data) {

    serial_no_ = std::stoi(*data);
    ++data;

    owner_ = *data;
    ++data;
}

/*!
 * \brief change owner
 * 
 * \param owner - the new owner
 */
void vehicle::changeOwner(const std::string owner) {
    owner_ = owner;
}

/*!
 * \brief helper function used to create polymorphic behaviour for stream insertion.  
 * 
 * \return the string representation of this vehicle
 */
std::string vehicle::streamHelper() const {

    std::stringstream os;
    os << "Vehicle number: " << serial_no_ << " Owner: " << owner_;
    return os.str();
}

/*
 * \brief insertion operator overload required for stream output template instantiation
 *  of a vehicle type 
 * 
 * \param os output stream 
 * \param v is the vehicle to be inserted in the stream
 * \return output stream 
 */
std::basic_ostream<char>& operator<<(
        std::basic_ostream<char>& os, const vehicle& v) {

    return os << v.streamHelper();
}


