#include "../../include/utils/FileHandler.h"
#include "../../include/utils/Algorithms.h"
#include "../../include/models/CheckingAccount.h"
#include "../../include/models/SavingsAccount.h"
#include <fstream>

FileHandler::FileHandler(const std::string& accFile, const std::string& transFile)
    : accountsFilePath(accFile), transactionsFilePath(transFile) {}

void FileHandler::loadAccounts(std::map<std::string, Account*>& accounts) {
    std::ifstream accountsFile(accountsFilePath);
    if (accountsFile.is_open()) {
        std::string line;
        while (std::getline(accountsFile, line)) {
            if (line.empty()) continue;
            
            try {
                std::vector<std::string> parts = splitString(line, ',');
                
                if (parts.size() >= 5) {
                    std::string id = parts[0];
                    std::string name = parts[1];
                    std::string type = parts[2];
                    double balance = std::stod(parts[3]);
                    std::string password = parts[4];
                    
                    if (type == "Checking" && parts.size() >= 6) {
                        double fee = std::stod(parts[5]);
                        accounts[id] = new CheckingAccount(id, name, balance, password, fee);
                    } else if (type == "Savings" && parts.size() >= 6) {
                        double rate = std::stod(parts[5]);
                        accounts[id] = new SavingsAccount(id, name, balance, password, rate);
                    } else {
                        // For any other account type, create a base Account
                        accounts[id] = new Account(id, name, type, balance, password);
                    }
                }
            } catch (const std::exception& e) {
                std::cerr << "Error loading account data: " << e.what() << std::endl;
                // Continue to next line
                continue;
            }
        }
        accountsFile.close();
    }
}

void FileHandler::loadTransactions(std::vector<Transaction>& transactions) {
    std::ifstream transactionsFile(transactionsFilePath);
    if (transactionsFile.is_open()) {
        std::string line;
        while (std::getline(transactionsFile, line)) {
            if (line.empty()) continue;
            
            std::vector<std::string> parts = splitString(line, ',');
            
            if (parts.size() >= 5) {
                std::string transId = parts[0];
                std::string accId = parts[1];
                std::string type = parts[2];
                double amount = std::stod(parts[3]);
                std::string date = parts[4];
                
                transactions.push_back(Transaction(transId, accId, type, amount, date));
            }
        }
        transactionsFile.close();
    }
}

void FileHandler::saveAccounts(const std::map<std::string, Account*>& accounts) {
    std::ofstream accountsFile(accountsFilePath);
    if (accountsFile.is_open()) {
        for (const auto& pair : accounts) {
            accountsFile << pair.second->toCSV() << std::endl;
        }
        accountsFile.close();
    }
}

void FileHandler::saveTransactions(const std::vector<Transaction>& transactions) {
    std::ofstream transactionsFile(transactionsFilePath);
    if (transactionsFile.is_open()) {
        for (const auto& trans : transactions) {
            transactionsFile << trans.toCSV() << std::endl;
        }
        transactionsFile.close();
    }
}