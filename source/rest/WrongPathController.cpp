#include "WrongPathController.hpp"

#include <proxygen/httpserver/RequestHandler.h>
#include <proxygen/httpserver/ResponseBuilder.h>


rest::WrongPathController::WrongPathController() {
    //
}

void rest::WrongPathController::onRequest(std::unique_ptr<proxygen::HTTPMessage> req) noexcept {
    proxygen::ResponseBuilder builder(downstream_);
    builder
        .status(404, "Not Found")
        .send();
}

