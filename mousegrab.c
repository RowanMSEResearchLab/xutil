#include "xutil.h"
#include <stdio.h>
#include <xcb/xcb.h>

void main ( int argc, char *argv[] ) {
	
	// use of extern decl: Links to "display" variable defined in xutil.c
	extern xcb_connection_t * display;  
	
	
	int windowId, lx, ly;
	
	grabMouse( ); // Initiate an active mouse grab

	xcb_generic_event_t * event;
	
	// Event loop
	// Wait for button event. Print mouse position on button release and print ouch on button pres
	
	while (event = xcb_wait_for_event (display)) {
		switch (event->response_type & ~0x80) {
			
		case XCB_BUTTON_PRESS:
			printf ("Ouch\n");  // Illustration of different processing for different events
			break;
			
		case XCB_BUTTON_RELEASE: 
			
			getMouseLocation (&windowId, &lx, &ly);
			printf ("Inside window %d at (%d, %d)\n", windowId, lx, ly);
			break;
			
		default:
			break;
		}
	}
	
}


