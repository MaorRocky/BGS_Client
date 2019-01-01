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

private:
    ConnectionHandler* handler;
    bool isTerminate;
    void shortToBytes(short num, char bytesArr[]);
};
#endif //BOOST_ECHO_CLIENT_FROMKEYBOARD_H

