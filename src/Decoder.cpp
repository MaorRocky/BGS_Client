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


std::string Decoder::decode(char nextByte) {
   if (counter ==0){
       x = nextByte;
       counter =1;
       cout<<"x is : " << to_string(x);
    }



    if (length == 2) {
        cout<<"x"<<endl;
        cout<<to_string(x)<<endl;
        cout<<"y"<<endl;
        cout<<to_string(y)<<endl;
        nextOpcodeBytes[0] = x;
        nextOpcodeBytes[1] = y;
        nextOpcode = bytesToShort(nextOpcodeBytes);
        cout<<nextOpcode<<endl;
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
            else if (length >= 4 && messageOpcode == (short)4) { // ACK for follow message
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
            else if (length >= 4 && messageOpcode == (short)7) { // ACK for user list message
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
            else if (length >= 4 && messageOpcode == (short)8) {
                if (length == 10) {
                    char numPosts[2]; // adding number of posts
                    numPosts[0] = bytes[4];
                    numPosts[1] = bytes[5];
                    short posts = bytesToShort(numPosts);
                    toReturn += to_string(posts);
                    toReturn += " ";

                    char numFollowers[2]; // adding number of followers
                    numFollowers[0] = bytes[6];
                    numFollowers[1] = bytes[7];
                    short followers = bytesToShort(numFollowers);
                    toReturn += to_string(followers);
                    toReturn += " ";

                    char numFollowing[2]; // adding number of following
                    numFollowing[0] = bytes[8];
                    numFollowing[1] = bytes[9];
                    short following = bytesToShort(numFollowing);
                    toReturn += to_string(following);

                    std::string tmp(toReturn);
                    reset();
                    return tmp;
                }
            }
            else if (length == 4 && (messageOpcode == (short)1 || messageOpcode == (short)2 || messageOpcode == (short)3
            || messageOpcode == (short)5 || messageOpcode == (short)6)) {
                std::string tmp(toReturn);
                reset();
                return toReturn;
            }
            break;

    }
    return "I AM STILL NOT A VALID MESSAGE";
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





