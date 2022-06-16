#ifndef __REST_VALIDATION__
#define __REST_VALIDATION__

#include <string>

namespace rest {
    class Validation {
        public:
            static bool validateName(std::string name);
            static bool validateId(std::string id);
            static bool validateAmount(int amount);
            static bool validateUUID(std::string id); 
    };
}

#endif
