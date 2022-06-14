#include "Config.hpp"
#include <iostream>

void custom::Config::readFromFile(std::string path) {
    std::ifstream file_reader (path, std::ifstream::in);
    std::string text = "", buffer = "";
    while(std::getline(file_reader, buffer)) {
        text.append(buffer);
    }
    file_reader.close();

    Json::Reader* text_reader = new Json::Reader();
    if (!(text_reader->parse(text, this->config))) {
        std::cerr << "WARNING: syntax error while reading : " + path << std::endl;
        this->config = Json::Value("");
    }

    delete text_reader;
};

custom::Config::Config() {
    //
}

custom::Config::Config(std::string path) {
    this->readFromFile(path);
}

custom::Config::~Config() {
    //
}

void custom::Config::set(std::string name, Json::Value value) {
    this->config[name] = value;
}

Json::Value custom::Config::get(std::string name) {
    return this->config.get(name, "");
}
