CC = gcc
CFLAGS = -Wall -g
SRC = src
OBJ = $(SRC)/main.o $(SRC)/cLexer.o $(SRC)/cParser.o
TARGET = c2

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^
	@echo "Cleaning up object files..."
	@rm -f $(OBJ)

$(SRC)/%.o: $(SRC)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

c:
	rm -f $(SRC)/*.o $(TARGET)
