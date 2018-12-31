#include "connectionHandler.h"

#ifndef BOOST_ECHO_CLIENT_FROMSERVER_H
#define BOOST_ECHO_CLIENT_FROMSERVER_H

class FromServer {
public:
    FromServer(ConnectionHandler &_handler);

private:
    ConnectionHandler handler;
};

#endif //BOOST_ECHO_CLIENT_FROMSERVER_H
