#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <string>
#include <map>
#include <vector>

// Algorithm 1: Binary search for account existence
template<typename KeyType, typename ValueType>
bool binarySearchMap(const std::map<KeyType, ValueType>& mapToSearch, const KeyType& key);

// Algorithm 2: Generate unique account ID
std::string generateUniqueAccountId(const std::map<std::string, void*>& existingAccounts);

// Algorithm 3: Password strength checker using stacks
int checkPasswordStrength(const std::string& password);

// Simulate password cracking and return estimated time to crack
std::string simulatePasswordCrack(const std::string& password);

// Helper function: Split string by delimiter (for CSV parsing)
std::vector<std::string> splitString(const std::string& str, char delimiter);

#endif // ALGORITHMS_H