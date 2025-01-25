// items.c
#include "items.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define _CRT_SECURE_NO_WARNINGS

// Add Item to Binary Search Tree
Item* addItem(Item* root, Item newItem) {
    if (root == NULL) {
        Item* node = (Item*)malloc(sizeof(Item));
        if (!node) {
            printf("Error: Memory allocation failed for item.\n");
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