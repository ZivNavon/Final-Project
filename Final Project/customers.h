#ifndef CUSTOMERS_H
#define CUSTOMERS_H

#include "items.h"
#define _CRT_SECURE_NO_WARNINGS

// Customer and Purchase Structures
typedef struct Purchase {
    unsigned long serialNumber;
    Date purchaseDate;
    struct Purchase* next;
} Purchase;

typedef struct Customer {
    unsigned long customerID;
    char fullName[50];
    Date joinDate;
    float totalPurchased;
    Purchase* purchaseHistory;
    struct Customer* next;
} Customer;

// Function prototypes

Customer* addCustomer(Customer* list, Customer newCustomer);
Customer* searchCustomer(Customer* list, unsigned long customerID);
void saveCustomersToFile(Customer* list, const char* filename);
Customer* loadCustomersFromFile(const char* filename);

#endif // CUSTOMERS_H