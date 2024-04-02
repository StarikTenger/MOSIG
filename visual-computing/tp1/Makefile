#######################################
# Makefile PBM                        #
#                                     #
# E.B.                                #
#######################################


PROG = component_extractor

all : $(PROG)

# Variables for file compilation
CC        =  gcc
CFLAGS    =  -g -Wall
CPPFLAGS  =  -DDEBUG
LDFLAGS   =  -g -lm

#pxmtopxm : pxmtopxm.o Util.o

component_extractor : component_extractor.o Util.o

clean :
	@rm -f *.o

cleanall : clean
	@rm -f $(PROG)

