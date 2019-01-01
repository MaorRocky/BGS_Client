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

#ifndef BOOST_ECHO_CLIENT_FROMKEYBOARD_H
#define BOOST_ECHO_CLIENT_FROMKEYBOARD_H

class FromKeyBoard {
public:
    FromKeyBoard(ConnectionHandler* handler);
    void operator()();
    void shortToBytes(short num, char bytesArr[]);
    void merge(std::vector<char> first, char second[]);

private:
    ConnectionHandler* handler;
    bool isTerminate;
};
#endif //BOOST_ECHO_CLIENT_FROMKEYBOARD_H

