/*
 * Copyright (C) 2003 by Dave J. Andruczyk <djandruczyk at yahoo dot com>
 *
 * Linux Megasquirt tuning software
 * 
 * 
 * This software comes under the GPL (GNU Public License)
 * You may freely copy,distribute, etc. this as long as all the source code
 * is made available for FREE.
 * 
 * No warranty is made or implied. You use this program at your own risk.
 */

#ifndef __LOGVIEWER_GUI_H__
#define __LOGVIEWER_GUI_H__

#include <gtk/gtk.h>

/* Prototypes */
void present_viewer_choices(void);
void scroll_logviewer_traces(void);
void reset_logviewer_state(void);
gboolean deregister_lv_buttons(gpointer);
gboolean lv_configure_event(GtkWidget *, GdkEventConfigure *, gpointer);
gboolean lv_expose_event(GtkWidget *, GdkEventExpose *, gpointer);
gboolean view_value_set(GtkWidget *, gpointer );
gboolean set_lview_choices_state(GtkWidget *, gpointer);
struct Viewable_Value * build_v_value(GtkWidget *, GObject * );
GdkGC * initialize_gc(GdkDrawable *, GcType );
GdkColor get_colors_from_hue(gfloat);
void draw_graticule(void * );
void draw_infotext(void * );
void draw_old_data(void * );
gboolean update_logview_traces(void);
void trace_update(gpointer, gpointer, gpointer );
gboolean logviewer_log_position_change(GtkWidget *, gpointer);
void set_realtime_mode(void);
void set_playback_mode(void);
void finish_logviewer(void);

/* Prototypes */

#endif
