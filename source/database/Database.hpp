#ifndef __DATABASE__
#define __DATABASE__

#include <string>
#include <vector>
#include <json/json.h>

#define DEFAULT_FILE_PATH "database.db"
#define DEFAULT_INIT_PATH "database.sql"

namespace database {
  class Database {
  private:
    // this first pointer is void
    // because sqlite3 needs a callback of this type!
    static int processResponse(
            void *result, int argc,
            char **argv, char **azColName);

  public:
    // returns array of lines,
    static Json::Value* launchQuery(std::string query);

    // returns nothing
    static void issueQuery(std::string query);

    // same but read query out of a file
    static void issueSQLFile(std::string filepath);

    // this function creates the database
    // and initialize the tables
    static void createIfNotExists();

    static void insertAccount(
            std::string accountId,
            std::string name,
            std::string surname);
    static void deleteAccount(
            std::string accountId);

    static Json::Value* getListOfAccounts();
  };
}

#endif
