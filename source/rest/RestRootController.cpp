#include "RestRootController.hpp"

#include <proxygen/httpserver/RequestHandler.h>
#include <proxygen/httpserver/ResponseBuilder.h>

namespace rest {

    RestRootController::RestRootController() {
        //
    }

    void RestRootController::onRequest(std::unique_ptr<proxygen::HTTPMessage> req) noexcept {
        proxygen::ResponseBuilder builder(downstream_);
      
        builder
            .status(200, "OK")
            .body("{ \"scemo\" : \"chi legge\" }")
            .send();
    }

    void RestRootController::onBody(std::unique_ptr<folly::IOBuf> body) noexcept {
      proxygen::ResponseBuilder(downstream_).body(std::move(body)).send();
    }

    void RestRootController::onEOM() noexcept {
      proxygen::ResponseBuilder(downstream_).sendWithEOM();
    }

    void RestRootController::onUpgrade(UpgradeProtocol /*protocol*/) noexcept {
      // handler doesn't support upgrades
    }

    void RestRootController::requestComplete() noexcept {
      delete this;
    }

    void RestRootController::onError(ProxygenError /*err*/) noexcept {
      delete this;
    }

}
