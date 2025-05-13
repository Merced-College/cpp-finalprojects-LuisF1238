#ifndef BANK_SYSTEM_H
#define BANK_SYSTEM_H

#include <map>
#include <vector>
#include <string>
#include "../models/Account.h"
#include "../models/CheckingAccount.h"
#include "../models/SavingsAccount.h"
#include "../models/Transaction.h"
#include "../utils/FileHandler.h"

class BankSystem {
private:
    std::map<std::string, Account*> accounts;
    std::vector<Transaction> allTransactions;
    FileHandler fileHandler;

public:
    BankSystem(const std::string& accFile = "data/accounts.csv", 
               const std::string& transFile = "data/transactions.csv");
    ~BankSystem();

    void loadData();
    void saveData();

    bool createAccount(const std::string& name, const std::string& accountType, 
                      double initialBalance, const std::string& password);
    bool closeAccount(const std::string& accountId, const std::string& password);
    Account* findAccount(const std::string& accountId);
    bool validateAccount(const std::string& accountId, const std::string& password);
    void cleanupDeletedAccounts(); // Remove accounts marked for deletion
    bool deposit(const std::string& accountId, double amount);
    bool withdraw(const std::string& accountId, double amount);
    void applyInterestToAllSavingsAccounts();
    void displayAllAccounts();
    void runPasswordCracker();
    std::string getCrackTime(const std::string& password);
};

#endif // BANK_SYSTEM_H