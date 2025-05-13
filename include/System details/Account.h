#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <string>
#include <queue>
#include <stack>
#include <iostream>
#include "../models/Transaction.h"

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

    // Getters
    std::string getAccountId() const;
    std::string getCustomerName() const;
    std::string getAccountType() const;
    double getBalance() const;
    std::string getPassword() const;
    bool isLocked() const;
    int getLockoutTime() const;
    
    // Security methods
    bool validatePassword(const std::string& pwd);
    void recordFailedAttempt();
    void resetFailedAttempts();
    void checkLockStatus();
    bool shouldBeDeleted() const; // Check if account should be deleted due to security

    // Account operations
    virtual bool deposit(double amount);
    virtual bool withdraw(double amount);

    // For CSV saving
    virtual std::string toCSV() const;

private:
    std::string generateTransactionId() const;
};

#endif // ACCOUNT_H