#ifndef __REST_ROOT_CONTROLLER__
#define __REST_ROOT_CONTROLLER__

#include <folly/Memory.h>
#include <proxygen/httpserver/RequestHandler.h>

namespace rest {

class RestRootController : public proxygen::RequestHandler {
 public:
  explicit RestRootController();

  void onRequest(
      std::unique_ptr<proxygen::HTTPMessage> headers) noexcept override;

  void onBody(std::unique_ptr<folly::IOBuf> body) noexcept override;

  void onEOM() noexcept override;

  void onUpgrade(proxygen::UpgradeProtocol proto) noexcept override;

  void requestComplete() noexcept override;

  void onError(proxygen::ProxygenError err) noexcept override;

 private:
  std::unique_ptr<folly::IOBuf> body_;
};

}

#endif
