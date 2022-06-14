#include "ApiAccountController.hpp"

#include <proxygen/httpserver/RequestHandler.h>
#include <proxygen/httpserver/ResponseBuilder.h>

#include "../database/Database.hpp"
#include "../utility/Utility.hpp"
#include "../codec/Codec.hpp"
#include "Validation.hpp"

rest::ApiAccountController::ApiAccountController() {
    //
}

void rest::ApiAccountController::onRequest
    (std::unique_ptr<proxygen::HTTPMessage> req) noexcept {
    proxygen::ResponseBuilder builder(downstream_);
    if (req->getMethod() == proxygen::HTTPMethod::GET) {
        try {
            Json::Value* data = database::Database::getListOfAccounts();

            Json::Value object = Json::objectValue;
            object["accounts"] = *data;
            
            std::string result = utility::jsonToString(object);
            delete data;

            builder
                .status(200, "OK")
                .body(result)
                .send();
            return;
        } catch(...) {
            builder
                .status(500, "Internal Server Error")
                .send();
        }
    } else if (req->getMethod() == proxygen::HTTPMethod::POST) { 
        // if is POST then we will check body but for now we set
        builder.status(400, "Bad Request");
    } else if (req->getMethod() == proxygen::HTTPMethod::DELETE) {
        if (
            (! req->hasQueryParam("id")) ||
            (! Validation::validateId(req->getQueryParam("id")))
        ) {
            builder
                .status(400, "Bad Request")
                .send();
            return;
        }

        try {
            std::string accountId = req->getQueryParam("id");

            database::Database::deleteAccount(accountId);
           
            builder
                .status(200, "OK")
                .send();
        } catch(...) {
            builder
                .status(500, "Internal Server Error")
                .send();
        }
    } else {
        builder
            .status(501, "Not implemented")
            .send();
        return;
    }
}

void rest::ApiAccountController::onEOM() noexcept {
    proxygen::ResponseBuilder builder(downstream_);
    if (
        !(this->body_) ||
        (this->body_->size() == 0)) {
        builder.sendWithEOM();
        return;
    }

    Json::Value parameters;
    Json::Reader* text_reader = new Json::Reader();
    if (!(text_reader->parse(*(this->body_), parameters))) {
        builder
            .status(400, "Bad Request")
            .sendWithEOM();
            return;
    }

    delete text_reader;

    if (
        (!(parameters.isObject())) ||
        (! parameters.isMember("name")) ||
        (! parameters.isMember("surname")) ||
        (! parameters["name"].isString()) ||
        (! parameters["surname"].isString()) ||
        (! Validation::validateName(parameters["name"].asString())) ||
        (! Validation::validateName(parameters["surname"].asString()))
    ) {
        builder
            .status(400, "Bad Request")
            .sendWithEOM();
        return;
    }

    try {
        std::string name = parameters["name"].asString();
        std::string surname = parameters["surname"].asString();
        std::string accountId = codec::computeAccountId(
            name, surname
        );

        database::Database::insertAccount(accountId, name, surname);

        builder
            .status(201, "Created")
            .body(accountId)
            .sendWithEOM();
        return;
    } catch(...) {
        builder
            .status(409, "Conflict")
            .sendWithEOM();
        return;
    }
}
