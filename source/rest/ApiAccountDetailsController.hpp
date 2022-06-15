#ifndef __API_ACCOUNT_DETAILS_CONTROLLER__
#define __API_ACCOUNT_DETAILS_CONTROLLER__

#include <folly/Memory.h>
#include <proxygen/httpserver/RequestHandler.h>
#include "RestEndpointController.hpp"

namespace rest {

  class ApiAccountDetailsController : public RestEndpointController {
  private:
    bool alreadySent = false;
    std::string accountId = ""; // assigned only if is valid
    folly::Optional<proxygen::HTTPMethod> invokedMethod; // used only with PUT, PATCH, POST
  public:
    // called this way because it will be checked inside the servlet
    ApiAccountDetailsController(std::string pseudo_accountId);
    
    void onEOM() noexcept override;
    
    void onRequest(
		   std::unique_ptr<proxygen::HTTPMessage> headers) noexcept override;
  };

}

#endif
