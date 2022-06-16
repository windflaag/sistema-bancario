#ifndef __POLITIK_DETECTION__
#define __POLITIK_DETECTION__

#include <string>
#include "../utility/Utility.hpp"

namespace politik {
  
  namespace detection {
    bool detectBrowserInUserAgent(std::string userAgent);
    bool detectAccountId(std::string path);  
    bool detectStatic(std::string path);  
  }
}

#endif
