
#ifndef _XUTIL_H
#define _XUTIL_H


// Returns the top level window that is the 
// ancestor of windowId.
int getAncestorBelowRoot ( int windowId ) ;
					
// Prints the window tree rooted at specified windowId
// It prints the level of each child along with its
// (x, y) location and width X height
void printTree ( int windowId );

void printWindowInfo ( int windowId );

// Returns the root window
int getRoot ( );

void getTopLevel(int ** toplevelIds,
               		  int * numTopLevel);

// Returns the current location of the mouse.
// windowId: window in which the mouse is located
// (x, y): absolute location of the mouse
void getMouseLocation ( int * windowId,
				      int * x,
				      int * y);

// Gets the (x,y) location and width/height of the specified window
void getLocGeom ( int windowId,
			      int *x,
			      int *y,
			      int *width,
			      int *height );

// Grabs the mouse and configures it 
// to report button press and button release 
// events
void grabMouse ( );


// void moveMouse ( int x, int y );


#endif


