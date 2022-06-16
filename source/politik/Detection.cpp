#include "Detection.hpp"
#include "../utility/Utility.hpp"
#include <algorithm>

bool politik::detection::detectBrowserInUserAgent(std::string userAgent) {
    return (
        utility::findSubstring(userAgent, "Mozilla") ||
        utility::findSubstring(userAgent, "WebKit") ||
        utility::findSubstring(userAgent, "Chrome") ||
        utility::findSubstring(userAgent, "Gecko")
    );
}

inline bool detectByBase(std::string base, std::string path) {
  if (path.size() <= base.size())
    return false;
  if (path.substr(0, base.size()) != base)
    return false;
  return true;
}

bool politik::detection::detectAccountId(std::string path) {
  return detectByBase("/api/account", path);
}

bool politik::detection::detectStatic(std::string path) {
  return detectByBase("/api/static", path);
}
