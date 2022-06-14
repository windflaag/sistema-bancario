#include "Database.hpp"
#include <sqlite3.h>
#include <cstdio>
#include "../singleton/Singleton.hpp"
#include "../utility/Utility.hpp"

inline std::string getDatabaseFilePath() {
  Json::Value databaseConfig = singleton::instance()->getConfig()->get("Database");
  return staticEngineConfig.get("filepath", DEFAULT_FILE_PATH).asString();
}

int database::Database::processResponse(void *result, int argc, char **argv, char **azColName) {
  // expect database::responseTable from result
  database::responseTable* response = result;
  database::responseLine* line = new database::responseLine;

  for(int i = 0; i < argc; i++) {
    line->push_back(std::string(argv[i]));
  }

  response->push_back(line);
  return 0;
}

void database::Database::launchQuery(std::string query) {
  database::responseTable* result = new database::responseTable;
  std::string filepath = getDatabaseFilePath();

  // open sqlite3 db
  sqlite3* db;
  if (sqlite3_open(filepath.c_str(), &db)) {
    std::fprintf(stderr, "cannot open database '%s': %s\n", filepath.c_str(), sqlite3_errmsg(db));
    sqlite3_close(db);
  }

  // launch the query to sqlite3 library
  char* errorMessage = NULL;
  if (SQLITE_OK != sqlite3_exec(db, query.c_str(),
				database::Database::processResponse,
				(void*) result, &errorMessage)) {
    std::fprintf(stderr, "sql error occurred: %s\n", errorMessage);
    sqlite3_free(errorMessage);
  }

  sqlite3_close(db);
  return result;
}

void database::Database::createIfNotExists() {
  if (!(utility::fileExists(getDatabaseFilePath()))) {
    //
  };
}
