#include "StaticController.hpp"

namespace static_engine {

    StaticController::StaticController() {
        //
    }

    void StaticController::onRequest(std::unique_ptr<proxygen::HTTPMessage> headers) noexcept {
      if (headers->getMethod() != proxygen::HTTPMethod::GET) {
        proxygen::ResponseBuilder(downstream_)
            .status(400, "Bad method")
            .body("Only GET is supported")
            .sendWithEOM();
        return;
      }
      // a real webserver would validate this path didn't contain malicious
      // characters like '//' or '..'
      try {
        // + 1 to kill leading /
        file_ = std::make_unique<folly::File>(
            headers->getPathAsStringPiece().subpiece(1));
      } catch (const std::system_error& ex) {
        proxygen::ResponseBuilder(downstream_)
            .status(404, "Not Found")
            .body(folly::to<std::string>("Could not find ",
                                         headers->getPathAsStringPiece(),
                                         " ex=",
                                         folly::exceptionStr(ex)))
            .sendWithEOM();
        return;
      }
      proxygen::ResponseBuilder(downstream_).status(200, "Ok").send();
      // use a CPU executor since read(2) of a file can block
      readFileScheduled_ = true;
      folly::getUnsafeMutableGlobalCPUExecutor()->add(
          std::bind(&StaticController::readFile,
                    this,
                    folly::EventBaseManager::get()->getEventBase()));
    }

    void StaticController::readFile(folly::EventBase* evb) {
      folly::IOBufQueue buf;
      while (file_ && !paused_) {
        // read 4k-ish chunks and foward each one to the client
        auto data = buf.preallocate(4000, 4000);
        auto rc = folly::readNoInt(file_->fd(), data.first, data.second);
        if (rc < 0) {
          // error
          VLOG(4) << "Read error=" << rc;
          file_.reset();
          evb->runInEventBaseThread([this] {
            LOG(ERROR) << "Error reading file";
            downstream_->sendAbort();
          });
          break;
        } else if (rc == 0) {
          // done
          file_.reset();
          VLOG(4) << "Read EOF";
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
          VLOG(4) << "Resuming deferred readFile";
          onEgressResumed();
        }
      });
    }

    void StaticController::onEgressPaused() noexcept {
      // This will terminate readFile soon
      VLOG(4) << "StaticController paused";
      paused_ = true;
    }

    void StaticController::onEgressResumed() noexcept {
      VLOG(4) << "StaticController resumed";
      paused_ = false;
      // If readFileScheduled_, it will reschedule itself
      if (!readFileScheduled_ && file_) {
        readFileScheduled_ = true;
        folly::getUnsafeMutableGlobalCPUExecutor()->add(
            std::bind(&StaticController::readFile,
                      this,
                      folly::EventBaseManager::get()->getEventBase()));
      } else {
        VLOG(4) << "Deferred scheduling readFile";
      }
    }

    void StaticController::onBody(std::unique_ptr<folly::IOBuf> /*body*/) noexcept {
      // ignore, only support GET
    }

    void StaticController::onEOM() noexcept {
    }

    void StaticController::onUpgrade(proxygen::UpgradeProtocol /*protocol*/) noexcept {
      // handler doesn't support upgrades
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
        VLOG(4) << "deleting StaticController";
        delete this;
        return true;
      }
      return false;
    }

}
