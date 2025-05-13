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

    // Getters (Accessor methods)
    std::string getTransactionId() const;  // Getter: Returns unique transaction identifier
    std::string getAccountId() const;      // Getter: Returns the account ID associated with this transaction
    std::string getType() const;           // Getter: Returns transaction type (deposit, withdrawal)
    double getAmount() const;              // Getter: Returns the transaction amount
    std::string getDate() const;           // Getter: Returns the date when transaction occurred

    // For CSV saving
    std::string toCSV() const;
};

#endif // TRANSACTION_H