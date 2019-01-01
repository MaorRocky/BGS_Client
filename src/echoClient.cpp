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

    ConnectionHandler* connectionHandler = new ConnectionHandler(host, port);
    if (!(*connectionHandler).connect()) {
        std::cerr << "Cannot connect to " << host << ":" << port << std::endl;
        return 1;
    }

    FromKeyBoard fromKeyBoard(connectionHandler);
    FromServer fromServer(connectionHandler, false);

    //fromKeyBoard.operator()();
    //fromServer.operator()();

    std::thread th1(std::ref(fromKeyBoard));
    std::thread th2(std::ref(fromServer));

    th1.join();
    th2.join();

}
