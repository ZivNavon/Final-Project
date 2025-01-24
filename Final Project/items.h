#ifndef ITEMS_H
#define ITEMS_H

#include <stdbool.h>
#define _CRT_SECURE_NO_WARNINGS


// Date structure
typedef struct Date {
    int year;
    int month;
    int day;
} Date;

// Item structure
typedef struct Item {
    unsigned long serialNumber;
    char manufacturer[50];
    char model[50];
    float price;
    int warrantyMonths;
    bool inStock;
    int stock;
    Date launchDate;
    Date discountEndDate;
    int discount;
    struct Item* left, * right;
} Item;

// Function prototypes
Item* addItem(Item* root, Item newItem);
Item* searchItem(Item* root, unsigned long serialNumber);
void printItems(Item* root);
void saveItemsToFile(Item* root, const char* filename);
Item* loadItemsFromFile(const char* filename);

#endif // ITEMS_H