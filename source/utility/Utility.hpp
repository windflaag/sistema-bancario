#ifndef __UTILITY__
#define __UTILITY__

#include <string>
#include "../database/Database.hpp"
#include <json/json.h> 

namespace utility {
    bool findSubstring(std::string line, std::string substring);
    bool fileExists(std::string filepath);
    Json::Value* responseTableToJsonArray(database::responseTable* data);
}

#endif
