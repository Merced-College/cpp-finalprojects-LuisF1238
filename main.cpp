/***************************************************************************************
*    Title: <Cpp Final - Banking System>
*    Author: <Luis Flores>
*    Date: <05/12/25>
*    Code version: <1.5>
*
***************************************************************************************/


#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <limits> // For std::numeric_limits

// Include header files from the project
#include "include/Bank System/BankSystem.h"
#include "include/Viewer POV/Menu.h"

/**
 * Main function for the Banking System application
 * Handles main menu loop and user interactions with the banking system
 */
 //MENU was enhanced using AI

int main() {
    // Initialize the banking system with data file paths
    BankSystem bankSystem("data/accounts.csv", "data/transactions.csv");
    int choice;
    
    std::cout << "Welcome to the Banking System\n";
    
    // Main application loop
    bool running = true;
    while (running) {
        displayMenu();
        
        // Input validation for main menu choices
        if (!(std::cin >> choice)) {
            // Handle non-numeric input
            std::cin.clear(); // Clear the error flag
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard invalid input
            std::cout << "Invalid input. Please enter a number between 1 and 5." << std::endl;
            continue; // Skip to next iteration of the loop
        }
        
        // Validate input range (1-5)
        if (choice < 1 || choice > 5) {
            std::cout << "Invalid choice. Please enter a number between 1 and 5." << std::endl;
            continue; // Skip to next iteration of the loop
        }
        
        switch (choice) {
            case 1: {
                // Create new account
                std::string name, accountType, password;
                double initialBalance;
                
                std::cin.ignore();
                std::cout << "Enter your name: ";
                std::getline(std::cin, name);
                
                std::cout << "Select account type (Checking/Savings): ";
                std::cin >> accountType;
                
                std::cout << "Enter initial deposit amount: $";
                std::cin >> initialBalance;
                
                std::cout << "Create a password: ";
                std::cin >> password;
                
                if (bankSystem.createAccount(name, accountType, initialBalance, password)) {
                    std::cout << "Account created successfully!" << std::endl;
                } else {
                    std::cout << "Failed to create account. Please try again." << std::endl;
                }
                break;
            }
            case 2: {
                // Login to account
                std::string accountId, password;
                
                std::cout << "Enter account ID: ";
                std::cin >> accountId;
                
                std::cout << "Enter password: ";
                std::cin >> password;
                
                if (bankSystem.validateAccount(accountId, password)) {
                    std::cout << "Login successful!" << std::endl;
                    
                    // Account submenu
                    bool accountMenuRunning = true;
                    while (accountMenuRunning) {
                        displayAccountMenu(accountId);
                        int accountChoice;
                        
                        // Input validation for account submenu choices
                        if (!(std::cin >> accountChoice)) {
                            // Handle non-numeric input
                            std::cin.clear(); // Clear the error flag
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard invalid input
                            std::cout << "Invalid input. Please enter a number between 1 and 5." << std::endl;
                            continue; // Skip to next iteration of the loop
                        }
                        
                        // Validate input range (1-5)
                        if (accountChoice < 1 || accountChoice > 5) {
                            std::cout << "Invalid choice. Please enter a number between 1 and 5." << std::endl;
                            continue; // Skip to next iteration of the loop
                        }
                        
                        switch (accountChoice) {
                            case 1: {
                                // Check balance
                                Account* acc = bankSystem.findAccount(accountId);
                                if (acc) {
                                    std::cout << "Account Balance: $" << std::fixed 
                                              << std::setprecision(2) << acc->getBalance() 
                                              << std::endl;
                                }
                                break;
                            }
                            case 2: {
                                // Deposit
                                double amount;
                                std::cout << "Enter amount to deposit: $";
                                
                                // Input validation for deposit amount
                                if (!(std::cin >> amount)) {
                                    std::cin.clear(); // Clear the error flag
                                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard invalid input
                                    std::cout << "Invalid input. Please enter a numeric amount." << std::endl;
                                    break;
                                }
                                
                                if (amount <= 0) {
                                    std::cout << "Amount must be greater than zero." << std::endl;
                                    break;
                                }
                                
                                if (bankSystem.deposit(accountId, amount)) {
                                    std::cout << "Deposit successful!" << std::endl;
                                } else {
                                    std::cout << "Deposit failed. Please try again." << std::endl;
                                }
                                break;
                            }
                            case 3: {
                                // Withdraw
                                double amount;
                                std::cout << "Enter amount to withdraw: $";
                                
                                // Input validation for withdrawal amount
                                if (!(std::cin >> amount)) {
                                    std::cin.clear(); // Clear the error flag
                                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard invalid input
                                    std::cout << "Invalid input. Please enter a numeric amount." << std::endl;
                                    break;
                                }
                                
                                if (amount <= 0) {
                                    std::cout << "Amount must be greater than zero." << std::endl;
                                    break;
                                }
                                
                                if (bankSystem.withdraw(accountId, amount)) {
                                    std::cout << "Withdrawal successful!" << std::endl;
                                } else {
                                    std::cout << "Withdrawal failed. Insufficient funds or invalid amount." 
                                              << std::endl;
                                }
                                break;
                            }
                            case 4: {
                                // Close account
                                std::string confirmPassword;
                                std::cout << "Enter password to confirm account closure: ";
                                std::cin >> confirmPassword;
                                
                                if (bankSystem.closeAccount(accountId, confirmPassword)) {
                                    std::cout << "Account closed successfully!" << std::endl;
                                    accountMenuRunning = false;
                                } else {
                                    std::cout << "Failed to close account. Incorrect password." 
                                              << std::endl;
                                }
                                break;
                            }
                            case 5:
                                // Back to main menu
                                accountMenuRunning = false;
                                break;
                            default:
                                std::cout << "Invalid choice. Please try again." << std::endl;
                        }
                    }
                } else {
                    std::cout << "Invalid account ID or password." << std::endl;
                }
                break;
            }
            case 3:
                // Display all accounts
                bankSystem.displayAllAccounts();
                break;
            case 4:
                // Test password strength
                bankSystem.runPasswordCracker();
                break;
            case 5:
                // Exit
                running = false;
                std::cout << "Thank you for using the Banking System. Goodbye!" << std::endl;
                break;
            default:
                std::cout << "Invalid choice. Please try again." << std::endl;
        }
    }
    
    return 0;
}