#include "ApiAccountDetailsController.hpp"

#include <proxygen/httpserver/RequestHandler.h>
#include <proxygen/httpserver/ResponseBuilder.h>

#include "../database/Database.hpp"
#include "../utility/Utility.hpp"
#include "../codec/Codec.hpp"
#include "Validation.hpp"
#include "FastResponse.hpp"

rest::ApiAccountDetailsController::ApiAccountDetailsController(std::string accountId) {
  if (rest::Validation::validateId(accountId)) {
    this->accountId = accountId;
  }
}

void rest::ApiAccountDetailsController::onRequest
(std::unique_ptr<proxygen::HTTPMessage> req) noexcept {
  proxygen::ResponseBuilder builder(downstream_);
  
  if (this->accountId == "") {
    this->alreadySent = true;
    rest::sendError(builder, 400, "Bad Request", "invalid accountId supplied");
    return;
  } else if (req->getMethod() == proxygen::HTTPMethod::GET) {
    Json::Value* metadata = NULL;
    try {
      metadata = database::getAccountMetadata(this->accountId);
    } catch(...) {
      this->alreadySent = true;
      rest::sendError(builder, 404, "Not Found", "accountId not in database");
      return;
    }

    Json::Value* transactions = NULL;
    try {
      transactions = database::getTransactions(this->accountId);
    } catch(...) {
      this->alreadySent = true;
      rest::sendError(builder, 500, "Internal Server Error", "unable to fetch data");
      return;
    }

    Json::Value object = Json::objectValue;
    object["name"] = metadata->operator[]("name");
    object["surname"] = metadata->operator[]("surname");
    object["credit"] = metadata->operator[]("credit");
    object["transactions"] = *transactions;

    std::string result = utility::jsonToString(object);
    std::string headerValue = object["name"].asString() + ";" + object["surname"].asString();
    delete metadata; delete transactions;
    this->alreadySent = true;
    builder
      .status(200, "OK")
      .header("X-Sistema-Bancario", headerValue)
      .header("Content-Type", "application/json")
      .body(result)
      .sendWithEOM();
  } else if (req->getMethod() == proxygen::HTTPMethod::HEAD) {
    Json::Value* metadata = NULL;
    try {
      metadata = database::getAccountMetadata(this->accountId);
    } catch(...) {
      this->alreadySent = true;
      rest::sendError(builder, 404, "Not Found", "accountId not in database");
      return;
    }

    std::string headerValue = metadata->operator[]("name").asString() +
      ";" + metadata->operator[]("surname").asString();
    delete metadata;

    this->alreadySent = true;
    builder
      .status(200, "OK")
      .header("X-Sistema-Bancario", headerValue)
      .sendWithEOM();
  } else if (req->getMethod() == proxygen::HTTPMethod::PUT) {
    // placeholder
    this->invokedMethod = proxygen::HTTPMethod::PUT;
  } else if (req->getMethod() == proxygen::HTTPMethod::PATCH) {
    // placeholder
    this->invokedMethod = proxygen::HTTPMethod::PATCH;
  } else if (req->getMethod() == proxygen::HTTPMethod::POST) {
    // placeholder
    this->invokedMethod = proxygen::HTTPMethod::POST;
  } else {
    this->alreadySent = true;
    rest::sendError(builder, 501, "Not Implemented", "method not implemented");
    return;
  }
}

void rest::ApiAccountDetailsController::onEOM() noexcept {
  proxygen::ResponseBuilder builder(downstream_);

  // already replied in onRequest
  if (this->alreadySent)
    return;

  // body doesn't exists or is empty
  if (!(this->body_) || (this->body_->size() == 0)) {
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

  // split account to invoked method
  if (this->invokedMethod == proxygen::HTTPMethod::POST) {
    if (
	(!(parameters.isObject())) ||
	(! parameters.isMember("amount")) ||
	(! parameters["amount"].isInt())
	) {
      rest::sendError(builder, 400, "Bad Request", "arguments are invalid");
      return;
    }

    try {
      int amount = parameters["amount"].asInt();
      std::string timestamp = utility::getCurrentTimeStampString();

      std::string transactionId = codec::computeUUID();
      if (amount >= 0)
	database::insertPayment(transactionId, this->accountId, amount, timestamp);
      else {
	Json::Value* currentCredit = database::getCredit(this->accountId);
	if (currentCredit->asInt() < amount) {
	  delete currentCredit;
	  rest::sendError(builder, 400, "Bad Request", "not enouth money for withdraw");
	  return;
	} else {
	  delete currentCredit;
	}
	database::insertWithdraw(transactionId, this->accountId, -amount, timestamp);
      }

      Json::Value* newCredit = database::getCredit(this->accountId);

      Json::Value object = Json::objectValue;
      object["credit"] = *newCredit;
      object["transaction"] = transactionId;
        
      delete newCredit;
      std::string result = utility::jsonToString(object);

      builder
	.status(201, "Created")
	.header("Content-Type", "application/json")
	.body(result)
	.sendWithEOM();
      return;
    } catch(std::exception &err) {
      rest::sendError(builder, 400, "Bad Request", "not enouth money on account");
      return;
    }
  } else if (this->invokedMethod == proxygen::HTTPMethod::PUT) {
    if (
	(!(parameters.isObject())) ||
	(! parameters.isMember("name")) ||
	(! parameters.isMember("surname")) ||
	(! parameters["surname"].isString()) ||
	(! parameters["name"].isString()) ||
	(! Validation::validateName(parameters["name"].asString())) ||
	(! Validation::validateName(parameters["surname"].asString()))
	) {
      rest::sendError(builder, 400, "Bad Request", "arguments are invalid");
      return;
    }
    
    try {
      std::string name = parameters["name"].asString();
      std::string surname = parameters["surname"].asString();

      database::updateNameAndSurname(this->accountId, name, surname);

      builder
	.status(200, "OK")
	.sendWithEOM();
      return;
    } catch(...) {
      rest::sendError(builder, 409, "Conflict", "conflict with existing data");
      return;
    }

  } else if (this->invokedMethod == proxygen::HTTPMethod::PATCH) {
    if (
	(!(parameters.isObject())) ||
	(
	 (! parameters.isMember("name")) &&
	 (! parameters.isMember("surname"))) ||
	(
	 (parameters.isMember("name")) &&
	 (parameters.isMember("surname")))
	) {
      rest::sendError(builder, 400, "Bad Request", "arguments are invalid");
      return;
    }
   
    if (parameters.isMember("name")) {
      if (
	  (! parameters["name"].isString()) ||
	  (! Validation::validateName(parameters["name"].asString()))
	  ) {
	rest::sendError(builder, 400, "Bad Request", "arguments are invalid");
        return;
      }

      try {
        std::string name = parameters["name"].asString();

        database::updateName(this->accountId, name);

        builder
          .status(200, "OK")
          .sendWithEOM();
        return;
      } catch(...) {
	rest::sendError(builder, 409, "Conflict", "conflict with existing data");
        return;
      }
    } else {
      if (
	  (! parameters["surname"].isString()) ||
	  (! Validation::validateName(parameters["surname"].asString()))
	  ) {
	rest::sendError(builder, 400, "Bad Request", "arguments are invalid");
        return;
      }

      try {
        std::string surname = parameters["surname"].asString();

        database::updateSurname(this->accountId, surname);

        builder
          .status(200, "OK")
          .sendWithEOM();
        return;
      } catch(...) {
	rest::sendError(builder, 409, "Conflict", "conflict with existing data");
        return;
      } 
    }
  }
}
