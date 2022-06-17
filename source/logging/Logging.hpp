#ifndef __LOGGING__
#define __LOGGING__

#include <string>

#define DEFAULT_LOG_PATH "./log/"

namespace logging {
  class Logger {
  private:
    std::string logpath;
  public:
    Logger();
    void log(std::string level, std::string message) const;
  };
}

#endif
