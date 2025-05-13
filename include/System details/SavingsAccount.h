#ifndef SAVINGS_ACCOUNT_H
#define SAVINGS_ACCOUNT_H

#include "Account.h"

class SavingsAccount : public Account {
private:
    double interestRate;

public:
    SavingsAccount(const std::string& id, const std::string& name, double initialBalance, 
                   const std::string& pwd, double rate = 0.02);

    void applyInterest();                  // Mutator: Applies interest rate to account balance
    std::string toCSV() const override;    // Accessor: Returns account data in CSV format
};

#endif // SAVINGS_ACCOUNT_H