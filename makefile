CC = gcc
CFLAGS = -Wall -g
TARGET = main

$(TARGET): src/main.o src/lexer.o
	$(CC) $(CFLAGS) -o $(TARGET) src/main.o src/lexer.o

src/main.o: src/main.c
	$(CC) $(CFLAGS) -c src/main.c -o src/main.o

src/lexer.o: src/lexer.c
	$(CC) $(CFLAGS) -c src/lexer.c -o src/lexer.o

clean:
	rm -f src/*.o $(TARGET)