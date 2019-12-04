CC=gcc
CC_OPTS=-Wall

executable: main.o comparator.o syntax.o comparing_algorithm.o
	$(CC) $(CC_OPTS) -o executable $^

%.o: %.c
	$(CC) $(CC_OPTS) -c -o $@ $<

all: executable

clean:
	rm -f executable *.o