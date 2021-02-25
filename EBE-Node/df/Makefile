OBJS = main.o bessel.o gamma.o bessik.o bessjy.o cbessik.o cbessjy.o
CC = c++
DEBUG = -g 
CFLAGS = -Wall -std=c++98 -c $(DEBUG)
LFLAGS = -Wall $(DEBUG)

fo : $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o fo

main.o: SPH.h vector.h int.h tables.h vnvar.h spectra.h bessel.h main.cpp 
	$(CC) $(CFLAGS) main.cpp

bessel.o: bessel.h bessel.cpp 
	$(CC) $(CFLAGS) bessel.cpp
	
gamma.o: gamma.cpp
	$(CC) $(CFLAGS) gamma.cpp

bessik.o: bessf.h bessik.cpp
	$(CC) $(CFLAGS) bessik.cpp

bessjy.o: bessf.h bessjy.cpp
	$(CC) $(CFLAGS) bessjy.cpp

cbessik.o: bessf.h cbessik.cpp
	$(CC) $(CFLAGS) cbessik.cpp

cbessjy.o: bessf.h cbessjy.cpp
	$(CC) $(CFLAGS) cbessjy.cpp


# deletes all .o files in case of error in makefile.  This should not be something needed regularly, only if you are updating the code and run into an error.	
clean:
	\rm -rf *.o *~ fo

# puts all the files in a fo.tar file
#tar:
#    tar cfv fo.tar vector.h main.cpp vector.cpp 
