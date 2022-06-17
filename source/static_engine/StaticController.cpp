#include "StaticController.hpp"
#include "../common/Validation.hpp"
#include "../singleton/Singleton.hpp"
#include "../common/FastResponse.hpp"

namespace static_engine {

  StaticController::StaticController(std::string filepath) {
    this->filepath = filepath;
  }

  void StaticController::onRequest(std::unique_ptr<proxygen::HTTPMessage> headers) noexcept {
    proxygen::ResponseBuilder builder (downstream_);

    // accept only GET requests
    if (headers->getMethod() != proxygen::HTTPMethod::GET) {
      common::sendError(builder, 400, "Bad Method", "Only GET is supported");
      return;
    }
    
    try {
      // validate the path
      this->filepath = common::validatePath(this->filepath);
    } catch (...) {
      common::sendError(builder, 403, "Forbidden", "Don't have permission to explore this path");
      return;
    }
    
    try {
      // find file in filesystem
      file_ = std::make_unique<folly::File>(filepath);
    } catch (...) {
      common::sendError(builder, 404, "Not Found", "Unable to find requested resource");
      return;
    }

    // init response
    builder
      .status(200, "Ok")
      .header("Content-Type", common::getContentType(this->filepath))
      .send();

    // start read iter
    readFileScheduled_ = true;
    folly::getUnsafeMutableGlobalCPUExecutor()->add(
						    std::bind(&StaticController::readFile,
							      this,
							      folly::EventBaseManager::get()->getEventBase()));
  }

  void StaticController::readFile(folly::EventBase* evb) {
    folly::IOBufQueue buf;
    while (file_ && !paused_) {

      // using chuncked read
      auto data = buf.preallocate(4000, 4000);
      auto rc = folly::readNoInt(file_->fd(), data.first, data.second);

      if (rc < 0) {
	// error
	file_.reset();
	evb->runInEventBaseThread([this] {
	  downstream_->sendAbort();
	});
	break;
      } else if (rc == 0) {
	// done
	file_.reset();
	evb->runInEventBaseThread(
				  [this] { proxygen::ResponseBuilder(downstream_).sendWithEOM(); });
	break;
      } else {
	buf.postallocate(rc);
	evb->runInEventBaseThread([this, body = buf.move()]() mutable {
	  proxygen::ResponseBuilder(downstream_).body(std::move(body)).send();
	});
      }
    }

    // Notify the request thread that we terminated the readFile loop
    evb->runInEventBaseThread([this] {
      readFileScheduled_ = false;
      if (!checkForCompletion() && !paused_) {
	onEgressResumed();
      }
    });
  }

  void StaticController::onEgressPaused() noexcept {
    // This will terminate readFile soon
    paused_ = true;
  }

  void StaticController::onEgressResumed() noexcept {
    paused_ = false;
    // If readFileScheduled_, it will reschedule itself
    if (!readFileScheduled_ && file_) {
      readFileScheduled_ = true;
      folly::getUnsafeMutableGlobalCPUExecutor()->add(
						      std::bind(&StaticController::readFile,
								this,
								folly::EventBaseManager::get()->getEventBase()));
    }
  }

  void StaticController::onBody(std::unique_ptr<folly::IOBuf> /*body*/) noexcept {
    // ignore, only support GET
  }

  void StaticController::onEOM() noexcept {
  }

  void StaticController::onUpgrade(proxygen::UpgradeProtocol /*protocol*/) noexcept {
    // this handler doesn't support upgrades
  }

  void StaticController::requestComplete() noexcept {
    finished_ = true;
    paused_ = true;
    checkForCompletion();
  }

  void StaticController::onError(proxygen::ProxygenError /*err*/) noexcept {
    finished_ = true;
    paused_ = true;
    checkForCompletion();
  }

  bool StaticController::checkForCompletion() {
    if (finished_ && !readFileScheduled_) {
      delete this;
      return true;
    }
    return false;
  }

}
