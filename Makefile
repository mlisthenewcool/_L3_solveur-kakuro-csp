CC      = gcc
CCFLAGS = -g3 -Wall -std=c99
OBJECTS = util.o parseur.o backtrack.o

all: main

main: src/main.c $(OBJECTS)
	$(CC) $(CCFLAGS) -o main src/main.c $(OBJECTS)

%.o: src/%.c
	$(CC) $(CCFLAGS) -c $^

.PHONY : clean

clean:
	rm -vf main *.o
