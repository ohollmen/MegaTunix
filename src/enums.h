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

#ifndef __ENUMS_H__
#define __ENUMS_H__

/* the core_gui.c uses these to build the main gui and branch out to build
 * each of the seperate Gui frames 
 */
typedef enum
{
	ABOUT_PAGE,
	GENERAL_PAGE,
	COMMS_PAGE,
	CONSTANTS_PAGE,
	RUNTIME_PAGE,
	ENRICHMENTS_PAGE,
	VETABLES_PAGE,
	TUNING_PAGE,
	TOOLS_PAGE,
	DATALOGGING_PAGE,
	LOWLEVEL_PAGE,
	IGNITION_PAGE
}GuiFramePage;

/* Serial data comes in and handled by the following possibilities.
 * dataio.c uses these to determine which course of action to take...
 */
typedef enum
{
	REALTIME_VARS,
	VE_AND_CONSTANTS,
	IGNITION_VARS,
	RAW_MEMORY
}InputData;

/* Regular Buttons */
typedef enum
{
	START_REALTIME,
	STOP_REALTIME,
	REQD_FUEL_POPUP,
	READ_FROM_MS,
	WRITE_TO_MS,
	SELECT_LOGFILE,
	TRUNCATE_LOGFILE,
	CLOSE_LOGFILE,
	START_DATALOGGING,
	STOP_DATALOGGING,
}StdButton;

/* Toggle/Radio buttons */
typedef enum
{
	TOOLTIPS_STATE,
        FAHRENHEIT,
        CELSIUS,
	CLASSIC_LOG,
	CUSTOM_LOG,
	COMMA,
	TAB,
	SPACE
}ToggleButton;

/* spinbuttons... */
typedef enum
{
	REQ_FUEL_DISP,
	REQ_FUEL_CYLS,
	REQ_FUEL_INJ_RATE,
	REQ_FUEL_AFR,
	REQ_FUEL,
	SER_POLL_TIMEO,
	SER_INTERVAL_DELAY,
	SET_SER_PORT,
	NUM_SQUIRTS,
	NUM_CYLINDERS,
	NUM_INJECTORS,
	GENERIC
}SpinButton;

/* Conversions for download, converse on upload.. */
typedef enum
{
	ADD,
	SUB,
	MULT,
	DIV
}Conversions;

/* Runtime Status flags */
typedef enum 
{       
	CONNECTED, 
        CRANKING, 
        RUNNING, 
        WARMUP, 
        AS_ENRICH, 
        ACCEL, 
        DECEL
}RuntimeStatus;




#endif
