SRC = $(wildcard src/*.cpp)

OBJ = $(wildcard *.o)

CC = g++ -std=c++11

COMPILER_FLAGS = -w -Wall -O
INCLUDE = -Iinclude/ -isystem /Library/Developer/CommandLineTools/usr/include/c++/v1

PROG_NAME = sudoku

obj: 
	$(CC) $(SRC) $(COMPILER_FLAGS) $(INCLUDE) -c

prog:
	$(CC) $(OBJ) -o $(PROG_NAME)

full:
	$(CC) $(SRC) $(COMPILER_FLAGS) $(INCLUDE) -o $(PROG_NAME)

clean:
	rm *.o $(PROG_NAME) 
