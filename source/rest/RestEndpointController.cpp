#include "RestEndpointController.hpp"

#include <proxygen/httpserver/RequestHandler.h>
#include <proxygen/httpserver/ResponseBuilder.h>

namespace rest {

    RestEndpointController::RestEndpointController() {
        //
    }
    
    void RestEndpointController::onBody(std::unique_ptr<folly::IOBuf> body) noexcept {
      proxygen::ResponseBuilder(downstream_).body(std::move(body)).send();
    }
    
    void RestEndpointController::onEOM() noexcept {
      proxygen::ResponseBuilder(downstream_).sendWithEOM();
    }
    
    void RestEndpointController::onUpgrade(proxygen::UpgradeProtocol protocol) noexcept {
      // handler doesn't support upgrades
    }
    
    void RestEndpointController::requestComplete() noexcept {
      delete this;
    }

    void RestEndpointController::onError(proxygen::ProxygenError err) noexcept {
      delete this;
    }

}
