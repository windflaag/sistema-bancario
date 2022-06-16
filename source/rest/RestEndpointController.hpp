#ifndef __REST_ENDPOINT_CONTROLLER__
#define __REST_ENDPOINT_CONTROLLER__

#include <folly/Memory.h>
#include <proxygen/httpserver/RequestHandler.h>

namespace rest {
    class RestEndpointController : public proxygen::RequestHandler {
     public:
      explicit RestEndpointController();

      void onBody(std::unique_ptr<folly::IOBuf> body) noexcept override;

      void onEOM() noexcept override;

      void onUpgrade(proxygen::UpgradeProtocol proto) noexcept override;

      void requestComplete() noexcept override;

      void onError(proxygen::ProxygenError err) noexcept override;

     protected:
      // std::unique_ptr<folly::IOBuf> body_;
      std::unique_ptr<std::string> body_;
    };

}

#endif
