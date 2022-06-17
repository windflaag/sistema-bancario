#include "ApiAccountController.hpp"

#include <proxygen/httpserver/RequestHandler.h>
#include <proxygen/httpserver/ResponseBuilder.h>

#include "../database/Database.hpp"
#include "../utility/Utility.hpp"
#include "../codec/Codec.hpp"
#include "Validation.hpp"
#include "FastResponse.hpp"

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
      rest::sendError(builder, 500, "Internal Server Error", "unable to fetch data");
      return;
    }
  } else if (req->getMethod() == proxygen::HTTPMethod::POST) {
    // placeholder
  } else if (req->getMethod() == proxygen::HTTPMethod::DELETE) {
    this->alreadySent = true;
    if (
	(! req->hasQueryParam("id")) ||
	(! Validation::validateId(req->getQueryParam("id")))
        ) {
      rest::sendError(builder, 400, "Bad Request", "arguments are invalid");
      return;
    }

    try {
      std::string accountId = req->getQueryParam("id");

      database::deleteAccount(accountId);
           
      builder
	.status(200, "OK")
	.sendWithEOM();
    } catch(...) {
      rest::sendError(builder, 500, "Internal Server Error", "unable to delete data");
      return;
    }
  } else {
    this->alreadySent = true;
      rest::sendError(builder, 501, "Not Implemented", "method not implemented");
    return;
  }
}

void rest::ApiAccountController::onEOM() noexcept {
  proxygen::ResponseBuilder builder(downstream_);
  if (
      !(this->body_) ||
      (this->body_->size() == 0)) {
    if (!(this->alreadySent))
      rest::sendError(builder, 400, "Bad Request", "body is empty");
    return;
  }

 Json::Value parameters;
  try {
    parameters = codec::parseBody(this->body_.get());
  } catch(...) {
      rest::sendError(builder, 400, "Bad Request", "cannot parse body");
      return;
  }

  if (
      (!(parameters.isObject())) ||
      (! parameters.isMember("name")) ||
      (! parameters.isMember("surname")) ||
      (! parameters["name"].isString()) ||
      (! parameters["surname"].isString()) ||
      (! Validation::validateName(parameters["name"].asString())) ||
      (! Validation::validateName(parameters["surname"].asString()))
      ) {
    rest::sendError(builder, 400, "Bad Request", "arguments are invalid");
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
      rest::sendError(builder, 409, "Conflict", "conflict with existing data");
    return;
  }
}
