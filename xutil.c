
#include "xutil.h"
#include <glib.h>
#include <xcb/xcb.h>
#include <xcb/xproto.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

struct _entry {
	int wid;
	int level;
};

typedef struct _entry * entry;

entry create ( int wid, int level ) {
	entry e;
	e = (entry) malloc(sizeof(struct _entry));
	// e = malloc (sizeof (*entry));
	e->wid = wid;
	e->level = level;
	return e;
}


xcb_connection_t * display = (xcb_connection_t *) NULL;
int screen_number = 0;
xcb_setup_t *setup;
xcb_screen_iterator_t screen_iter;
int theRoot;

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
	xcb_connect(displayname, &screen_number);
	
	
	qtcookie = xcb_query_tree (dpy, theRoot );
	reply = xcb_query_tree_reply(dpy, qtcookie, NULL);
	// if (!reply)
	// goto done;
	
	child = xcb_query_tree_children(reply);
	*numTopLevel = reply->children_len;
	printf ("Number of toplevel windows: %d\n", *numTopLevel);
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
		
		/*
		xcb_get_geometry_cookie_t  geomCookie = 
		xcb_get_geometry (dpy, child[i]);  
		
		xcb_get_geometry_reply_t  *geom = 
		xcb_get_geometry_reply (dpy, geomCookie, NULL);
		
		
		// printf ("%d->%d, nc= %d : %dx%d\n", child[i], qtreply->parent, 
		//	qtreply->children_len, geom->width, geom->height);
		
		free (geom);
		*/
		(*toplevel)[i] = child[i];
		
		// topgeom[i] = geom;
	}
	// return toplevel;
	
	
}


