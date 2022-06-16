#ifndef __CODEC__
#define __CODEC__

#include <string>
#include <memory>
#include <json/json.h>

namespace codec {

    std::string to_hex(unsigned char s);
    std::string sha256(std::string);
    std::string computeAccountId(std::string name,
                                 std::string surname);
    std::string computeUUID();

    Json::Value parseUrlEncoded(std::string* urlencoded);
    Json::Value parseJSON(std::string* json);
    Json::Value parseBody(std::string* body);
}

#endif
