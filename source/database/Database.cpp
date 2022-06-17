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

int database::processResponse(void *result, int length, char **values, char **columns) {
  Json::Value* arrayOfResults = (Json::Value*) result;

  Json::Value object = Json::objectValue;
  for(int i = 0; i < length; i++) {
    if (values[i])
      object[std::string(columns[i])] = std::string(values[i]);
    else
      object[std::string(columns[i])] = Json::nullValue;
  }

  arrayOfResults->operator[](arrayOfResults->size()) = object;
  return 0;
}

Json::Value* database::launchQuery(std::string query) {
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
				database::processResponse,
				(void*) arrayOfResults, &errorMessage)) {
    std::fprintf(stderr, "sql error occurred: %s\n", errorMessage);
    sqlite3_free(errorMessage);

    throw std::runtime_error("");
  }

  sqlite3_close(db);
  return arrayOfResults;
}

void database::issueQuery(std::string query) {
  try {
    Json::Value *arr = database::launchQuery(query);
    delete arr;
  } catch(std::runtime_error& err) {
    throw err;
  }
}

void database::issueSQLFile(std::string filepath) {
  try {
    database::issueQuery(
				   utility::readFile(filepath)
				   );
  } catch(std::runtime_error& err) {
    throw err;
  }
}

void database::createIfNotExists() {
  if (!(utility::fileExists(getDatabaseFilePath()))) {
    database::issueSQLFile(getInitFilePath());
  };
}

void database::insertAccount(
				       std::string accountId,
				       std::string name,
				       std::string surname) {
  database::issueQuery(
				 "insert into Accounts (accountId, name, surname) values (\"" +
				 accountId + "\", \"" +
				 name + "\", \"" +
				 surname + "\");"
				 );
}

void database::deleteAccount(
				       std::string accountId) {
  database::issueQuery(
				 "delete from Accounts where accountId = \"" +
				 accountId + "\";"
				 ); 
}

Json::Value* database::getListOfAccounts() {
  Json::Value* data = database::launchQuery(
						      "select accountId from Accounts"
						      );

  Json::Value* array = new Json::Value(Json::arrayValue);
  int length = data->size();
  for (int i = 0; i < length; i++) {
    array->operator[](i) = data->operator[](i)["accountId"];
  }

  delete data;
  return array;
}

void database::insertTransaction(
					   std::string transactionId,
					   std::string fromId,
					   int amount,
					   std::string toId,
					   std::string timestamp) {
  database::issueQuery(
    "begin transaction;"
    "insert or rollback into Transactions values (\"" + transactionId + "\", \"" + fromId + "\", \"" + std::to_string(amount) + "\", \"" + toId + "\", \"" + timestamp + "\");"
    "update or rollback Accounts set credit = credit + " + std::to_string(amount) + " where accountId = \"" + toId + "\";"
    "update or rollback Accounts set credit = credit - " + std::to_string(amount) + " where accountId = \"" + fromId + "\";"
    "commit;"
  );
}

Json::Value* database::getTransactions(std::string accountId) {
  Json::Value* data = database::launchQuery(
						      "select transactionId from Transactions where (fromId = \"" + accountId +
						      "\") or (toId = \"" + accountId +
						      "\" and fromId is NULL) order by timestamp;"
						      );

  Json::Value* array = new Json::Value(Json::arrayValue);
  int length = data->size();
  for (int i = 0; i < length; i++) {
    array->operator[](i) = data->operator[](i)["transactionId"];
  }

  delete data;
  return array;
}

Json::Value* database::getAccountMetadata(std::string accountId) {
  Json::Value* data = database::launchQuery("select * from Accounts where accountId = \"" + accountId + "\"");

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

Json::Value* database::getTransactionMetadata(std::string transactionId) {
  Json::Value* data = database::launchQuery(
      "select TAT.fromId as fromId, TAT.amount as amount, TAT.toId as toId, FR.credit as fromCredit, TAT.credit as toCredit, TAT.timestamp as timestamp "
      "from Accounts as FR join (Transactions as T join Accounts as AT on AT.accountId = T.toId) as TAT on FR.accountId = TAT.fromId "
      "where TAT.transactionId = \"" + transactionId + "\";");

  if (data->size() == 0) {
    delete data;
    throw std::runtime_error("");
  }
  
  Json::Value* metadata = new Json::Value(Json::objectValue);
  
  metadata->operator[]("fromId") = data->operator[](0)["fromId"];
  metadata->operator[]("fromCredit") = std::stoi(data->operator[](0)["fromCredit"].asString());
  metadata->operator[]("toId") = data->operator[](0)["toId"];
  metadata->operator[]("toCredit") = std::stoi(data->operator[](0)["toCredit"].asString());
  metadata->operator[]("amount") = std::stoi(data->operator[](0)["amount"].asString());
  metadata->operator[]("timestamp") = data->operator[](0)["timestamp"].asString();
  delete data;

  return metadata;
}

Json::Value* database::getTransactionData(std::string transactionId) {
  Json::Value* data = database::launchQuery(
      "select * from Transactions where transactionId = \"" + transactionId + "\";");

  if (data->size() == 0) {
    delete data;
    throw std::runtime_error("");
  }
  
  Json::Value* metadata = new Json::Value(Json::objectValue);
  
  metadata->operator[]("transactionId") = transactionId;
  metadata->operator[]("recipient") = data->operator[](0)["toId"].asString();
  metadata->operator[]("amount") = std::stoi(data->operator[](0)["amount"].asString());
  metadata->operator[]("timestamp") = data->operator[](0)["timestamp"].asString();
  delete data;

  return metadata;
}


void database::insertPayment(
					   std::string transactionId,
					   std::string accountId,
					   int amount,
					   std::string timestamp) { 
  database::issueQuery(
    "begin transaction;"
    "insert or rollback into Transactions values (\"" + transactionId + "\", NULL, \"" + std::to_string(amount) + "\", \"" + accountId + "\", \"" + timestamp + "\");"
    "update or rollback Accounts set credit = credit + " + std::to_string(amount) + " where accountId = \"" + accountId + "\";"
    "commit;"
          );
}

void database::insertWithdraw(
					   std::string transactionId,
					   std::string accountId,
					   int amount,
					   std::string timestamp) {
  database::issueQuery(
    "begin transaction;"
    "insert or rollback into Transactions values (\"" + transactionId + "\", \"" + accountId + "\", \"" + std::to_string(amount) + "\", NULL, \"" + timestamp + "\");"
    "update or rollback Accounts set credit = credit - " + std::to_string(amount) + " where accountId = \"" + accountId + "\";"
    "commit;"
          );
}

void database::updateName(std::string accountId, std::string name) {
  database::issueQuery(
     "update Accounts set name = \"" + name +
     "\" where accountId = \"" + accountId + "\";"
  );
}

void database::updateSurname(std::string accountId, std::string surname) {
  database::issueQuery(
     "update Accounts set surname = \"" + surname +
     "\" where accountId = \"" + accountId + "\";"
  );
}

void database::updateNameAndSurname(std::string accountId, std::string name, std::string surname) {
  database::issueQuery(
				 "update Accounts set name = \"" + name +
				 "\", surname = \"" + surname +
				 "\" where accountId = \"" + accountId + "\";"
				 );
}

Json::Value* database::getCredit(std::string accountId) {
    Json::Value* arr = database::launchQuery(
            "select credit from Accounts where accountId = \"" +
            accountId + "\";"
            );

    if (arr->size() == 0)
        throw std::runtime_error("");

    Json::Value* result = new Json::Value(std::stoi(arr->operator[](0)["credit"].asString()));
    
    delete arr;
    return result;
}
