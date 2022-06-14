#include "ApiController.hpp"

#include <proxygen/httpserver/RequestHandler.h>
#include <proxygen/httpserver/ResponseBuilder.h>

#include "../database/Database.hpp"
#include "../utility/Utility.hpp"

rest::ApiController::ApiController() {
    //
}

void rest::ApiController::onRequest
    (std::unique_ptr<proxygen::HTTPMessage> req) noexcept {
    
    Json::Value* jsonArray = database::Database::launchQuery(
        "select accountId, name, surname from Account;"
    );

    Json::Value object;
    object["list"] = *jsonArray;
    
    std::string strData = utility::jsonToString(object);
    delete jsonArray;

    proxygen::ResponseBuilder builder(downstream_);
    builder
        .status(200, "OK")
        .header("Content-Type", "application/json")
        .body(strData)
        .send();
}

