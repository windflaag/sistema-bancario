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
				 "insert into Accounts (accountId, name, surname) values (\"" +
				 accountId + "\", \"" +
				 name + "\", \"" +
				 surname + "\");"
				 );
}

void database::Database::deleteAccount(
				       std::string accountId) {
  database::Database::issueQuery(
				 "delete from Accounts where accountId = \"" +
				 accountId + "\";"
				 ); 
}

Json::Value* database::Database::getListOfAccounts() {
  Json::Value* data = database::Database::launchQuery(
						      "select accountId from Accounts"
						      );

  Json::Value* array = new Json::Value(Json::arrayValue);
  for (int i = 0; i < data->size(); i++) {
    array->operator[](i) = data->operator[](i)["accountId"];
  }

  delete data;
  return array;
}

void database::Database::insertTransaction(
					   std::string transactionId,
					   std::string fromId,
					   int amount,
					   std::string toId,
					   std::string timestamp) {
  database::Database::issueQuery(
				 "begin transaction; update Accounts set credit = credit - " +
				 std::to_string(amount) + " where accountId = \"" + fromId +
				 "\"; update Accounts set credit = credit + " + std::to_string(amount) +
				 " where accountId = \"" + toId +
				 "\"; insert into Transactions values (\"" + transactionId +
				 "\", \"" + fromId +
				 "\", \"" + std::to_string(amount) +
				 "\", \"" + toId +
				 "\", \"" + timestamp +
				 "\"); commit;");
}

Json::Value* database::Database::getTransactions(std::string accountId) {
  Json::Value* data = database::Database::launchQuery(
						      "select transactionId from Transactions where (fromId = \"" + accountId +
						      "\") or (toId = \"" + accountId +
						      "\" and fromId is NULL) order by timestamp;"
						      );

  Json::Value* array = new Json::Value(Json::arrayValue);
  for (int i = 0; i < data->size(); i++) {
    array->operator[](i) = data->operator[](i)["transactionId"];
  }

  delete data;
  return array;
}

Json::Value* database::Database::getAccountMetadata(std::string accountId) {
  Json::Value* data = database::Database::launchQuery("select * from Accounts where accountId = \"" + accountId + "\"");

  if (data->size() == 0) {
    delete data;
    throw std::runtime_error("");
  }
  
  Json::Value* metadata = new Json::Value(Json::objectValue);
  metadata->operator[]("name") = data->operator[](0)["name"];
  metadata->operator[]("surname") = data->operator[](0)["surname"];
  metadata->operator[]("credit") = data->operator[](0)["credit"];
  delete data;

  return metadata;
}

void database::Database::insertPayment(
					   std::string transactionId,
					   std::string accountId,
					   int amount,
					   std::string timestamp) { 
  database::Database::issueQuery(
				 "begin transaction; update Accounts set credit = credit + " + std::to_string(amount) +
				 " where accountId = \"" + accountId +
				 "\"; insert into Transactions values (\"" + transactionId +
				 "\", NULL, \"" + std::to_string(amount) +
				 "\", \"" + accountId +
				 "\", \"" + timestamp +
				 "\"); commit;");
}

void database::Database::insertWithdraw(
					   std::string transactionId,
					   std::string accountId,
					   int amount,
					   std::string timestamp) {
  database::Database::issueQuery(
				 "begin transaction; update Accounts set credit = credit - " +
				 std::to_string(amount) + " where accountId = \"" + accountId +
				 "\"; insert into Transactions values (\"" + transactionId +
				 "\", \"" + accountId +
				 "\", \"" + std::to_string(amount) +
				 "\", NULL, \"" + timestamp +
				 "\"); commit;");
}

void database::Database::updateName(std::string accountId, std::string name) {
  database::Database::issueQuery(
				 "update Accounts set name = \"" + name +
				 "\" where accountId = \"" + accountId + "\";"
				 );
}

void database::Database::updateSurname(std::string accountId, std::string surname) {
  database::Database::issueQuery(
				 "update Accounts set surname = \"" + surname +
				 "\" where accountId = \"" + accountId + "\";"
				 );
}

void database::Database::updateNameAndSurname(std::string accountId, std::string name, std::string surname) {
  database::Database::issueQuery(
				 "update Accounts set name = \"" + name +
				 "\", surname = \"" + surname +
				 "\" where accountId = \"" + accountId + "\";"
				 );
}
