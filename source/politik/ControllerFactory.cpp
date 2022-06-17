#include "ControllerFactory.hpp"
#include "Detection.hpp"

#include "../rest/ApiTransferController.hpp"
#include "../rest/ApiDivertController.hpp"
#include "../rest/ApiAccountController.hpp"
#include "../rest/ApiTransactionDetailsController.hpp"
#include "../rest/ApiAccountDetailsController.hpp"
#include "../static_engine/StaticController.hpp"

void politik::ControllerFactory::onServerStart(folly::EventBase* /*evb*/) noexcept {}

void politik::ControllerFactory::onServerStop() noexcept {}

proxygen::RequestHandler* politik::ControllerFactory::onRequest(
								proxygen::RequestHandler* /*handler*/,
								proxygen::HTTPMessage* message) noexcept {
  if (message->getPath() == "/api/account") {
    return new rest::ApiAccountController();
  } else if (message->getPath() == "/api/transfer") {
    return new rest::ApiTransferController();
  } else if (message->getPath() == "/api/divert") {
    return new rest::ApiDivertController();
  } else if (politik::detection::detectAccountId(message->getPath())) {
    return new rest::ApiAccountDetailsController(message->getPath().substr(13));
  } else if (politik::detection::detectTransactionId(message->getPath())) {
    return new rest::ApiTransactionDetailsController(message->getPath().substr(17));
  } else {
    return new static_engine::StaticController(message->getPath());
  }
}
