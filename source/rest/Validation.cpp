#include "Validation.hpp"

bool charIsValidForId(char c) {
    return (
        (c >= 'a' && c <= 'f') ||
        (c >= '0' && c <= '9')
    );
}

bool rest::Validation::validateId(std::string id) {
    if (id.size() != 20)
        return false;

    for (char c : id)
        if (!(charIsValidForId(c)))
            return false;
    return true;
}

bool charIsValidForName(char c) {
    return (
        (c >= 'A' && c <= 'Z') ||
        (c >= 'a' && c <= 'z') ||
        c == ' '
    );
}

bool rest::Validation::validateName(std::string name) {
    if (name.size() > 80)
        return false;

    for (char c : name)
        if (!(charIsValidForName(c)))
            return false;
    return true;
}

bool rest::Validation::validateAmount(int amount) {
    return amount > 0;
}

bool charIsValidForUUID(char c) {
    return (
        (c == '-') ||
        (c >= 'a' && c <= 'f') ||
        (c >= '0' && c <= '9')
    );
}

bool rest::Validation::validateUUID(std::string id) {
    if (id.size() != 36)
        return false;

    // only check if characters are safe
    for (char c : id)
        if (!(charIsValidForUUID(c)))
            return false;
    return true;
}
