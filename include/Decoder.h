
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

#ifndef BOOST_ECHO_CLIENT_DECODER_H
#define BOOST_ECHO_CLIENT_DECODER_H

class Decoder {
public:
    //Decoder();

    std::string decode(char nextByte);
private:
    std::vector<char> bytes;
    char nextOpcodeBytes[2];
    short nextOpcode;
    int length;
    int nextZeroByte;
    char numOfUsersBytes[2];
    short numOfUsers;
    char messageOpcodeBytes[2];
    short messageOpcode;

    void pushByte(char nextByte);
    short bytesToShort(char *bytesArr);
    std::string toReturn;
    void reset();
    void fromVectorToString(std::vector<char> bytes, std::string str, int start);





};

#endif //BOOST_ECHO_CLIENT_DECODER_H
