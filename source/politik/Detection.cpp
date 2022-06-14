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
