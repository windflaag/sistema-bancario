#include "ApiTransferController.hpp"

#include <proxygen/httpserver/RequestHandler.h>
#include <proxygen/httpserver/ResponseBuilder.h>

#include "../database/Database.hpp"
#include "../utility/Utility.hpp"
#include "../codec/Codec.hpp"
#include "Validation.hpp"
#include "FastResponse.hpp"

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
    rest::sendError(builder, 501, "Not Implemented", "method not implemented");
    return;
  }
}

void rest::ApiTransferController::onEOM() noexcept {
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
    rest::sendError(builder, 400, "Bad Request", "arguments are invalid");
    return;
  }

  try {
    std::string fromId = parameters["from"].asString();
    int amount = parameters["amount"].asInt();
    std::string toId = parameters["to"].asString();
    std::string timestamp = utility::getCurrentTimeStampString();

    Json::Value* fromCredit = database::getCredit(fromId);
    if (fromCredit->asInt() < amount) {
      delete fromCredit;
      rest::sendError(builder, 400, "Bad Request", "not enouth money for transaction");
      return;
    }

    Json::Value* toCredit = database::getCredit(toId);
    std::string transactionId = codec::computeUUID();
    database::insertTransaction(transactionId, fromId, amount, toId, timestamp);

    Json::Value object = Json::objectValue;
    object[fromId] = fromCredit->asInt() - amount;
    object[toId] = toCredit->asInt() + amount;
    object["transaction"] = transactionId;
    
    delete fromCredit; delete toCredit;
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
