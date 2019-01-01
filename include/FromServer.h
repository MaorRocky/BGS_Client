#include <stdlib.h>
#include <connectionHandler.h>
#include <mutex>
#include <thread>
#include <bits/stdc++.h>
#include <boost/algorithm/string.hpp>
#include <iostream>
#include <iostream>
#include <iterator>
#include <string>
#include <regex>

#ifndef BOOST_ECHO_CLIENT_FROMSERVER_H
#define BOOST_ECHO_CLIENT_FROMSERVER_H

class FromServer {
public:
    FromServer(ConnectionHandler* handler, bool isTerminate);
    void operator()();
private:
    bool isTerminate;
    ConnectionHandler* handler;
    void getNextBytesPart(std::vector<char> bytes);
    short bytesToShort(char* bytesArr);
    void bytesArrayToString(std::vector<char> bytes, std::string str, int start);

};

#endif //BOOST_ECHO_CLIENT_FROMSERVER_H
