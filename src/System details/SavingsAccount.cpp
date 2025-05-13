#include "../../include/System details/SavingsAccount.h"

SavingsAccount::SavingsAccount(const std::string& id, const std::string& name, double initialBalance, 
                              const std::string& pwd, double rate)
    : Account(id, name, "Savings", initialBalance, pwd), interestRate(rate) {}

void SavingsAccount::applyInterest() {
    double interest = getBalance() * interestRate;
    deposit(interest);
}

std::string SavingsAccount::toCSV() const {
    return Account::toCSV() + "," + std::to_string(interestRate);
}