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

using namespace std;
using namespace boost;


/**
* This code assumes that the server replies the exact text the client sent it (as opposed to the practical session example)
*/
int main(int argc, char *argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " host port" << std::endl << std::endl;
        return -1;
    }
    std::string host = argv[1];
    short port = atoi(argv[2]);

    ConnectionHandler connectionHandler(host, port);
    if (!connectionHandler.connect()) {
        std::cerr << "Cannot connect to " << host << ":" << port << std::endl;
        return 1;
    }
    const std::vector<char> vector;

    //From here we will see the rest of the ehco client implementation:
    while (1) {
        const short bufsize = 1024;
        char buf[bufsize];
        std::cin.getline(buf, bufsize);
        std::string line(buf);
        int len = line.length();
        std::regex REGISTER("REGISTER",
                            std::regex_constants::ECMAScript | std::regex_constants::icase);
        std::regex LOGIN("LOGIN",
                         std::regex_constants::ECMAScript | std::regex_constants::icase);
        std::regex LOGOUT("LOGOUT",
                          std::regex_constants::ECMAScript | std::regex_constants::icase);
        std::regex FOLLOW("FOLLOW",
                          std::regex_constants::ECMAScript | std::regex_constants::icase);
        std::regex POST("POST",
                        std::regex_constants::ECMAScript | std::regex_constants::icase);
        std::regex PM("PM",
                      std::regex_constants::ECMAScript | std::regex_constants::icase);
        std::regex USERLIST("USERLIST",
                            std::regex_constants::ECMAScript | std::regex_constants::icase);
        std::regex STAT("STAT",
                        std::regex_constants::ECMAScript | std::regex_constants::icase);

        std::vector<string> results;
        string toSend;
        char delimiter = '\0';
        split(results, line, is_any_of(" "));
        if (std::regex_search(line, REGISTER)) {
            char opcode = '1';
            toSend += opcode;
            toSend += results[1];
            toSend += delimiter;
            toSend += results[2];
        } else if (std::regex_search(line, LOGIN)) {
            char opcode = '2';
            toSend += opcode;
            toSend += results[1];
            toSend += delimiter;
            toSend += results[2];
        } else if (std::regex_search(line, LOGOUT)) {
            char opcode = '3';
            toSend += opcode;
        } else if (std::regex_search(line, FOLLOW)) {
            char opcode = '4';
            toSend += opcode;
            char isFollow;
            if (results[1] == "0") {
                isFollow = '0';
            } else if (results[1] == "1") {
                isFollow = '1';
            }
            toSend += isFollow;
            short numOfUsers = (short) stoi(results[2]);
            toSend += numOfUsers;
            for (int i = 3; i < results.size(); i++) {
                toSend += results[i];
                if (i < results.size() - 1) {
                    toSend += delimiter;
                }
            }
        } else if (std::regex_search(line, POST)) {
            char opcode = '5';
            toSend += opcode;
            for (int i = 1; i < results.size(); i++) {
                toSend += results[i];
                if (i < results.size() - 1) {
                    toSend += " ";
                }
            }
        } else if (std::regex_search(line, PM)) {
            char opcode = '6';
            toSend += opcode;
            toSend += results[1];
            for (int i = 2; i < results.size(); i++) {
                toSend += results[i];
                if (i < results.size() - 1) {
                    toSend += " ";
                }
            }
        } else if (std::regex_search(line, USERLIST)) {
            char opcode = '7';
            toSend += opcode;
        } else if (std::regex_search(line, STAT)) {
            char opcode = '8';
            toSend += opcode;
            toSend += results[1];
        }

        if (!connectionHandler.sendLine(toSend)) {
            std::cout << "Disconnected. Exiting...\n" << std::endl;
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
            std::cout << "Disconnected. Exiting...\n" << std::endl;
            break;
        }


        short opcode = ((short) std::stoi(answer.substr(0, 2)));
        string toPrint;

        switch (opcode) {
            case 9:
                toPrint += "NOTIFICATION ";
                if (answer[2] == '0') {
                    toPrint += "PM";
                } else if (answer[2] == '1') {
                    toPrint += "Public";
                }
                toPrint += answer.substr(3, answer.size() - 1);
                answer = "";
                if (!connectionHandler.getLine(answer)) {
                    std::cout << "Disconnected. Exiting...\n" << std::endl;
                    break;
                }
                toPrint += answer.substr(0, answer.size() - 1);

            case 10:
                toPrint += "ACK ";
                if (std::stoi(answer.substr(2, 4)) == 4) {
                    toPrint += answer.substr(2, 3);
                    toPrint += " ";
                    toPrint += answer.substr(4, 6);
                    toPrint += " ";
                    toPrint += answer.substr(6, answer.size() - 1);
                    int numOfUsers = std::stoi(answer.substr(4, 6));
                    answer = "";
                    for (int i = 1; i <= numOfUsers; i++) {
                        if (!connectionHandler.getLine(answer)) {
                            std::cout << "Disconnected. Exiting...\n" << std::endl;
                            break;
                        }
                        toPrint += answer;
                        if (i < numOfUsers) {
                            toPrint += " ";
                        }
                        answer = "";
                    }
                } else if(std::stoi(answer.substr(2, 4)) == 3) {
                   /*TODO we need to end the thread*/
                }
                else if (std::stoi(answer.substr(2, 4)) == 6) {
                    toPrint += answer.substr(2, 4);
                    toPrint += " ";
                    toPrint += answer.substr(4, 6);
                    toPrint += " ";
                    int numOfUsers = std::stoi(answer.substr(4, 6));
                    for (int i = 1; i <= numOfUsers; i++) {
                        if (!connectionHandler.getLine(answer)) {
                            std::cout << "Disconnected. Exiting...\n" << std::endl;
                            break;
                        }
                        toPrint += answer;
                        if (i < numOfUsers) {
                            toPrint += " ";
                        }
                        answer = "";
                    }
                } else {
                    toPrint += answer.substr(2);
                }
                break;
            case 11:
                toPrint += "ERROR " + answer.substr(2);
                break;
        }
        len = answer.length();
        // A C string must end with a 0 char delimiter.  When we filled the answer buffer from the socket
        // we filled up to the \n char - we must make sure now that a 0 char is also present. So we truncate last character.
        answer.resize(len - 1);
        std::cout << toPrint << std::endl;

        /*TODO we need to change this*/
        if (answer == "bye") {
            std::cout << "Exiting...\n" << std::endl;
            break;
        }
    }
    return 0;
}


