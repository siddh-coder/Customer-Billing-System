#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "heads.h"

typedef struct {
    char name[50];
    char address[100];
    char phone[15];
} Shop;

void displayShopDetails(Shop *shop);
void editShopDetails(Shop *shop);
void saveShopDetails(Shop *shop, const char *filename);
void loadShopDetails(Shop *shop, const char *filename);
void printCentered(char *str);

void editDetails() 
{
    Shop shop;
    const char *filename = "shop_details.txt";
    int choice;

    loadShopDetails(&shop, filename);
    printShopDetails("Welcome to The Green Basket Grocery Shop");

    while (1) 
    {
        //printCentered(shop.name);
        printf("\n1. Display Shop Details\n");
        printf("2. Edit Shop Details\n");
        printf("3. Save and Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                displayShopDetails(&shop);
                break;
            case 2:
                editShopDetails(&shop);
                break;
            case 3:
                saveShopDetails(&shop, filename);
                printf("Shop details saved. Exiting program.\n");
                char ch = 'A';
                while(ch!='Y' && ch!='y' && ch!='N' && ch!='n')
                {
                    printf("Do you want to continue to main menu? (Y/N): ");
                    scanf(" %c", &ch);
                }
                if (ch == 'Y' || ch == 'y')
                {
                    showMenu();
                    return;
                }
                else
                {
                    clear();
                    return;
                }
                break;
            default:
                printf("Invalid choice! Please try again.\n");
        }
    }
}

void printCentered(char *str) {
    system("cls");
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hConsole == NULL) {
        printf("Error getting console handle\n");
        return;
    }

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (!GetConsoleScreenBufferInfo(hConsole, &csbi)) {
        printf("Error getting console buffer info\n");
        return;
    }

    int consoleWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    int strLength = strlen(str);
    int xPosition = (consoleWidth - strLength) / 2;

    for (int i = 0; i < consoleWidth; i++) printf("-");
    printf("\n");

    COORD coord;
    coord.X = xPosition;
    coord.Y = 1;
    SetConsoleCursorPosition(hConsole, coord);
    printf("%s\n", str);

    for (int i = 0; i < consoleWidth; i++) printf("-");
    printf("\n");
}

void displayShopDetails(Shop *shop) {
    printf("\n=== Shop Details ===\n");
    printf("Name   : %s\n", shop->name[0] ? shop->name : "Not set");
    printf("Address: %s\n", shop->address[0] ? shop->address : "Not set");
    printf("Phone  : %s\n", shop->phone[0] ? shop->phone : "Not set");
}

void editShopDetails(Shop *shop) 
{
    printShopDetails("Welcome to The Green Basket Grocery Shop");
    int choice;
    while (1) 
    {
        //printCentered(shop->name);
        printf("\n=== Edit Shop Details ===\n");
        printf("1. Edit Shop Name\n");
        printf("2. Edit Shop Address\n");
        printf("3. Edit Shop Phone\n");
        printf("4. Go Back\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        getchar();
        switch (choice) 
        {
            case 1:
                printf("Enter new shop name: ");
                fgets(shop->name, sizeof(shop->name), stdin);
                shop->name[strcspn(shop->name, "\n")] = 0;
                printCentered(shop->name);  // Update the title instantly
                printf("Shop name updated successfully!\n");
                break;
            case 2:
                printf("Enter new shop address: ");
                fgets(shop->address, sizeof(shop->address), stdin);
                shop->address[strcspn(shop->address, "\n")] = 0;
                printf("Shop address updated successfully!\n");
                break;
            case 3:
                printf("Enter new shop phone: ");
                fgets(shop->phone, sizeof(shop->phone), stdin);
                shop->phone[strcspn(shop->phone, "\n")] = 0;
                printf("Shop phone updated successfully!\n");
                break;
            case 4:
                return;
            default:
                printf("Invalid choice! Please try again.\n");
        }
    }
}

void saveShopDetails(Shop *shop, const char *filename) 
{
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Failed to open file for saving");
        return;
    }

    fprintf(file, "%s\n%s\n%s\n", shop->name, shop->address, shop->phone);
    fclose(file);
}

void loadShopDetails(Shop *shop, const char *filename) 
{
    FILE *file = fopen(filename, "r");
    if (file == NULL) 
    {
        strcpy(shop->name, "Green Grocery Shop");
        strcpy(shop->address, "James Bond,Shop Number 15,Bangalore Bazaar, Bangalore");
        strcpy(shop->phone, "Ph:1234567890");
        return;
    }

    fgets(shop->name, sizeof(shop->name), file);
    shop->name[strcspn(shop->name, "\n")] = 0;

    fgets(shop->address, sizeof(shop->address), file);
    shop->address[strcspn(shop->address, "\n")] = 0;

    fgets(shop->phone, sizeof(shop->phone), file);
    shop->phone[strcspn(shop->phone, "\n")] = 0;

    fclose(file);
}
