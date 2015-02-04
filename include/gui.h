/*
 * hello.h
 *
 *  Created on: Jan 30, 2015
 *      Author: root
 */

#ifndef INCLUDE_HELLO_H_
#define INCLUDE_HELLO_H_

#include "gtk/gtk.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "glib-2.0/glib.h"
#include <string.h>
#include "fno.h"
#include "bz2lib.h"

#define INVALIED	0
#define COMPRESS	1
#define DECOMPRESS	2


typedef struct
{
	GtkWidget *tfDestination;
	GtkWidget *tfSource;
	GtkWidget *combo;
	gpointer window;
	GtkWidget *btnDest;
	GtkWidget *check;
}argWidgets;


void setSourcePath(GtkWidget*, argWidgets*);
void setDestPath(GtkWidget*, argWidgets*);
void entry_toggle_visibility(GtkWidget*, argWidgets*);
void compressOrDecompress(GtkWindow*, argWidgets*);


#endif /* INCLUDE_HELLO_H_ */
