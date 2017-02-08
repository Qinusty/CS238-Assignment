//
// Created by qinusty on 20/11/16.
//

#ifndef SERVER_CLIENT_H
#define SERVER_CLIENT_H
#include <boost/asio/ip/tcp.hpp>
#include "YardManager.h"

using namespace boost::asio;
class client {
private:

    int port;
    ip::tcp::socket sock;
    void sendRecvAct(std::string &command, YardManager &yardManager);
    bool responseIsSuccess(std::string &resp);
    bool awaitResponse(bool take, int n, int from, int to, YardManager& yardManager);
public:

    client(std::string& hostName, int _port, boost::asio::io_service& ios);

    ~client();

    void run(YardManager& yardManager);
};


#endif //SERVER_CLIENT_H
