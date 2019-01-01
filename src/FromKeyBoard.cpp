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
        char delimiter = '\0';
        split(results, line, is_any_of(" "));
        //**********Register**********
        if (std::regex_search(line, REGISTER)) {
            //sending opcode
            short opcode = 1;
            char opcodeBytes[2];
            shortToBytes(opcode, opcodeBytes);
            (*handler).sendOpcode(opcodeBytes);
            //sending username
            (*handler).sendLine(results[1]);
            //sending password
            (*handler).sendLine(results[2]);
            //**********LOGIN*******
        } else if (std::regex_search(line, LOGIN)) {
            //sending opcode
            short opcode = 2;
            char opcodeBytes[2];
            shortToBytes(opcode, opcodeBytes);
            (*handler).sendOpcode(opcodeBytes);
            //sending username
            (*handler).sendLine(results[1]);
            //sending password
            (*handler).sendLine(results[2]);
            //**********LOGIN*******
        } else if (std::regex_search(line, LOGOUT)) {
            isTerminate = true;
            //sending opcode
            short opcode = 3;
            char opcodeBytes[2];
            shortToBytes(opcode, opcodeBytes);
            (*handler).sendOpcode(opcodeBytes);
            //**********FOLLOW*******
        } else if (std::regex_search(line, FOLLOW)) {
            //sending opcode
            short opcode = 4;
            char opcodeBytes[2];
            shortToBytes(opcode, opcodeBytes);
            (*handler).sendOpcode(opcodeBytes);
            // sending follow or unfollow
            char isFollow;
            if (results[1] == "0") {
                isFollow = '0';
            } else if (results[1] == "1") {
                isFollow = '1';
            }
            (*handler).sendBytes(&isFollow, 1);
            //sending number of users to follow
            short numOfUsers = (short) stoi(results[2]);
            char number[2];
            shortToBytes(numOfUsers, number);
            (*handler).sendOpcode(number);
            // sending usernames to follow
            for (int i = 3; i < results.size(); i++) {
                (*handler).sendLine(results[i]);
            }
            //**********POST*******
        } else if (std::regex_search(line, POST)) {
            //sending opcode
            short opcode = 5;
            char opcodeBytes[2];
            shortToBytes(opcode, opcodeBytes);
            (*handler).sendOpcode(opcodeBytes);
            // sending content
            std::string content("");
            for (int i = 1; i < results.size(); i++) {
                content += results[i];
                if (i < results.size()-1) {
                    content += " ";
                }
            }
            (*handler).sendLine(content);
            //**********PM*******
        } else if (std::regex_search(line, PM)) {
            //sending opcode
            short opcode = 6;
            char opcodeBytes[2];
            shortToBytes(opcode, opcodeBytes);
            (*handler).sendOpcode(opcodeBytes);
            // sending username
            (*handler).sendLine(results[1]);
            // sending personal message
            std:: string content("");
            for (int i = 2; i < results.size(); i++) {
                content += results[i];
                if (i < results.size()-1) {
                    content += " ";
                }
            }
            (*handler).sendLine(content);
            //**********USERLIST*******
        } else if (std::regex_search(line, USERLIST)) {
            //sending opcode
            short opcode = 7;
            char opcodeBytes[2];
            shortToBytes(opcode, opcodeBytes);
            (*handler).sendOpcode(opcodeBytes);
            //**********STAT*******
        } else if (std::regex_search(line, STAT)) {
            //sending opcode
            short opcode = 8;
            char opcodeBytes[2];
            shortToBytes(opcode, opcodeBytes);
            (*handler).sendOpcode(opcodeBytes);
            // sending username
            (*handler).sendLine(results[1]);
        }
    }
}

void FromKeyBoard::shortToBytes(short num, char *bytesArr) {
    bytesArr[0] = ((num >> 8) & 0xFF);
    bytesArr[1] = (num & 0xFF);
}