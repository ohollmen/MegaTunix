# MegaTunix subsystems

## Serial Communications

### Low level comms

Declared and implemented by src/serialio.h and src/serialio.c (500+ lines).
Linux/Unix implementations depends/relies on POSIX termios (termios.h) serial communications interface.
Ports are named by Windows conventions (COM1,COM2 ... as opposed to UNIX Device file ????). Note: See config in ~/mtx/default/config Section [Serial] (keys: potential_ports, override_port)
- Serial API interfaces: close_serial, flush_serial, lock_serial, open_serial,
setup_serial_params, unlock_serial (Also win32_* versions get compiled in)
- Note missing read/write interfaces - All red/write access later happens via serial_params->fd file descriptor using read_wrapper() / write_wrapper() (implemented by high level dataio.c, see below)
- Open Port handle (file descriptor)  is stored in serial_params->fd.
- serial_params is of type Serial_Params and it is declared in serialio.h
- The instance of Serial_Params - serial_params - gets stored in global_data, gets set in main.c
- Serial stuff is NOT built separately as shared lib (.so), but linked statically to main binary.

### High level comms/IO

Implemented by src/dataio.c - ~500 lines (no dataio.h !?), with interfaces read_data(), write_data() (Helper hex-dumper dump_output()) read_wrapper(), write_wrapper(). These iterfaces utilize guint8 buffers and type Io_Message (from include/threads.h) ... write_wrapper_f()
Message formation:
src/plugins/mscommon/mscommon_comms.c (mscommon_comms.h)
- build_output_message() for sending message to ECU.
- comms_test() - Ping ECU (by asking time packet, "c")
- read_data_f()
- signal_read_rtvars() - Request (chunk of) realtime variables ("A", given in firmware->rt_command)
- io_cmd_f
- Data writing (irrelevant for read/only monitoring, instrument app)
  - ms_table_write() - Write a (setttings/lookup/...) data table to ECU
  - chunk_write() - send a block of data to ECU.
  - send_to_ecu() Extract data from GTK Widget and send to ECU (!)
Hi level comms (dataio.c) is also compiled into main binary.

TODO: Allow compiling a new shared library libmtxecuio.so (No coupling to UI, should be also applicable to CL apps). Other possible names (pick your favorite): libmtxcommio.so libmtxsercommio.so libmtxsercomm.so (references to some of following would be nice comm=Communications, io=Input/Output,
ser=Serial, ecu=Engine Control Unit)

### Operating comms and polling in general
All Polling takes place by g_timeout_add and g_timeout_add_full (~66 in codebase), e.g. autolog_dump,run_function,update_ve3d_wrapper, delayed_expose_wrapper,sleep_and_redraw_wrapper,pb_update_logview_traces_wrapper,update_errcounts_wrapper,hide_dash_resizers_wrapper,run_function,flush_binary_logs,reset_infolabel_wrapper,jimstim_rpm_sweep_wrapper,benchtest_clock_update_wrapper,queue_function_f,table_color_refresh_wrapper_f,update_multi_expression_wrapper,force_update_table_wrapper,force_view_recompute_wrapper,trigger_group_update_wrapper,
signal_toothtrig_read,update_gauge_wrapper,update_stripchart_wrapper,auto_rescale_wrapper,cancel_peak_wrapper,update_curve_marker_wrapper,mtx_progress_bar_peak_reset_wrapper,update_pbar_wrapper,auto_rescale_wrapper
The timer ID:s are usually stored in priv variables
Interesing ones for Serial interaction: update_gauge_wrapper
Command to send to read realtime variables is configured by ecu firmware type (Last_ECU_Family=... in mtx/default/config ?) firmware->rt_command (e.g. ...). Other commands: firmware->burn_command
(See include/firmware.h)
- Baud rate comes from Interrogator/Profiles/$ECUNAME/details.cfg
- Other firmware communication details: Interrogator/Profiles/$ECUNAME/$FIRMWARENAME.prof
- Example RT Vars command: ms2_std_rtvars (Also: ms2_read_all) See: RT_total_bytes=112
- Mapping/Documentation of commands: /home/ohollmen/MegaTunix/Interrogator/Profiles/MS2/comm.xml
- Read realtime variables: ms2_std_rtvars => "a",  ms1_std_rtvars => "A", RT_total_bytes=112 Bytes expected

See:
- io_cmd_f - callback function holder (Also io_cmd ../src/threads.c)
- rtv_data of type Rtv_Data - A structure of ~50 members about firmware (e.g. FW config file names, unit scaling factors ...)

## Configuration
- Some of this seems to get stored in variable global_data as k-v pairs (value can be of any type and must be correctly casted when used).
- Any data (of misc types) from this k-v map/table is set and gotten by Following:
  - DATA_SET(global_data,"mykey",(gpointer)mynamedthing); - as seen everything is a gpointer
    - g_dataset_set_data(dataset, name, data)
  - DATA_GET(global_data,"mykey"); and casting to type known for the key "mykey"
    - g_dataset_get_data(dataset,name)
    - e.g. Firmware_Details * firmware = (Firmware_Details *)DATA_GET(global_data,"firmware");
- About 77 vars are stored here (even before loading firmware, gotten from console messages)
- gconstpointer *global_data is constructed by global_data = g_new0(gconstpointer, 1);
# Build process and dependencies

- Majority of main GUI sources and binaries in src/
- Codebase has a wrapper shell script src/megatunix, but final installed executable is binary (/usr/local/bin/megatunix)

Megatunix codebase implemented dependencies (IN USE, analyzed by ldd /usr/local/bin/megatunix, prefixed by libmtx*)
- libmtxcommon.so.0 => /usr/local/lib/libmtxcommon.so.0 (0xb6ba5000)
  - Configuration stuff, cfg_*, generic_xml_* functions (INI ? XML ?)
- libmtxprogress.so.0 => /usr/local/lib/libmtxprogress.so.0 (0xb6ba0000)
  - mtx_progress_bar_*
- libmtxstripchart.so.0 => /usr/local/lib/libmtxstripchart.so.0 (0xb6b98000)
  - mtx_stripchart_*
- libmtxgauge.so.0 => /usr/local/lib/libmtxgauge.so.0 (0xb6b82000)
  - calc_bounding_box(), generate_gauge_background(), mtx_gauge_*, mtx_gauge_face_*
- libmtxcombo_mask.so.0 => /usr/local/lib/libmtxcombo_mask.so.0 (0xb6b7e000)
  - mask_entry_*
- libmtxyamlcpp.so.0 => /usr/local/lib/libmtxyamlcpp.so.0 (0xb6b0b000)
  - Mangled (C++) symbols for YAML stuff (YAML appears in mangled names)

Left-over shared lib files not used by main executable (ls -al /usr/local/lib/ | grep libmtx)
- libmtxcurve.so -> libmtxcurve.so.0.0.0
  - mtx_curve_*()
- libmtxpie.so -> libmtxpie.so.0.0.0
  - mtx_pie_gauge_*()

## Gauge utilities

Codebase: dashdesigner/dashdesigner and gaugedesigner/gaugedesigner (TODO:mtxloader)
Both Installed / available in /usr/local/bin/ after make install.
dashdesigner allows also a very nice browsing of gauges via scrollable list.

### Gauges
- Example gauges in subdirs of /usr/local/share/MegaTunix/Gauges/*/*.xml
- Users own Private gauges can be stored in ~/mtx/default/Gauges
- Properties of gauge (From XML files)
  - Colors: ..._day/..._nite for bg,needle, value font, gradients
  - Geometry: for needle, gauge width/height, start and sweep angles
  - Fonts for: values, text blocks
- Typical Gauge XML LOC (lines of code): 180-250 lines (max 375)
### Dashboards
- Example dashboards directly in /usr/local/share/MegaTunix/Dashboards/*.xml
- User gauges ~/mtx/default/Dashboards
- Properties of dashboard:
  - Dash geometry: width / height
  - Collection of gauges with
    - (redundant) width/height (gauge originally has its own width / height)
    - location by x_offset,y_offset within dashboard
    - gauge XML filename (relative path with subdir and xml filename)
    - datasource property name (within firmware type), e.g. "afr1", "dcycle1", "tpsaccel" ...
  
### Errors
DASH PROBLEM, gauge filename is UNDEFINED for a gauge!
** (dashdesigner:32212): CRITICAL **: mtx_gauge_face_import_xml: assertion 'filename' failed
ERROR, gauge has no associated filename!!
**
ERROR:events.c:860:update_properties: assertion failed: (tmpbuf)
Aborted (core dumped)

## Gauge Functions (in dashdesigner)
parse_rtv_xml_for_dash(xmlNode *node, Rtv_Data *rtv_data)
import_dash_xml(filename) // Pops up dialogs
load_elements(GtkWidget *dash, xmlNode *a_node) // pass root initially recursive
 - Dash is gotten by dash = GTK_WIDGET(gtk_builder_get_object(toplevel,"dashboard")); from dashdesigner/main.ui
load_geometry(GtkWidget *dash, xmlNode *node) // Load width,height
load_gauge(GtkWidget *dash, xmlNode *node) // width,height,x_offset,y_offset,gauge_xml_name,datasource

Use: properties = gtk_builder_new();gtk_builder_add_from_file(properties,filename,&error)

gauge = mtx_gauge_face_new (); mtx_gauge_face_import_xml(MTX_GAUGE_FACE(gauge),argv[1]);

src/dashboard.c Uses 	mtx_gauge_face_import_xml(MTX_GAUGE_FACE(gauge),filename);
GtkWidget * load_dashboard(const gchar *filename, gint index)

## Reusing MegaTunix libraries

### Installing header files

Copy header files to /usr/local/include:

    # 
    export MEGA_TUNIX_PROJECT_ROOT=~/MegaTunix/
    export MEGA_TUNIX_INCLUDE_DEST=/usr/local/include/
    cd $MEGA_TUNIX_PROJECT_ROOT
    sudo cp config.h widgets/*.h mtxmatheval/*.h include/*.h $MEGA_TUNIX_INCLUDE_DEST
    ls -al $MEGA_TUNIX_INCLUDE_DEST

Add line -I/usr/local/include/ to compilation (may be a built-in for gcc)

### Linking
During linking, add the library path /usr/local/lib (-L/usr/local/lib) and required libs.

Create a special shared (.so) library out of few essential files (must have load_elements(dast, root)):

    # Already first line is sufficient as DSO may have undefined symbols
    cd $MEGA_TUNIX_PROJECT_ROOT/src
    # rtv_processor.o
    # This does NOT seem to work - too many symbol problems.
    gcc -shared -o libmtxdash.so dashboard.o widgetmgmt.o stringmatch.o keyparser.o 
    # ...   `pkg-config --libs gtk+-2.0 libxml-2.0 json-glib-1.0` -lmtxgauge -lmtxcommon
    sudo cp libmtxdash.so /usr/local/lib/

### Problems
Some attractive functionality in linked into main executable. These modules should be in a separate DSO (Dynamic shared object, *.so) Examples of this (with respective solution) are:
- src/serialio.c / src/dataio.c (All comms routines) => Compile into Communications DSO.
- init.c, read_config(), init() (Also msave_config())
