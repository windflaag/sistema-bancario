#include "ApiTransferController.hpp"

#include <proxygen/httpserver/RequestHandler.h>
#include <proxygen/httpserver/ResponseBuilder.h>

#include "../database/Database.hpp"
#include "../utility/Utility.hpp"
#include "../codec/Codec.hpp"
#include "Validation.hpp"

rest::ApiTransferController::ApiTransferController() {
  //
}

void rest::ApiTransferController::onRequest
(std::unique_ptr<proxygen::HTTPMessage> req) noexcept {
  proxygen::ResponseBuilder builder(downstream_);
  if (req->getMethod() == proxygen::HTTPMethod::POST) {
    // placeholder
  } else {
    this->alreadySent = true;
    builder
      .status(501, "Not implemented")
      .send();
    return;
  }
}

void rest::ApiTransferController::onEOM() noexcept {
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

  if (
      (!(parameters.isObject())) ||
      (! parameters.isMember("from")) ||
      (! parameters.isMember("amount")) ||
      (! parameters.isMember("to")) ||
      (! parameters["from"].isString()) ||
      (! parameters["amount"].isInt()) ||
      (! parameters["to"].isString()) ||
      (! Validation::validateId(parameters["from"].asString())) ||
      (! Validation::validateAmount(parameters["amount"].asInt())) ||
      (! Validation::validateId(parameters["to"].asString()))
      ) {
    builder
      .status(400, "Bad Request")
      .sendWithEOM();
    return;
  }

  try {
    std::string fromId = parameters["from"].asString();
    int amount = parameters["amount"].asInt();
    std::string toId = parameters["to"].asString();
    std::string timestamp = utility::getCurrentTimeStampString();

    Json::Value* currentCredit = database::getCredit(fromId);
    if (currentCredit->asInt() < amount) {
        delete currentCredit;
        throw std::runtime_error("");
    } else {
        delete currentCredit;
    }

    std::string transactionId = codec::computeUUID();
    database::insertTransaction(transactionId, fromId, amount, toId, timestamp);

    Json::Value* newCreditFrom = database::getCredit(fromId);
    Json::Value* newCreditTo = database::getCredit(toId);

    Json::Value object = Json::objectValue;
    object[fromId] = *newCreditFrom;
    object[toId] = *newCreditTo;
    object["transaction"] = transactionId;
    
    delete newCreditFrom; delete newCreditTo;
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
}
