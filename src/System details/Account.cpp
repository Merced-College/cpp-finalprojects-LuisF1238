#include "../../include/models/Account.h"
#include <ctime>
#include <random>

Account::Account(const std::string& id, const std::string& name, const std::string& type, 
                double initialBalance, const std::string& pwd)
    : accountId(id), customerName(name), accountType(type), 
      balance(initialBalance), password(pwd), locked(false), lockoutTime(0) {
    // The stack and queue are automatically initialized
}

std::string Account::getAccountId() const { 
    return accountId; 
}

std::string Account::getCustomerName() const { 
    return customerName; 
}

std::string Account::getAccountType() const { 
    return accountType; 
}

double Account::getBalance() const { 
    return balance; 
}

std::string Account::getPassword() const { 
    return password; 
}

bool Account::isLocked() const {
    return locked;
}

int Account::getLockoutTime() const {
    return lockoutTime;
}

bool Account::shouldBeDeleted() const {
    // Account should be deleted if it has 5 or more failed login attempts
    return (failedAttempts.size() >= 5);
}

/**
 * Validates the provided password against the account's password
 * Implements security measures including:
 * - Account locking after 3 failed attempts
 * - Account deletion after 5 failed attempts
 * - Failed attempt tracking even when the account is already locked
 * 
 * @param pwd The password to validate
 * @return bool True if password is correct, false otherwise
 */
bool Account::validatePassword(const std::string& pwd) {
    try {
        // First check and display the lock status to the user
        checkLockStatus();
        
        if (locked) {
            // SECURITY FIX: Even though the account is locked, we still count this as a failed attempt
            // This ensures the remaining attempts counter decreases with each invalid try
            recordFailedAttempt();
            return false;
        }
        
        // If password is correct, reset all failed attempts and unlock the account
        if (pwd == password) {
            resetFailedAttempts();
            return true;
        } else {
            // Record failed attempt which may lock the account depending on the number of previous failures
            recordFailedAttempt();
            return false;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error validating password: " << e.what() << std::endl;
        return false;
    }
}

/**
 * Records a failed login attempt and updates the account's security status
 * Implements a progressive security protocol:
 * - First 2 attempts: Warning only
 * - 3 attempts: Account locked for 1 minute
 * - 4 attempts: Final warning, account still locked
 * - 5+ attempts: Account marked for deletion
 * 
 * Using a stack to track attempts allows for potential future features like
 * time-based attempt expiration or more sophisticated security analytics.
 */
void Account::recordFailedAttempt() {
    try {
        // Push a new failed attempt onto the stack (value 1 is arbitrary, could track timestamp instead)
        failedAttempts.push(1);
        
        // Get the total number of failed attempts
        int attempts = failedAttempts.size();
        
        // Set locked status and lockout time based on number of failed attempts
        if (attempts < 3) {
            // First 2 attempts: Warning only
            lockoutTime = 0;
            locked = false;
            std::cout << "Warning: Failed login attempt " << attempts << " of 3 before temporary lockout." << std::endl;
        } else if (attempts == 3) {
            // 3 attempts: Lock for 1 minute (60 seconds)
            lockoutTime = 60;
            locked = true;
            std::cout << "WARNING: Account locked for 1 minute due to 3 failed login attempts." << std::endl;
            std::cout << "After 5 failed attempts, your account will be DELETED for security reasons." << std::endl;
        } else if (attempts == 4) {
            // 4 attempts: Last warning before deletion
            lockoutTime = 60;
            locked = true;
            std::cout << "CRITICAL WARNING: This is your LAST attempt before account deletion!" << std::endl;
            std::cout << "Account remains locked for 1 minute." << std::endl;
        } else if (attempts >= 5) {
            // 5 or more attempts: Mark for deletion
            lockoutTime = 999999; // Effectively permanent
            locked = true;
            std::cout << "ACCOUNT SECURITY BREACH: Your account has been marked for DELETION due to " 
                    << "multiple failed login attempts." << std::endl;
            std::cout << "Please contact customer support if this was a mistake." << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error recording failed attempt: " << e.what() << std::endl;
        // Ensure account is still secured even if there's an error
        locked = true;
        lockoutTime = 60;
    }
}

/**
 * Resets all failed login attempts and unlocks the account
 * Called when a user successfully authenticates with the correct password
 */
void Account::resetFailedAttempts() {
    try {
        // Clear all failed attempts from the stack by popping them one by one
        while (!failedAttempts.empty()) {
            failedAttempts.pop();
        }
        
        // Reset the account security state to default (unlocked)
        locked = false;
        lockoutTime = 0;
    } catch (const std::exception& e) {
        std::cerr << "Error resetting failed attempts: " << e.what() << std::endl;
        // Ensure account is unlocked even if there's an error with the stack operations
        locked = false;
        lockoutTime = 0;
    }
}

/**
 * Checks and displays the current lock status of the account
 * Provides user feedback about:
 * - Whether the account is locked
 * - How long the lockout period is
 * - How many attempts remain before account deletion
 */
void Account::checkLockStatus() {
    try {
        // First check if account is already marked for deletion (5+ failed attempts)
        if (locked && failedAttempts.size() >= 5) {
            std::cout << "This account has been marked for deletion due to security concerns." << std::endl;
            std::cout << "Please contact customer support for assistance." << std::endl;
            return;
        }
        
        // If account is temporarily locked (3-4 failed attempts), display lockout info
        if (locked) {
            std::cout << "Account is temporarily locked due to multiple failed login attempts." << std::endl;
            std::cout << "Please try again after " << lockoutTime << " seconds." << std::endl;
            
            // Calculate and display remaining attempts before account deletion
            // Formula: 5 (max attempts) - current attempts = remaining attempts
            int remainingAttempts = 5 - static_cast<int>(failedAttempts.size());
            if (remainingAttempts < 0) remainingAttempts = 0;
            
            std::cout << "Remaining attempts before deletion: " << remainingAttempts << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error checking lock status: " << e.what() << std::endl;
    }
}

bool Account::deposit(double amount) {
    if (amount <= 0) return false;
    
    balance += amount;
    
    // Create transaction record
    std::time_t now = std::time(nullptr);
    std::string date = std::ctime(&now);
    date.pop_back(); // Remove trailing newline
    
    Transaction trans(generateTransactionId(), accountId, "deposit", amount, date);
    recentTransactions.push(trans);
    
    while (recentTransactions.size() > 10) {
        recentTransactions.pop();
    }
    
    return true;
}

bool Account::withdraw(double amount) {
    if (amount <= 0 || amount > balance) return false;
    
    balance -= amount;
    
    // Create transaction record
    std::time_t now = std::time(nullptr);
    std::string date = std::ctime(&now);
    date.pop_back(); // Remove trailing newline
    
    Transaction trans(generateTransactionId(), accountId, "withdrawal", amount, date);
    recentTransactions.push(trans);
    
    while (recentTransactions.size() > 10) {
        recentTransactions.pop();
    }
    
    return true;
}

std::string Account::toCSV() const {
    return accountId + "," + customerName + "," + accountType + "," + 
            std::to_string(balance) + "," + password;
}

std::string Account::generateTransactionId() const {
    // Simple transaction ID generator
    std::string chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    std::string id = "TXN";
    
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<> distribution(0, chars.size() - 1);

    for (int i = 0; i < 8; ++i) {
        id += chars[distribution(generator)];
    }
    
    return id;
}