# MegaTunix subsystems

## Serial Communications

### Low level comms

Declared and implemented by src/serialio.h and src/serialio.c (500+ lines).
Linux/Unix implementations depends/relies on POSIX termios (termios.h) serial communications interface.
Ports are named by Windows conventions (COM1,COM2 ... as opposed to UNIX Device file ????). Note: See config in ~/mtx/default/config Section [Serial] (keys: potential_ports, override_port)
- Serial API interfaces: close_serial, flush_serial, lock_serial, open_serial,
setup_serial_params, unlock_serial (Also win32_* versions get compiled in)
- Note missing red/write interfaces - All red/write access later happens via serial_params->fd
- Open Port handle is stored in serial_params->fd.
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

TODO: Allow compiling a new shared library libmtxecuio.so (No coupling to UI, should be also applicable to CL apps).

## Configuration
- Some of this seems to get stored in variable global_data as k-v pairs (value can be ...).
- Any data (of misc types) from this k-v map/table is gotten by:
  DATA_GET(global_data,"mykey"); and casting to type known for the key "mykey"
  (e.g. Firmware_Details * firmware = (Firmware_Details *)DATA_GET(global_data,"firmware");
- About 77 vars are stored here (even before loading firmware, gotten from console messages)

# Build process and dependencies

- Majority of main GUI sources and binaries in src/
- Codebase has a wrapper shell script src/megatunix, but final installed executable is binary (/usr/local/bin/megatunix)
- Internal dependencies (analyzed by ldd, prefixed by libmtx*)
  - libmtxcommon.so.0 => /usr/local/lib/libmtxcommon.so.0 (0xb6ba5000)
    - Configuration stuff (INI ? XML ?)
  - libmtxprogress.so.0 => /usr/local/lib/libmtxprogress.so.0 (0xb6ba0000)
  - libmtxstripchart.so.0 => /usr/local/lib/libmtxstripchart.so.0 (0xb6b98000)
  - libmtxgauge.so.0 => /usr/local/lib/libmtxgauge.so.0 (0xb6b82000)
  - libmtxcombo_mask.so.0 => /usr/local/lib/libmtxcombo_mask.so.0 (0xb6b7e000)
  - libmtxyamlcpp.so.0 => /usr/local/lib/libmtxyamlcpp.so.0 (0xb6b0b000)

## Gauge utilities

Codebase: dashdesigner/dashdesigner and gaugedesigner/gaugedesigner (TODO:mtxloader)
Both Installed / available in /usr/local/bin/ after make install.
dashdesigner allows also a very nice browsing of gauges via 
### Gauges
- Example gauges in subdirs of /usr/local/share/MegaTunix/Gauges/*/*.xml
- Users own Private gauges can be stored in ~/mtx/default/Gauges
- Properties of gauge
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

