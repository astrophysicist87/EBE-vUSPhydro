OBJS = main.o vector.o enteric.o hydrosim.o newspline.o random.o eos.o matrix.o
CC = g++
DEBUG = -g -std=c++11
CFLAGS = -Wall -c $(DEBUG)
LFLAGS = -Wall $(DEBUG)

vusphydro : $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o vusphydro

main.o: vector.h mathdef.h particle.h eos.h enteric.h LinkList.h random.h matrix.h hydrosim.h eostables.h main.cpp 
	$(CC) $(CFLAGS) main.cpp

vector.o: vector.cpp vector.h mathdef.h
	$(CC) $(CFLAGS) vector.cpp
matrix.o: matrix.cpp matrix.h vector.h mathdef.h
	$(CC) $(CFLAGS) matrix.cpp
hydrosim.o: vector.h mathdef.h particle.h hydrosim.h LinkList.h rungekutta4.h eos.h output.h matrix.h hydrosim.cpp
	$(CC) $(CFLAGS) hydrosim.cpp
enteric.o:  vector.h particle.h eos.h tables.h enteric.h LinkList.h matrix.h eostables.h enteric.cpp 
	$(CC) $(CFLAGS) enteric.cpp
spline.o: newspline.cpp newspline.hpp
	$(CC) $(CFLAGS) newspline.cpp
eos.o: tables.h eos.h eostables.h eos.cpp
	$(CC) $(CFLAGS) eos.cpp
random.o: random.cpp random.h
	$(CC) $(CFLAGS) random.cpp

# deletes all .o files in case of error in makefile.  This should not be something needed regularly, only if you are updating the code and run into an error.	
clean:
	\rm -rf *.o *~ vusphydro

# puts all the files in a vusphydro.tar file
#tar:
#    tar cfv vusphydro.tar vector.h main.cpp vector.cpp 
