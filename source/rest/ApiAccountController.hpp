#ifndef __API_ACCOUNT_CONTROLLER__
#define __API_ACCOUNT_CONTROLLER__

#include <folly/Memory.h>
#include <proxygen/httpserver/RequestHandler.h>
#include "RestEndpointController.hpp"

namespace rest {

  class ApiAccountController : public RestEndpointController {
  private:
    bool alreadySent = false;
  public:
    ApiAccountController();
    
    void onEOM() noexcept override;
    
    void onRequest(
		   std::unique_ptr<proxygen::HTTPMessage> headers) noexcept override;
  };

}

#endif
