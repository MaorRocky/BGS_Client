#include "connectionHandler.h"

#ifndef BOOST_ECHO_CLIENT_FROMKEYBOARD_H
#define BOOST_ECHO_CLIENT_FROMKEYBOARD_H

class FromKeyBoard {
public:
    FromKeyBoard(ConnectionHandler &handler);

    void operator()();

private:
    ConnectionHandler handler;
    bool isTerminate;
};

#endif //BOOST_ECHO_CLIENT_FROMKEYBOARD_H

