#include "WrongPathController.hpp"

#include <proxygen/httpserver/RequestHandler.h>
#include <proxygen/httpserver/ResponseBuilder.h>

#include "FastResponse.hpp"

rest::WrongPathController::WrongPathController() {
    //
}

void rest::WrongPathController::onEOM() noexcept {
    proxygen::ResponseBuilder builder(downstream_);
    rest::sendError(builder, 404, "Not Found", "unable to find requested data");
}
