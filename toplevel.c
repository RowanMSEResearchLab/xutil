#include "xutil.h"

int main (int argc, char *argv[])
{
	int i;
	// numTopLevel = 0;
	
	int * topw;
	int numWin;
	
	getTopLevel (&topw, &numWin );
	for (i = 0; i < numWin; i++) {
		printWindowInfo (topw[i]);
		printf("\n");
	}
	exit(0);
}
