#include "StaticControllerFactory.hpp"
#include "StaticController.hpp"

void StaticControllerFactory::onServerStart(folly::EventBase* evb) noexcept {}

void StaticControllerFactory::onServerStop() noexcept {}

proxygen::RequestHandler* StaticControllerFactory::onRequest(proxygen::RequestHandler*, proxygen::HTTPMessage*) noexcept {
    return new StaticController();
}
