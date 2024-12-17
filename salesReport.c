#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "heads.h"

typedef struct {
    char product[50];
    double revenue;
} ProductRevenue;

// Function to find or add a product in the revenue list
int findOrAddProduct(ProductRevenue *products, int *count, char *product, double revenue) 
{
    for (int i = 0; i < *count; i++) {
        if (strcmp(products[i].product, product) == 0) 
        {
            products[i].revenue += revenue;
            return 1;
        }
    }
    strcpy(products[*count].product, product);
    products[*count].revenue = revenue;
    (*count)++;
    return 0;
}

// Function to process the orders file
void processFile(FILE *file, int inputYear, int inputMonth) {
    char line[256];
    int currentYear = 0, currentMonth = 0;
    ProductRevenue products[100];
    int productCount = 0;
    double totalRevenue = 0;

    // Read the file line by line
    while (fgets(line, sizeof(line), file)) {
        
        if (strstr(line, "Order Date:")) {
            sscanf(line, "Order Date: %*d/%d/%d %*d:%*d:%*d", &currentMonth, &currentYear);
        }

        // Process the orders for the matching year and month
        if (currentYear == inputYear && currentMonth == inputMonth) {
            if (strstr(line, "Product:")) {
                char product[50];
                double price = 0.0;
                int quantity = 0;

        
                sscanf(line, "    Product: %s", product);
                fgets(line, sizeof(line), file);
                sscanf(line, "    Price: %lf", &price);
                fgets(line, sizeof(line), file);
                sscanf(line, "    Quantity: %d", &quantity);

                double revenue = price * quantity;
                totalRevenue += revenue;
                findOrAddProduct(products, &productCount, product, revenue);
            }
        }
    }

    // Display the sales report
    printf("\nSales Report for %d/%d\n", inputMonth, inputYear);
    printf("------------------------------\n");
    if (productCount == 0) {
        printf("No sales data available for this period.\n");
    } else {
        printf("Product\t\tRevenue\n");
        printf("------------------------------\n");
        for (int i = 0; i < productCount; i++) {
            printf("%s\t\t%7.2f\n", products[i].product, products[i].revenue);
        }
        printf("------------------------------\n");
        printf("Total Revenue: %.2f\n", totalRevenue);
    }
}

void report() 
{
    FILE *file = fopen("orders.txt", "r");
    if (!file) {
        printf("Could not open orders.txt\n");
        return;
    }

    int year, month;
    printf("Enter the year: ");
    scanf("%d", &year);
    printf("Enter the month: ");
    scanf("%d", &month);

    processFile(file, year, month);

    fclose(file);
    char ch = 'A';
    while(ch!='Y' && ch!='y' && ch!='N' && ch!='n')
    {
        printf("Do you want to continue? (Y/N): ");
        scanf(" %c", &ch);
    }
    if (ch == 'Y' || ch == 'y')
        showMenu();
    else
        return;
}
