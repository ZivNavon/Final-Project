#include "menu.h"
#include <stdio.h>
#include "employees.h"
#include "items.h"
#include "customers.h"
#define _CRT_SECURE_NO_WARNINGS

extern Employee* employees;
extern Item* items;
extern Customer* customers;

// Menu Function
void showMenu(int permissionLevel) {
    int choice = -1;

    while (choice != 0) {
        printf("\n--- Menu (Permission Level: %d) ---\n", permissionLevel);

        if (permissionLevel == 1) { // Admin menu
            printf("1. Add Employee\n");
            printf("2. Add Item\n");
            printf("3. View Customers\n");
            printf("4. Add Customers\n");
            printf("5. View Items\n");
            printf("6. Delete Item\n");
            printf("7. Product Sale\n");
            printf("8. Product Return\n");
            printf("9. View Customer Purchases\n");
            printf("0. Exit\n");
        }

        else if (permissionLevel == 2) { // Employee menu
            printf("1. View Items\n");
            printf("2. View Customers\n");
            printf("3. Add Customers\n");
            printf("4. Product Sale\n");
            printf("5. View Customer Purchases\n");
            printf("0. Exit\n");
        }
        else if (permissionLevel == 3) { // Trainee menu
            printf("1. View Items\n");
            printf("2. View Customers\n");
            printf("3. View Customer Purchases\n");
            printf("0. Exit\n");
        }

        printf("Choose an option: ");
        scanf("%d", &choice);

        switch (permissionLevel) {
        case 1: // Admin options
            handleAdminOptions(choice);
            break;
        case 2: // Employee options
            handleEmployeeOptions(choice);
            break;
        case 3: // Trainee options
            handleTraineeOptions(choice);
            break;
        default:
            printf("Invalid permission level. Exiting...\n");
            return;
        }
    }
}

// Handle Admin Menu Options
void handleAdminOptions(int choice) {
    switch (choice) {
    case 1:
        addEmployeeOption();
        break;
    case 2:
        addItemOption();
        break;
    case 3:
        viewCustomersOption();
        break;
    case 4:
        addCustomerOption();
        break;
    case 5:
        viewItemsOption();
        break;
    case 6:
        deleteItemOption();
        break;
    case 7:
        productSaleOption();
        break;
    case 8:
        productReturnOption();
        break;
    case 9:
        viewCustomerPurchasesOption();
        break;
    case 0:
        printf("Exiting...\n");
        break;
    default:
        printf("Invalid option. Try again.\n");
    }
}

// Handle Employee Menu Options
void handleEmployeeOptions(int choice) {
    switch (choice) {
    case 1:
        viewItemsOption();
        break;
    case 2:
        viewCustomersOption();
        break;
    case 3:
        addCustomerOption();
        break;
    case 4:
        productSaleOption();
        break;
    case 5:
        viewCustomerPurchasesOption();
        break;
    case 0:
        printf("Exiting...\n");
        break;
    default:
        printf("Invalid option. Try again.\n");
    }
}


void handleTraineeOptions(int choice) {
    switch (choice) {
    case 1:
        viewItemsOption();
        break;
    case 2:
        viewCustomersOption();
        break;
    case 3:
        viewCustomerPurchasesOption();
        break;
    case 0:
        printf("Exiting...\n");
        break;
    default:
        printf("Invalid option. Try again.\n");
    }
}

void addEmployeeOption() {
    Employee newEmployee;

    printf("\n--- Add Employee ---\n");

    // Get employee details
    printf("Enter Username: ");
    scanf("%29s", newEmployee.username);

    // Check if the username already exists
    Employee* existingEmployee = searchEmployee(employees, newEmployee.username);
    if (existingEmployee) {
        printf("Error: Username '%s' already exists. Cannot add duplicate employees.\n", newEmployee.username);
        return;
    }

    printf("Enter Password: ");
    scanf("%29s", newEmployee.password);

    printf("Enter First Name: ");
    scanf("%29s", newEmployee.firstName);

    printf("Enter Permission Level (1 = Admin, 2 = Employee, 3 = Trainee): ");
    scanf("%d", &newEmployee.permissionLevel);

    // Validate permission level
    if (newEmployee.permissionLevel < 1 || newEmployee.permissionLevel > 3) {
        printf("Error: Invalid permission level. Must be 1, 2, or 3.\n");
        return;
    }

    // Add the new employee to the linked list
    employees = addEmployee(employees, newEmployee);

    // Save employees to file
    saveEmployeesToFile(employees);

    // Log the action
    char logMessage[100];
    snprintf(logMessage, sizeof(logMessage), "Added a new employee: %s", newEmployee.username);
    logAction("admin", logMessage);

    printf("Employee '%s' added successfully.\n", newEmployee.username);
}

void addItemOption() {
    char choice; // To handle "Add another item?" prompt

    do {
        Item newItem;

        // Clear the newItem structure to avoid uninitialized memory
        memset(&newItem, 0, sizeof(Item));

        printf("\n--- Add Item ---\n");

        // Input details for the new item
        printf("Serial Number (numeric, e.g., 12345): ");
        if (scanf("%lu", &newItem.serialNumber) != 1) {
            printf("Invalid input. Please enter a valid numeric serial number (e.g., 12345).\n");
            while (getchar() != '\n'); // Clear input buffer
            continue;
        }

        printf("Manufacturer (string, max 49 characters, e.g., Nike): ");
        if (scanf("%49s", newItem.manufacturer) != 1) {
            printf("Invalid input. Please enter a valid manufacturer name (e.g., Nike).\n");
            while (getchar() != '\n'); // Clear input buffer
            continue;
        }

        printf("Model (string, max 49 characters, e.g., AirMax): ");
        if (scanf("%49s", newItem.model) != 1) {
            printf("Invalid input. Please enter a valid model name (e.g., AirMax).\n");
            while (getchar() != '\n'); // Clear input buffer
            continue;
        }

        printf("Price (decimal, e.g., 59.99): ");
        if (scanf("%f", &newItem.price) != 1) {
            printf("Invalid input. Please enter a valid decimal price (e.g., 59.99).\n");
            while (getchar() != '\n'); // Clear input buffer
            continue;
        }

        printf("Warranty (months, numeric, e.g., 12): ");
        if (scanf("%d", &newItem.warrantyMonths) != 1) {
            printf("Invalid input. Please enter a valid numeric warranty period in months (e.g., 12).\n");
            while (getchar() != '\n'); // Clear input buffer
            continue;
        }

        printf("Stock (numeric, e.g., 50): ");
        if (scanf("%d", &newItem.stock) != 1) {
            printf("Invalid input. Please enter a valid numeric stock quantity (e.g., 50).\n");
            while (getchar() != '\n'); // Clear input buffer
            continue;
        }

        printf("Launch Date (DD MM YYYY, e.g., 25 01 2025): ");
        if (scanf("%d %d %d", &newItem.launchDate.day, &newItem.launchDate.month, &newItem.launchDate.year) != 3) {
            printf("Invalid input. Please enter a valid date in the format DD MM YYYY (e.g., 25 01 2025).\n");
            while (getchar() != '\n'); // Clear input buffer
            continue;
        }

        printf("Discount End Date (DD MM YYYY, e.g., 31 12 2025): ");
        if (scanf("%d %d %d", &newItem.discountEndDate.day, &newItem.discountEndDate.month, &newItem.discountEndDate.year) != 3) {
            printf("Invalid input. Please enter a valid date in the format DD MM YYYY (e.g., 31 12 2025).\n");
            while (getchar() != '\n'); // Clear input buffer
            continue;
        }

        // Automatically set the inStock and memberDiscount fields
        newItem.inStock = newItem.stock > 0;
        newItem.memberDiscount = false; // Default: no discount

        // Add item to the binary search tree
        items = addItem(items, newItem);

        // Save the updated inventory to the binary file
        saveItemsToFile(items, "items.dat");

        // Log the action
        logAction("admin", "Added a new item to the inventory");

        printf("\nItem added successfully:\n");
        printf("Serial Number: %lu\n", newItem.serialNumber);
        printf("Manufacturer: %s\n", newItem.manufacturer);
        printf("Model: %s\n", newItem.model);
        printf("Price: %.2f\n", newItem.price);
        printf("Warranty: %d months\n", newItem.warrantyMonths);
        printf("In Stock: %s\n", newItem.inStock ? "Yes" : "No");
        printf("Member Discount: %s\n", newItem.memberDiscount ? "Yes" : "No");
        printf("Launch Date: %02d/%02d/%04d\n", newItem.launchDate.day, newItem.launchDate.month, newItem.launchDate.year);
        printf("Discount End Date: %02d/%02d/%04d\n", newItem.discountEndDate.day, newItem.discountEndDate.month, newItem.discountEndDate.year);
        printf("Stock: %d\n", newItem.stock);

        // Ask if the user wants to add another item
        printf("\nDo you want to add another item? (y/n): ");
        scanf(" %c", &choice); // Note the space before %c to ignore leftover whitespace

    } while (choice == 'y' || choice == 'Y');

    printf("\nReturning to the main menu...\n");
}

void viewCustomersOption() {
    printf("\n--- View Customers ---\n");

    Customer* current = customers;
    while (current != NULL) {
        printf("Customer ID: %lu, Name: %s, Joined: %04d-%02d-%02d, Total Purchased: %.2f\n",
            current->customerID, current->fullName,
            current->joinDate.year, current->joinDate.month, current->joinDate.day,
            current->totalPurchased);
        current = current->next;
    }
}

void addCustomerOption() {
    Customer newCustomer;

    printf("\n--- Add Customer ---\n");

    // Input customer details
    printf("Customer ID: ");
    scanf("%lu", &newCustomer.customerID);

    printf("Full Name: ");
    scanf(" %[^\n]s", newCustomer.fullName);

    printf("Joining Date (YYYY MM DD): ");
    scanf("%d %d %d", &newCustomer.joinDate.year, &newCustomer.joinDate.month, &newCustomer.joinDate.day);

    newCustomer.totalPurchased = 0.0;
    newCustomer.purchaseHistory = NULL;

    // Add customer to the linked list
    customers = addCustomer(customers, newCustomer);

    // Save customers to file
    saveCustomersToFile(customers, "customers.txt");

    // Log the action
    logAction("admin", "Added a new customer");

    printf("Customer added successfully.\n");
}

void productSaleOption() {
    unsigned long serialNumber;
    unsigned long customerID;
    int quantity;

    printf("\n--- Product Sale ---\n");

    // Input customer ID
    printf("Customer ID: ");
    scanf("%lu", &customerID);

    Customer* customer = searchCustomer(customers, customerID);
    if (!customer) {
        printf("Error: Customer not found.\n");
        return;
    }

    // Input product details
    printf("Product Serial Number: ");
    scanf("%lu", &serialNumber);

    Item* item = searchItem(items, serialNumber);
    if (!item) {
        printf("Error: Item not found.\n");
        return;
    }

    printf("Quantity: ");
    scanf("%d", &quantity);

    if (item->stock < quantity) {
        printf("Error: Insufficient stock.\n");
        return;
    }

    // Update stock and purchase history
    item->stock -= quantity;
    customer->totalPurchased += item->price * quantity;

    // Save changes
    saveItemsToFile(items, "items.dat");
    saveCustomersToFile(customers, "customers.txt");

    // Log the action
    logAction("admin", "Processed a product sale");

    printf("Product sold successfully.\n");
}

void productReturnOption() {
    unsigned long serialNumber;
    unsigned long customerID;

    printf("\n--- Product Return ---\n");

    // Input customer ID
    printf("Customer ID: ");
    scanf("%lu", &customerID);

    Customer* customer = searchCustomer(customers, customerID);
    if (!customer) {
        printf("Error: Customer not found.\n");
        return;
    }

    // Input product serial number
    printf("Product Serial Number: ");
    scanf("%lu", &serialNumber);

    Item* item = searchItem(items, serialNumber);
    if (!item) {
        printf("Error: Item not found.\n");
        return;
    }

    // Return the item (stock adjustment)
    item->stock++;
    customer->totalPurchased -= item->price;

    // Save changes
    saveItemsToFile(items, "items.dat");
    saveCustomersToFile(customers, "customers.txt");

    // Log the action
    logAction("admin", "Processed a product return");

    printf("Product returned successfully.\n");
}

void viewCustomerPurchasesOption() {
    unsigned long customerID;

    printf("\n--- View Customer Purchases ---\n");

    // Input customer ID
    printf("Customer ID: ");
    scanf("%lu", &customerID);

    Customer* customer = searchCustomer(customers, customerID);
    if (!customer) {
        printf("Error: Customer not found.\n");
        return;
    }

    printf("Customer: %s\n", customer->fullName);
    printf("Total Purchased: %.2f\n", customer->totalPurchased);
}

void viewItemsOption() {
    printf("\n--- View Items ---\n");
    printItems(items);
}

void deleteItemOption() {
    unsigned long serialNumber;

    printf("\n--- Delete Item ---\n");
    printf("Enter the serial number of the item to delete: ");
    if (scanf("%lu", &serialNumber) != 1) {
        printf("Invalid input. Please enter a valid numeric serial number.\n");
        while (getchar() != '\n'); // Clear input buffer
        return;
    }

    // Search for the item before attempting to delete
    Item* item = searchItem(items, serialNumber);
    if (!item) {
        printf("Error: Item with serial number %lu not found.\n", serialNumber);
        return;
    }

    // Delete the item from the BST
    items = deleteItem(items, serialNumber);

    // Save the updated inventory to the binary file
    saveItemsToFile(items, "items.dat");

    // Log the action
    char logMessage[100];
    snprintf(logMessage, sizeof(logMessage), "Deleted item with serial number: %lu", serialNumber);
    logAction("admin", logMessage);

    printf("Item with serial number %lu deleted successfully.\n", serialNumber);
}