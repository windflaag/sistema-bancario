#ifndef __DATABASE__
#define __DATABASE__

#include <string>
#include <vector>
#include <json/json.h>

#define DEFAULT_FILE_PATH "database.db"
#define DEFAULT_INIT_PATH "database.sql"

namespace database {
    // this first pointer is void
    // because sqlite3 needs a callback of this type!
    int processResponse(
            void *result, int argc,
            char **argv, char **azColName);

    // returns array of lines,
    Json::Value* launchQuery(std::string query);

    // returns nothing
    void issueQuery(std::string query);

    // same but read query out of a file
    void issueSQLFile(std::string filepath);

    // this function creates the database
    // and initialize the tables
    void createIfNotExists();

    void insertAccount(
            std::string accountId,
            std::string name,
            std::string surname);
    
    void deleteAccount(
            std::string accountId);

    void insertTransaction(
            std::string transactionId,
            std::string fromID,
            int amount,
            std::string toID,
            std::string timestamp);

    // money goes up
    void insertPayment(
            std::string transactionId,
            std::string accountID,
            int amount,
            std::string timestamp);

    // money goes down
    void insertWithdraw(
            std::string transactionId,
            std::string accountID,
            int amount,
            std::string timestamp);

    Json::Value* getListOfAccounts();
    Json::Value* getTransactions(std::string accountId);
    Json::Value* getAccountMetadata(std::string accountId);
    Json::Value* getTransactionMetadata(std::string transactionId);
    Json::Value* getCredit(std::string accountId);

    void updateName(std::string accountId, std::string name);
    void updateSurname(std::string accountId, std::string surname);
    void updateNameAndSurname(std::string accountId, std::string name, std::string surname);
}

#endif
