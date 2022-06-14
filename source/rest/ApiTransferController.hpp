#ifndef __API_TRANSFER_CONTROLLER__
#define __API_TRANSFER_CONTROLLER__

#include <folly/Memory.h>
#include <proxygen/httpserver/RequestHandler.h>
#include "RestEndpointController.hpp"

namespace rest {

    class ApiTransferController : public RestEndpointController {
     public:
      ApiTransferController();

      void onEOM() noexcept override;

      void onRequest(
          std::unique_ptr<proxygen::HTTPMessage> headers) noexcept override;
    };

}

#endif
