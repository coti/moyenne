CC = gcc
CCOPT = -O3
LD = gcc
LDOPT = -lm

all: moyenne

moyenne: moyenne.o
	$(LD) -o moyenne moyenne.o $(LDOPT)

moyenne.o: moyenne.c
	$(CC) $(CCOPT) -c moyenne.c

clean:
	rm moyenne.o moyenne
