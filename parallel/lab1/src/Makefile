CC=gcc
CFLAGS= -fopenmp -g -Wall -O0
LDFLAGS= -fopenmp

SRC=$(wildcard *.c)
TARGET=$(SRC:.c=.run)

all: $(TARGET)

%.run: %.o
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) -c $^ $(CFLAGS)

clean:
	rm -rf *.o *.run
