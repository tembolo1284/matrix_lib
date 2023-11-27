CC = gcc
CFLAGS = -Wall -g 

TARGET = main

SRC = matrix.c 
OBJ = matrix.o main.o

all: $(TARGET)

$(TARGET): $(OBJ) 
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

main.o: main.c matrix.h
	$(CC) $(CFLAGS) -c main.c

matrix.o: matrix.c matrix.h 
	$(CC) $(CFLAGS) -c matrix.c
	
clean:
	rm -f $(TARGET) $(OBJ)

run:
	./$(TARGET)
