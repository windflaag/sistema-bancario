#ifndef __DATABASE__
#define __DATABASE__

#include <string>
#include <vector>
#define DEFAULT_FILE_PATH "database.db"

namespace database {
  typdef std::vector<std::string> responseLine;
  typdef std::vector<responseLine*> responseTable;

  class Database {
  private:
    // this first pointer is void because sqlite3 needs a callback of this type!
    static int processResponse(void *result, int argc, char **argv, char **azColName);

  public:
    // this function returns the result of a query,
    // empty if no output is provided or if error occurrs
    static responseTable launchQuery(std::string query);

    static void createIfNotExists();
  };
}

#endif
