#ifndef __WRONG_PATH_CONTROLLER__
#define __WRONG_PATH_CONTROLLER__

#include <folly/Memory.h>
#include <proxygen/httpserver/RequestHandler.h>
#include "RestEndpointController.hpp"

namespace rest {

    class WrongPathController : public RestEndpointController {
     public:
      WrongPathController();

      void onRequest(
          std::unique_ptr<proxygen::HTTPMessage> headers) noexcept override;
    };

}

#endif
