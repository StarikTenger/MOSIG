# This makefile introduces some of the main techniques to define rules
# for C programm compilation. Take some time to have a look at it.

# To compile a single program, you can execute 'make my_prog.run'
# To compile all the programs at once, run 'make all'

CC=gcc
CFLAGS=-Wall -Wextra -Wpointer-arith -pedantic -g -std=c99
LDFLAGS=


EXEC=ex1.run ex2.run rec.run prog_0.run prog_1.run prog_2.run prog_3.run prog_4.run pointer_arithmetic.run

# In the case where each .c file defines a separate program, we could
# have also defined the EXEC variable in the following way using the
# wildcard function and suffix substitution:
# SRC=$(wildcard *.c)
# EXEC=$(SRC:.c=.run)


# make all should generate all executables
all: $(EXEC)

# we start with very basic rules to compile ex1.run. 
ex1.run: ex1.o
	$(CC) -o ex1.run ex1.o $(LDFLAGS)

ex1.o: ex1.c
	$(CC) -c ex1.c $(CFLAGS)


# Note that it is possible to define multiple dependencies for one
# target, as it is done for the target "all". This is useful for
# instance when an executable file must be built from several object
# files.

# makefile defines automatic variables that helps simplifying the way
# rules are written
# for more details https://www.gnu.org/software/make/manual/make.html#Automatic-Variables

# example with ex2.run
ex2.run: ex2.o
	$(CC) -o $@ $< $(LDFLAGS)

ex2.o: ex2.c
	$(CC) -c $< $(CFLAGS) 


#fortunately we do not have to write new rules for each new
#executable we want to generate. To define generic rules we can define
#static patterns.
# for more details: https://www.gnu.org/software/make/manual/make.html#Static-Pattern
%.run: %.o
	$(CC) -o $@ $< $(LDFLAGS)

%.o: %.c
	$(CC) -c $< $(CFLAGS)


#finally we add a rule to do some cleaning using a phony target: https://www.gnu.org/software/make/manual/html_node/Phony-Targets.html
.PHONY: clean
clean:
	rm -rf *.o *.run
