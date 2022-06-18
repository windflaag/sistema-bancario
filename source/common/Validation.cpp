#include "Validation.hpp"
#include "../singleton/Singleton.hpp"
#include "../utility/Utility.hpp"

#include <filesystem>

std::string common::validatePath(std::string filepath) {
  if (utility::findSubstring(filepath, ".."))
    throw std::runtime_error("'..' in filepath");

  if (filepath.back() == '/') {
    filepath += DEFAULT_ROOT_PAGE;
  }

  Json::Value staticEngineConfig = singleton::instance()->getConfig()->get("StaticEngine");
  return staticEngineConfig.get("basePath", DEFAULT_BASE_PATH).asString() + filepath;
}

std::string common::getContentType(std::string filepath) {
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

bool charIsValidForId(char c) {
  return (
	  (c >= 'a' && c <= 'f') ||
	  (c >= '0' && c <= '9')
	  );
}

bool common::validateId(std::string id) {
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
	  c == ' ' || c == '.'
	  );
}

bool common::validateName(std::string name) {
  if (name.size() > 80)
    return false;

  for (char c : name)
    if (!(charIsValidForName(c)))
      return false;
  return true;
}

bool common::validateAmount(int amount) {
  return amount >= 0;
}

bool charIsValidForUUID(char c) {
  return (
	  (c == '-') ||
	  (c >= 'a' && c <= 'f') ||
	  (c >= '0' && c <= '9')
	  );
}

bool common::validateUUID(std::string id) {
  if (id.size() != 36)
    return false;

  // only check if characters are safe
  for (char c : id)
    if (!(charIsValidForUUID(c)))
      return false;
  return true;
}
