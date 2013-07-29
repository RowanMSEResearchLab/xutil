
windowtree: xutil.o windowtree.o
	gcc -g -o $@ xutil.o windowtree.o -lxcb -lglib-2.0

xutil.o: xutil.h xutil.c
	gcc -c -g `pkg-config --cflags glib-2.0` xutil.c 

windowtree.o: xutil.h windowtree.c
	gcc -c -g `pkg-config --cflags glib-2.0` windowtree.c 

clean:
	rm -f *.o windowtree
