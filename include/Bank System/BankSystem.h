#ifndef BANK_SYSTEM_H
#define BANK_SYSTEM_H

#include <map>
#include <vector>
#include <string>
#include "../System details/Account.h"
#include "../System details/CheckingAccount.h"
#include "../System details/SavingsAccount.h"
#include "../System details/Transaction.h"
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

    // Data I/O methods
    void loadData();      // Mutator: Loads accounts and transactions from files
    void saveData();      // Accessor: Saves accounts and transactions to files

    // Account management methods (Mutators)
    bool createAccount(const std::string& name, const std::string& accountType, 
                      double initialBalance, const std::string& password);   // Creates a new account
    bool closeAccount(const std::string& accountId, const std::string& password);  // Removes an account
    
    // Account retrieval methods (Accessors)
    Account* findAccount(const std::string& accountId);  // Getter: Returns pointer to account or nullptr
    bool validateAccount(const std::string& accountId, const std::string& password);  // Validates credentials
    
    // Security methods
    void cleanupDeletedAccounts(); // Mutator: Removes accounts marked for deletion
    
    // Transaction methods (Mutators)
    bool deposit(const std::string& accountId, double amount);  // Adds funds to account
    bool withdraw(const std::string& accountId, double amount); // Removes funds from account
    void applyInterestToAllSavingsAccounts();  // Applies interest to all savings accounts
    
    // Display and utility methods
    void displayAllAccounts();  // Accessor: Displays all accounts information
    void runPasswordCracker();  // Utility: Tests password strength
    std::string getCrackTime(const std::string& password);  // Getter: Returns estimated time to crack a password
};

#endif // BANK_SYSTEM_H