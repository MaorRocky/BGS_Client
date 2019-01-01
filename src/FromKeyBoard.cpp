#include <connectionHandler.h>
#include "FromKeyBoard.h"
#include <bits/stdc++.h>
#include <boost/algorithm/string.hpp>

using namespace std;
using namespace boost;


FromKeyBoard::FromKeyBoard(ConnectionHandler &handler) : handler(handler), isTerminate(false) {}

void FromKeyBoard::operator()() {
    while (!isTerminate) {
        const short bufsize = 1024;
        char buf[bufsize];
        std::cin.getline(buf, bufsize);
        std::string line(buf);
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
            std::cout << "im here and im register" << std::endl;
            char opcode = '1';
            toSend += opcode;
            toSend += results[1];
            toSend += delimiter;
            toSend += results[2];
            std::cout << toSend << std::endl;
        } else if (std::regex_search(line, LOGIN)) {
            char opcode = '2';
            toSend += opcode;
            toSend += results[1];
            toSend += delimiter;
            toSend += results[2];
        } else if (std::regex_search(line, LOGOUT)) {
            isTerminate = true;
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


        handler.sendLine(toSend);


        /*if (!handler.sendLine(toSend)) {
            std::cout << "Disconnected. Exiting...\n" << std::endl;
            break;
        }*/
    }
}
