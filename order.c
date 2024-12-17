#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <time.h>
#include <ctype.h>
#include "heads.h"

struct order 
{
    char *product;
    int price;
    int quantity;
    struct order *next;
};

void addOrder(struct order **head, char *prod, int pric, int quant) 
{
    struct order *newNode = (struct order *)malloc(sizeof(struct order));
    newNode->product = (char *)malloc(strlen(prod) + 1);
    strcpy(newNode->product, prod);
    newNode->price = pric;
    newNode->quantity = quant;
    newNode->next = NULL;

    if (*head == NULL)
        *head = newNode;
    else {
        struct order *temp = *head;
        while (temp->next != NULL)
            temp = temp->next;
        temp->next = newNode;
    }
}

void getCurrentDateTime(char *date, char *tim) 
{
    time_t now = time(NULL);
    struct tm *local = localtime(&now);

    sprintf(date, "%02d/%02d/%04d", local->tm_mday, local->tm_mon + 1, local->tm_year + 1900);
    sprintf(tim, "%02d:%02d:%02d", local->tm_hour, local->tm_min, local->tm_sec);
}

void trimSpaces(char *str) 
{
    char *end;
    while (isspace((unsigned char)*str)) str++;
    if (*str != '\0') 
    {
        end = str + strlen(str) - 1;
        while (end > str && isspace((unsigned char)*end)) end--;
        *(end + 1) = '\0';
    }
}

int getPriceAndUpdateInventory(const char *productName, int quantityOrdered) 
{
    FILE *file = fopen("inventory.txt", "r");
    FILE *tempFile = fopen("temp_inventory.txt", "w");
    if (file == NULL || tempFile == NULL) 
    {
        perror("Could not open inventory.txt");
        return -1;
    }

    char line[256];
    char currentProduct[50];
    int price, new_price=0;
    int productFound = 0;
    int quantity;

    while (fgets(line, sizeof(line), file)) 
    {
        // Parse product details
        int serial;
        char expiryDate[20], unit[50];

        if (sscanf(line, "%d %s %d %d %s %[^\n]", &serial, currentProduct, &quantity, &price, expiryDate, unit) == 6) 
        {
            trimSpaces(currentProduct);
            if (strcasecmp(currentProduct, productName) == 0) 
            {
                new_price = price;
                productFound = 1;
                if (quantityOrdered > quantity)
                {
                    setColor(12);
                    printf("Error: Not enough quantity of '%s' in inventory. Available: %d\n", productName, quantity);
                    setColor(7);
                    fclose(file);
                    fclose(tempFile);
                    remove("temp_inventory.txt");
                    return -1;
                }
                quantity -= quantityOrdered; // Update quantity
                if (quantity == 0)
                    printf("Warning: Quantity of '%s' has reached zero.\n", productName);
                fprintf(tempFile, "%-10d %-20s %-15d %-20d %-15s %-20s\n", serial, currentProduct, quantity, price, expiryDate, unit);
            } 
            else 
                fputs(line, tempFile); // Keep lines unchanged
        }
        else
            fputs(line, tempFile);
    }

    fclose(file);
    fclose(tempFile);

    if (productFound) 
    {
        remove("inventory.txt");
        rename("temp_inventory.txt", "inventory.txt");
        return new_price;
    } 
    else 
    {
        setColor(12);
        printf("Error: Product '%s' not found in inventory.\n", productName);
        setColor(7);
        remove("temp_inventory.txt");
        return -1;
    }
}

void saveOrdersToFile(struct order *head, const char *customerName) 
{
    FILE *file = fopen("orders.txt", "a");
    if (!file) 
    {
        perror("Could not open orders.txt");
        return;
    }

    char date[11], time[9];
    getCurrentDateTime(date, time);

    fprintf(file, "Customer Name: %s\n", customerName);
    fprintf(file, "Order Date: %s %s\n", date, time);
    fprintf(file, "Orders:\n");

    struct order *temp = head;
    int orderNumber = 1;
    while (temp) {
        fprintf(file, "  Order #%d:\n", orderNumber++);
        fprintf(file, "    Product: %s\n", temp->product);
        fprintf(file, "    Price: %d\n", temp->price);
        fprintf(file, "    Quantity: %d\n", temp->quantity);
        temp = temp->next;
    }
    fprintf(file, "---------------------------------------\n");
    fclose(file);
}

void displayBill(struct order *head, const char *customerName) 
{
    char date[11], time[9];
    getCurrentDateTime(date, time);

    printf("\nCustomer Name: %s\n", customerName);
    printf("Order Date: %s %s\n", date, time);
    printf("===============================================================\n");
    printf("| %-15s | %-8s | %-10s | %-10s |\n", "Product", "Price", "Quantity", "Subtotal");
    printf("===============================================================\n");

    struct order *temp = head;
    int grandTotal = 0;

    while (temp) 
    {
        int subtotal = temp->price * temp->quantity;
        grandTotal += subtotal;

        printf("| %-15s | %-8d | %-10d | %-10d |\n", temp->product, temp->price, temp->quantity, subtotal);
        temp = temp->next;
    }

    printf("===============================================================\n");
    printf("| %-35s | %-10d |\n", "Grand Total", grandTotal);
    printf("===============================================================\n");
}

void freeOrders(struct order *head) 
{
    struct order *temp;
    while (head) 
    {
        temp = head;
        head = head->next;
        free(temp->product);
        free(temp);
    }
}

void order() 
{
    struct order *or = NULL;
    char customerName[30];
    printf("Enter the name of the customer: ");
    getchar();
    fgets(customerName, sizeof(customerName), stdin);
    trimSpaces(customerName);

    char input[50];
    char productName[30];
    int quantity;

    FILE *file = fopen("inventory.txt", "r");
    if (file == NULL) 
    {
        setColor(12);
        printf("Error: Could not open inventory.txt. First add an item!\n");
        setColor(7);
        char ch = 'A';
        while(ch!='Y' && ch!='y' && ch!='N' && ch!='n')
        {
            printf("Do you want to continue to add items? (Y/N): ");
            scanf(" %c", &ch);
        }
        if (ch == 'Y' || ch == 'y')
            addItem();
        else
            showMenu();
        return;
    }

    char line[256];
    printf("\n=== Inventory ===\n\n");

    while (fgets(line, sizeof(line), file))
        printf("%s", line);

    fclose(file);
    printf("\n");

    while (1) 
    {
        printf("Enter the product name and quantity separated by space (or 0 to finish): ");
        fgets(input, sizeof(input), stdin);
        if (sscanf(input, "%s %d", productName, &quantity) != 2 || strcmp(productName, "0") == 0)
            break;

        int price = getPriceAndUpdateInventory(productName, quantity);
        if (price == -1)
            continue;
        addOrder(&or, productName, price, quantity);
    }

    if (or) 
    {
        displayBill(or, customerName); // Display the bill after placing the order
        saveOrdersToFile(or, customerName); // Save order to file
    }
    freeOrders(or);

    char ch = 'A';
    while(ch!='Y' && ch!='y' && ch!='N' && ch!='n')
    {
        printf("Would you like to place another order? (Y/N): ");
        scanf(" %c", &ch);
    }
    if (ch == 'Y' || ch == 'y')
        order();
    else
        showMenu();
}

