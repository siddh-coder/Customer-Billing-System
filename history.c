#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "heads.h"

#define MAX_LINE_LENGTH 256

// Function to validate date in dd/mm/yyyy format
int validateDate(const char *date) {
    int day, month, year;
    if (sscanf(date, "%d/%d/%d", &day, &month, &year) != 3) return 0;
    if (year < 0 || month < 1 || month > 12 || day < 1 || day > 31) return 0;

    // Simplistic validation for month-day combinations
    if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30) return 0;
    if (month == 2 && (day > 29 || (day == 29 && year % 4 != 0))) return 0;

    return 1;
}

// Function to validate time in hh:mm:ss format
int validateTime(const char *time) {
    int hour, minute, second;
    if (sscanf(time, "%d:%d:%d", &hour, &minute, &second) != 3) return 0;
    if (hour < 0 || hour > 23 || minute < 0 || minute > 59 || second < 0 || second > 59) return 0;
    return 1;
}

// Custom function to compare dates (returns -1, 0, 1)
int compareDates(const char *date1, const char *date2) {
    int d1, m1, y1, d2, m2, y2;
    sscanf(date1, "%d/%d/%d", &d1, &m1, &y1);
    sscanf(date2, "%d/%d/%d", &d2, &m2, &y2);

    if (y1 != y2) return y1 - y2;
    if (m1 != m2) return m1 - m2;
    return d1 - d2;
}

// Custom function to compare times (returns -1, 0, 1)
int compareTimes(const char *time1, const char *time2) {
    int h1, m1, s1, h2, m2, s2;
    sscanf(time1, "%d:%d:%d", &h1, &m1, &s1);
    sscanf(time2, "%d:%d:%d", &h2, &m2, &s2);

    if (h1 != h2) return h1 - h2;
    if (m1 != m2) return m1 - m2;
    return s1 - s2;
}

// Function to read orders and display those within the given date range
void displayOrdersInRange(const char *fileName, const char *startDate, const char *endDate) {
    FILE *file = fopen(fileName, "r");
    if (!file) {
        perror("Could not open orders.txt");
        return;
    }

    char line[MAX_LINE_LENGTH];
    char orderDate[20];
    char orderTime[10];
    char customerName[50];
    int inDateRange = 0;

    printf("+-------------------------------------------------------------+\n");
    printf("| Customer Name | Product   | Quantity | Price | Total       |\n");
    printf("+-------------------------------------------------------------+\n");

    while (fgets(line, MAX_LINE_LENGTH, file)) {
        line[strcspn(line, "\n")] = 0;

        if (strncmp(line, "Order Date:", 11) == 0) {
            sscanf(line, "Order Date: %s %s", orderDate, orderTime);

            // Compare date and time ranges
            inDateRange = compareDates(orderDate, startDate) >= 0 &&
                          compareDates(orderDate, endDate) <= 0;
        } else if (strncmp(line, "Customer Name:", 14) == 0) {
            sscanf(line, "Customer Name: %[^\n]", customerName);
        } else if (inDateRange && strstr(line, "Product:")) {
            char productName[50];
            int price = 0, quantity = 0;

            sscanf(line, "    Product: %s", productName);
            fgets(line, MAX_LINE_LENGTH, file); // Read Price
            sscanf(line, "    Price: %d", &price);
            fgets(line, MAX_LINE_LENGTH, file); // Read Quantity
            sscanf(line, "    Quantity: %d", &quantity);

            int total = price * quantity;
            printf("| %-13s | %-9s | %-8d | %-5d | %-10d |\n", customerName, productName, quantity, price, total);
        }
    }

    printf("+-------------------------------------------------------------+\n");
    fclose(file);
}

void history() {
    char startDateStr[20], endDateStr[20];
    printf("Enter the start date (dd/mm/yyyy): ");
    getchar();
    fgets(startDateStr, sizeof(startDateStr), stdin);
    startDateStr[strcspn(startDateStr, "\n")] = '\0';

    printf("Enter the end date (dd/mm/yyyy): ");
    fgets(endDateStr, sizeof(endDateStr), stdin);
    endDateStr[strcspn(endDateStr, "\n")] = '\0';

    if (!validateDate(startDateStr) || !validateDate(endDateStr)) {
        printf("Invalid date format. Please enter dates in dd/mm/yyyy format.\n");
        history();
    }

    if (compareDates(startDateStr, endDateStr) > 0) {
        setColor(12);
        printf("Error: Start date cannot be later than end date.\n");
        setColor(7);
        history();
    }

    displayOrdersInRange("orders.txt", startDateStr, endDateStr);
    char ch='A';
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
