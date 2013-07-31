#include "xutil.h"
#include <stdlib.h>
#include <stdio.h>

void main (int argc, char *argv[])
{
	int i;
	// numTopLevel = 0;
	
	int * topw;
	int numWin;
	
	initialize ( );
	getTopLevel (&topw, &numWin );
	for (i = 0; i < numWin; i++) {
		printWindowInfo (topw[i]);
		printf("\n");
	}
	exit(0);
}
