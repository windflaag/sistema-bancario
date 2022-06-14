#include "Utility.hpp"
#include <filesystem>

bool utility::findSubstring(std::string line, std::string substring) {
    return (std::string::npos != line.find(substring));
}

bool utility::fileExists(std::string filepath) {
    return std::filesystem::exists(std::filesystem::path(filepath));
}


Json::Value* utility::responseTableToJsonArray(database::responseTable* data) {
    Json::Value *array = new Json::Value();

    return array;
};
