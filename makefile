# Compiler and flags
CC = gcc
CFLAGS = -Wall -g

# Source files
SRC = final.c history.c inventory.c order.c shopDetails.c salesReport.c

# Object files (generated from the source files)
OBJ = $(SRC:.c=.o)

# Output executable name
EXEC = shopApp

# Default target: build the application
all: $(EXEC)

# Link the object files into the executable
$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $(EXEC)

# Rule to compile the source files into object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up object files and the executable
clean:
	rm -f $(OBJ) $(EXEC)

# Optional: to remove object files, executable, and backups
fclean: clean
	rm -f $(EXEC)

# Rebuild the project from scratch
re: fclean all

# Phony targets (not actual files)
.PHONY: all clean fclean re
