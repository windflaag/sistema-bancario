#ifndef __CODEC__
#define __CODEC__

#include <string>

namespace codec {

    std::string to_hex(unsigned char s);
    std::string sha256(std::string);
    std::string computeAccountId(std::string name,
                                 std::string surname);
    std::string computeUUID();
}

#endif
