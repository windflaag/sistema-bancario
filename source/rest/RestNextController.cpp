#include "RestNextController.hpp"

#include <proxygen/httpserver/RequestHandler.h>
#include <proxygen/httpserver/ResponseBuilder.h>

namespace rest {

    RestNextController::RestNextController() {
        //
    }

    void RestNextController::onRequest(std::unique_ptr<proxygen::HTTPMessage> req) noexcept {
        proxygen::ResponseBuilder builder(downstream_);
      
        builder
            .status(200, "OK")
            .body("{ \"nome\" : \"qualcosa\" }")
            .send();
    }

    void RestNextController::onBody(std::unique_ptr<folly::IOBuf> body) noexcept {
      proxygen::ResponseBuilder(downstream_).body(std::move(body)).send();
    }

    void RestNextController::onEOM() noexcept {
      proxygen::ResponseBuilder(downstream_).sendWithEOM();
    }

    void RestNextController::onUpgrade(UpgradeProtocol /*protocol*/) noexcept {
      // handler doesn't support upgrades
    }

    void RestNextController::requestComplete() noexcept {
      delete this;
    }

    void RestNextController::onError(ProxygenError /*err*/) noexcept {
      delete this;
    }

}
