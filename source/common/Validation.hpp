#ifndef __COMMON_VALITATION__
#define __COMMON_VALITATION__

#define DEFAULT_BASE_PATH "."
#define DEFAULT_ROOT_PAGE "index.html"

#include <string>

namespace common {
  std::string validatePath(std::string filepath);
  std::string getContentType(std::string filepath);
  bool validateName(std::string name);
  bool validateId(std::string id);
  bool validateAmount(int amount);
  bool validateUUID(std::string id);
}

#endif
