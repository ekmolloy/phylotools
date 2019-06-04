CC	= G++
CFLAGS	= -std=c++11 -O3 -Wall -funroll-loops -fomit-frame-pointer
LIBS	= -lm

SRCS    = binary_character_matrix.cpp
SRCS    += run_binary_character_matrix_to_newick.cpp
OBJS    = binary_character_matrix.o
OBJS    += run_binary_character_matrix_to_newick.o
HDRS    = binary_character_matrix.hpp

all : run_binary_character_matrix_to_newick

clean:
	rm -f *.o

.o : $(SRCS) $(HDRS)
	$(CC) -c $(INCDIRS) $(CFLAGS) $(OPTFLAG) $(DEFS) $<

run_binary_character_matrix_to_newick : $(OBJS) $(LIBOBJS)
	$(CC) $(LINKFLAGS) -o $@ $(OBJS) $(LIBDIRS) $(LIBS)
