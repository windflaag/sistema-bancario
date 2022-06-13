#include "ApiController.hpp"

#include <proxygen/httpserver/RequestHandler.h>
#include <proxygen/httpserver/ResponseBuilder.h>


rest::ApiController::ApiController() {
    //
}

void rest::ApiController::onRequest(std::unique_ptr<proxygen::HTTPMessage> req) noexcept {
    proxygen::ResponseBuilder builder(downstream_);
  
    builder
        .status(200, "OK")
        .header("Content-Type", "application/json")
        .body("{ \"scemo\" : \"chi legge\" }")
        .send();
}

