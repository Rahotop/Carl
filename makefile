CC=g++
LDFLAGS=-W -Wall -Wextra -ansi -pedantic -std=c++11 -O3 -fopenmp
SRC= $(wildcard src/*.cpp)
OBJ= $(SRC:.c=.o)

all: carl

carl: $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

clean:
	rm -rf *.o
