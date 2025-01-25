#include "customers.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define _CRT_SECURE_NO_WARNINGS

// Add Customer to Linked List (Sorted by Joining Date)
Customer* addCustomer(Customer* list, Customer newCustomer) {
    Customer* node = (Customer*)malloc(sizeof(Customer));
    if (!node) {
        printf("Error: Memory allocation failed for customer.\n");
        return list;
    }
    *node = newCustomer;
    node->purchaseHistory = NULL;
    node->next = NULL;

    if (!list || (newCustomer.joinDate.year < list->joinDate.year) ||
        (newCustomer.joinDate.year == list->joinDate.year && newCustomer.joinDate.month < list->joinDate.month) ||
        (newCustomer.joinDate.year == list->joinDate.year && newCustomer.joinDate.month == list->joinDate.month && newCustomer.joinDate.day < list->joinDate.day)) {
        node->next = list;
        return node;
    }

    Customer* current = list;
    while (current->next && ((current->next->joinDate.year < newCustomer.joinDate.year) ||
        (current->next->joinDate.year == newCustomer.joinDate.year && current->next->joinDate.month < newCustomer.joinDate.month) ||
        (current->next->joinDate.year == newCustomer.joinDate.year && current->next->joinDate.month == newCustomer.joinDate.month && current->next->joinDate.day < newCustomer.joinDate.day))) {
        current = current->next;
    }

    node->next = current->next;
    current->next = node;
    return list;
}

// Search Customer by ID
Customer* searchCustomer(Customer* list, unsigned long customerID) {
    while (list) {
        if (list->customerID == customerID) return list;
        list = list->next;
    }
    return NULL;
}

// Save Customers to File (Sorted by Joining Date)
void saveCustomersToFile(Customer* list, const char* filename) {
    FILE* fp = fopen(filename, "w");
    if (!fp) {
        printf("Error: Could not open file %s for writing.\n", filename);
        return;
    }

    while (list) {
        fprintf(fp, "%lu %s %d-%d-%d %.2f\n",
            list->customerID, list->fullName,
            list->joinDate.year, list->joinDate.month, list->joinDate.day,
            list->totalPurchased);
        list = list->next;
    }

    fclose(fp);
    printf("Customers saved successfully to %s\n", filename);
}

// Load Customers from File (Sorted by Joining Date)
Customer* loadCustomersFromFile(const char* filename) {
    FILE* fp = fopen(filename, "r");
    if (!fp) {
        printf("Error: Could not open file %s for reading.\n", filename);
        return NULL;
    }

    Customer* list = NULL;
    Customer temp;

    while (fscanf(fp, "%lu %49s %d-%d-%d %f",
        &temp.customerID, temp.fullName,
        &temp.joinDate.year, &temp.joinDate.month, &temp.joinDate.day,
        &temp.totalPurchased) == 6) {
        list = addCustomer(list, temp);
    }

    fclose(fp);
    return list;
}
