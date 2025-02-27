// Ziv Navon | TZ: 207423955     
// Ofek Iram | TZ: 319089884

#ifndef ITEMS_H
#define ITEMS_H

#include <stdbool.h>
#define _CRT_SECURE_NO_WARNINGS


typedef struct Date {
    int year;
    int month;
    int day;
} Date;

// Item structure
typedef struct Item {
    unsigned long serialNumber;    // Unique identifier for the item
    char manufacturer[50];         // Manufacturer name
    char model[50];                // Model name
    float price;                   // Price of the item
    int warrantyMonths;            // Warranty period in months
    bool inStock;                  // True if item is in stock
    bool memberDiscount;           // True if there's a member discount
    Date launchDate;               // Launch date of the item
    Date discountEndDate;          // Discount end date
    int stock;                     // Number of units in stock
    struct Item* left;             // Pointer to the left child (BST)
    struct Item* right;            // Pointer to the right child (BST)
} Item;

// Function prototypes
Item* addItem(Item* root, Item newItem);
Item* searchItem(Item* root, unsigned long serialNumber);
void printItems(Item* root);
void saveItemsToFile(Item* root, const char* filename);
Item* loadItemsFromFile(const char* filename);
Item* deleteItem(Item* root, unsigned long serialNumber);
void searchItems(Item* root);

#endif // ITEMS_H