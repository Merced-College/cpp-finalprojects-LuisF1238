# C++ Banking System

## Features

- Create checking and savings accounts
- Deposit and withdraw money
- View account balance
- Close accounts
- Password strength checker
- Data persistence with CSV files


### Data Structures Used
- **Maps**: Used to store and efficiently look up accounts by their IDs 
- **Binary Search**: Implemented in account lookup and ID generation 
- **Stacks**: Used for:
  - Tracking failed login attempts and implementing account lockout 
  - Password strength analysis 
- **Queues**: Used for storing recent transactions 

### Algorithms
1. **Account Number Generator**: Creates unique account IDs 
2. **Password Strength Checker**: Tests password security using stack data structure 
3. **Advanced Security System**: Uses a stack to track login failures with progressive consequences 
   - After 3 failed attempts: Account is locked for 1 minute
   - After 5 failed attempts: Account is automatically deleted for security
   - Correct password entry clears the entire stack of failed attempts
4. **Transaction Processing**: Manages deposits, withdrawals, and interest calculations 

## Running Program

```bash
  ./bank_system
```
