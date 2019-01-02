#include "FromServer.h"
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

FromServer::FromServer(ConnectionHandler* handler, bool isTerminate) :handler(handler), isTerminate(isTerminate) {
}

std::string FromServer::operator()() {
    bool terminate(false);
    while (!terminate) {
        std::vector<char> bytes;
        //getNextBytesPart(bytes);
        getVectorBytes(bytes, 1);
        getVectorBytes(bytes, 1);
        cout << std::to_string(bytes[0]) << endl;
        cout << std::to_string(bytes[1]) << endl;
        char *opcodeBytes = new char[2];
        opcodeBytes[0] = bytes[0];
        opcodeBytes[1] = bytes[1];
        short opcode = bytesToShort(opcodeBytes);
        cout << "I am here!!!!" << endl;
        string toPrint("");
        // ***********NOTIFICATION***********
        if (opcode == 9) {
            bytes.clear();
            getNextBytesPart(bytes);
            toPrint += "NOTIFICATION ";
            if (bytes[0] == '0') {
                toPrint += "PM ";
            } else if (bytes[0] == '1') {
                toPrint += "Public ";
            }
            // adding username
            bytesArrayToString(bytes, toPrint, 1);
            // adding content
            bytes.clear();
            getNextBytesPart(bytes);
            bytesArrayToString(bytes, toPrint, 0);
            //**********ERROR**********
        } else if (opcode == 11) {
            toPrint += "ERROR ";
            char *messageOpcodeBytes = new char[2];
            messageOpcodeBytes[0] = bytes[2];
            messageOpcodeBytes[1] = bytes[3];
            short messageOpcode = bytesToShort(messageOpcodeBytes);
            toPrint += std::to_string(messageOpcode);
            bytes.clear();
            //**********ACK***********
        } else if (opcode == 10) {
            bytes.clear();
            getVectorBytes(bytes, 2);
            toPrint += "ACK ";
            char *messageOpcodeBytes = new char[2];
            // adding message opcode
            messageOpcodeBytes[0] = bytes[0];
            messageOpcodeBytes[1] = bytes[1];
            short messageOpcode = bytesToShort(messageOpcodeBytes);
            toPrint += std::to_string(messageOpcode);
            toPrint += " ";
            bytes.clear();
            if (messageOpcode == 4) {
                //******ACK FOLLOW********
                getNextBytesPart(bytes);
                // adding number of users to follow
                char *numOfUsersBytes = new char[2];
                numOfUsersBytes[0] = bytes[0];
                numOfUsersBytes[1] = bytes[1];
                short numOfUsers = bytesToShort(numOfUsersBytes);
                int numUsers = numOfUsers;
                toPrint += std::to_string(numUsers);
                toPrint += " ";
                bytesArrayToString(bytes, toPrint, 2);
                bytes.clear();
                // adding names of users to follow
                for (int i = 1; i < numUsers; i++) {
                    getNextBytesPart(bytes);
                    bytesArrayToString(bytes, toPrint, 0);
                    toPrint += " ";
                    bytes.clear();
                }
                bytes.clear();
                //*******ACK USERLIST*******
            } else if (messageOpcode == 7) {
                getNextBytesPart(bytes);
                // adding number of users
                char *numOfUsersBytes = new char[2];
                numOfUsersBytes[0] = bytes[0];
                numOfUsersBytes[1] = bytes[1];
                short numOfUsers = bytesToShort(numOfUsersBytes);
                int numUsers = numOfUsers;
                toPrint += std::to_string(numUsers);
                toPrint += " ";
                bytesArrayToString(bytes, toPrint, 2);
                bytes.clear();
                // adding names of users
                for (int i = 1; i < numUsers; i++) {
                    getNextBytesPart(bytes);
                    bytesArrayToString(bytes, toPrint, 0);
                    toPrint += " ";
                    bytes.clear();
                }
                bytes.clear();
                //*******ACK STAT**********
            } else if (messageOpcode == 8) {
                // adding number of posts
                getVectorBytes(bytes, 2);
                char *numOfPostsBytes = new char[2];
                numOfPostsBytes[0] = bytes[0];
                numOfPostsBytes[1] = bytes[1];
                short numOfPosts = bytesToShort(numOfPostsBytes);
                toPrint += std::to_string(numOfPosts);
                toPrint += " ";
                bytes.clear();
                // adding number of followers
                getVectorBytes(bytes, 2);
                char *numOfFollowersBytes = new char[2];
                numOfFollowersBytes[0] = bytes[0];
                numOfFollowersBytes[1] = bytes[1];
                short numOfFollowers = bytesToShort(numOfFollowersBytes);
                toPrint += std::to_string(numOfFollowers);
                toPrint += " ";
                bytes.clear();
                // adding number of following
                getVectorBytes(bytes, 2);
                char *numOfFollowingBytes = new char[2];
                numOfFollowingBytes[0] = bytes[0];
                numOfFollowingBytes[1] = bytes[1];
                short numOfFollowings = bytesToShort(numOfFollowingBytes);
                toPrint += std::to_string(numOfFollowings);
                bytes.clear();
                //************LOGOUT***********
            } else if (messageOpcode == 3) {
                terminate = true;
                bytes.clear();
            }
        }
        std::cout << toPrint << endl;
    }
    (*handler).close();
}

void FromServer::getNextBytesPart(std::vector<char> bytes) {
    char c;
    while ((*handler).getBytes(&c, 1) != '\0') {
        bytes.push_back(c);
        cout << std::to_string(c) << endl;
    }
}

short FromServer::bytesToShort(char *bytesArr) {
        short result = (short) ((bytesArr[0] & 0xff) << 8);
        result += (short) (bytesArr[1] & 0xff);
        return result;
}

void FromServer::bytesArrayToString(std::vector<char> bytes, string str, int start) {
        for (int i = start; bytes[i] != '\0'; i++) {
            str += bytes[i];
        }
        return;
    }

void FromServer::getVectorBytes(std::vector<char> bytes, int numOfBytes) {
    char c;
    //for (int i = 1;(*handler).getBytes(&c, 1) && i <= numOfBytes; i++) {
    bytes.push_back((*handler).getBytes(&c, 1));
    //}
}
