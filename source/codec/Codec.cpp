#include "Codec.hpp"

#include <iostream>
#include <sstream>
#include <openssl/sha.h>

std::string codec::to_hex(unsigned char s) {
    std::stringstream ss;
    ss << std::hex << (int) s;
    return ss.str();
}   

std::string codec::sha256(std::string text) {    
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, text.c_str(), text.length());
    SHA256_Final(hash, &sha256);

    std::string output = "";    
    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        output += codec::to_hex(hash[i]);
    }
    return output;
}

std::string codec::computeAccountId(std::string name,
                                    std::string surname) {
    return codec::sha256(name + surname).substr(0, 20);
}
