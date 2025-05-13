#include "../../include/Bank System/BankSystem.h"
#include "../../include/utils/Algorithms.h"
#include <iostream>
#include <iomanip>
#include <ctime>
#include <random>
#include <algorithm>

BankSystem::BankSystem(const std::string& accFile, const std::string& transFile)
    : fileHandler(accFile, transFile) {
    loadData();
}

BankSystem::~BankSystem() {
    saveData();
    // Clean up dynamically allocated Account objects
    for (auto& pair : accounts) {
        delete pair.second;
    }
}

void BankSystem::loadData() {
    fileHandler.loadAccounts(accounts);
    fileHandler.loadTransactions(allTransactions);
}

void BankSystem::saveData() {
    fileHandler.saveAccounts(accounts);
    fileHandler.saveTransactions(allTransactions);
}

bool BankSystem::createAccount(const std::string& name, const std::string& accountType, 
                              double initialBalance, const std::string& password) {
    if (initialBalance < 0) return false;
    
    // Check password strength
    int strength = checkPasswordStrength(password);
    if (strength < 3) {
        std::cout << "Warning: Password is weak (strength: " << strength << "/10)" << std::endl;
        std::cout << "Consider using a stronger password with uppercase, lowercase, " 
                  << "numbers, and special characters." << std::endl;
    }
    
    // Create account with map of void pointers for the generateUniqueAccountId function
    std::map<std::string, void*> accountsPtr;
    for (const auto& pair : accounts) {
        accountsPtr[pair.first] = nullptr;
    }
    
    std::string id = generateUniqueAccountId(accountsPtr);
    
    if (accountType == "Checking") {
        accounts[id] = new CheckingAccount(id, name, initialBalance, password);
    } else if (accountType == "Savings") {
        accounts[id] = new SavingsAccount(id, name, initialBalance, password);
    } else {
        return false;
    }
    
    std::cout << "Account created successfully. Your account number is: " << id << std::endl;
    return true;
}

bool BankSystem::closeAccount(const std::string& accountId, const std::string& password) {
    if (accountId.empty() || password.empty()) {
        return false;
    }
    
    auto it = accounts.find(accountId);
    if (it == accounts.end()) {
        std::cout << "Account not found." << std::endl;
        return false;
    }
    
    try {
        // Check if account is marked for deletion due to security
        if (it->second->shouldBeDeleted()) {
            std::cout << "This account has been marked for deletion due to security concerns." << std::endl;
            cleanupDeletedAccounts(); // Remove it now
            return false;
        }
        
        // Use the validatePassword method to check the password and track attempts
        if (!it->second->validatePassword(password)) {
            // Clean up any accounts that should be deleted after failed attempts
            cleanupDeletedAccounts();
            return false;
        }
        
        // Account validated successfully, now close it
        Account* accountToDelete = it->second;
        accounts.erase(it); // Remove from map first to prevent dangling references
        delete accountToDelete; // Then delete the object
        
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error during account closure: " << e.what() << std::endl;
        return false;
    }
}

Account* BankSystem::findAccount(const std::string& accountId) {
    auto it = accounts.find(accountId);
    return (it != accounts.end()) ? it->second : nullptr;
}

/**
 * Validates an account login using account ID and password
 * This is the main entry point for the security system, which:
 * 1. Checks if the account exists
 * 2. Verifies if it's not already marked for deletion
 * 3. Validates the password, tracking failed attempts
 * 4. Cleans up any accounts marked for deletion
 *
 * @param accountId The account ID to validate
 * @param password The password to check
 * @return bool True if login is successful, false otherwise
 */
bool BankSystem::validateAccount(const std::string& accountId, const std::string& password) {
    // Basic input validation
    if (accountId.empty() || password.empty()) {
        return false;
    }
    
    // Find the account in the system
    Account* acc = findAccount(accountId);
    if (acc == nullptr) {
        std::cout << "Account not found or invalid credentials." << std::endl;
        return false;
    }
    
    // Security check: account might already be marked for deletion
    if (acc->shouldBeDeleted()) {
        std::cout << "This account has been marked for deletion due to security concerns." << std::endl;
        return false;
    }
    
    try {
        // Delegate to the account's password validation which handles failed attempts tracking
        bool result = acc->validatePassword(password);
        
        // After validation attempt (success or failure), clean up any accounts
        // that should be deleted due to excessive failed attempts
        cleanupDeletedAccounts();
        
        return result;
    } catch (const std::exception& e) {
        std::cerr << "Error during account validation: " << e.what() << std::endl;
        return false;
    }
}

/**
 * Removes accounts that have been marked for deletion due to security concerns
 * This is an important security measure to prevent brute force attacks
 * 
 * Implementation note: This uses a two-phase approach for safe cleanup:
 * 1. First identify all accounts to delete (to avoid modifying the map during iteration)
 * 2. Then perform the actual deletion of those accounts
 */
void BankSystem::cleanupDeletedAccounts() {
    try {
        // Create a list to store account IDs that need to be deleted
        std::vector<std::string> accountsToDelete;
        
        // Phase 1: Identify accounts that should be deleted
        // We can't delete during iteration as it would invalidate the iterator
        for (const auto& pair : accounts) {
            if (pair.second != nullptr && pair.second->shouldBeDeleted()) {
                accountsToDelete.push_back(pair.first);
            }
        }
        
        // Phase 2: Delete the identified accounts
        for (const auto& accountId : accountsToDelete) {
            std::cout << "SECURITY ALERT: Account " << accountId << " has been DELETED due to multiple failed login attempts." << std::endl;
            
            auto it = accounts.find(accountId);
            if (it != accounts.end() && it->second != nullptr) {
                // Store pointer to avoid memory leak
                Account* accountToDelete = it->second;
                
                // First remove from map to prevent dangling pointers
                accounts.erase(it);
                
                // Then delete the account object to free memory
                delete accountToDelete;
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error cleaning up deleted accounts: " << e.what() << std::endl;
    }
}

bool BankSystem::deposit(const std::string& accountId, double amount) {
    Account* acc = findAccount(accountId);
    if (acc == nullptr) return false;
    
    bool success = acc->deposit(amount);
    if (success) {
        // Add to global transactions
        std::time_t now = std::time(nullptr);
        std::string date = std::ctime(&now);
        date.pop_back(); // Remove trailing newline
        
        std::string transId = "TXN";
        std::random_device rd;
        std::mt19937 generator(rd());
        std::uniform_int_distribution<> distribution(0, 35);
        
        std::string chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
        for (int i = 0; i < 8; ++i) {
            transId += chars[distribution(generator)];
        }
        
        allTransactions.push_back(Transaction(transId, accountId, "deposit", amount, date));
    }
    
    return success;
}

bool BankSystem::withdraw(const std::string& accountId, double amount) {
    Account* acc = findAccount(accountId);
    if (acc == nullptr) return false;
    
    bool success = acc->withdraw(amount);
    if (success) {
        // Add to global transactions
        std::time_t now = std::time(nullptr);
        std::string date = std::ctime(&now);
        date.pop_back(); // Remove trailing newline
        
        std::string transId = "TXN";
        std::random_device rd;
        std::mt19937 generator(rd());
        std::uniform_int_distribution<> distribution(0, 35);
        
        std::string chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
        for (int i = 0; i < 8; ++i) {
            transId += chars[distribution(generator)];
        }
        
        allTransactions.push_back(Transaction(transId, accountId, "withdrawal", amount, date));
    }
    
    return success;
}

void BankSystem::applyInterestToAllSavingsAccounts() {
    for (auto& pair : accounts) {
        if (pair.second->getAccountType() == "Savings") {
            SavingsAccount* savingsAcc = static_cast<SavingsAccount*>(pair.second);
            savingsAcc->applyInterest();
        }
    }
}

void BankSystem::displayAllAccounts() {
    if (accounts.empty()) {
        std::cout << "No accounts found." << std::endl;
        return;
    }
    
    std::cout << "\n===== All Accounts =====\n";
    std::cout << std::left << std::setw(12) << "Account ID" 
              << std::setw(20) << "Customer Name" 
              << std::setw(10) << "Type" 
              << std::right << std::setw(12) << "Balance" << std::endl;
    std::cout << std::string(54, '-') << std::endl;
    
    for (const auto& pair : accounts) {
        Account* acc = pair.second;
        std::cout << std::left << std::setw(12) << acc->getAccountId() 
                  << std::setw(20) << acc->getCustomerName() 
                  << std::setw(10) << acc->getAccountType() 
                  << std::right << std::setw(12) << std::fixed << std::setprecision(2) 
                  << acc->getBalance() << std::endl;
    }
    std::cout << std::string(54, '-') << std::endl;
}

std::string BankSystem::getCrackTime(const std::string& password) {
    return simulatePasswordCrack(password);
}

void BankSystem::runPasswordCracker() {
    std::string testPassword;
    std::cout << "Enter a password to test its strength: ";
    std::getline(std::cin, testPassword);
    
    // Check for empty input
    if (testPassword.empty()) {
        std::cout << "Error: No password provided. Please try again." << std::endl;
        return;
    }
    
    // Check if input is only whitespace
    bool onlyWhitespace = true;
    for (char c : testPassword) {
        if (!std::isspace(c)) {
            onlyWhitespace = false;
            break;
        }
    }
    
    if (onlyWhitespace) {
        std::cout << "Error: Empty password. Please try again with a non-empty password." << std::endl;
        return;
    }
    
    int strength = checkPasswordStrength(testPassword);
    std::string crackTime = getCrackTime(testPassword);
    
    std::cout << "Password Strength: " << strength << "/10" << std::endl;
    std::cout << "Estimated time to crack: " << crackTime << std::endl;
    
    if (strength <= 2) {
        std::cout << "Very Weak Password. This could be cracked instantly." << std::endl;
    } else if (strength <= 4) {
        std::cout << "Weak Password. This could be cracked within minutes to hours." << std::endl;
    } else if (strength <= 6) {
        std::cout << "Moderate Password. This could take days to weeks to crack." << std::endl;
    } else if (strength <= 8) {
        std::cout << "Strong Password. This could take months to years to crack." << std::endl;
    } else {
        std::cout << "Very Strong Password. This would be extremely difficult to crack." << std::endl;
    }
    
    // Simulate a password cracking attempt with progress
    std::cout << "\nSimulating password crack attempt..." << std::endl;
    
    // Display a fake progress bar for visual effect
    std::cout << "[";
    
    // If password is strong, we only show partial progress
    int progressLength = 50;
    int progressToShow = strength <= 3 ? progressLength : (progressLength * 5) / strength;
    
    for (int i = 0; i < progressLength; i++) {
        if (i < progressToShow) {
            std::cout << "=";
        } else {
            std::cout << " ";
        }
    }
    
    if (strength <= 3) {
        std::cout << "] 100% Complete!" << std::endl;
        std::cout << "Password cracked in: " << crackTime << std::endl;
    } else {
        std::cout << "] " << (progressToShow * 100) / progressLength << "% Complete" << std::endl;
        std::cout << "Password crack attempt stopped after 1 hour of trying." << std::endl;
        std::cout << "Estimated total time required: " << crackTime << std::endl;
    }
    
    // Offer suggestions to improve
    if (strength < 8) {
        std::cout << "\nSuggestions to improve password strength:" << std::endl;
        if (testPassword.length() < 12)
            std::cout << "- Increase length to at least 12 characters" << std::endl;
        if (!std::any_of(testPassword.begin(), testPassword.end(), ::isupper))
            std::cout << "- Add uppercase letters" << std::endl;
        if (!std::any_of(testPassword.begin(), testPassword.end(), ::islower))
            std::cout << "- Add lowercase letters" << std::endl;
        if (!std::any_of(testPassword.begin(), testPassword.end(), ::isdigit))
            std::cout << "- Add numbers" << std::endl;
        if (std::all_of(testPassword.begin(), testPassword.end(), 
                        [](char c){ return ::isalnum(c); }))
            std::cout << "- Add special characters (!@#$%^&*)" << std::endl;
    }
}