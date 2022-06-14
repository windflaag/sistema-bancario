#include "Database.hpp"
#include <sqlite3.h>
#include <cstdio>
#include <exception>
#include "../singleton/Singleton.hpp"
#include "../utility/Utility.hpp"

inline std::string getDatabaseFilePath() {
  Json::Value databaseConfig = 
    singleton::instance()
        ->getConfig()
        ->get("Database");
  return databaseConfig.get("filePath", DEFAULT_FILE_PATH).asString();
}

inline std::string getInitFilePath() {
  Json::Value databaseConfig = 
    singleton::instance()
        ->getConfig()
        ->get("Database");
  return databaseConfig.get("initPath", DEFAULT_INIT_PATH).asString();
}

int database::Database::processResponse(void *result, int length, char **values, char **columns) {
  Json::Value* arrayOfResults = (Json::Value*) result;

  Json::Value object = Json::objectValue;
  for(int i = 0; i < length; i++) {
    object[std::string(columns[i])] = std::string(values[i]);
  }

  arrayOfResults->operator[](arrayOfResults->size()) = object;
  return 0;
}

Json::Value* database::Database::launchQuery(std::string query) {
  Json::Value* arrayOfResults = new Json::Value(Json::arrayValue);
  std::string filepath = getDatabaseFilePath();

  // open sqlite3 db
  sqlite3* db;
  if (sqlite3_open(filepath.c_str(), &db)) {
    std::fprintf(
            stderr, "cannot open database '%s': %s\n",
            filepath.c_str(), sqlite3_errmsg(db));
    sqlite3_close(db);
  }

  // launch the query to sqlite3 library
  char* errorMessage = NULL;
  if (SQLITE_OK != sqlite3_exec(
            db, query.c_str(),
			database::Database::processResponse,
		    (void*) arrayOfResults, &errorMessage)) {
    std::fprintf(stderr, "sql error occurred: %s\n", errorMessage);
    sqlite3_free(errorMessage);

    throw std::runtime_error("");
  }

  sqlite3_close(db);
  return arrayOfResults;
}

void database::Database::issueQuery(std::string query) {
    try {
        Json::Value *arr = database::Database::launchQuery(query);
        delete arr;
    } catch(std::runtime_error& err) {
        throw err;
    }
}

void database::Database::issueSQLFile(std::string filepath) {
    try {
        database::Database::issueQuery(
            utility::readFile(filepath)
        );
    } catch(std::runtime_error& err) {
        throw err;
    }
}

void database::Database::createIfNotExists() {
  if (!(utility::fileExists(getDatabaseFilePath()))) {
      database::Database::issueSQLFile(getInitFilePath());
  };
}

void database::Database::insertAccount(
        std::string accountId,
        std::string name,
        std::string surname) {
    database::Database::issueQuery(
            "insert into Account values (\"" +
            accountId + "\", \"" +
            name + "\", \"" +
            surname + "\");"
    );
}

void database::Database::deleteAccount(
        std::string accountId) {
   database::Database::issueQuery(
            "delete from Account where accountId = \"" +
            accountId + "\";"
    ); 
}

Json::Value* database::Database::getListOfAccounts() {
    Json::Value* data = database::Database::launchQuery(
            "select accountId from Account"
    );

    Json::Value* array = new Json::Value(Json::arrayValue);
    for (int i = 0; i < data->size(); i++) {
        array->operator[](i) = data->operator[](i)["accountId"];
    }

    delete data;
    return array;
}
