#include "Detection.hpp"
#include "../utility/Utility.hpp"
#include <algorithm>

bool politik::Detection::detectBrowserInUserAgent(std::string userAgent) {
    return (
        utility::findSubstring(userAgent, "Mozilla") ||
        utility::findSubstring(userAgent, "WebKit") ||
        utility::findSubstring(userAgent, "Chrome") ||
        utility::findSubstring(userAgent, "Gecko")
    );
}

bool politik::Detection::detectAccountId(std::string path) {
  if (path.size() != 33)
    return false;
  if (path.substr(0, 13) != "/api/account/")
    return false;
  return true;
}
