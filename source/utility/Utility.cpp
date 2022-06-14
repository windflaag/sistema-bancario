#include "Utility.hpp"
#include <filesystem>
#include <fstream>

bool utility::findSubstring(std::string line, std::string substring) {
    return (std::string::npos != line.find(substring));
}

bool utility::fileExists(std::string filepath) {
    return std::filesystem::exists(std::filesystem::path(filepath));
}

std::string utility::jsonToString(Json::Value json) {
    Json::StreamWriterBuilder builder;
    
    // set no-indentation
    builder["indentation"] = "";
    return Json::writeString(builder, json);
}

std::string utility::readFile(std::string filepath) {
    std::ifstream file_reader (filepath, std::ifstream::in);
    std::string text = "", buffer = "";
    
    while(std::getline(file_reader, buffer)) {
        text.append(buffer);
    }

    file_reader.close();
    return text;
}
