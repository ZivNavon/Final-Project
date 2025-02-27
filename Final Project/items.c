// Ziv Navon | TZ: 207423955     
// Ofek Iram | TZ: 319089884

#include "items.h"
#include "log.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


#define _CRT_SECURE_NO_WARNINGS

// Add Item to Binary Search Tree
Item* addItem(Item* root, Item newItem) {
    if (root == NULL) {
        Item* node = (Item*)malloc(sizeof(Item));
        if (!node) {
            printf("Error: Memory allocation failed for item.\n");
            logAction("System", "Error: Memory allocation failed for new item.");
            return NULL;
        }
        *node = newItem;
        node->left = node->right = NULL;
        return node;
    }

    if (newItem.serialNumber < root->serialNumber) {
        root->left = addItem(root->left, newItem);
    }
    else if (newItem.serialNumber > root->serialNumber) {
        root->right = addItem(root->right, newItem);
    }
    return root;
}

// Search for an Item in the BST
Item* searchItem(Item* root, unsigned long serialNumber) {
    if (!root) return NULL;

    if (serialNumber == root->serialNumber) return root;
    if (serialNumber < root->serialNumber) return searchItem(root->left, serialNumber);
    return searchItem(root->right, serialNumber);
}

// Print Items in Sorted Order (In-order Traversal)
void printItems(Item* root) {
    if (root == NULL) return;

    printItems(root->left);
    printf("Serial Number: %lu\n", root->serialNumber);
    printf("Manufacturer: %s\n", root->manufacturer);
    printf("Model: %s\n", root->model);
    printf("Price: %.2f\n", root->price);
    printf("Warranty: %d months\n", root->warrantyMonths);
    printf("In Stock: %s\n", root->inStock ? "Yes" : "No");
    printf("Member Discount: %s\n", root->memberDiscount ? "Yes" : "No");
    printf("Launch Date: %04d-%02d-%02d\n", root->launchDate.year, root->launchDate.month, root->launchDate.day);
    printf("Discount End Date: %04d-%02d-%02d\n", root->discountEndDate.year, root->discountEndDate.month, root->discountEndDate.day);
    printf("Stock: %d\n\n", root->stock);
    printItems(root->right);
}

// Helper Function to Save BST to a Binary File
void saveItemsHelper(Item* root, FILE* fp) {
    if (root == NULL) return;

    fwrite(root, sizeof(Item), 1, fp);
    saveItemsHelper(root->left, fp);
    saveItemsHelper(root->right, fp);
}

// Save Items to a Binary File in Sorted Order
void saveItemsToFile(Item* root, const char* filename) {
    FILE* fp = fopen(filename, "wb");
    if (!fp) {
        printf("Error: Could not open file %s for writing.\n", filename);
        return;
    }

    saveItemsHelper(root, fp);
    fclose(fp);
    printf("Items saved successfully to %s\n", filename);
}

// Load Items from a Binary File and Build BST
Item* loadItemsFromFile(const char* filename) {
    FILE* fp = fopen(filename, "rb");
    if (!fp) {
        printf("Error: Could not open file %s for reading.\n", filename);
        return NULL;
    }

    Item* root = NULL;
    Item temp;

    while (fread(&temp, sizeof(Item), 1, fp)) {
        root = addItem(root, temp);
    }

    fclose(fp);
    return root;
}

Item* deleteItem(Item* root, unsigned long serialNumber) {
    if (root == NULL) {
        return NULL;
    }

    if (serialNumber < root->serialNumber) {
        root->left = deleteItem(root->left, serialNumber);
    }
    else if (serialNumber > root->serialNumber) {
        root->right = deleteItem(root->right, serialNumber);
    }
    else {
        // Node to be deleted found
        if (root->left == NULL) {
            Item* temp = root->right;
            free(root);
            return temp;
        }
        else if (root->right == NULL) {
            Item* temp = root->left;
            free(root);
            return temp;
        }
        else {
            // Node with two children
            Item* successor = root->right;
            while (successor->left != NULL) {
                successor = successor->left;
            }
            root->serialNumber = successor->serialNumber;
            strcpy(root->manufacturer, successor->manufacturer);
            strcpy(root->model, successor->model);
            root->price = successor->price;
            root->warrantyMonths = successor->warrantyMonths;
            root->stock = successor->stock;
            root->launchDate = successor->launchDate;
            root->discountEndDate = successor->discountEndDate;
            root->inStock = successor->inStock;
            root->memberDiscount = successor->memberDiscount;
            root->right = deleteItem(root->right, successor->serialNumber);
        }
    }

    return root;
}

time_t dateToTimeT(Date date) {
    struct tm tm = { 0 };
    tm.tm_year = date.year - 1900;
    tm.tm_mon = date.month - 1;
    tm.tm_mday = date.day;
    return mktime(&tm);
}

// Helper: Parse a DD-MM-YYYY string into a Date struct
Date parseDate(const char* dateStr) {
    Date date;
    sscanf(dateStr, "%4d-%2d-%2d", &date.day, &date.month, &date.year);
    return date;
}

// Match item based on criteria of the employe
int matchItem(const Item* item, const char* manufacturer, const char* model,
    float minPrice, float maxPrice, int booleanFilter, int hasFilter,
    time_t startDate, time_t endDate) {
    int matches = 1;
    

    if (manufacturer && strlen(manufacturer) > 0) {
        matches = matches && (strstr(item->manufacturer, manufacturer) != NULL);
    }
    if (model && strlen(model) > 0) {
        matches = matches && (strstr(item->model, model) != NULL);
    }
    if (minPrice >= 0) {
        matches = matches && (item->price >= minPrice);
    }
    if (maxPrice >= 0) {
        matches = matches && (item->price <= maxPrice);
    }
    if (hasFilter) {
        matches = matches && (item->memberDiscount == booleanFilter);
    }
    if (startDate > 0) {
        matches = matches && (dateToTimeT(item->launchDate) >= startDate);
    }
    if (endDate > 0) {
        matches = matches && (dateToTimeT(item->launchDate) <= endDate);
    }

    return matches;
}

// Recursive function to search and display items
void searchAndDisplayItems(Item* root, const char* manufacturer, const char* model,
    float minPrice, float maxPrice, int booleanFilter, int hasFilter,
    time_t startDate, time_t endDate) {
    if (!root) return;

    searchAndDisplayItems(root->left, manufacturer, model, minPrice, maxPrice, booleanFilter, hasFilter, startDate, endDate);

    if (matchItem(root, manufacturer, model, minPrice, maxPrice, booleanFilter, hasFilter, startDate, endDate)) {
        printf("Serial: %lu | Manufacturer: %s | Model: %s | Price: %.2f | Stock: %d | Launch Date: %d-%02d-%02d\n",
            root->serialNumber, root->manufacturer, root->model, root->price, root->stock,
            root->launchDate.year, root->launchDate.month, root->launchDate.day);
    }

    searchAndDisplayItems(root->right, manufacturer, model, minPrice, maxPrice, booleanFilter, hasFilter, startDate, endDate);
}

// Main search function
void searchItems(Item* root) {
    char manufacturer[50] = "", model[50] = "";
    float minPrice = -1, maxPrice = -1;
    int booleanFilter = 0, hasFilter = 0;
    time_t startDate = 0, endDate = 0;
    char logMessage[300] = "";

    int choice;
    printf("Select search criteria:\n");
    printf("1. Manufacturer\n");
    printf("2. Model\n");
    printf("3. Price range\n");
    printf("4. Boolean value (Has member discount)\n");
    printf("5. Launch date range\n");
    printf("6. Search with all criteria\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    switch (choice) {
    case 1:
        printf("Enter manufacturer: ");
        scanf("%49s", manufacturer);
        snprintf(logMessage, sizeof(logMessage), "Search - Manufacturer: %s", manufacturer);
        break;
    case 2:
        printf("Enter model: ");
        scanf("%49s", model);
        snprintf(logMessage, sizeof(logMessage), "Search - Model: %s", model);
        break;
    case 3:
        printf("Enter minimum price (or -1 to skip): ");
        scanf("%f", &minPrice);
        printf("Enter maximum price (or -1 to skip): ");
        scanf("%f", &maxPrice);
        snprintf(logMessage, sizeof(logMessage), "Search - Price range: %.2f to %.2f", minPrice, maxPrice);
        break;
    case 4:
        printf("Filter by member discount (1 for yes, 0 for no): ");
        scanf("%d", &booleanFilter);
        hasFilter = 1;
        snprintf(logMessage, sizeof(logMessage), "Search - Member discount: %s", booleanFilter ? "Yes" : "No");
        break;
    case 5: {
        char start[11], end[11];
        printf("Enter start date (YYYY-MM-DD): ");
        scanf("%10s", start);
        printf("Enter end date (YYYY-MM-DD): ");
        scanf("%10s", end);

        startDate = dateToTimeT(parseDate(start));
        endDate = dateToTimeT(parseDate(end));

        snprintf(logMessage, sizeof(logMessage), "Search - Date range: %s to %s", start, end);
        break;
    }
    case 6:
        printf("Enter manufacturer: ");
        scanf("%49s", manufacturer);
        printf("Enter model: ");
        scanf("%49s", model);
        printf("Enter minimum price (or -1 to skip): ");
        scanf("%f", &minPrice);
        printf("Enter maximum price (or -1 to skip): ");
        scanf("%f", &maxPrice);
        printf("Filter by member discount (1 for yes, 0 for no): ");
        scanf("%d", &booleanFilter);
        hasFilter = 1;

        char start[11], end[11];
        printf("Enter start date (YYYY-MM-DD): ");
        scanf("%10s", start);
        printf("Enter end date (YYYY-MM-DD): ");
        scanf("%10s", end);

        startDate = dateToTimeT(parseDate(start));
        endDate = dateToTimeT(parseDate(end));

        snprintf(logMessage, sizeof(logMessage),
            "Search - Manufacturer: %s, Model: %s, Price: %.2f to %.2f, Member Discount: %s, Date range: %s to %s",
            manufacturer, model, minPrice, maxPrice, booleanFilter ? "Yes" : "No", start, end);
        break;
    default:
        printf("Invalid choice.\n");
        return;
    }

    //Log the search operation
    if (strlen(logMessage) > 0 && strcmp(logMessage, "") != 0) {
        logAction("User", logMessage);
    }
    else {
        logAction("User", "Search operation performed but criteria were not captured correctly.");
    }


    printf("\nMatching items:\n");
    searchAndDisplayItems(root, manufacturer, model, minPrice, maxPrice, booleanFilter, hasFilter, startDate, endDate);
}