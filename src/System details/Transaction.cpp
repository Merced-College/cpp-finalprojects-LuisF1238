#include "../../include/models/Transaction.h"

Transaction::Transaction(const std::string& id, const std::string& accId, const std::string& transType, 
                        double amt, const std::string& transDate)
    : transactionId(id), accountId(accId), type(transType), amount(amt), date(transDate) {}

std::string Transaction::getTransactionId() const { 
    return transactionId; 
}

std::string Transaction::getAccountId() const { 
    return accountId; 
}

std::string Transaction::getType() const { 
    return type; 
}

double Transaction::getAmount() const { 
    return amount; 
}

std::string Transaction::getDate() const { 
    return date; 
}

std::string Transaction::toCSV() const {
    return transactionId + "," + accountId + "," + type + "," + 
            std::to_string(amount) + "," + date;
}