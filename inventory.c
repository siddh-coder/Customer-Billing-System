#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "heads.h"

#define MAX_LINE 256
#define FILENAME "inventory.txt"

// Helper function to convert a string to lowercase
void toLowercase(char *str) 
{
    for (int i = 0; str[i]; i++) 
        str[i] = tolower(str[i]);
}

char toLower(char c) {
    return (char)tolower((unsigned char)c);
}

// Custom case-insensitive strcasestr function
char *strcasestr(const char *haystack, const char *needle) {
    if (!haystack || !needle) return NULL;

    size_t needleLen = strlen(needle);
    if (needleLen == 0) return (char *)haystack;

    for (const char *h = haystack; *h; h++) {
        // Compare substrings case-insensitively
        if (tolower((unsigned char)*h) == tolower((unsigned char)*needle)) {
            const char *hTemp = h;
            const char *nTemp = needle;
            while (*nTemp && toLower(*hTemp) == toLower(*nTemp)) {
                hTemp++;
                nTemp++;
            }
            if (*nTemp == '\0') return (char *)h; // Match found
        }
    }

    return NULL; // No match found
}

// Function to display the inventory
void showInventory() 
{
    FILE *file = fopen(FILENAME, "r");
    if (file == NULL) 
    {
        setColor(12);
        printf("Error: Could not open %s. First add an item!\n", FILENAME);
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

    char line[MAX_LINE];
    printf("\n=== Inventory ===\n\n");

    while (fgets(line, sizeof(line), file))
        printf("%s", line);

    fclose(file);
    printf("\n");
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

}

// Function to add an item to a specific category
void addItem() 
{
    FILE *file = fopen(FILENAME, "r");
    if (file == NULL) 
    {
        file = fopen(FILENAME, "w");
        fclose(file);
        file = fopen(FILENAME, "a+");
    }

    FILE *temp = fopen("temp.txt", "w");
    if (temp == NULL) 
    {
        setColor(12);
        printf("Error: Could not create temporary file.\n");
        fclose(file);
        setColor(7);
        char choice;
        while(choice!='Y' && choice!='y' && choice!='N' && choice!='n')
        {
            printf("Do you want to continue? (Y/N): ");
            scanf(" %c", &choice);
        }
        if (choice == 'Y' || choice == 'y') {
            showMenu();
        } else {
            return;
        }
        return;
    }

    char category[50], productName[50], expiryDate[20], unit[50], line[MAX_LINE];
    int quantity, price;

    printf("\n=== Add Item ===\n\n");

    // Get product details from the user
    getchar(); // Consume leftover newline
    printf("Enter category: ");
    fgets(category, sizeof(category), stdin);
    strtok(category, "\n"); // Remove newline

    printf("Enter product name(without spaces): ");
    fgets(productName, sizeof(productName), stdin);
    strtok(productName, "\n");

    printf("Enter quantity: ");
    scanf("%d", &quantity);

    printf("Enter price: ");
    scanf("%d", &price);

    getchar(); // Consume leftover newline
    printf("Enter expiry date: ");
    fgets(expiryDate, sizeof(expiryDate), stdin);
    strtok(expiryDate, "\n");

    printf("Enter unit (e.g., Rs per kg): ");
    fgets(unit, sizeof(unit), stdin);
    strtok(unit, "\n");

    char categoryLower[50], productNameLower[50];
    strcpy(categoryLower, category);
    strcpy(productNameLower, productName);
    toLowercase(categoryLower);
    toLowercase(productNameLower);

    int categoryFound = 0, productFound = 0, serialNumber = 1;
    int isInCategory = 0;

    // Process the file line by line
    while (fgets(line, sizeof(line), file)) 
    {
        char lineLower[MAX_LINE];
        strcpy(lineLower, line);
        toLowercase(lineLower);

        // If we encounter the target category
        if (!categoryFound && strncmp(lineLower, categoryLower, strlen(categoryLower)) == 0 && line[strlen(category)] == '\n') 
        {
            categoryFound = 1;
            isInCategory = 1;
            fprintf(temp, "%s", line); // Write the category line
            continue;
        }

        // If within the category, maintain sequence and check for duplicates
        if (categoryFound && isInCategory) {
            if (line[0] == '\n') { // End of category section
                isInCategory = 0;

                // Add the new product here if not found
                if (!productFound) {
                    fprintf(temp, "%-10d %-20s %-15d %-20d %-15s %-20s\n", serialNumber, productName, quantity, price,
                            expiryDate, unit);
                }
                fprintf(temp, "\n"); // Preserve spacing
            } else if (line[0] >= '0' && line[0] <= '9') {
                char existingProductName[50];
                sscanf(line, "%*d %49s", existingProductName);

                // Convert to lowercase for comparison
                char existingProductNameLower[50];
                strcpy(existingProductNameLower, existingProductName);
                toLowercase(existingProductNameLower);

                if (strcmp(existingProductNameLower, productNameLower) == 0) {
                    productFound = 1; // Duplicate found
                }

                // Maintain sequence numbering
                fprintf(temp, "%-10d%s", serialNumber++, line + 10);
            } else {
                fprintf(temp, "%s", line); // Write non-product lines unchanged
            }
        } else {
            fprintf(temp, "%s", line); // Write lines outside the category unchanged
        }
    }

    // Handle the last category if still in it and the product wasn't added
    if (categoryFound && isInCategory && !productFound) {
        fprintf(temp, "%-10d %-20s %-15d %-20d %-15s %-20s\n", serialNumber, productName, quantity, price,
                expiryDate, unit);
    }

    // If the category doesn't exist, add it at the end with the new product
    if (!categoryFound) {
        fprintf(temp, "\n%s\n", category);
        fprintf(temp, "%-10s %-20s %-15s %-20s %-15s %-20s\n", "SNo.", "Product Name", "Quantity", "Price in Rs",
                "Expiry Date", "Unit");
        fprintf(temp, "%-10d %-20s %-15d %-20d %-15s %-20s\n", serialNumber, productName, quantity, price, expiryDate,
                unit);
    } else if (productFound) {
        setColor(12);
        printf("Error: Product '%s' already exists in category '%s'.\n", productName, category);
        setColor(7);
    }
    fprintf(temp, "%s", "\n");

    fclose(file);
    fclose(temp);

    // If no duplicate was found, update the original file
    if (!productFound) 
    {
        remove(FILENAME);
        rename("temp.txt", FILENAME);
        printf("Product '%s' added successfully to category '%s'.\n", productName, category);
    } 
    else 
    {
        remove("temp.txt");
    }

    char ch = 'A';
    while(ch!='Y' && ch!='y' && ch!='N' && ch!='n')
    {
        printf("Do you want to add more items? (Y/N): ");
        scanf(" %c", &ch);
    }
    if (ch == 'Y' || ch == 'y')
        addItem();
    else
        showMenu();
}


// Function to remove an item by name
void removeItem() 
{
    char line[MAX_LINE];
    char itemName[50], productName[50], expiryDate[50], unit[50];
    int serial = 0, quantity = 0, price = 0;
    char currentSection[50] = "";
    int itemFound = 0;

    printf("\n=== Remove Item ===\n\n");
    // Prompt user for item name
    getchar();
    printf("Enter the name of the item to remove: ");
    fgets(itemName, sizeof(itemName), stdin);
    itemName[strcspn(itemName, "\n")] = '\0'; // Remove trailing newline

    FILE *file = fopen(FILENAME, "r");
    if (file == NULL) 
    {
        setColor(12);
        printf("Error: Could not open %s.\n", FILENAME);
        setColor(7);
        char choice;
        while(choice!='Y' && choice!='y' && choice!='N' && choice!='n')
        {
            printf("Do you want to continue? (Y/N): ");
            scanf(" %c", &choice);
        }
        if (choice == 'Y' || choice == 'y') {
            showMenu();
        } else {
            return;
        }
        setColor(7);
        return;
    }

    FILE *temp = fopen("temp.txt", "w");
    if (temp == NULL) {
        setColor(12);
        printf("Error: Could not create temporary file.\n");
        setColor(7);
        fclose(file);
        return;
    }

    int sectionSerialOffset = 0; // Reset serial offset for each section

    // Process file line by line
    while (fgets(line, sizeof(line), file)) 
    {
        // Detect new section headers
        if (strlen(line) > 1 && !strcasestr(line, "SNo.") && !isdigit(line[0]) && !strchr(line, '.')) {
            fprintf(temp, "%s", line);
            strcpy(currentSection, line); // Update current section
            sectionSerialOffset = 0;     // Reset serial offset for new section
            continue;
        }

        // Copy headers or empty lines
        if (strcasestr(line, "SNo.") || strlen(line) == 1) {
            fprintf(temp, "%s", line);
            continue;
        }

        // Parse item lines
        if (sscanf(line, "%d %s %d %d %s %[^\n]", &serial, productName, &quantity, &price, expiryDate, unit) == 6) {
            if (strcasecmp(productName, itemName) == 0) {
                // Item matches, skip this line
                itemFound = 1;
                sectionSerialOffset = 1; // Start decrementing subsequent serial numbers
                continue;
            } else {
                // Adjust serial number only if an item was removed in this section
                if (sectionSerialOffset) {
                    serial -= sectionSerialOffset;
                }
                fprintf(temp, "%-10d %-20s %-15d %-20d %-15s %-20s\n", serial, productName, quantity, price, expiryDate, unit);
            }
        } else {
            // Copy lines that don't match expected format
            fprintf(temp, "%s", line);
        }
    }
    fprintf(temp, "%s", "\n");

    fclose(file);
    fclose(temp);

    // Handle the case where the item was found or not
    if (itemFound) {
        if (remove(FILENAME) == 0 && rename("temp.txt", FILENAME) == 0) {
            printf("Item '%s' removed successfully!\n", itemName);
        } else {
            setColor(12);
            printf("Error: Could not update the inventory file.\n");
            setColor(7);
            char choice;
            while(choice!='Y' && choice!='y' && choice!='N' && choice!='n')
            {
                printf("Do you want to remove more items? (Y/N): ");
                scanf(" %c", &choice);
            }
            if (choice == 'Y' || choice == 'y') {
                removeItem();
            } else {
                showMenu();
            }
        }
    } else {
        remove("temp.txt");
        setColor(12);
        printf("Item '%s' not found in the inventory.\n", itemName);
        setColor(7);
    }
    char ch = 'A';
    printf("Do you want to remove more items? (Y/N): ");
    scanf(" %c", &ch);
    if (ch == 'Y' || ch == 'y')
        removeItem();
    else
        showMenu();
}

// Function to edit the quantity of an item
void editQuantity() 
{
    printf("\n=== Edit Quantity ===\n\n");
    char itemNamea[50];
    int newQuantity=0;
    printf("Enter the name of item: ");
    scanf("%s", itemNamea);
    char *itemName = itemNamea;
    printf("Enter new Quantity: ");
    scanf("%d", &newQuantity);
    FILE *file = fopen(FILENAME, "r");
    if (file == NULL) 
    {
        setColor(12);
        printf("Error: Could not open %s.\n", FILENAME);
        setColor(7);
        char choice;
        while(choice!='Y' && choice!='y' && choice!='N' && choice!='n')
        {
            printf("Do you want to continue? (Y/N): ");
            scanf(" %c", &choice);
        }
        if (choice == 'Y' || choice == 'y') {
            showMenu();
        } else {
            return;
        }
        return;
    }

    FILE *temp = fopen("temp.txt", "w");
    if (temp == NULL) 
    {
        setColor(12);
        printf("Error: Could not create temporary file.\n");
        fclose(file);
        setColor(7);
        char choice;
        while(choice!='Y' && choice!='y' && choice!='N' && choice!='n')
        {
            printf("Do you want to continue? (Y/N): ");
            scanf(" %c", &choice);
        }
        if (choice == 'Y' || choice == 'y') {
            showMenu();
        } else {
            return;
        }
        return;
    }

    char line[MAX_LINE];
    int itemFound = 0;

    while (fgets(line, sizeof(line), file)) 
    {
        if (strcasestr(line, itemName)) 
        {
            itemFound = 1;
            char name[50], unit[50], expiryDate[20];
            int serial, quantity, price;

            sscanf(line, "%d %s %d %d %s %[^\n]%*c", &serial, name, &quantity, &price, expiryDate, unit);
            fprintf(temp, "%-10d %-20s %-15d %-20d %-15s %-20s\n", serial, name, newQuantity, price, expiryDate, unit);
        } else {
            fputs(line, temp);
        }
    }

    fclose(file);
    fclose(temp);

    if (itemFound) {
        remove(FILENAME);
        rename("temp.txt", FILENAME);
        printf("Quantity updated successfully!\n");
    } else {
        remove("temp.txt");
        printf("Item '%s' not found.\n", itemName);
    }
    char choice;
    while(choice!='Y' && choice!='y' && choice!='N' && choice!='n')
    {
        printf("Do you want to edit quantity of more items? (Y/N): ");
        scanf(" %c", &choice);
    }
    if (choice == 'Y' || choice == 'y') {
        editQuantity();
    } else {
        showMenu();
    }
}

// Function to edit the price of an item
void editPrice() 
{
    printf("\n=== Edit Price ===\n\n");
    char itemNamea[50];
    int newPrice=0;
    printf("Enter the name of item: ");
    scanf("%s", itemNamea);
    char *itemName = itemNamea;
    printf("Enter new Price: ");
    scanf("%d", &newPrice);
    FILE *file = fopen(FILENAME, "r");
    if (file == NULL) 
    {
        setColor(12);
        printf("Error: Could not open %s.\n", FILENAME);
        setColor(7);
        char choice;
        while(choice!='Y' && choice!='y' && choice!='N' && choice!='n')
        {
            printf("Do you want to continue? (Y/N): ");
            scanf(" %c", &choice);
        }
        if (choice == 'Y' || choice == 'y') {
            showMenu();
        } else {
            return;
        }
        return;
    }

    FILE *temp = fopen("temp.txt", "w");
    if (temp == NULL) {
        setColor(12);
        printf("Error: Could not create temporary file.\n");
        setColor(7);
        char choice;
        while(choice!='Y' && choice!='y' && choice!='N' && choice!='n')
        {
            printf("Do you want to continue? (Y/N): ");
            scanf(" %c", &choice);
        }
        if (choice == 'Y' || choice == 'y') {
            showMenu();
        } else {
            return;
        }
        fclose(file);
        return;
    }

    char line[MAX_LINE];
    int itemFound = 0;

    while (fgets(line, sizeof(line), file)) 
    {
        if (strcasestr(line, itemName)) {
            itemFound = 1;
            char name[50], unit[50], expiryDate[20];
            int serial, quantity, price;

            sscanf(line, "%d %s %d %d %s %[^\n]%*c", &serial, name, &quantity, &price, expiryDate, unit);
            fprintf(temp, "%-10d %-20s %-15d %-20d %-15s %-20s\n", serial, name, quantity, newPrice, expiryDate, unit);
        } else {
            fputs(line, temp);
        }
    }

    fclose(file);
    fclose(temp);

    if (itemFound) {
        remove(FILENAME);
        rename("temp.txt", FILENAME);
        printf("Price updated successfully!\n");
    } else {
        remove("temp.txt");
        printf("Item '%s' not found.\n", itemName);
    }
    char choice;
    while(choice!='Y' && choice!='y' && choice!='N' && choice!='n')
    {
        printf("Do you want to edit price of more items? (Y/N): ");
        scanf(" %c", &choice);
    }
    if (choice == 'Y' || choice == 'y') {
        editPrice();
    } else {
        showMenu();
    }
}

void showGroup() 
{
    char groupName[50];
    
    FILE *file = fopen("inventory.txt", "r");
    if (file == NULL) 
    {
        setColor(12);
        printf("Error: Could not open inventory.txt. Make sure the file exists.\n");
        setColor(7);
        char choice;
        while(choice!='Y' && choice!='y' && choice!='N' && choice!='n')
        {
            printf("Do you want to continue? (Y/N): ");
            scanf(" %c", &choice);
        }
        if (choice == 'Y' || choice == 'y')
            showMenu();
        else
            return;
    }

    getchar();
    printf("Enter the group name: ");
    scanf("%[^\n]%*c", groupName);

    char line[256], line2[256];
    char word[50];
    int groupFound = 0;

    printf("\n=== Items in Group: %s ===\n\n", groupName);

    while (fgets(line, sizeof(line), file)) 
    {
        sscanf(line, "%s", word);
        if(strcasecmp(word, groupName)==0)
        {
            printf("%s", fgets(line, sizeof(line), file));
            groupFound=1;
            while(fgets(line2, sizeof(line), file))
            {
                if(isalpha(line2[0]))
                    break;
                printf("%s", line2);
            }
        }
    }

    fclose(file);

    if (!groupFound) {
        setColor(12);
        printf("Error: Group '%s' not found in the inventory.\n", groupName);
        setColor(7);
    }
    char choice;
    while(choice!='Y' && choice!='y' && choice!='N' && choice!='n')
    {
        printf("Do you want to see more groups? (Y/N): ");
        scanf(" %c", &choice);
    }
    if (choice == 'Y' || choice == 'y') {
        showGroup();
    } else {
        showMenu();
    }
}