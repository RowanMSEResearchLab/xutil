
#ifndef _XUTIL_H
#define _XUTIL_H


// Returns the top level window that is the 
// ancestor of windowId.
int getAncestorBelowRoot ( int windowId ) ;
					
// Prints the window tree rooted at specified windowId
// It prints the level of each child along with its
// (x, y) location and width X height
void printTree ( int windowId );

// Returns the root window
int getRoot ( );






#endif


