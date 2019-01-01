#include "connectionHandler.h"

#ifndef BOOST_ECHO_CLIENT_FROMSERVER_H
#define BOOST_ECHO_CLIENT_FROMSERVER_H

class FromServer {
public:
    FromServer(ConnectionHandler &handler, bool isTerminate);
    void operator()();
private:
    bool isTerminate;
    ConnectionHandler handler;
    int getNextBytesPart(char bytes[], int toStart);
    short bytesToShort(char* bytesArr);
    void bytesArrayToString(char bytes[], std::string str, int start);

};

#endif //BOOST_ECHO_CLIENT_FROMSERVER_H
