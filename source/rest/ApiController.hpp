#ifndef __API_CONTROLLER__
#define __API_CONTROLLER__

#include <folly/Memory.h>
#include <proxygen/httpserver/RequestHandler.h>
#include "RestEndpointController.hpp"

namespace rest {

    class ApiController : public RestEndpointController {
     public:
      ApiController();

      void onRequest(
          std::unique_ptr<proxygen::HTTPMessage> headers) noexcept override;
    };

}

#endif
