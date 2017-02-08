
#include <boost/asio.hpp>
#include <iostream>

#include "YardManager.h"
#include "client.h"
#define PORT 10090
char const *wagPositions = "config/wagon_positions1.txt";
char const *wagData = "config/wagon_data.txt";
//#define DEBUG

int main() {
    using namespace boost::asio;
    char input[16];
    std::cin.getline(input, 16);
    std::string host(input);
    YardManager yM(wagPositions, wagData);
    std::cout << "starting client to : " << host << ":" <<PORT << std::endl;
    boost::asio::io_service ios;
    client c(host, PORT, ios);
    c.run(yM);
}

