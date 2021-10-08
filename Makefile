CC=g++
CFLAGS=-std=c++20 -Wall -Wextra -lncurses -lfmt -O3
SRC=src/main.cpp

all: $(SRC)
	$(CC) $(SRC) $(CFLAGS) -o todo

clean:
	@-rm todo