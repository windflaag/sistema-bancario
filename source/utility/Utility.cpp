#include "Utility.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>

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

int utility::getCurrentTimeStampSeed() {
    std::time_t result = std::time(nullptr);
   return result;
}

std::string utility::getCurrentTimeStampString() {
    std::time_t now = std::time(NULL);
    std::tm * ptm = std::localtime(&now);
    char buffer[32];

    // format: 15-06-2022 19-30-00
    std::strftime(buffer, 32, "%d-%m-%Y %H-%M-%S", ptm);
    return std::string(buffer);
}
