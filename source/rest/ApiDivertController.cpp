#include "ApiDivertController.hpp"

#include <proxygen/httpserver/RequestHandler.h>
#include <proxygen/httpserver/ResponseBuilder.h>

#include "../database/Database.hpp"
#include "../utility/Utility.hpp"
#include "../codec/Codec.hpp"
#include "Validation.hpp"
#include "FastResponse.hpp"

rest::ApiDivertController::ApiDivertController() {
  //
}

void rest::ApiDivertController::onRequest
(std::unique_ptr<proxygen::HTTPMessage> req) noexcept {
  proxygen::ResponseBuilder builder(downstream_);
  if (req->getMethod() == proxygen::HTTPMethod::POST) {
    // placeholder
  } else {
    this->alreadySent = true;
    rest::sendError(builder, 501, "Not Implemented", "on this endpoint this method isn't be implemented");
    return;
  }
}

void rest::ApiDivertController::onEOM() noexcept {
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
      (! parameters.isMember("id")) ||
      (! parameters["id"].isString()) ||
      (! Validation::validateUUID(parameters["id"].asString()))
      ) {
    rest::sendError(builder, 400, "Bad Request", "wrong arguments supplied");
    return;
  }

  try {
    std::string transactionId = parameters["id"].asString();
    Json::Value* metadata = database::getTransactionMetadata(transactionId);

    int fromCredit = metadata->operator[]("toCredit").asInt();
    int toCredit = metadata->operator[]("fromCredit").asInt();
    int amount = metadata->operator[]("amount").asInt();

    if (fromCredit < amount) {
      rest::sendError(builder, 400, "Bad Request", "no enouth money on transaction#from account");
      return;
    }

    std::string fromId = metadata->operator[]("toId").asString();
    std::string toId = metadata->operator[]("fromId").asString();

    std::string timestamp = utility::getCurrentTimeStampString();
    transactionId = codec::computeUUID();
    database::insertTransaction(transactionId, fromId, amount, toId, timestamp);

    Json::Value object = Json::objectValue;
    object[fromId] = fromCredit - amount;
    object[toId] = toCredit + amount;
    object["transaction"] = transactionId;
    
    delete metadata;
    std::string result = utility::jsonToString(object);

    builder
      .status(201, "Created")
      .header("Content-Type", "application/json")
      .body(result)
      .sendWithEOM();
    return;
  } catch(std::exception &err) {
    rest::sendError(builder, 500, "Internal Server Error", std::string("something went wrong with execution of correct request: ") + err.what());
    return;
  }
}
