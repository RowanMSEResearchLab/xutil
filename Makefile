
all: windowtree toplevel mouseloc


windowtree: xutil.o windowtree.o
	gcc -g -o $@ xutil.o windowtree.o -lxcb -lglib-2.0

xutil.o: xutil.h xutil.c
	gcc -c -g `pkg-config --cflags glib-2.0` xutil.c 

windowtree.o: xutil.h windowtree.c
	gcc -c -g `pkg-config --cflags glib-2.0` windowtree.c 

toplevel: xutil.o toplevel.o
	gcc -g -o $@ xutil.o toplevel.o -lxcb -lglib-2.0

mouseloc: xutil.o mouseloc.o
	gcc -g -o $@ xutil.o mouseloc.o -lxcb -lglib-2.0

mouseloc.o: xutil.h mouseloc.c
	gcc -c -g `pkg-config --cflags glib-2.0` mouseloc.c 



clean:
	rm -f *.o windowtree
