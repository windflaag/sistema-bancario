#include "ApiTransactionDetailsController.hpp"

#include <proxygen/httpserver/RequestHandler.h>
#include <proxygen/httpserver/ResponseBuilder.h>

#include "../database/Database.hpp"
#include "../utility/Utility.hpp"
#include "../codec/Codec.hpp"
#include "../common/Validation.hpp"
#include "../common/FastResponse.hpp"

rest::ApiTransactionDetailsController::ApiTransactionDetailsController(std::string transactionId) {
  if (common::validateUUID(transactionId)) {
    this->transactionId = transactionId;
  }
}

void rest::ApiTransactionDetailsController::onRequest
(std::unique_ptr<proxygen::HTTPMessage> req) noexcept {
  proxygen::ResponseBuilder builder(downstream_);
  
  if (this->transactionId == "") {
    this->alreadySent = true;
    common::sendError(builder, 400, "Bad Request", "invalid transactionId supplied");
    return;
  } else if (req->getMethod() == proxygen::HTTPMethod::GET) {
    Json::Value* metadata = NULL;
    
    try {
      metadata = database::getTransactionData(this->transactionId);
    } catch(std::exception &err) {
      this->alreadySent = true;
      common::sendError(builder, 404, "Not Found", "transactionId not in database");
      return;
    }

    Json::Value object = Json::objectValue;
    object["transactionId"] = metadata->operator[]("transactionId");
    object["recipient"] = metadata->operator[]("recipient");
    object["timestamp"] = metadata->operator[]("timestamp");
    object["amount"] = metadata->operator[]("amount");

    std::string result = utility::jsonToString(object);
    delete metadata;
    this->alreadySent = true;
    builder
      .status(200, "OK")
      .header("Content-Type", "application/json")
      .body(result)
      .sendWithEOM();
  } else {
    this->alreadySent = true;
    common::sendError(builder, 501, "Not Implemented", "method not implemented");
    return;
  }
}

void rest::ApiTransactionDetailsController::onEOM() noexcept {
  //
}
