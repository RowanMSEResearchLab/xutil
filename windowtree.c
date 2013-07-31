

#include "xutil.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

void usage ( char * cmd) {
	printf ("Usage: %s [-top] windowId\n", cmd );
	exit ( 0 );
}


void main ( int argc, char *argv[] ) {
	
	int topwid, i;
	int top;
	
	top = 0;
	topwid = -1;
	
	initialize ( );
	for (i = 1; i < argc; i++) {
		if (strcmp (argv[i], "-top") == 0)
			top = 1;
		else
			topwid = atoi (argv[i]);
	}
	
	if (topwid == -1) 
		usage (argv[0]);
			
	if (top) {
		printf ("Level 0 (root)--> %d\n", getRoot());
		topwid = getAncestorBelowRoot ( topwid);
	}
	
	printTree ( topwid );
	
	exit ( 0 );	
}

