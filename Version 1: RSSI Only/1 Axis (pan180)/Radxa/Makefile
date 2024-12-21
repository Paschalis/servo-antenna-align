CC = gcc
CFLAGS = -Wall -g
LIBS = -lgpiod
SRC = main.c
OBJ = $(SRC:.c=.o)
EXEC = antenna_alignment

# Compiler and flags
$(EXEC): $(OBJ)
	$(CC) -o $(EXEC) $(OBJ) $(LIBS)

# Compile source files to object files
%.o: %.c
	$(CC) $(CFLAGS) -c $<

# Clean up generated files
clean:
	rm -f $(OBJ) $(EXEC)

# To run the program after build
run: $(EXEC)
	./$(EXEC)

.PHONY: clean run
