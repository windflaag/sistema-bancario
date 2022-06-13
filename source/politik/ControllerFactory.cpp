#include "ControllerFactory.hpp"

#include "../rest/ApiController.hpp"
#include "../static_engine/StaticController.hpp"

void politik::ControllerFactory::onServerStart(folly::EventBase* evb) noexcept {}

void politik::ControllerFactory::onServerStop() noexcept {}

proxygen::RequestHandler* politik::ControllerFactory::onRequest(
        proxygen::RequestHandler* handler,
        proxygen::HTTPMessage* message) noexcept {
    if (message->getPath() == "/api")
        return new rest::ApiController();

    return new static_engine::StaticController();
}
