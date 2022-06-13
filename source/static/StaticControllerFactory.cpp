#include "StaticControllerFactory.hpp"
#include "StaticController.hpp"

namespace static_engine {

    void StaticControllerFactory::onServerStart(folly::EventBase* evb) noexcept {}

    void StaticControllerFactory::onServerStop() noexcept {}

    proxygen::RequestHandler* StaticControllerFactory::onRequest(proxygen::RequestHandler*, proxygen::HTTPMessage*) noexcept {
        return new StaticController();
    }

}
