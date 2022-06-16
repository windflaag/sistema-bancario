#ifndef __API_DIVERT_CONTROLLER__
#define __API_DIVERT_CONTROLLER__

#include <folly/Memory.h>
#include <proxygen/httpserver/RequestHandler.h>
#include "RestEndpointController.hpp"

namespace rest {

  class ApiDivertController : public RestEndpointController {
  private:
    bool alreadySent = false;
  public:
    ApiDivertController();
    
    void onEOM() noexcept override;
    
    void onRequest(
		   std::unique_ptr<proxygen::HTTPMessage> headers) noexcept override;
  };

}

#endif
