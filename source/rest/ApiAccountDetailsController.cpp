#include "ApiAccountDetailsController.hpp"

#include <proxygen/httpserver/RequestHandler.h>
#include <proxygen/httpserver/ResponseBuilder.h>

#include "../database/Database.hpp"
#include "../utility/Utility.hpp"
#include "../codec/Codec.hpp"
#include "Validation.hpp"

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
    builder
      .status(400, "Bad Request")
      .send();
    return;
  } else if (req->getMethod() == proxygen::HTTPMethod::GET) {
    Json::Value* metadata = NULL;
    try {
      metadata = database::getAccountMetadata(this->accountId);
    } catch(...) {
      this->alreadySent = true;
      builder
	.status(404, "Not Found")
	.send();
      return;
    }

    Json::Value* transactions = NULL;
    try {
      transactions = database::getTransactions(this->accountId);
    } catch(...) {
      this->alreadySent = true;
      builder
	.status(500, "Internal Server Error")
	.send();
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
      .send();
  } else if (req->getMethod() == proxygen::HTTPMethod::HEAD) {
    Json::Value* metadata = NULL;
    try {
      metadata = database::getAccountMetadata(this->accountId);
    } catch(...) {
      this->alreadySent = true;
      builder
	.status(404, "Not Found")
	.send();
      return;
    }

    std::string headerValue = metadata->operator[]("name").asString() +
      ";" + metadata->operator[]("surname").asString();
    delete metadata;

    this->alreadySent = true;
    builder
      .status(200, "OK")
      .header("X-Sistema-Bancario", headerValue)
      .send();
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
    builder
      .status(501, "Not implemented")
      .send();
    return;
  }
}

void rest::ApiAccountDetailsController::onEOM() noexcept {
  proxygen::ResponseBuilder builder(downstream_);
  if (
      !(this->body_) ||
      (this->body_->size() == 0)) {
    if (!(this->alreadySent))
      builder.status(400, "Bad Request");
    builder.sendWithEOM();
    return;
  }

 Json::Value parameters;
  Json::Reader* text_reader = new Json::Reader();
  if (!(text_reader->parse(*(this->body_), parameters))) {
    builder
      .status(400, "Bad Request")
      .sendWithEOM();
    return;
  }

  delete text_reader;

  // split account to invoked method
  if (this->invokedMethod == proxygen::HTTPMethod::POST) {
      if (
          (!(parameters.isObject())) ||
          (! parameters.isMember("amount")) ||
          (! parameters["amount"].isInt())
          ) {
        builder
          .status(400, "Bad Request")
          .sendWithEOM();
        return;
      }

      try {
        int amount = parameters["amount"].asInt();
        std::string timestamp = utility::getCurrentTimeStampString();

        std::string transactionId = codec::computeUUID();
        if (amount > 0)
            database::insertPayment(transactionId, this->accountId, amount, timestamp);
        else {
            Json::Value* currentCredit = database::getCredit(this->accountId);
            if (currentCredit->asInt() < amount) {
                delete currentCredit;
                throw std::runtime_error("");
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
      } catch(...) {
        builder
          .status(409, "Conflict")
          .sendWithEOM();
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
        builder
          .status(400, "Bad Request")
          .sendWithEOM();
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
        builder
          .status(409, "Conflict")
          .sendWithEOM();
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
        builder
          .status(400, "Bad Request")
          .sendWithEOM();
        return;
    }
   
    if (parameters.isMember("name")) {
      if (
            (! parameters["name"].isString()) ||
            (! Validation::validateName(parameters["name"].asString()))
        ) {
        builder
            .status(400, "Bad Request")
            .sendWithEOM();
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
        builder
          .status(409, "Conflict")
          .sendWithEOM();
        return;
      }
    } else {
       if (
            (! parameters["surname"].isString()) ||
            (! Validation::validateName(parameters["surname"].asString()))
        ) {
        builder
            .status(400, "Bad Request")
            .sendWithEOM();
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
        builder
          .status(409, "Conflict")
          .sendWithEOM();
        return;
      } 
    }
  }
}
