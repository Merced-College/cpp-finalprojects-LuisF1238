#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>

class Transaction {
private:
    std::string transactionId;
    std::string accountId;
    std::string type; // deposit, withdrawal
    double amount;
    std::string date;

public:
    Transaction(const std::string& id, const std::string& accId, const std::string& transType, 
                double amt, const std::string& transDate);

    // Getters
    std::string getTransactionId() const;
    std::string getAccountId() const;
    std::string getType() const;
    double getAmount() const;
    std::string getDate() const;

    // For CSV saving
    std::string toCSV() const;
};

#endif // TRANSACTION_H