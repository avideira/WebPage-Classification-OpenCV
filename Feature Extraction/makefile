CFLAGS  = `pkg-config --cflags opencv`
LIBS 	= `pkg-config --libs opencv`

colorhistogram : colorhistogram.c
	gcc -g $< -o $@ $(CFLAGS) $(LIBS) 
	
gaborfeatures : gaborfeatures.cpp
	g++ $(CFLAGS) $(LIBS) -o $@ $<

edgehistogram : edgehistogram.c edgehistogram.h
	gcc $(CFLAGS) $(LIBS) -o $@ $<

tamurafeatures : tamurafeatures.c tamurafeatures.h
	gcc $(CFLAGS) $(LIBS) -o $@ $<
