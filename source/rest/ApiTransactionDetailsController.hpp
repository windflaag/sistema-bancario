#ifndef __API_TRANSACTION_DETAILS_CONTROLLER__
#define __API_TRANSACTION_DETAILS_CONTROLLER__

#include <folly/Memory.h>
#include <proxygen/httpserver/RequestHandler.h>
#include "RestEndpointController.hpp"

namespace rest {

  class ApiTransactionDetailsController : public RestEndpointController {
  private:
    bool alreadySent = false;
    std::string transactionId = ""; // assigned if is valid
  public:
    // called this way because it will be checked inside the servlet
    ApiTransactionDetailsController(std::string pseudo_transactionId);

    void onEOM() noexcept override;
    
    void onRequest(
		   std::unique_ptr<proxygen::HTTPMessage> headers) noexcept override;
  };

}

#endif
