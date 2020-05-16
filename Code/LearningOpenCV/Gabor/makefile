CFLAGS = -g -Wall `pkg-config --cflags opencv`

LDFLAGS = `pkg-config --libs opencv `

gfilter : gfilter.c
	g++ $(CFLAGS) $(LIBS) -o  $@ $<

clean:
		rm -f *~ *.o $(TARG)