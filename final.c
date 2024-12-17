#include <stdio.h>
#include <windows.h>
#include <string.h>
#include "heads.h"

typedef struct {
    char name[50];
    char address[100];
    char phone[15];
} Shop;

// Function to set console text color
void setColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

void clear() {
    system("cls || clear");
}

void printShopDetails(char *title) 
{
	Shop shop;
    const char *filename = "shop_details.txt";
    loadShopDetails(&shop, filename);
	clear();
    // Get console handle
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hConsole == NULL) {
        printf("Error getting console handle\n");
        return;
    }

    // Get console screen buffer info
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (!GetConsoleScreenBufferInfo(hConsole, &csbi)) {
        printf("Error getting console buffer info\n");
        return;
    }

    // Calculate the console width
    int consoleWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;

    // Print top border
    setColor(10); // Green color for borders
    for (int i = 0; i < consoleWidth; i++) {
        printf("-");
    }
    printf("\n");

    // Center the title
    int strLength = strlen(title);
    int xPosition = (consoleWidth - strLength) / 2;
    COORD coord;
    coord.X = xPosition;
    coord.Y = 1;
    SetConsoleCursorPosition(hConsole, coord);
    setColor(14); // Yellow color for the title
    printf("%s %s\n", "Welcome to", shop.name);

    int startY = 2; // Start printing below the title
    coord.X = 0; // Left alignment
    coord.Y = startY;
    SetConsoleCursorPosition(hConsole, coord);
    setColor(11); // Light blue for address lines
    printf("%s", shop.address);
    coord.X = consoleWidth - strlen(shop.phone) - 1; // Right-align phone number
    SetConsoleCursorPosition(hConsole, coord);
    setColor(13); // Pink for phone number
    printf("%s", shop.phone);
        

    // Print bottom border
    coord.X = 0;
    coord.Y = startY + 1;
    SetConsoleCursorPosition(hConsole, coord);
    setColor(10); // Green color for borders
    for (int i = 0; i < consoleWidth; i++) {
        printf("-");
    }
    printf("\n");

    // Reset color to default
    setColor(7); // White
}

void showMenu() {
    clear();
    int t = 0;
    printShopDetails("Welcome to The Green Basket Grocery Shop");

    setColor(14); // Yellow for the menu title
    printf("Choose:\n");
    setColor(11); // Light blue for menu options
    printf("1  for seeing the inventory\n");
    printf("2  for seeing a particular group of items\n");
    printf("3  for adding an item in inventory\n");
    printf("4  for deleting an item from inventory\n");
    printf("5  for edit price of an Item\n");
    printf("6  for edit quantity of item\n");
    printf("7  for Placing Order and Generate Bill\n");
    printf("8  for editing shop details\n");
    printf("9  for seeing the Sales Report\n");
    printf("10 for seeing the Order history\n");
    printf("0  for Exit\n");
    setColor(14); // Yellow for input prompt
    printf("Enter your choice: ");

    int p = scanf("%d", &t);
    if (p != 1) 
    {
        char ch;
        while ((ch = getchar()) != '\n' && ch != EOF); // Clear input buffer
        showMenu();
    }
    switch (t) 
    {
        case 0 : clear(); return; break;
        case 1 : showInventory(); break;
        case 2 : showGroup(); break;
        case 3 : addItem(); break;
        case 4 : removeItem(); break;
        case 5 : editPrice(); break;
        case 6 : editQuantity(); break;
        case 7 : order(); break;
        case 8 : editDetails(); break;
        case 9 : report(); break;
        case 10: history(); break;
        default: showMenu();
    }
}

int main(int argc, char *argv[]) 
{
    if (argc == 2) {
        if (strcasecmp(argv[1], "-i") == 0) 
        {
        	printShopDetails("Welcome to The Green Basket Grocery Shop");
            setColor(14); // Yellow for specific outputs
            showInventory();
        } 
        else if (strcasecmp(argv[1], "-o") == 0) 
        {
        	printShopDetails("Welcome to The Green Basket Grocery Shop");
            setColor(11); // Light blue
            order();
        } 
        else if (strcasecmp(argv[1], "-h") == 0) 
        {
        	printShopDetails("Welcome to The Green Basket Grocery Shop");
            setColor(11); // Light blue
            history();
        } 
        else if (strcasecmp(argv[1], "-g") == 0)
        {
        	printShopDetails("Welcome to The Green Basket Grocery Shop");
            setColor(10); // Green
            showGroup();
        } 
        else if (strcasecmp(argv[1], "-a") == 0 || strcasecmp(argv[1], "--add") == 0) 
        {
        	printShopDetails("Welcome to The Green Basket Grocery Shop");
            setColor(10); // Green
            addItem();
        } 
        else if (strcasecmp(argv[1], "-ep") == 0) 
        {
        	printShopDetails("Welcome to The Green Basket Grocery Shop");
            setColor(13); // Pink
            editPrice();
        }
        else if (strcasecmp(argv[1], "-eq") == 0) 
        {
        	printShopDetails("Welcome to The Green Basket Grocery Shop");
            setColor(13); // Pink
            editQuantity();
        }  
        else if (strcasecmp(argv[1], "--rem") == 0 || strcasecmp(argv[1], "-r") == 0)
        {
        	printShopDetails("Welcome to The Green Basket Grocery Shop");
            setColor(13); // Pink
            removeItem();
        } 
        else 
        {
            setColor(12); // Red for errors
            fprintf(stderr, "Enter Valid Command!\n");
        }
    } else if (argc == 1)
        showMenu();

    // Reset to default color
    setColor(7);
    return 0;
}
