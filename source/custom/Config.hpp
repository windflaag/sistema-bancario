#ifndef __CONFIG__
#define __CONFIG__

#include <json/json.h>
#include <string>
#include <fstream>

namespace custom {

    class Config {
        private:
            Json::Value config;
            void readFromFile(std::string path);

        public:
            Config();
            Config(std::string path);
            ~Config();

            void set(std::string, Json::Value);
            Json::Value get(std::string);
    };

}

#endif
