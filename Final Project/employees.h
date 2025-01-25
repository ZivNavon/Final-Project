#ifndef EMPLOYEES_H
#define EMPLOYEES_H
#define _CRT_SECURE_NO_WARNINGS

// Employee structure
typedef struct Employee {
    char username[30];      // Username of the employee
    char password[30];      // Password for login
    char firstName[30];     // First name of the employee
    int permissionLevel;    // Permission level: 1 = Admin, 2 = Employee, 3 = Trainee
    struct Employee* next;  // Pointer to the next employee in the linked list
} Employee;

// Function prototypes
Employee* addEmployee(Employee* list, Employee newEmployee);
Employee* searchEmployee(Employee* list, const char* username);
void saveEmployeesToFile(Employee* list);
Employee* loadEmployeesFromFile();
void createDefaultAdmin();
int login(Employee* employees);

#endif // EMPLOYEES_H
