/*
 * Main.c
 *
 *  Created on: Jan 30, 2015
 *      Author: root
 */

#include <gui.h>
#include <bz2lib.h>
#include <fno.h>

int main(int argc, char** argv) {

	GtkWidget *lSource;
	GtkWidget *lDest;
	GtkWidget *frame;
	GtkWidget *btnSource;
	GtkWidget *btnOK;
	GtkWidget *btnCancel;

	argWidgets *widgets = malloc(sizeof(argWidgets));

	gtk_init(&argc, &argv);

	widgets->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_position(GTK_WINDOW(widgets->window), GTK_WIN_POS_CENTER);
	gtk_window_set_default_size(GTK_WINDOW(widgets->window), 500, 200);
	gtk_window_set_title(GTK_WINDOW(widgets->window), "bzip archiever");

	frame = gtk_fixed_new();
	gtk_container_add(GTK_CONTAINER(widgets->window), frame);

	btnSource = gtk_button_new_with_label("Browse");
	gtk_widget_set_size_request(btnSource, 80, 35);
	gtk_fixed_put(GTK_FIXED(frame), btnSource, 400, 32);

	widgets->btnDest = gtk_button_new_with_label("Browse");
	gtk_widget_set_size_request(widgets->btnDest, 80, 35);
	gtk_fixed_put(GTK_FIXED(frame), widgets->btnDest, 400, 82);

	btnOK = gtk_button_new_with_label("OK");
	gtk_widget_set_size_request(btnOK, 80, 35);
	gtk_fixed_put(GTK_FIXED(frame), btnOK, 375, 142);

	btnCancel = gtk_button_new_with_label("Cancel");
	gtk_widget_set_size_request(btnCancel, 80, 35);
	gtk_fixed_put(GTK_FIXED(frame), btnCancel, 280, 142);

	lSource = gtk_label_new("Source");
	gtk_fixed_put(GTK_FIXED(frame), lSource, 30, 40);

	lDest = gtk_label_new("Destination");
	gtk_fixed_put(GTK_FIXED(frame), lDest, 30, 90);

	widgets->tfDestination = gtk_entry_new();
	gtk_entry_set_width_chars(GTK_ENTRY(widgets->tfDestination), 34);
	gtk_editable_set_editable(GTK_EDITABLE(widgets->tfDestination), true);
	gtk_entry_set_text(GTK_ENTRY(widgets->tfDestination), "/root/Music/1");
	gtk_fixed_put(GTK_FIXED(frame), widgets->tfDestination, 110, 88);
	gtk_widget_set_sensitive(widgets->tfDestination, false);

	widgets->tfSource = gtk_entry_new();
	gtk_entry_set_width_chars(GTK_ENTRY(widgets->tfSource), 34);
	gtk_editable_set_editable(GTK_EDITABLE(widgets->tfSource), true);
	gtk_entry_set_text(GTK_ENTRY(widgets->tfSource), "/root/Music/bzlib.h");
	gtk_fixed_put(GTK_FIXED(frame), widgets->tfSource, 110, 38);
	gtk_widget_set_sensitive(widgets->tfSource, false);


	widgets->check = gtk_check_button_new_with_label("Same destination");
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widgets->check), false);
	gtk_fixed_put(GTK_FIXED(frame), widgets->check, 110, 117);

	widgets->combo = gtk_combo_box_text_new_with_entry();
	gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(widgets->combo), "Compress");
	gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(widgets->combo), "Decompress");
	gtk_fixed_put(GTK_FIXED(frame), widgets->combo, 80, 148);
	gtk_combo_box_set_active(GTK_COMBO_BOX(widgets->combo),0);

	gtk_widget_show_all(widgets->window);

	g_signal_connect(widgets->window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

	g_signal_connect(btnSource, "clicked", G_CALLBACK(setSourcePath), widgets);

	g_signal_connect(widgets->btnDest, "clicked", G_CALLBACK(setDestPath), widgets);

	g_signal_connect(widgets->check, "toggled", G_CALLBACK(entry_toggle_visibility), widgets);

	g_signal_connect(btnOK, "clicked", G_CALLBACK(compressOrDecompress),
			widgets);

	g_signal_connect(btnCancel,"clicked",G_CALLBACK(gtk_main_quit), NULL);

	gtk_main();
	free(widgets);
	return 0;
}


