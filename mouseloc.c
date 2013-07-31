#include "xutil.h"
#include <stdio.h>

void main ( int argc, char *argv[] ) {

	int windowId, x, y;
	
	while (1) {
	
		getMouseLocation ( & windowId, &x, &y);
		printf ("Inside window %d at (%d, %d)\n", windowId, x, y);
		sleep ( 5 );
		
	}

}


