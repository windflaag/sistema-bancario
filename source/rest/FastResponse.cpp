#include "FastResponse.hpp"
#include "../utility/Utility.hpp"

#include <json/json.h>

void rest::sendError(proxygen::ResponseBuilder& builder, int status_code,
		     std::string status_msg, std::string error_msg) {

  Json::Value object = Json::objectValue;
  object["error"] = error_msg;
  object["status"] = status_code;
  object["message"] = status_msg;
  std::string result = utility::jsonToString(object);

  builder
    .status(status_code, status_msg)
    .header("Content-Type", "application/json")
    .body(result)
    .sendWithEOM();
}
