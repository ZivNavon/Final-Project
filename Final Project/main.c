// Ziv Navon | TZ: 207423955     
// Ofek Iram | TZ: 319089884

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "employees.h"
#include "items.h"
#include "customers.h"
#include "menu.h"
#include "log.h"

// Global variables
Employee* employees = NULL;
Item* items = NULL;
Customer* customers = NULL;

void systemStart();
void loadData();
void saveData();

int main() {
    logSystemStart(); // Log system startup
    systemStart(); // Initialize system (e.g., create default admin if needed)
    loadData();    // Load data from files into data structures

    int permissionLevel = login(employees);
    if (permissionLevel == -1) {
        printf("Too many failed attempts. Exiting...\n");
        logAction("Unknown", "Failed login attempt - system exit");
        return 1;
    }

    // Show the menu based on the logged-in user's permission level
    showMenu(permissionLevel);

    saveData();       // Save all data back to files before exiting
    logSystemShutdown(); // Log system shutdown
    return 0;
}

void systemStart() {
    FILE* fp = fopen("employees.txt", "r");
    if (!fp) {
        printf("No employee file found. Creating default admin...\n");
        createDefaultAdmin();
        //Log missing employee file
        logAction("System", "Employee file missing. Creating default admin.");
    }
    else {
        fclose(fp);
    }
}

void loadData() {
    employees = loadEmployeesFromFile();
    if (employees) {
        logAction("System", "Employee data loaded successfully");
    }
    else {
        logAction("System", "Error: Failed to load employee data.");
    }

    items = loadItemsFromFile("items.dat");
    if (items) {
        logAction("System", "Item data loaded successfully");
    }
    else {
        logAction("System", "Error: Failed to load item data.");
    }

    customers = loadCustomersFromFile("customers.txt");
    if (customers) {
        logAction("System", "Customer data loaded successfully");
    }
    else {
        logAction("System", "Error: Failed to load customer data.");
    }
}

void saveData() {
    saveEmployeesToFile(employees);
    logAction("system", "Employee data saved successfully");

    saveItemsToFile(items, "items.dat");
    logAction("system", "Item data saved successfully");

    saveCustomersToFile(customers, "customers.txt");
    logAction("system", "Customer data saved successfully");
}