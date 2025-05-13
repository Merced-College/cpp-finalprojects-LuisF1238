#include "../../include/Viewer POV/Menu.h"
#include <iostream>

void displayMenu() {
    std::cout << "\n===== Banking System Menu =====\n";
    std::cout << "1. Create New Account\n";
    std::cout << "2. Login to Account\n";
    std::cout << "3. Display All Accounts\n";
    std::cout << "4. Test Password Strength\n";
    std::cout << "5. Exit\n";
    std::cout << "==============================\n";
    std::cout << "Enter your choice: ";
}

void displayAccountMenu(const std::string& accountId) {
    std::cout << "\n===== Account " << accountId << " Menu =====\n";
    std::cout << "1. Check Balance\n";
    std::cout << "2. Deposit Money\n";
    std::cout << "3. Withdraw Money\n";
    std::cout << "4. Close Account\n";
    std::cout << "5. Back to Main Menu\n";
    std::cout << "==============================\n";
    std::cout << "Enter your choice: ";
}