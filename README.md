Grocery Shop Management System

Overview
This is a Grocery Shop Management System implemented in C. 
It provides functionalities to manage a grocery shop's inventory, place orders, generate bills, and maintain sales reports. 
The system features an interactive console interface with color-coded outputs for a better user experience.
The program is specifically tailored for any Grocery Shop and includes customizable shop details displayed prominently.

------------------------------------------------------------------------------
Features

1. Inventory Management
   - Add new items to the inventory.
   - View all inventory items.
   - Edit the price or quantity of existing items.
   - Remove items from the inventory.
   - View inventory items by category.

2. Shop Operations
   - Place orders and generate bills.
   - View sales reports.
   - Maintain and view order history.
   - Edit shop details, such as name, address, and contact information.

3. Interactive Menu
   - Color-coded and user-friendly interface for easier navigation.
   - Command-line arguments for quick access to specific features.

-------------------------------------------------------------------------------

Requirements

- A C compiler (e.g., GCC).
- The code is designed for Windows and relies on Windows-specific APIs.

-------------------------------------------------------------------------------
Installation and Setup

1. Clone the repository or copy the source files into your project directory.

2. Ensure the following files are available:
   - 'final.c': The main program source file.
   - 'heads.h': Header file with function declarations.
   - 'Makefile': Used to build the executable out of the source(.c) files. 

3. Compile the program using a C compiler, such as GCC:
   Run the makefile by typing 'make' on your computer. 
   (Make sure that environment variable to "C:\MinGW\msys\1.0\bin" is set.)

4. Run the program as per the instructions below.

-----------------------------------------------------------------------------------------

Usage

The program can be used in two modes: **Command-line mode** and **Interactive menu mode**.

Command-Line Mode
- Run the program with specific arguments for quick actions. Examples:
  -            '-i': View the inventory.
  -            '-o': Place an order and generate a bill.
  -            '-h': View order history.
  -            '-g': View items in a specific group.
  -           '-ep': Edit the price of an item.
  -           '-eq': Edit the quantity of an item.
  - '-r' or '--rem': Remove an item from the inventory.
  - '-a' or '--add': Add a new item to the inventory.

Example command:
```
shopApp.exe -i

```
------------------------------------------------------------------------------------------------------------------
Interactive Menu Mode
- Run the program without arguments to start the interactive menu:
  ```
  shopApp.exe

  ```
- Follow the prompts to perform various operations, such as viewing the inventory, adding items, or placing orders.
-------------------------------------------------------------------------------------------------------------------

File Descriptions

1. "shop_details.txt":
   This file contains the shop's name, address, and phone number. The information is displayed at the top of the menu and reports.

   Example:
   ```
   The Green Basket
   123 Market Lane, Downtown
   +1-800-555-1234
   ```

2. "inventory.txt":
   This file stores inventory data, including categories, product names, quantities, prices, expiry dates, and units.

   Example:
   ```
   Fruits
   SNo.       Product Name         Quantity       Price in Rs       Expiry Date         Unit
   1          Apple                50             100               2024-12-31         Rs per kg
   ```

--------------------------------------------------------------------------------------------------------------------

Features Breakdown
Inventory Management
- Add Item: Specify details such as category, product name, quantity, price, expiry date, and unit to add a new item.
- View Inventory: Display all items in the inventory, grouped by categories.
- Edit Price/Quantity: Modify the price or quantity of an existing item.
- Remove Item: Delete an item from the inventory.
- View by Group: Filter items based on specific categories.

Order Management
- Place an order by selecting items and quantities.
- Automatically deduct ordered quantities from the inventory.
- Generate a bill for the order.

Reports and History
- View a detailed sales report, including total revenue generated.
- Access the history of all previous orders.

---

Developer Notes

- The program uses Windows-specific APIs for console operations (e.g., setting text color and clearing the screen). 
  It will not work on non-Windows systems without modification.

---

Limitations

1. Windows-Specific: The program relies on Windows APIs and is not cross-platform.
2. File Corruption: If `inventory.txt` or `shop_details.txt` is corrupted, the program may crash or behave unpredictably.

---
Credits

Edit Shop Details and Sales Report - Romir Gupta(BT2024195)

Show Inventory and Show Group      - Aditya Mittal(BT2024188)

Edit Price and Edit Quantity       - Divyanshu Ghosh(IMT2024068)

Add and Delete Inventory           - Devaang Rastogi(BT2024031)

Exit and Testing                   - Mayank Ray(IMT2024052)

Place order and Order History      - Siddharth Brijesh Tripathi(IMT2024011)
