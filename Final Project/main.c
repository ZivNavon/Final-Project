#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "employees.h"
#include "items.h"
#include "customers.h"
#include "menu.h"

#define _CRT_SECURE_NO_WARNINGS

// Global variables
Item* items = NULL;
Customer* customers = NULL;

void systemStart();
void loadData();
void saveData();

int main() {
    systemStart();
    loadData();

    int permissionLevel = login();
    if (permissionLevel == -1) {
        printf("Login failed. Exiting...\n");
        return 1;
    }

    showMenu(permissionLevel);

    saveData();
    return 0;
}

void systemStart() {
    FILE* fp = fopen("employees.txt", "r");
    if (!fp) {
        printf("Creating default admin...\n");
        createDefaultAdmin();
    }
    else {
        fclose(fp);
    }
}

void loadData() {
    items = loadItemsFromFile("items.dat");
    customers = loadCustomersFromFile("customers.txt");
    loadEmployees();
}

void saveData() {
    saveItemsToFile(items, "items.dat");
    saveCustomersToFile(customers, "customers.txt");
    saveEmployees();
}

// Menu Function (Placeholder)
void showMenu(int permissionLevel) {
    int choice = -1;

    while (choice != 0) {
        printf("\n--- Menu (Permission Level: %d) ---\n", permissionLevel);
        if (permissionLevel == 1) { // Admin menu
            printf("1. Add Employee\n");
            printf("2. Add Item\n");
            printf("3. View Customers\n");
            printf("0. Exit\n");
        }
        else if (permissionLevel == 2) { // Employee menu
            printf("1. Add Item\n");
            printf("2. View Items\n");
            printf("0. Exit\n");
        }
        else if (permissionLevel == 3) { // Trainee menu
            printf("1. View Items\n");
            printf("0. Exit\n");
        }

        printf("Choose an option: ");
        scanf("%d", &choice);

        switch (permissionLevel) {
        case 1: // Admin options
            if (choice == 1) {
                addEmployeeOption();
            }
            else if (choice == 2) {
                addItemOption();
            }
            else if (choice == 3) {
                viewCustomersOption();
            }
            break;

        case 2: // Employee options
            if (choice == 1) {
                addItemOption();
            }
            else if (choice == 2) {
                viewItemsOption();
            }
            break;

        case 3: // Trainee options
            if (choice == 1) {
                viewItemsOption();
            }
            break;

        default:
            break;
        }

        if (choice == 0) {
            printf("Exiting...\n");
        }
    }
}
void addEmployeeOption() {
    Employee newEmployee;

    printf("\n--- Add Employee ---\n");
    printf("Username: ");
    scanf("%29s", newEmployee.username);
    printf("Password: ");
    scanf("%29s", newEmployee.password);
    printf("First Name: ");
    scanf("%29s", newEmployee.firstName);
    printf("Permission Level (1 = Admin, 2 = Employee, 3 = Trainee): ");
    scanf("%d", &newEmployee.permissionLevel);

    employees = addEmployee(employees, newEmployee);
    saveEmployees();
    printf("Employee added successfully.\n");
}

void addItemOption() {
    Item newItem;

    printf("\n--- Add Item ---\n");
    printf("Serial Number: ");
    scanf("%lu", &newItem.serialNumber);
    printf("Manufacturer: ");
    scanf("%49s", newItem.manufacturer);
    printf("Model: ");
    scanf("%49s", newItem.model);
    printf("Price: ");
    scanf("%f", &newItem.price);
    printf("Warranty (months): ");
    scanf("%d", &newItem.warrantyMonths);
    printf("Stock: ");
    scanf("%d", &newItem.stock);
    printf("Launch Date (YYYY MM DD): ");
    scanf("%d %d %d", &newItem.launchDate.year, &newItem.launchDate.month, &newItem.launchDate.day);
    newItem.inStock = newItem.stock > 0;
    newItem.discount = 0;

    items = addItem(items, newItem);
    saveItemsToFile(items, "items.dat");
    printf("Item added successfully.\n");
}

void viewCustomersOption() {
    printf("\n--- View Customers ---\n");
    Customer* current = customers;

    while (current) {
        printf("Customer ID: %lu, Name: %s, Joined: %04d-%02d-%02d, Total Purchased: %.2f\n",
            current->customerID, current->fullName,
            current->joinDate.year, current->joinDate.month, current->joinDate.day,
            current->totalPurchased);
        current = current->next;
    }
}
void viewItemsOption() {
    printf("\n--- View Items ---\n");
    printItems(items);
}

void saveItemsHelper(Item* root, FILE* fp) {
    if (root == NULL) return;

    fwrite(root, sizeof(Item), 1, fp);
    saveItemsHelper(root->left, fp);
    saveItemsHelper(root->right, fp);
}

// Function to save items to a binary file
void saveItemsToFile(Item* root, const char* filename) {
    FILE* fp = fopen(filename, "wb");
    if (!fp) {
        printf("Error: Could not open file %s for writing.\n", filename);
        return;
    }

    saveItemsHelper(root, fp);
    fclose(fp);
}

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
}

Customer* loadCustomersFromFile(const char* filename) {
    FILE* fp = fopen(filename, "r");
    if (!fp) {
        printf("Error: Could not open file %s for reading.\n", filename);
        return NULL;
    }

    Customer* list = NULL, temp;

    while (scanf(fp, "%lu %49s %d-%d-%d %f",
        &temp.customerID, temp.fullName,
        &temp.joinDate.year, &temp.joinDate.month, &temp.joinDate.day,
        &temp.totalPurchased) == 6) {
        list = addCustomer(list, temp);
    }

    fclose(fp);
    return list;
}

// Load items from a binary file
Item* loadItemsFromFile(const char* filename) {
    FILE* fp = fopen(filename, "rb");
    if (!fp) {
        printf("Error: Could not open file %s for reading.\n", filename);
        return NULL;
    }

    Item* root = NULL, temp;
    while (fread(&temp, sizeof(Item), 1, fp)) {
        root = addItem(root, temp);
    }

    fclose(fp);
    return root;
}

Customer* addCustomer(Customer* list, Customer newCustomer) {
    Customer* node = (Customer*)malloc(sizeof(Customer));
    if (!node) {
        printf("Error: Memory allocation failed for new customer.\n");
        return list;
    }

    *node = newCustomer;
    node->purchaseHistory = NULL; // Initialize purchase history
    node->next = NULL;

    // Insert the customer at the beginning of the list
    node->next = list;
    return node;
}

Item* addItem(Item* root, Item newItem) {
    if (root == NULL) {
        Item* node = (Item*)malloc(sizeof(Item));
        if (!node) {
            printf("Error: Memory allocation failed for new item.\n");
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