#include "Logging.hpp"
#include "../utility/Utility.hpp"
#include "../singleton/Singleton.hpp"

#include <filesystem>
#include <cstdio>

namespace logging {
    inline std::string getLogPath() {
      Json::Value loggerConfig =
        singleton::instance()
        ->getConfig()
        ->get("Logger");
      return loggerConfig.get("logPath", DEFAULT_LOG_PATH).asString();
    }

    Logger::Logger() {
        this->logpath = getLogPath();
        
        std::filesystem::path PATH = this->logpath;
        if (!(std::filesystem::exists(PATH)))
            std::filesystem::create_directory(PATH);
    }

    void Logger::log(std::string level, std::string message) const {
        std::string PATH = this->logpath + level + ".log";
        std::FILE* file = std::fopen(PATH.c_str(), "a");
        std::fprintf(file, "%s: %s\n", utility::getCurrentTimeStampString().c_str(), message.c_str());
        std::fclose(file);
    }
}
