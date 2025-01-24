#ifndef EMPLOYEES_H
#define EMPLOYEES_H
#define _CRT_SECURE_NO_WARNINGS

// Employee structure
typedef struct Employee {
    char username[30];
    char password[30];
    char firstName[30];
    int permissionLevel;
    struct Employee* next;
} Employee;

// Declare global employee list
extern Employee* employees;

// Function prototypes
void loadEmployees();
void saveEmployees();
int login();
void createDefaultAdmin();
Employee* addEmployee(Employee* list, Employee newEmployee);
Employee* findEmployee(Employee* list, const char* username, const char* password);

#endif // EMPLOYEES_H
