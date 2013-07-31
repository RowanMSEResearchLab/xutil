
#include "xutil.h"
#include <glib.h>
#include <xcb/xcb.h>
#include <xcb/xproto.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <X11/cursorfont.h>

struct _entry {
	int wid;
	int level;
};

typedef struct _entry * entry;

xcb_connection_t * display = (xcb_connection_t *) NULL;
int screen_number = 0;
xcb_setup_t *setup;
xcb_screen_iterator_t screen_iter;
int theRoot;

entry create ( int wid, int level ) {
	entry e;
	e = (entry) malloc(sizeof(struct _entry));
	// e = malloc (sizeof (*entry));
	e->wid = wid;
	e->level = level;
	return e;
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

void initialize ( ) {
	init_xcb ( );
}


void pushChildren ( int wid, int level, GQueue * queue ) {
	
	if (display == NULL) 
		init_xcb ( );
	
    	xcb_query_tree_cookie_t qtcookie = xcb_query_tree (display, wid );
	xcb_query_tree_reply_t * reply = xcb_query_tree_reply(display, qtcookie, NULL);
	
	
	xcb_window_t * children = xcb_query_tree_children(reply);
	int i;
	// printf ("Children of %d\n", wid );
	for (i = 0; i < reply->children_len; i++) {
		// printGeometry(children[i], dpy);
		g_queue_push_tail ( queue, create ( children[i], level));
		
	}
	// printf ("\n");
	
	
}


int getAncestorBelowRoot ( int wid ) {
	if (display == (xcb_connection_t *) 0)
		init_xcb();
	if (wid == theRoot) // wid needs to be a window below root
		return -1;
	else {
		xcb_query_tree_cookie_t qtcookie;
		xcb_query_tree_reply_t *reply;
		xcb_window_t *children;
		int i;
		
		qtcookie = xcb_query_tree ( display, wid );
		reply = xcb_query_tree_reply ( display, qtcookie, NULL);
		if (!reply) {
			printf ("Invalid window id : %d\n", wid );
			return -1;
		}
		
		if (reply->parent == theRoot)
			return wid;
		else
			return getAncestorBelowRoot ( reply->parent);
	}
	
	
	
}

void printWindowInfo (int wid ) {
	
	xcb_get_geometry_cookie_t  geomCookie = 
	xcb_get_geometry (display, wid);  
	
	xcb_get_geometry_reply_t  *geom = 
	xcb_get_geometry_reply (display, geomCookie, NULL);
	printf ("%d [(%d , %d) %d x %d] ", wid,
		geom->x, geom->y, geom->width, geom->height);
}

void printTree ( int wid ) {
	
	
	
	GQueue * queue = g_queue_new( );
	int curlevel = 0;
	
	entry temp;
	
	
	g_queue_push_tail ( queue, create(wid,1));
	
	
	
	while (g_queue_is_empty(queue) == FALSE) {
		temp = g_queue_pop_head ( queue );
		
		if (temp->level > curlevel) {
			curlevel = temp->level;
			printf ("\n");
			printf ("Level %d --> ", temp->level);
		}
		
		printWindowInfo (temp->wid);
		pushChildren ( temp->wid, temp->level + 1, queue );
	}
	printf ("\n");
	
	
	
	
	
	
}

int getRoot ( ) {
	if (display == NULL)
		init_xcb ( );
	return theRoot;
}


void getTopLevel(int ** toplevel, int * numTopLevel)
{
	if (display == NULL)
		init_xcb ( );
	
	int i;
	xcb_window_t *child;
	xcb_query_tree_reply_t *reply;
	xcb_query_tree_cookie_t qtcookie;
	xcb_connection_t * dpy;
	xcb_setup_t * setup;
	char *displayname = NULL;
	
	
	
	dpy = display;
	
	
	qtcookie = xcb_query_tree (dpy, theRoot );
	reply = xcb_query_tree_reply(dpy, qtcookie, NULL);
	// if (!reply)
	// goto done;
	
	child = xcb_query_tree_children(reply);
	*numTopLevel = reply->children_len;
	// printf ("Number of toplevel windows: %d\n", *numTopLevel);
	*toplevel =  malloc ((*numTopLevel )* sizeof(int));
	for (i = 0; i < reply->children_len; i++) {
		
		xcb_query_tree_cookie_t qtcookie = 
		xcb_query_tree ( dpy, child[i]);
		
		xcb_query_tree_reply_t * qtreply =
		xcb_query_tree_reply ( dpy, qtcookie, NULL);
		
		if (!qtreply)
		{
			printf ("GB: Error, unable to query tree for a child %d\n",
				child[i]);
			exit ( 0 );
		}
		
		
		(*toplevel)[i] = child[i];
		
		// topgeom[i] = geom;
	}
	// return toplevel;
	
	
}


void getMouseLocation ( int * windowId,
	int * x,
	int * y)
{
	if (display == NULL)
		init_xcb ( );
	
	xcb_query_pointer_cookie_t qpcookie;
	xcb_query_pointer_reply_t * qpreply;
	
	qpcookie = xcb_query_pointer ( display, theRoot );
	qpreply = xcb_query_pointer_reply ( display, qpcookie, NULL);
	
	*windowId = qpreply->child;
	*x = qpreply->win_x;
	*y = qpreply->win_y;
	
	// printf ("%d : (%d, %d)\n", *windowId, *x, *y );
	// printf ("%d, %d\n", qpreply->root_x, qpreply->root_y);
	
	
	
}

void getLocGeom ( int windowId,
	int *x,
	int *y,
	int *width,
	int *height )
{
	
	if (display == NULL)
		init_xcb();
	xcb_get_geometry_cookie_t  geomCookie = 
	xcb_get_geometry (display, windowId);  
	
	xcb_get_geometry_reply_t  *geom = 
	xcb_get_geometry_reply (display, geomCookie, NULL);
	
	
	// printf ("%d->%d, nc= %d : %dx%d\n", child[i], qtreply->parent, 
	//	qtreply->children_len, geom->width, geom->height);
	
	*x = geom->x;
	*y = geom->y;
	*width = geom->width;
	*height = geom->height;
	
	free (geom);
	
	
	
}


xcb_cursor_t
Create_Font_Cursor (xcb_connection_t *dpy, uint16_t glyph)
{
	static xcb_font_t cursor_font;
	xcb_cursor_t cursor;
	
	if (!cursor_font) {
		cursor_font = xcb_generate_id (dpy);
		xcb_open_font (dpy, cursor_font, strlen ("cursor"), "cursor");
	}
	
	cursor = xcb_generate_id (dpy);
	xcb_create_glyph_cursor (dpy, cursor, cursor_font, cursor_font,
		glyph, glyph + 1,
		0, 0, 0, 0xffff, 0xffff, 0xffff);  /* rgb, rgb */
	
	return cursor;
}

void Fatal_Error ( char * message ) {
	printf ("%s\n", message);
	exit ( 0 );
}

void grabMouse ( ) {
	
	if (display == NULL)
		init_xcb ( );
	xcb_cursor_t cursor;
	cursor = Create_Font_Cursor (display, XC_crosshair);
	xcb_grab_pointer_cookie_t gpcookie;
	
	gpcookie = xcb_grab_pointer(
		display,
		0,
		theRoot,       
		XCB_EVENT_MASK_BUTTON_PRESS | 
		XCB_EVENT_MASK_BUTTON_RELEASE,
		XCB_GRAB_MODE_ASYNC, 
		XCB_GRAB_MODE_ASYNC,
		theRoot, 
		cursor, 
		XCB_TIME_CURRENT_TIME);
	
	xcb_grab_pointer_reply_t * gpreply;
	xcb_generic_error_t *err;
	
	gpreply = xcb_grab_pointer_reply (display, gpcookie, &err);
	if (gpreply->status != XCB_GRAB_STATUS_SUCCESS)
		Fatal_Error ("Can't grab the mouse.");
}

/*
void moveMouse (int x, int y) {
	
	
	
}

*/
