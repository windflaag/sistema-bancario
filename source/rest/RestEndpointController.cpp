#include "RestEndpointController.hpp"

#include <proxygen/httpserver/RequestHandler.h>
#include <proxygen/httpserver/ResponseBuilder.h>

namespace rest {

  RestEndpointController::RestEndpointController() {
    //
  }

  void RestEndpointController::onRequest(
		 std::unique_ptr<proxygen::HTTPMessage> /*headers*/) noexcept {
    // empty
  }
    
  void RestEndpointController::onBody(std::unique_ptr<folly::IOBuf> body) noexcept {
    if (!(this->body_))
      this->body_ = std::unique_ptr<std::string>(new std::string());

    std::string nextPartOfBody = ""; body->appendTo(nextPartOfBody);
    this->body_->append(nextPartOfBody);
  }
    
  void RestEndpointController::onEOM() noexcept {
    proxygen::ResponseBuilder(downstream_).sendWithEOM();
  }
    
  void RestEndpointController::onUpgrade(proxygen::UpgradeProtocol /*protocol*/) noexcept {
    // handler doesn't support upgrades
  }
    
  void RestEndpointController::requestComplete() noexcept {
    delete this;
  }

  void RestEndpointController::onError(proxygen::ProxygenError /*err*/) noexcept {
    delete this;
  }

}
