#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <string>
#include <queue>
#include <stack>
#include <iostream>
#include "Transaction.h"

class Account {
protected:
    std::string accountId;
    std::string customerName;
    std::string accountType;
    double balance;
    std::string password;
    std::queue<Transaction> recentTransactions;
    std::stack<int> failedAttempts; // Stack to track failed login attempts
    bool locked;                    // Flag to indicate if account is locked
    int lockoutTime;                // Current lockout time in seconds

public:
    Account(const std::string& id, const std::string& name, const std::string& type, 
            double initialBalance, const std::string& pwd);
    
    virtual ~Account() = default;

    // Getters (Accessor methods)
    std::string getAccountId() const;       // Getter: Returns the account's unique identifier
    std::string getCustomerName() const;    // Getter: Returns the account holder's name
    std::string getAccountType() const;     // Getter: Returns the type of account (Checking, Savings, etc.)
    double getBalance() const;              // Getter: Returns the current account balance
    std::string getPassword() const;        // Getter: Returns the account password (for internal use only)
    bool isLocked() const;                  // Getter: Returns whether the account is locked due to failed attempts
    int getLockoutTime() const;             // Getter: Returns the remaining lockout time in seconds
    
    // Security methods
    bool validatePassword(const std::string& pwd);
    void recordFailedAttempt();
    void resetFailedAttempts();
    void checkLockStatus();
    bool shouldBeDeleted() const; // Check if account should be deleted due to security

    // Account operations (Mutator methods)
    virtual bool deposit(double amount);     // Setter: Adds funds to account and updates balance
    virtual bool withdraw(double amount);    // Setter: Removes funds from account and updates balance

    // For CSV saving
    virtual std::string toCSV() const;

private:
    std::string generateTransactionId() const;
};

#endif // ACCOUNT_H