CC=g++
CFLAGS=-Wall -Wextra -g -std=c++17
DEPS = iset.h
OBJ = test.o iset.o

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm *.o
