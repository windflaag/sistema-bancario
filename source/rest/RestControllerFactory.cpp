#include "RestControllerFactory.hpp"
#include "RestRootController.hpp"
#include "RestNextController.hpp"

void rest::RestControllerFactory::onServerStart(folly::EventBase* evb) noexcept {}

void rest::RestControllerFactory::onServerStop() noexcept {}

proxygen::RequestHandler* rest::RestControllerFactory::onRequest(proxygen::RequestHandler*, proxygen::HTTPMessage*) noexcept {
    return new rest::RestRootController();
}
