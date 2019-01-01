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

    ConnectionHandler connectionHandler(host, port);
    if (!connectionHandler.connect()) {
        std::cerr << "Cannot connect to " << host << ":" << port << std::endl;
        return 1;
    }

    FromKeyBoard fromKeyBoard(connectionHandler);
    FromServer fromServer(connectionHandler, false);

    fromKeyBoard.operator()();
    fromServer.operator()();

    //std::thread th1(fromKeyBoard);
    //std::thread th2(fromServer);
    //th1.join();
    //th2.join();








    //From here we will see the rest of the ehco client implementation:
    /*while (1) {
        const short bufsize = 1024;
        char buf[bufsize];
        std::cin.getline(buf, bufsize);
        std::string line(buf);
        int len = line.length();
        if (!connectionHandler.sendLine(line)) {
            break;
        }
        // connectionHandler.sendLine(line) appends '\n' to the message. Therefor we send len+1 bytes.
        std::cout << "Sent " << len + 1 << " bytes to server" << std::endl;


        // We can use one of three options to read data from the server:
        // 1. Read a fixed number of characters
        // 2. Read a line (up to the newline character using the getline() buffered reader
        // 3. Read up to the null character
        std::string answer;
        // Get back an answer: by using the expected number of bytes (len bytes + newline delimiter)
        // We could also use: connectionHandler.getline(answer) and then get the answer without the newline char at the end
        if (!connectionHandler.getLine(answer)) {
            break;
        }

        len = answer.length();
        // A C string must end with a 0 char delimiter.  When we filled the answer buffer from the socket
        // we filled up to the \n char - we must make sure now that a 0 char is also present. So we truncate last character.
        answer.resize(len - 1);
        std::cout << "Reply: " << answer << " " << len << " bytes " << std::endl << std::endl;
    }
    return 0;*/
}
