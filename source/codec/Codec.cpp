#include "Codec.hpp"
#include "../singleton/Singleton.hpp"

#include <iostream>
#include <sstream>
#include <openssl/sha.h>

inline char nibble_to_hex(short c) {
  switch(c) {
  case 0 : return '0';
  case 1 : return '1';
  case 2 : return '2';
  case 3 : return '3';
  case 4 : return '4';
  case 5 : return '5';
  case 6 : return '6';
  case 7 : return '7';
  case 8 : return '8';
  case 9 : return '9';
  case 10 : return 'a';
  case 11 : return 'b';
  case 12 : return 'c';
  case 13 : return 'd';
  case 14 : return 'e';
  case 15 : return 'f';
  }
  
  // = error
  return ' ';
}

inline std::string byte_to_hex(unsigned char s) {
  return
    std::string(1, nibble_to_hex((s >> 4))) +
    std::string(1, nibble_to_hex((s % 16)));
}   

std::string codec::sha256(std::string text) {    
  unsigned char hash[SHA256_DIGEST_LENGTH];
  SHA256_CTX sha256;
  SHA256_Init(&sha256);
  SHA256_Update(&sha256, text.c_str(), text.length());
  SHA256_Final(hash, &sha256);

  std::string output = "";    
  for(int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
    output += byte_to_hex(hash[i]);
  }
  return output;
}

std::string codec::computeAccountId(std::string name,
                                    std::string surname) {
  auto randomizer = singleton::instance()->getRandomizer();
  return codec::sha256(name + surname + std::to_string(randomizer->operator()())).substr(0, 20);
}

inline void write_int64_into_buf(long long int64, unsigned char *buf) {
  for (int i = 0; i < 8; i++)
    buf[i] = (int64 >> i) % 256;
}

inline std::string arrangeUUID(std::string rawUUID) {
  return (
	  rawUUID.substr(0, 8) + "-" + 
	  rawUUID.substr(8, 4) + "-" + 
	  rawUUID.substr(12, 4) + "-" + 
	  rawUUID.substr(16, 4) + "-" + 
	  rawUUID.substr(20, 12));
}

std::string codec::computeUUID() {
  unsigned char *buffer = new unsigned char[16];

  auto randomizer = singleton::instance()->getRandomizer();
  write_int64_into_buf(randomizer->operator()(), buffer);
  write_int64_into_buf(randomizer->operator()(), buffer + 8);

  buffer[6] = ((buffer[6] % 16) | (4 << 4)); // version of UUID
  buffer[8] = ((buffer[8] % 64) | (2 << 6)); // variant of UUIDv4
  
  std::string output = "";
  for(int i = 0; i < 16; i++) {
    output += byte_to_hex(buffer[i]);
  }
  
  return arrangeUUID(output);
}

Json::Value codec::parseUrlEncoded(std::string* /*urlencoded*/) {
  Json::Value parameters;
  // TODO:
  return parameters;
}

Json::Value codec::parseJSON(std::string* json) {
  Json::Value parameters;
  Json::Reader* text_reader = new Json::Reader();
  if (!(text_reader->parse(*json, parameters))) {
    delete text_reader;
    throw std::runtime_error("");
  }

  delete text_reader;
  return parameters;
}

Json::Value codec::parseBody(std::string* body) {
  return codec::parseJSON(body);
  /* was JSON and UrlEncoded, but now is JSON
     try {
     return codec::parseJSON(body);
     } catch(...) {
     try {
     return codec::parseUrlEncoded(body);
     } catch(...) {
     throw std::runtime_error("");
     }
     }
  */
}
