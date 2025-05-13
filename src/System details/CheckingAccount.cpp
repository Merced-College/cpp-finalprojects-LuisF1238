#include "../../include/models/CheckingAccount.h"

CheckingAccount::CheckingAccount(const std::string& id, const std::string& name, double initialBalance, 
                                const std::string& pwd, double fee)
    : Account(id, name, "Checking", initialBalance, pwd), transactionFee(fee) {}

bool CheckingAccount::withdraw(double amount) {
    // Add transaction fee
    return Account::withdraw(amount + transactionFee);
}

std::string CheckingAccount::toCSV() const {
    return Account::toCSV() + "," + std::to_string(transactionFee);
}