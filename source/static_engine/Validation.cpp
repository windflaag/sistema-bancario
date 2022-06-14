#include "Validation.hpp"
#include "../singleton/Singleton.hpp"
#include "../utility/Utility.hpp"

std::string static_engine::Validation::validatePath(std::string filepath) {
    if (utility::findSubstring(filepath, ".."))
        throw std::runtime_error("'..' in filepath");

    if (filepath.back() == '/') {
        filepath += DEFAULT_ROOT_PAGE;
    }

    Json::Value staticEngineConfig = singleton::instance()->getConfig()->get("StaticEngine");
    return staticEngineConfig.get("basePath", DEFAULT_BASE_PATH).asString() + filepath;
}
