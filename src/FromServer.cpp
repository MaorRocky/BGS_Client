#include "FromServer.h"

using namespace std;
using namespace boost;

FromServer::FromServer(ConnectionHandler *handler, bool isTerminate) : handler(handler), isTerminate(isTerminate) {
}

void FromServer::operator()() {
    bool terminate(false);
    while (!terminate) {
        std::vector<char> bytes;
        getNextBytesPart(bytes);
        char *opcodeBytes = new char[2];
        opcodeBytes[0] = bytes[0];
        opcodeBytes[1] = bytes[1];
        short opcode = bytesToShort(opcodeBytes);
        string toPrint("");
        // ***********NOTIFICATION***********
        if (opcode == 9) {
            toPrint += "NOTIFICATION ";
            if (bytes[2] == '0') {
                toPrint += "PM ";
            } else if (bytes[2] == '1') {
                toPrint += "Public ";
            }
            // adding username
            bytesArrayToString(bytes, toPrint, 3);
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
            toPrint += "ACK ";
            char *messageOpcodeBytes = new char[2];
            messageOpcodeBytes[0] = bytes[2];
            messageOpcodeBytes[1] = bytes[3];
            short messageOpcode = bytesToShort(messageOpcodeBytes);
            toPrint += std::to_string(messageOpcode);
            toPrint += " ";
            if (messageOpcode == 4) {
                //******ACK FOLLOW********
                // adding number of users to follow
                char *numOfUsersBytes = new char[2];
                numOfUsersBytes[0] = bytes[4];
                numOfUsersBytes[1] = bytes[5];
                short numOfUsers = bytesToShort(numOfUsersBytes);
                int numUsers = numOfUsers;
                toPrint += std::to_string(numUsers);
                toPrint += " ";
                bytesArrayToString(bytes, toPrint, 6);
                bytes.clear();
                // adding names of users to follow
                for (int i = 1; i < numUsers; i++) {
                    getNextBytesPart(bytes);
                    bytesArrayToString(bytes, toPrint, 0);
                    toPrint += " ";
                    bytes.clear();
                }
                //*******ACK USERLIST*******
            } else if (messageOpcode == 7) {
                // adding number of users
                char *numOfUsersBytes = new char[2];
                numOfUsersBytes[0] = bytes[4];
                numOfUsersBytes[1] = bytes[5];
                short numOfUsers = bytesToShort(numOfUsersBytes);
                int numUsers = numOfUsers;
                toPrint += std::to_string(numUsers);
                toPrint += " ";
                bytesArrayToString(bytes, toPrint, 6);
                bytes.clear();
                // adding names of users
                for (int i = 1; i < numUsers; i++) {
                    getNextBytesPart(bytes);
                    bytesArrayToString(bytes, toPrint, 0);
                    toPrint += " ";
                    bytes.clear();
                }
                //*******ACK STAT**********
            } else if (messageOpcode == 8) {
                // adding number of posts
                char *numOfPostsBytes = new char[2];
                numOfPostsBytes[0] = bytes[4];
                numOfPostsBytes[1] = bytes[5];
                short numOfPosts = bytesToShort(numOfPostsBytes);
                toPrint += std::to_string(numOfPosts);
                toPrint += " ";
                // adding number of followers
                char *numOfFollowersBytes = new char[2];
                numOfFollowersBytes[0] = bytes[6];
                numOfFollowersBytes[1] = bytes[7];
                short numOfFollowers = bytesToShort(numOfFollowersBytes);
                toPrint += std::to_string(numOfFollowers);
                toPrint += " ";
                // adding number of following
                char *numOfFollowingBytes = new char[2];
                numOfFollowingBytes[0] = bytes[8];
                numOfFollowingBytes[1] = bytes[9];
                short numOfFollowings = bytesToShort(numOfFollowingBytes);
                toPrint += std::to_string(numOfFollowings);
            } else if (messageOpcode == 3) {
                terminate = true;
            }
        }
        std::cout << toPrint << endl;
    }
    (*handler).close();
}

void FromServer::getNextBytesPart(std::vector<char> bytes) {
    char c;
    cout<<"im here"<<endl;
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
}
