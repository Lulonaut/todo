CC=g++
CFLAGS=-std=c++20 -Wall -Wextra -lncurses -O3 -o todo
SRC=src/main.cpp

all: $(SRC)
	$(CC) $(SRC) $(CFLAGS)

clean:
	@-rm todo