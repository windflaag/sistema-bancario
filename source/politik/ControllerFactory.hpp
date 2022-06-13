#ifndef __CONTROLLER_FACTORY__
#define __CONTROLLER_FACTORY__

#include <proxygen/httpserver/RequestHandlerFactory.h>

namespace politik {

    class ControllerFactory : public proxygen::RequestHandlerFactory {
        public:
            void onServerStart(folly::EventBase* evb) noexcept override;
            void onServerStop() noexcept override;
            proxygen::RequestHandler* onRequest(proxygen::RequestHandler*, proxygen::HTTPMessage*) noexcept override;
    };

}

#endif
