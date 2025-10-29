CC = gcc
CFLAGS = -Wall -g
SRC = src
OBJ = $(SRC)/main.o $(SRC)/lexer.o
TARGET = c2

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

$(SRC)/%.o: $(SRC)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(SRC)/*.o $(TARGET)