// Ziv Navon | TZ: 207423955     
// Ofek Iram | TZ: 319089884

#include "employees.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "log.h"
#define _CRT_SECURE_NO_WARNINGS

// Add a new employee to the linked list
Employee* addEmployee(Employee* list, Employee newEmployee) {
    Employee* node = (Employee*)malloc(sizeof(Employee));
    if (!node) {
        printf("Error: Memory allocation failed for employee.\n");
        return list;
    }
    *node = newEmployee;
    node->next = list;

    // Log the action
    logAction("admin", "Added a new employee");

    return node;
}

// Search for an employee by username
Employee* searchEmployee(Employee* list, const char* username) {
    while (list) {
        if (strcmp(list->username, username) == 0) return list;
        list = list->next;
    }
    return NULL;
}

// Create the default admin account
void createDefaultAdmin() {
    Employee admin = { "admin", "12345678", "manager", 1, NULL };
    Employee* employees = addEmployee(NULL, admin);
    saveEmployeesToFile(employees);

    // Log the action
    logAction("system", "Default admin account created");
}

// Login functionality
int login(Employee* employees) {
    char username[30], password[30];
    int attempts = 0;

    while (attempts < 3) {
        printf("Username: ");
        scanf("%29s", username);
        printf("Password: ");
        scanf("%29s", password);

        Employee* employee = searchEmployee(employees, username);
        if (employee && strcmp(employee->password, password) == 0) {
            printf("Login successful! Welcome, %s.\n", employee->firstName);

            //Log successful login
            char logMessage[100];
            snprintf(logMessage, sizeof(logMessage), "Successful Login - User: %s", username);
            logAction(username, logMessage);

            return employee->permissionLevel;
        }

        attempts++;
        printf("Invalid username or password. Attempts left: %d\n", 3 - attempts);

        //Log failed login attempt
        char logMessage[100];
        snprintf(logMessage, sizeof(logMessage), "Failed Login Attempt - User: %s, Attempt: %d", username, attempts);
        logAction("System", logMessage);
    }

    // If 3 failed attempts, log system lockout
    logAction("System", "User locked out due to multiple failed login attempts.");
    printf("Too many failed login attempts. Exiting system.\n");
    return -1;
}

// Save employees to a file
void saveEmployeesToFile(Employee* list) {
    FILE* fp = fopen("employees.txt", "w");
    if (!fp) {
        printf("Error: Could not open employees.txt for writing.\n");
        return;
    }

    while (list) {
        fprintf(fp, "%s %s %s %d\n", list->username, list->password, list->firstName, list->permissionLevel);
        list = list->next;
    }

    fclose(fp);

    // Log the action
    logAction("system", "Employee data saved to file");
}

// Load employees from a file
Employee* loadEmployeesFromFile() {
    FILE* fp = fopen("employees.txt", "r");
    if (!fp) {
        printf("No employees file found. Creating default admin...\n");
        createDefaultAdmin();
        return loadEmployeesFromFile();
    }

    Employee* list = NULL;
    Employee temp;

    while (fscanf(fp, "%29s %29s %29s %d", temp.username, temp.password, temp.firstName, &temp.permissionLevel) == 4) {
        list = addEmployee(list, temp);
    }

    fclose(fp);

    // Log the action
    logAction("system", "Employee data loaded from file");

    return list;
}