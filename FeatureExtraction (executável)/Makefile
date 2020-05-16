all: main

CFLAGS = `pkg-config --cflags opencv`
LIBS = `pkg-config --libs opencv`

colorhistogram.o: colorhistogram.cpp colorhistogram.h
	g++ -Wall -g -c $< $(CFLAGS)

edgehistogram.o: edgehistogram.cpp edgehistogram.h
	g++ -Wall -g -c $< $(CFLAGS)
 
tamurafeatures.o: tamurafeatures.cpp tamurafeatures.h
	g++ -Wall -g -c $< $(CFLAGS) 

gaborfeatures.o: gaborfeatures.cpp gaborfeatures.h
	g++ -Wall -g -c $< $(CFLAGS)
	
ExtractFeatures.o: ExtractFeatures.cpp ExtractFeatures.h
	g++ -Wall -g -c $< $(CFLAGS)
	
main: main.cpp ExtractFeatures.o gaborfeatures.o tamurafeatures.o edgehistogram.o colorhistogram.o
	g++ -Wall -g -o $@ $^ $(CFLAGS) $(LIBS)

clean: 
	rm -f *.o main
