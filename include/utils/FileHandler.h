#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include <string>
#include <vector>
#include <map>
#include "../models/Account.h"
#include "../models/Transaction.h"

class FileHandler {
private:
    std::string accountsFilePath;
    std::string transactionsFilePath;

public:
    FileHandler(const std::string& accFile = "data/accounts.csv", 
              const std::string& transFile = "data/transactions.csv");

    // Load data from CSV files
    void loadAccounts(std::map<std::string, Account*>& accounts);
    void loadTransactions(std::vector<Transaction>& transactions);

    // Save data to CSV files
    void saveAccounts(const std::map<std::string, Account*>& accounts);
    void saveTransactions(const std::vector<Transaction>& transactions);
};

#endif // FILE_HANDLER_H