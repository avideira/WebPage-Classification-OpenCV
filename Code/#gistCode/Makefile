# Lear's GIST implementation, version 1.1, (c) INRIA 2009, Licence: PSFL


all: compute_gist

WFFTINC=
WFFTLIB=
# WFFTINC=-I/scratch/adonis/mordelet/install/include
# WFFTLIB=-L/scratch/adonis/mordelet/install/lib

CFLAGS = `pkg-config --cflags opencv`
LIBS = `pkg-config --libs opencv`
# gist.c: ../../descriptors/gist.c 
#	cp $< $@


gist.o: gist.c gist.h standalone_image.h
	gcc -c -Wall -g $< $(WFFTINC) $(CFLAGS) $(LIBS) -DUSE_GIST -DSTANDALONE_GIST

standalone_image.o: standalone_image.c standalone_image.h
	gcc -c -Wall -g $< 

compute_gist: compute_gist.cpp lbp.cpp gist.o standalone_image.o
	g++ -Wall -g -o $@ $^ $(WFFTLIB) $(CFLAGS) $(LIBS) -lfftw3f



clean: 
	rm -f *.o compute_gist

