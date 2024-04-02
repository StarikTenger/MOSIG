CC = gcc
CFLAGS = -O2 -fopenmp
LDFLAGS = -fopenmp

EXEC = 	bubble.exe	\
	mergesort.exe	\
	odd-even.exe

HEADER_FILES = $(wildcard *.h)

RAND_INIT=0

ifeq ($(RAND_INIT), 1)
$(Initialization of the vector with random values)
CONFIG_FLAGS += -DRINIT
endif

all: $(EXEC)

%.exe: %.o utils.o cpu_stats.o
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c $(HEADER_FILES)
	$(CC) -c $(CONFIG_FLAGS) $(CFLAGS) $< -o $@

clean:
	rm -f $(EXEC) *.o *~ qsort.exe

.PHONY: clean
