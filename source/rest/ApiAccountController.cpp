#include "ApiAccountController.hpp"

#include <proxygen/httpserver/RequestHandler.h>
#include <proxygen/httpserver/ResponseBuilder.h>

#include "../database/Database.hpp"
#include "../utility/Utility.hpp"
#include "../codec/Codec.hpp"
#include "../common/Validation.hpp"
#include "../common/FastResponse.hpp"

rest::ApiAccountController::ApiAccountController() {
  //
}

void rest::ApiAccountController::onRequest
(std::unique_ptr<proxygen::HTTPMessage> req) noexcept {
  proxygen::ResponseBuilder builder(downstream_);
  if (req->getMethod() == proxygen::HTTPMethod::GET) {
    this->alreadySent = true;
    try {
      Json::Value* data = database::getListOfAccounts();

      Json::Value object = Json::objectValue;
      object["accounts"] = *data;
            
      std::string result = utility::jsonToString(object);
      delete data;

      builder
	.status(200, "OK")
	.header("Content-Type", "application/json")
	.body(result)
	.sendWithEOM();
      return;
    } catch(...) {
      common::sendError(builder, 500, "Internal Server Error", "unable to fetch data");
      return;
    }
  } else if (req->getMethod() == proxygen::HTTPMethod::POST) {
    // placeholder
  } else if (req->getMethod() == proxygen::HTTPMethod::DELETE) {
    this->alreadySent = true;
    if (
	(! req->hasQueryParam("id")) ||
	(! common::validateId(req->getQueryParam("id")))
        ) {
      common::sendError(builder, 400, "Bad Request", "arguments are invalid");
      return;
    }

    try {
      std::string accountId = req->getQueryParam("id");

      database::deleteAccount(accountId);
           
      builder
	.status(200, "OK")
	.sendWithEOM();
    } catch(...) {
      common::sendError(builder, 500, "Internal Server Error", "unable to delete data");
      return;
    }
  } else {
    this->alreadySent = true;
    common::sendError(builder, 501, "Not Implemented", "method not implemented");
    return;
  }
}

void rest::ApiAccountController::onEOM() noexcept {
  proxygen::ResponseBuilder builder(downstream_);

  // already replied in onRequest
  if (this->alreadySent)
    return;

  // body doesn't exists or is empty
  if (!(this->body_) || (this->body_->size() == 0)) {
    common::sendError(builder, 400, "Bad Request", "body is empty");
    return;
  }

  Json::Value parameters;
  try {
    parameters = codec::parseBody(this->body_.get());
  } catch(...) {
    common::sendError(builder, 400, "Bad Request", "cannot parse body");
    return;
  }

  if (
      (!(parameters.isObject())) ||
      (! parameters.isMember("name")) ||
      (! parameters.isMember("surname")) ||
      (! parameters["name"].isString()) ||
      (! parameters["surname"].isString()) ||
      (! common::validateName(parameters["name"].asString())) ||
      (! common::validateName(parameters["surname"].asString()))
      ) {
    common::sendError(builder, 400, "Bad Request", "arguments are invalid");
    return;
  }

  try {
    std::string name = parameters["name"].asString();
    std::string surname = parameters["surname"].asString();
    std::string accountId = codec::computeAccountId(name, surname);

    database::insertAccount(accountId, name, surname);
    Json::Value object = Json::objectValue;
    object["accountId"] = accountId;
    std::string result = utility::jsonToString(object);

    builder
      .status(201, "Created")
      .header("Content-Type", "application/json")
      .body(result)
      .sendWithEOM();
    return;
  } catch(...) {
    common::sendError(builder, 409, "Conflict", "conflict with existing data");
    return;
  }
}
