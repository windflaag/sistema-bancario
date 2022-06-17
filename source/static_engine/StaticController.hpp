#ifndef __STATIC_CONTROLLER__
#define __STATIC_CONTROLLER__

#include <folly/File.h>
#include <folly/Memory.h>
#include <folly/FileUtil.h>
#include <folly/executors/GlobalExecutor.h>
#include <folly/io/async/EventBaseManager.h>
#include <proxygen/httpserver/RequestHandler.h>
#include <proxygen/httpserver/ResponseBuilder.h>

namespace static_engine {

  class StaticController : public proxygen::RequestHandler {
  public:
    StaticController(std::string filepath);

    void onRequest(
		   std::unique_ptr<proxygen::HTTPMessage> headers) noexcept override;

    void onBody(std::unique_ptr<folly::IOBuf> body) noexcept override;

    void onEOM() noexcept override;

    void onUpgrade(proxygen::UpgradeProtocol proto) noexcept override;

    void requestComplete() noexcept override;

    void onError(proxygen::ProxygenError err) noexcept override;

    void onEgressPaused() noexcept override;

    void onEgressResumed() noexcept override;

  private:
    void readFile(folly::EventBase* evb);
    bool checkForCompletion();

    std::unique_ptr<folly::File> file_;
    bool readFileScheduled_{false};
    std::atomic<bool> paused_{false};
    bool finished_{false};
    std::string filepath;
  };

}

#endif
