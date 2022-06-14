#ifndef __POLITIK_DETECTION__
#define __POLITIK_DETECTION__

#include <string>
#include "../utility/Utility.hpp"

namespace politik {

    class Detection {
        public:
            static bool detectBrowserInUserAgent(std::string userAgent);
    };
}

#endif
