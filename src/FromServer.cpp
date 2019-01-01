#include <connectionHandler.h>
#include "FromServer.h"
#include <stdlib.h>
#include <mutex>
#include <thread>
#include <bits/stdc++.h>
#include <boost/algorithm/string.hpp>
#include <iostream>
#include <iostream>


using namespace std;
using namespace boost;

FromServer::FromServer(ConnectionHandler* handler, bool isTerminate) :thandler(handler), isTerminate(isTerminate) {
}

void FromServer::operator()() {
    bool terminate(false);
    while (!terminate) {
        char bytes[1024];
        int index = getNextBytesPart(bytes, 0);
        char *opcodeBytes = new char[2];
        opcodeBytes[0] = bytes[0];
        opcodeBytes[1] = bytes[1];
        short opcode = bytesToShort(opcodeBytes);
        string toPrint("");
        if (opcode == 9) {
            toPrint += "NOTIFICATION ";
            if (bytes[2] == '0') {
                toPrint += "PM ";
            } else if (bytes[2] == '1') {
                toPrint += "Public ";
            }
            bytesArrayToString(bytes, toPrint, 3);
            getNextBytesPart(bytes, index);
            bytesArrayToString(bytes, toPrint, index);
        } else if (opcode == 11) {
            toPrint += "ERROR ";
            char *messageOpcodeBytes = new char[2];
            messageOpcodeBytes[0] = bytes[2];
            messageOpcodeBytes[1] = bytes[3];
            short messageOpcode = bytesToShort(messageOpcodeBytes);
            toPrint += std::to_string(messageOpcode);

        } else if (opcode == 10) {
            toPrint += "ACK ";
            char *messageOpcodeBytes = new char[2];
            messageOpcodeBytes[0] = bytes[2];
            messageOpcodeBytes[1] = bytes[3];
            short messageOpcode = bytesToShort(messageOpcodeBytes);
            toPrint += std::to_string(messageOpcode);
            if (messageOpcode == 4) {
                toPrint += " ";
                char *numOfUsersBytes = new char[2];
                numOfUsersBytes[0] = bytes[4];
                numOfUsersBytes[1] = bytes[5];
                short numOfUsers = bytesToShort(numOfUsersBytes);
                int numUsers = numOfUsers;
                toPrint += std::to_string(numUsers);
                toPrint += " ";
                bytesArrayToString(bytes, toPrint, 6);
                for (int i = 1; i < numUsers; i++) {
                    int tmp = index;
                    index = getNextBytesPart(bytes, tmp);
                    bytesArrayToString(bytes, toPrint, tmp);
                }
            } else if (messageOpcode == 7) {
                toPrint += " ";
                char *numOfUsersBytes = new char[2];
                numOfUsersBytes[0] = bytes[4];
                numOfUsersBytes[1] = bytes[5];
                short numOfUsers = bytesToShort(numOfUsersBytes);
                int numUsers = numOfUsers;
                toPrint += std::to_string(numUsers);
                toPrint += " ";
                bytesArrayToString(bytes, toPrint, 6);
                for (int i = 1; i < numUsers; i++) {
                    int tmp = index;
                    index = getNextBytesPart(bytes, tmp);
                    bytesArrayToString(bytes, toPrint, tmp);
                }
            } else if (messageOpcode == 8) {
                toPrint += " ";
                char *numOfPostsBytes = new char[2];
                numOfPostsBytes[0] = bytes[4];
                numOfPostsBytes[1] = bytes[5];
                short numOfPosts = bytesToShort(numOfPostsBytes);
                toPrint += std::to_string(numOfPosts);
                toPrint += " ";

                char *numOfFollowersBytes = new char[2];
                numOfFollowersBytes[0] = bytes[6];
                numOfFollowersBytes[1] = bytes[7];
                short numOfFollowers = bytesToShort(numOfFollowersBytes);
                toPrint += std::to_string(numOfFollowers);
                toPrint += " ";

                char *numOfFollowingBytes = new char[2];
                numOfFollowingBytes[0] = bytes[8];
                numOfFollowingBytes[1] = bytes[9];
                short numOfFollowings = bytesToShort(numOfFollowingBytes);
                toPrint += std::to_string(numOfFollowings);
            } else if (messageOpcode == 3) {
                terminate = true;


            }
        }

    }
    (*thandler).close();
}

    int FromServer::getNextBytesPart(char bytes[], int toStart) {
        int i(0);
        while ((*thandler).getBytes(bytes, 1) != '\0') {
            char c;
            bytes[toStart + i] = (*thandler).getBytes(&c, 1);
            i++;
        }
        bytes[i + toStart] = '\0';
        return i + toStart + 1;
    }

    short FromServer::bytesToShort(char *bytesArr) {
        short result = (short) ((bytesArr[0] & 0xff) << 8);
        result += (short) (bytesArr[1] & 0xff);
        return result;
    }




    void FromServer::bytesArrayToString(char *bytes, string str, int start) {
        for (int i = start; bytes[i] != '\0'; i++) {
            str += bytes[i];
        }
        str += " ";
    }
