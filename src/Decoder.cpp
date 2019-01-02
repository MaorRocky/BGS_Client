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
#include "Decoder.h"

using namespace std;
using namespace boost;

Decoder::Decoder() {}

std::string Decoder::decode(char nextByte) {
    pushByte(nextByte);
    if (length < 2) {
        return nullptr;
    }
    if (length == 2) {
        nextOpcodeBytes[0] = bytes[0];
        nextOpcodeBytes[1] = bytes[1];
        nextOpcode = bytesToShort(nextOpcodeBytes);
    }
    switch(nextOpcode) {
        case 9:
            if (length == 2){
                toReturn += "NOTIFICATION ";
            }
            if (length == 3) {
                if(nextByte == '0') {
                    toReturn += "PM ";
                }
                else if (nextByte == '1') {
                    toReturn += "Public ";
                }
            }
            if (nextByte == '\0') {
                nextZeroByte++;
            }
            if (nextZeroByte == 2) {
                fromVectorToString(bytes, toReturn, 3);
                std::string tmp(toReturn);
                reset();
                return tmp;
            }
            break;

        case 11:
            if (length == 2) {
                toReturn += to_string(nextOpcode);
                toReturn += " ";
            }
            if (length == 4) {
                messageOpcodeBytes[0] = bytes[2];
                messageOpcodeBytes[1] = bytes[3];
                messageOpcode = bytesToShort(messageOpcodeBytes);
                toReturn += to_string(messageOpcode);
                std::string tmp(toReturn);
                reset();
                return tmp;
            }
            break;

        case 10:
            if (length == 4) { // adding message opcode
                toReturn += "ACK ";
                messageOpcodeBytes[0] = bytes[2];
                messageOpcodeBytes[1] = bytes[3];
                messageOpcode = bytesToShort(messageOpcodeBytes);
                toReturn += to_string(messageOpcode);
            }
            else if (messageOpcode == (short)4) { // ACK for follow message
                if (length == 6) { // adding number of users
                    toReturn += " ";
                    numOfUsersBytes[0] = bytes[4];
                    numOfUsersBytes[1] = bytes[5];
                    numOfUsers = bytesToShort(numOfUsersBytes);
                    toReturn += to_string(numOfUsers);
                    toReturn += " ";
                }
                if (nextByte == '\0') {
                    nextZeroByte++;
                }
                if ((int)numOfUsers == nextZeroByte) { // adding usernames
                    fromVectorToString(bytes, toReturn, 6);
                    std::string tmp(toReturn);
                    reset();
                    return tmp;
                }
            }
            else if (messageOpcode == (short)7) { // ACK for
                if (length == 6) { // adding number of users
                    toReturn += " ";
                    numOfUsersBytes[0] = bytes[4];
                    numOfUsersBytes[1] = bytes[5];
                    numOfUsers = bytesToShort(numOfUsersBytes);
                    toReturn += to_string(numOfUsers);
                    toReturn += " ";
                }
                if (nextByte == '\0') {
                    nextZeroByte++;
                }
                if ((int)numOfUsers == nextZeroByte) { // adding usernames
                    fromVectorToString(bytes, toReturn, 6);
                    std::string tmp(toReturn);
                    reset();
                    return tmp;
                }
            }

            break;

    }
    return nullptr;
}

void Decoder::pushByte(char nextByte) {
    bytes.push_back(nextByte);
    length++;
}

short Decoder::bytesToShort(char *bytesArr) {
    short result = (short) ((bytesArr[0] & 0xff) << 8);
    result += (short) (bytesArr[1] & 0xff);
    return result;
}

void Decoder::reset() {
    bytes.clear();
    length = 0;
    nextZeroByte = 0;
    toReturn = "";
    nextOpcode = 0;
}

void Decoder::fromVectorToString(std::vector<char> bytes, std::string str, int start) {
    for (int i = start; i < bytes.size(); i++) {
        if (i < bytes.size() - 1 && bytes[i] == '\0') {
            str += " ";
        }
        else {
            str += bytes[i];
        }
    }
}





