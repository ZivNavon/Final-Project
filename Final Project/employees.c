#include "employees.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define _CRT_SECURE_NO_WARNINGS


// Global variable
Employee* employees = NULL;

void loadEmployees() {
    FILE* fp = fopen("employees.txt", "r");
    if (!fp) return;

    Employee temp;
    while (fscanf(fp, "%29s %29s %29s %d", temp.username, temp.password, temp.firstName, &temp.permissionLevel) == 4) {
        employees = addEmployee(employees, temp);
    }
    fclose(fp);
}

void saveEmployees() {
    FILE* fp = fopen("employees.txt", "w");
    if (!fp) return;

    Employee* current = employees;
    while (current) {
        fprintf(fp, "%s %s %s %d\n", current->username, current->password, current->firstName, current->permissionLevel);
        current = current->next;
    }
    fclose(fp);
}

int login() {
    char username[30] = { 0 }, password[30] = { 0 };
    int attempts = 3;

    while (attempts > 0) {
        printf("Username: ");
        scanf("%29s", username);
        printf("Password: ");
        scanf("%29s", password);

        Employee* found = findEmployee(employees, username, password);
        if (found) {
            printf("Login successful! Welcome %s\n", found->firstName);
            return found->permissionLevel;
        }

        attempts--;
        printf("Invalid credentials. %d attempts remaining.\n", attempts);
    }

    return -1;
}

void createDefaultAdmin() {
    Employee admin = { "admin", "12345678", "manager", 1 };
    employees = addEmployee(employees, admin);
    saveEmployees();
}

Employee* addEmployee(Employee* list, Employee newEmployee) {
    Employee* node = malloc(sizeof(Employee));
    if (!node) {
        printf("Memory allocation failed!\n");
        return list;
    }
    *node = newEmployee;
    node->next = list;
    return node;
}

Employee* findEmployee(Employee* list, const char* username, const char* password) {
    while (list) {
        if (strcmp(list->username, username) == 0 && strcmp(list->password, password) == 0) {
            return list;
        }
        list = list->next;
    }
    return NULL;
}