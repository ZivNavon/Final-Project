#ifndef MENU_H
#define MENU_H

#include "employees.h"
#include "items.h"
#include "customers.h"

// Function Prototypes
void handleAdminOptions(int choice);
void handleEmployeeOptions(int choice);
void handleTraineeOptions(int choice);
void addEmployeeOption();
void addItemOption();
void viewCustomersOption();
void addCustomerOption();
void productSaleOption();
void productReturnOption();
void viewCustomerPurchasesOption();
void viewItemsOption();
void deleteItemOption();

#endif // MENU_H
