// Ziv Navon | TZ: 207423955     
// Ofek Iram | TZ: 319089884

#ifndef CUSTOMERS_H
#define CUSTOMERS_H

#include "items.h" // Include for the Date structure
#define _CRT_SECURE_NO_WARNINGS



// Purchase Structure
typedef struct Purchase {
    unsigned long serialNumber;  // Item serial number
    float price;                 // Store price of item at the time of purchase
    int quantity;                // Store how many were bought
    Date purchaseDate;           // Purchase date
    struct Purchase* next;       // Pointer to next purchase
} Purchase;

// Customer Structure
typedef struct Customer {
    unsigned long customerID;    // Unique customer ID
    char fullName[50];           // Full name of the customer
    Date joinDate;               // Joining date
    float totalPurchased;        // Total amount purchased by the customer
    Purchase* purchaseHistory;   // Linked list of purchases
    struct Customer* next;       // Pointer to next customer in the linked list
} Customer;

// Function Prototypes
Customer* addCustomer(Customer* list, Customer newCustomer);
Customer* searchCustomer(Customer* list, unsigned long customerID);
void saveCustomersToFile(Customer* list, const char* filename);
Customer* loadCustomersFromFile(const char* filename);

#endif // CUSTOMERS_H