# ============================================================================
#  Makefile CFWR                             Chris Plumberg, September 8, 2015
# ============================================================================
##
##  Environments :	MAIN	= 	main sourcefile	
##
##  Usage : 	(g)make	[all]		compile the whole project		
##			install	make all and copy binary to $INSTPATH
##			clean		remove objectfiles in obj_$TYPE 
##			distclean	remove all objectsfiles and binaries
##  

CC := $(shell ../../crank/configure_compiler_C++.sh)
#running with icpc
CFLAGS= $(shell ../../crank/configure_compiler_C++_Flags.sh)
#running with g++
#CFLAGS= -O3 -Wall
#CFLAGS += -lgsl -lgslcblas -g -Wall -Warray-bounds -Wchar-subscripts -Wcomment \
#						-Wenum-compare -Wformat -Wuninitialized -Wmaybe-uninitialized \
#						-Wmain -Wnarrowing -Wnonnull -Wparentheses -Wpointer-sign \
#						-Wreorder -Wreturn-type -Wsign-compare -Wsequence-point -Wtrigraphs \
#						-Wunused-function -Wunused-but-set-variable -Wunused-variable \
#						-Wwrite-strings
CFLAGS += -lgsl -lgslcblas -g
#$(MKL_LIBS_INT64)
#-openmp
# -w3 -wd981
###CFLAGS += -lgsl -lgslcblas -g -profile-functions -profile-loops=all -profile-loops-report=2

RM		=	rm -f
O               =       .o
LDFLAGS         =       $(CFLAGS)
SYSTEMFILES     =       $(SRCGNU)
SRCDIR		=	src

# --------------- Files involved ------------------

ifeq "$(MAIN)" ""
MAIN		=	fit_correlation_function.e
endif

SRC		=	main.cpp \
			Arsenal.cpp \
			ParameterReader.cpp \
			fit.cpp \
			gauss_quadrature.cpp

INC		= 	fit.h \
			Arsenal.h \
			ParameterReader.h \
			gauss_quadrature.h


# -------------------------------------------------

OBJDIR		=	.
SRCFILES 	= 	$(SRC) $(INC) GNUmakefile
OBJECTS		=	$(addprefix $(OBJDIR)/, $(addsuffix $O, \
			$(basename $(SRC))))
TARGET		=	$(MAIN)
INSTPATH	=	..
#INSTPATH	=	$(HOME)/local/bin

# --------------- Pattern rules -------------------

$(OBJDIR)/%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

%.cpp:
	if [ -f $@ ] ; then touch $@ ; else false ; fi

# -------------------------------------------------

.PHONY:		all mkobjdir clean distclean install

all:		mkobjdir $(TARGET)

help:
		@grep '^##' GNUmakefile

mkobjdir:	
		-@mkdir -p $(OBJDIR)

$(TARGET):	$(OBJECTS)	
		$(CC) $(OBJECTS) -o $(TARGET) $(LDFLAGS) 
#		strip $(TARGET)

clean:		
		-rm $(OBJECTS)

distclean:	
		-rm $(TARGET)
		-rm $(OBJECTS)
		-rm $(MAIN)

install:	$(TARGET)
		cp $(TARGET) $(INSTPATH)

# --------------- Dependencies -------------------
gauss_quadrature.cpp: gauss_quadrature.h
Arsenal.cpp: Arsenal.h
ParameterReader.cpp: ParameterReader.h Arsenal.h
fit.cpp: fit.h ParameterReader.h Arsenal.h gauss_quadrature.h

./main.cpp: ParameterReader.h fit.h Arsenal.h gauss_quadrature.h

