#include "ApiController.hpp"

#include <proxygen/httpserver/RequestHandler.h>
#include <proxygen/httpserver/ResponseBuilder.h>

#include "../database/Database.hpp"

rest::ApiController::ApiController() {
    //
}

void rest::ApiController::onRequest(std::unique_ptr<proxygen::HTTPMessage> req) noexcept {
    proxygen::ResponseBuilder builder(downstream_);

    database::responseTable* data = database::Database::launchQuery("select * from tabella;");
    Json::Value jsonArray* = utility::responseTableToJsonArray(data);
    Json::Value jsonData* = new Json::Value();
    jsonData->operator[]("") = *jsonArray;
    
    std::string strData = jsonData->asString();
    delete jsonData; delete jsonArray; delete data;

    builder
        .status(200, "OK")
        .header("Content-Type", "application/json")
        .body(strData)
        .send();
}

