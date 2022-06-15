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

    std::string transactionId = codec::computeUUID();
    database::Database::insertTransaction(transactionId, fromId, amount, toId, timestamp);

    builder
      .status(201, "Created")
      .body(transactionId)
      .sendWithEOM();
    return;
  } catch(...) {
    builder
      .status(409, "Conflict")
      .sendWithEOM();
    return;
  }
}
