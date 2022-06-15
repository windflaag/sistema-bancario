#include "ControllerFactory.hpp"
#include "Detection.hpp"

#include "../rest/ApiTransferController.hpp"
#include "../rest/ApiAccountController.hpp"
#include "../rest/ApiAccountDetailsController.hpp"
#include "../rest/WrongPathController.hpp"
#include "../static_engine/StaticController.hpp"

void politik::ControllerFactory::onServerStart(folly::EventBase* /*evb*/) noexcept {}

void politik::ControllerFactory::onServerStop() noexcept {}

proxygen::RequestHandler* politik::ControllerFactory::onRequest(
        proxygen::RequestHandler* /*handler*/,
        proxygen::HTTPMessage* message) noexcept {
    proxygen::HTTPHeaders headers = message->getHeaders();

    if (politik::Detection::detectBrowserInUserAgent(headers.getSingleOrEmpty("User-Agent").c_str())) {
        // from Static to Browser
        return new static_engine::StaticController();
    } else {
        // from API to Curler
        if (message->getPath() == "/api/account") {
            return new rest::ApiAccountController();
        } else if (politik::Detection::detectAccountId(message->getPath())) {
            return new rest::ApiAccountDetailsController(message->getPath().substr(13));
        } else if (message->getPath() == "/api/transfer")
            return new rest::ApiTransferController();
       return new rest::WrongPathController();
    }
}
