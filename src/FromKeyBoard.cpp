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

#include "FromKeyBoard.h"


FromKeyBoard::FromKeyBoard(ConnectionHandler* handler) : handler(handler), isTerminate(false) {}

void FromKeyBoard::operator()() {
    cout << "I am here2" << endl;
    while (!isTerminate) {
        const short bufsize = 4096;
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
        string toSend("");
        char delimiter = '\0';
        split(results, line, is_any_of(" "));
        if (std::regex_search(line, REGISTER)) {
            short opcode = 1;
            char opcodeBytes[2];
            //sending opcode
            shortToBytes(opcode, opcodeBytes);
            (*handler).sendLine(opcodeBytes, 2);
            //sending username
            char tmpName[results[1].length() + 1];
            std::strcpy(tmpName, results[1].c_str());
            char name[results[1].length() + 2];
            for (int i = 0; i < results[1].length() + 1; i++) {
                name[i] = tmpName[i];
            }
            name[results[1].length() + 1] = '\0';
            int j = (int)results[1].length() + 2;
            (*handler).sendLine(name, j);
            //sending password
            char tmpPassword[results[2].length() + 1];
            std::strcpy(tmpPassword, results[2].c_str());

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
        cout << toSend << endl;
        (*handler).sendLine(toSend);

        /*if (!handler.sendLine(toSend)) {
            std::cout << "Disconnected. Exiting...\n" << std::endl;
            break;
        }*/
    }
}

void FromKeyBoard::shortToBytes(short num, char *bytesArr) {
    bytesArr[0] = ((num >> 8) & 0xFF);
    bytesArr[1] = (num & 0xFF);
}

void FromKeyBoard::merge(std::vector<char> first, char *second) {

    for (int i = 0; i < second)
}
