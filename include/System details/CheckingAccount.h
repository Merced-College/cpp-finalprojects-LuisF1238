#ifndef CHECKING_ACCOUNT_H
#define CHECKING_ACCOUNT_H

#include "Account.h"

class CheckingAccount : public Account {
private:
    double transactionFee;

public:
    CheckingAccount(const std::string& id, const std::string& name, double initialBalance, 
                    const std::string& pwd, double fee = 1.0);

    bool withdraw(double amount) override;
    std::string toCSV() const override;
};

#endif // CHECKING_ACCOUNT_H