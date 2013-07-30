CC = gcc
PKGS = xcb glib-2.0
CFLAGS = -g `pkg-config --cflags $(PKGS)`
LDFLAGS = `pkg-config --libs $(PKGS)`

all: windowtree toplevel mouseloc


windowtree: xutil.o windowtree.o
	$(CC) -g -o $@ xutil.o windowtree.o $(LDFLAGS)

xutil.o: xutil.h xutil.c

windowtree.o: xutil.h windowtree.c

toplevel: xutil.o toplevel.o
	$(CC) -g -o $@ xutil.o toplevel.o $(LDFLAGS)

mouseloc: xutil.o mouseloc.o
	$(CC) -g -o $@ xutil.o mouseloc.o $(LDFLAGS)

mouseloc.o: xutil.h mouseloc.c

clean:
	rm -f *.o windowtree
