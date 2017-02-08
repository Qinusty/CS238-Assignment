/* 
 * File:   vehicle.h
 * 
 * \brief definition of a class to represent a basic vehicle.
 * 
 * \author Neal Snooke
 *
 * Created on: 05 October 2016, 15:03
 */

#ifndef VEHICLE_H
#define	VEHICLE_H

#include <boost/tokenizer.hpp>
#include <sstream> 

/*!
 * \brief Class to represent a basic vehicle.
 */
class vehicle {
    friend std::basic_ostream<char>& operator<<(
            std::basic_ostream<char>&, const vehicle&);

public:

    /*
     * \brief default constructor.
     */
    vehicle() : serial_no_(-1), owner_("unknown owner") {
    }

    /*
     * \brief constructor.
     * \param serial_number - a 5 digit integer (first digit non zero)
     * \param owner - the owner of the vehicle
     */
    vehicle(const int serial_number, const std::string owner)
    : serial_no_(serial_number), owner_(owner) {
    }

    virtual ~vehicle() {};

    /*!
     * \brief tokenizer constructor.
     * \param data - a tokeniser iterator. Allow the Vehicle to be construct ed from
     *               the next two tokens. 
     *               token 1 must be an integer token two a string.
     * this function will throw run time exceptions if the iterator fails 
     * this function will throw run time exceptions if the integer conversion fails
     */
    vehicle(boost::tokenizer<boost::escaped_list_separator<char> >::iterator &data);

    /*!
     * \brief getter for wagon number
     * \return the serial number
     */
    int getSerialNumber() const {
        return serial_no_;
    }

    /*!
     * \brief change owner
     * \param owner - the new owner
     */
    void changeOwner(const std::string owner);

protected:
    /*!
     * \brief helper function used to create polymorphic behaviour for stream insertion.
     *        Overrides of this function should call the parent class version and then
     *        add information from this class to it. 
     *        The insertion operator for each subclass then only needs to call 
     *        this function (see operator<< below) for an example.    
     */
    virtual std::string streamHelper() const;

private:

    /*!
     * \brief serial number of the vehicle 
     */
    int serial_no_;

    /*!
     * \brief owner 
     */
    std::string owner_;
};

#endif	/* VEHICLE_H */

