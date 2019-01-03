#include <stdlib.h>
#include <connectionHandler.h>
#include <FromKeyBoard.h>
#include <FromServer.h>
#include <iostream>
#include <thread>

/**
* This code assumes that the server replies the exact text the client sent it (as opposed to the practical session example)
*/
int main(int argc, char *argv[]) {
    std::string host = "127.0.0.1";
    short port = 7777;
    bool terminate(false);

    ConnectionHandler* connectionHandler = new ConnectionHandler(host, port);
    if (!(*connectionHandler).connect()) {
        std::cerr << "Cannot connect to " << host << ":" << port << std::endl;
        return 1;
    }
    FromKeyBoard fromKeyBoard(connectionHandler);
    std::thread th1(std::ref(fromKeyBoard));

    std::string toPrint("");
    while (!terminate) {
        connectionHandler->getLine(toPrint);
        cout << toPrint << endl;
       /* if (toPrint.substr(4,5) == "3") {
            terminate = true;
        }*/
    }







}


