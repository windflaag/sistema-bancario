#include "Codec.hpp"
#include "../singleton/Singleton.hpp"

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

inline void write_int64_into_buf(long long int64, unsigned char *buf) {
  for (int i = 0; i < 8; i++)
    buf[i] = (int64 >> i) % 256;
}

inline std::string arrangeUUID(std::string rawUUID) {
  return (
	  rawUUID.substr(0, 4) + "-" + 
	  rawUUID.substr(4, 2) + "-" + 
	  rawUUID.substr(6, 2) + "-" + 
	  rawUUID.substr(8, 2) + "-" + 
	  rawUUID.substr(10, 6));
}

std::string codec::computeUUID() {
  unsigned char *buffer = new unsigned char[16];

  auto randomizer = singleton::instance()->getRandomizer();
  write_int64_into_buf(randomizer->operator()(), buffer);
  write_int64_into_buf(randomizer->operator()(), buffer + 8);
  
  std::string output = "";
  for(int i = 0; i < 16; i++) {
    output += codec::to_hex(buffer[i]);
  }
  return arrangeUUID(output);
}
