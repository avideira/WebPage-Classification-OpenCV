CFLAGS = `pkg-config --cflags opencv`
LIBS = `pkg-config --libs opencv`

% : %.c
	g++ $(CFLAGS) $(LIBS) -o $@ $<
