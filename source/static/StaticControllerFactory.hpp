#ifndef __STATIC_CONTROLLER_FACTORY__
#define __STATIC_CONTROLLER_FACTORY__

#include <proxygen/httpserver/RequestHandlerFactory.h>
namespace static_engine {

    class StaticControllerFactory : public proxygen::RequestHandlerFactory {
        public:
            void onServerStart(folly::EventBase* evb) noexcept override;
            void onServerStop() noexcept override;
            proxygen::RequestHandler* onRequest(proxygen::RequestHandler*, proxygen::HTTPMessage*) noexcept override;
    };

}

#endif
