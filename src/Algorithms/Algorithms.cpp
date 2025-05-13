#include "../../include/utils/Algorithms.h"
#include <algorithm>
#include <random>
#include <stack>
#include <cctype>
#include <sstream>
#include <cmath> // For std::pow

// Algorithm 1: Binary search (Using map and Array)
template<typename KeyType, typename ValueType>
bool binarySearchMap(const std::map<KeyType, ValueType>& mapToSearch, const KeyType& key) {
    return mapToSearch.find(key) != mapToSearch.end();
}

// Explicit template instantiation for the types we'll use
template bool binarySearchMap<std::string, void*>(
    const std::map<std::string, void*>& mapToSearch, 
    const std::string& key
);

// Algorithm 2: Generate unique account ID
std::string generateUniqueAccountId(const std::map<std::string, void*>& existingAccounts) {
    std::string prefix = "ACC";
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<> distribution(10000, 99999);
    
    std::string candidate;
    do {
        candidate = prefix + std::to_string(distribution(generator));
    } while (binarySearchMap(existingAccounts, candidate));
    
    return candidate;
}

// Algorithm 3: Password strength checker using stack
int checkPasswordStrength(const std::string& password) {
    if (password.length() < 8) return 0; // Too short
    
    std::stack<char> chars;
    bool hasUpper = false, hasLower = false, hasDigit = false, hasSpecial = false;
    
    for (char c : password) {
        chars.push(c);
        
        if (std::isupper(c)) hasUpper = true;
        else if (std::islower(c)) hasLower = true;
        else if (std::isdigit(c)) hasDigit = true;
        else hasSpecial = true;
    }
    
    // TODO: Implement entropy calculation in the future
    // for more accurate password strength measurement
    std::map<char, int> charFreq;
    
    while (!chars.empty()) {
        charFreq[chars.top()]++;
        chars.pop();
    }
    
    int strength = 0;
    if (hasUpper) strength++;
    if (hasLower) strength++;
    if (hasDigit) strength++;
    if (hasSpecial) strength++;
    
    // Add complexity based on password length
    strength += password.length() / 4;
    
    return strength;
}

// Simulate password cracking and return estimated time to crack
std::string simulatePasswordCrack(const std::string& password) {
    // Calculate possible character set size
    int charsetSize = 0;
    bool hasUpper = false, hasLower = false, hasDigit = false, hasSpecial = false;
    
    for (char c : password) {
        if (std::isupper(c)) hasUpper = true;
        else if (std::islower(c)) hasLower = true;
        else if (std::isdigit(c)) hasDigit = true;
        else hasSpecial = true;
    }
    
    if (hasLower) charsetSize += 26;    // a-z
    if (hasUpper) charsetSize += 26;    // A-Z
    if (hasDigit) charsetSize += 10;    // 0-9
    if (hasSpecial) charsetSize += 33;  // Special characters
    
    // Simulate cracking time
    // Assuming 1 billion attempts per second
    double combinations = std::pow(charsetSize, password.length());
    double secondsToCrack = combinations / 1000000000.0;
    
    // Convert to appropriate time unit
    if (secondsToCrack < 60) {
        return "Instant to seconds";
    } else if (secondsToCrack < 3600) {
        return "Minutes: " + std::to_string(static_cast<int>(secondsToCrack / 60));
    } else if (secondsToCrack < 86400) {
        return "Hours: " + std::to_string(static_cast<int>(secondsToCrack / 3600));
    } else if (secondsToCrack < 31536000) {
        return "Days: " + std::to_string(static_cast<int>(secondsToCrack / 86400));
    } else if (secondsToCrack < 315360000) {
        return "Years: " + std::to_string(static_cast<int>(secondsToCrack / 31536000));
    } else if (secondsToCrack < 31536000000) {
        return "Centuries: " + std::to_string(static_cast<int>(secondsToCrack / 315360000));
    } else {
        return "Millennia or more";
    }
}

// Helper function: Split string by delimiter (for CSV parsing)
std::vector<std::string> splitString(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(str);
    
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    
    return tokens;
}