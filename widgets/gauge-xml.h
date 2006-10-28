/*
 * Copyright (C) 2006 by Dave J. Andruczyk <djandruczyk at yahoo dot com>
 * 
 * Megasquirt gauge widget XML I/O header
 * 
 * 
 * This software comes under the GPL (GNU Public License)
 * You may freely copy,distribute etc. this as long as the source code
 * is made available for FREE.
 * 
 * No warranty is made or implied. You use this program at your own risk.
 * This is a the PRIVATE implementation header file for INTERNAL functions
 * of the widget.  Public functions as well the the gauge structure are 
 * defined in the gauge.h header file
 *
 */

#ifndef __GAUGE_XML_H__
#define  __GAUGE_XML_H__

#include <gauge.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

/* Prototypes */
void testload();
void output_xml(MtxGaugeFace * );


#endif
