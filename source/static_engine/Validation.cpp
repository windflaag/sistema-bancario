#include "Validation.hpp"
#include "../singleton/Singleton.hpp"
#include "../utility/Utility.hpp"

#include <filesystem>

std::string static_engine::validation::validatePath(std::string filepath) {
    if (utility::findSubstring(filepath, ".."))
        throw std::runtime_error("'..' in filepath");

    if (filepath.back() == '/') {
        filepath += DEFAULT_ROOT_PAGE;
    }

    Json::Value staticEngineConfig = singleton::instance()->getConfig()->get("StaticEngine");
    return staticEngineConfig.get("basePath", DEFAULT_BASE_PATH).asString() + filepath;
}

std::string static_engine::validation::getContentType(std::string filepath) {
    std::string extension = std::filesystem::path(filepath).extension();

    if (extension == ".html") {
        return "text/html";
    } else if (extension == ".css") {
        return "text/css";
    } else if (extension == ".js") {
        return "text/javascript";
    } else if (extension == ".png") {
        return "image/png";
    }
    
    // else threat it like a monster
    return "text/html";
}
