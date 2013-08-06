#include <stdio.h>
#include <xcb/xcb.h>
#include <xcb/xcb_keysyms.h>
#include <xcb/xtest.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>


/* xcb_test_fake_input(type,detail,time,window,x,y,device) */
static void fake_input(xcb_connection_t *c, uint8_t type, uint8_t detail, int x, int y)
{
  xcb_window_t none = { XCB_NONE };

  xcb_test_fake_input( c, type, detail, 0, none, x, y, 0 );
}

void mouse_click( xcb_connection_t *c, int button, int x, int y ) {
  dmsg( 1, "Clicking mouse button %d\n", button );
  fake_input( c, XCB_BUTTON_PRESS, button, x, y );
  fake_input( c, XCB_BUTTON_RELEASE, button, x, y );
}

void init_xcb ( ) {

        display = xcb_connect ( NULL, &screen_number);
        if (xcb_connection_has_error(display)) {
                fprintf (stderr, "Unable to open display\n");

                exit (1);
        }
        setup = xcb_get_setup ( display );
        screen_iter = xcb_setup_roots_iterator(setup);

        int i;
        for (i = 0; i < screen_number; i++)
                xcb_screen_next(&screen_iter);
        theRoot = screen_iter.data->root;
}

void main (int argc, char *argv[]) 
{
	init_xcb( );
  	xcb_connection_t *c = NULL;
  	char *buf, *display = NULL;
	
	display = argv[1];
	if ( display == NULL) {
    		fprintf( stderr, "Unable to open display '%s'\n", display == NULL ? "default" : display );
		exit(1);
	}

	c = xcb_connect( display, NULL );
  	if( c == NULL ) {
    		fprintf( stderr, "Unable to open display '%s'\n", display == NULL ? "default" : display );
    		exit(1);
  	}

	while (1)
	{
		printf("Please enter the x coord for the click.");
		int x = getchar();
		printf("Please enter the y coord for the click.");
		int y = getchar();
		printf("Coordinates received. Clicking @ %d, %d", x, y);
		//execute click buddy
		//fake_input(c, XCB_BUTTON_PRESS, DETAIL, 0, none, x, y, 0);
		mouse_click(c, 1, x, y);
	}
}
