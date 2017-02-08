//
// Created by qinusty on 20/11/16.
//

#include <iostream>
#include "client.h"
#include <boost/asio/connect.hpp>
#include <boost/lexical_cast.hpp>
//#define DEBUG
#define INPUT_BUFFER_SIZE 255

using namespace boost::asio::ip;
client::client(std::string& hostName, int _port, boost::asio::io_service& ios)
        : sock(ios) {
    port = _port;
    tcp::resolver res(ios);
    tcp::resolver::query q(hostName, std::to_string(port));
    tcp::resolver::iterator endpoint_iter = res.resolve(q);
    try {
        boost::asio::connect(sock, endpoint_iter);
    } catch (boost::system::system_error e) {
        std::cerr << "ERROR: Unable to make connection to server!" << std::endl;
        exit(0);
    }
}

client::~client() {
}

/*!
 * Converts command to take/send commands and sends them, returns
 * the number of responses to expect
 * @param command The command e.g. 3 0 2
 * @return The number of responses to expect
 */
void client::sendRecvAct(std::string &command, YardManager &yardManager) {
    using namespace boost;
    char_separator<char> sep(" ");
    tokenizer<boost::char_separator<char>> tok(command, sep);
    auto token = tok.begin();
    int from, to, number;
    try {
        from = lexical_cast<int>(*(token++));
        to = lexical_cast<int>(*(token++));;
        number = lexical_cast<int>(*(token++));;
    } catch (boost::bad_lexical_cast) {
        std::cout << "Conversion of input to integers failed, please enter 3 integers.\n"
                "In the form <from siding> <to siding> <number of wagons>" << std::endl;
    }
    // only gets here if from, to and number are valid ints
    std::ostringstream takeStream;
    std::ostringstream putStream;
    bool fromSuccess = true;
    if (from != 0) {
        takeStream << "take " << from << " " << number;
        std::cout << "Sending: " << takeStream.str() << std::endl;
        std::cout.flush();
        sock.send(asio::buffer(takeStream.str()));
        fromSuccess = awaitResponse(true, number, from, to, yardManager);
    }
    if (to != 0 && fromSuccess) {
        putStream << "put " << to << " " << number;
        std::cout << "Sending: " << putStream.str() << std::endl;
        sock.send(asio::buffer(putStream.str()));
        std::cout.flush();
        awaitResponse(false, number, from, to, yardManager);
    }

}

/*!
 * Sets the config and load of the server and then enters the loop
 * for user input and sending commands.
 * @param yardManager
 */
void client::run(YardManager& yardManager) {
    using namespace std;
    using namespace boost::asio;
    boost::array<char, INPUT_BUFFER_SIZE> resp;
    boost::system::error_code error;
    // read the intro message
    sock.read_some(buffer(resp), error);
    cout << resp.data() << std::endl;
    // send config and load
    sock.send(buffer(yardManager.getConfigCommand()));
    sock.read_some(buffer(resp), error); // read config response, ignore
    sock.send(buffer(yardManager.getLoadCommand()));
    sock.read_some(buffer(resp), error); // read load command, ignore.

    cout << "Configured Signal Box for this setup." << endl;
    while(true) {
        cout << yardManager << endl;
        string command = "";
        getline(std::cin, command);

        // prevents ctrl-d or ctrl-c causing errors with tokenizer before closing
        if (command.length() > 1) {
            sendRecvAct(command, yardManager);
            command.clear();
        }
    }
}

bool client::awaitResponse(bool take, int n,
                           int from, int to, YardManager& yardManager) {
    using namespace std;
    using namespace boost::asio;
    boost::array<char, INPUT_BUFFER_SIZE> resp;
    boost::system::error_code error;
    do {
        sock.read_some(buffer(resp), error);
    } while (resp[0] == '\000');
    string s = resp.data();
    if (responseIsSuccess(s)) {
        yardManager.updatePoints(s);
        if (take)
            yardManager.take(from, n);
        else
            yardManager.put(to, n);
        return true;
    }
    return false;

}

bool client::responseIsSuccess(std::string& resp) {
    using namespace boost;
    using namespace std;
    int respCode = 0;

#ifdef DEBUG
    cout << "Attempting to parse -> " << resp << endl;
#endif
    char_separator<char> sep(" ");
    tokenizer<char_separator<char>> respTok(resp, sep);
    auto token = respTok.begin();
    if (lexical_cast<string>(*(token++)).compare("Status") == 0) { // get
        respCode = lexical_cast<int>(*(token++));
    }
    if (respCode > 0)

        return true;
    else if (respCode == -1)
        cerr << "Not enough space in destination siding!" << endl;
    else if (respCode == -2)
        cerr << "Not enough wagons on source siding!" << endl;
    else if (respCode == -3)
        cerr << "Source or destination references a non existent siding!" << endl;
    else
        cerr << "Something is wrong with respCode (client::responseIsSuccessful)" << endl;
    return false;
}
