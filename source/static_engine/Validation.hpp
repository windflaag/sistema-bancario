#ifndef __STATIC_ENGINE_VALITATION__
#define __STATIC_ENGINE_VALITATION__

#define DEFAULT_BASE_PATH "."
#define DEFAULT_ROOT_PAGE "index.html"

#include <string>

namespace static_engine {
    class Validation {
        public:
            static std::string validatePath(std::string filepath);
    };
}

#endif
