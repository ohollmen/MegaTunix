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

#ifndef __SERIALIO_H__
#define __SERIALIO_H__

#include <gtk/gtk.h>
#include <enums.h>

/* Prototypes */
void open_serial(gchar *);
int setup_serial_params(void);
void close_serial(void);
gboolean check_ecu_comms(GtkWidget *, gpointer);
void read_ve_const(void);
void read_raw_memory(gint);
void update_ve_const(void);
void write_ve_const(gint, gint, gboolean);
void burn_flash(void);
void set_ms_page(gint);
/* Prototypes */

#endif
