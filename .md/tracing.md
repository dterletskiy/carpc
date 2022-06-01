[Go to contents](../README.md#table-of_contents)

# ***Tracing***

**CARPC** framework provides its own tracing. Usinf simple tracing macros developer could send logging messages with different log levels to **console**, **dlt-daemon** or event android **logcat** server in case of using **CARPC** for android. Logging destination called log stratagy in **CARPC** framework.

Log strategy could be configured in application configuration file, command line or environment variables during starting application and does not require recompilation.

There are 6 log levels supported by **CARPC** tracing system using different trace macros:
   1. verbose (MSG_VRB)
   2. debug (MSG_DBG)
   3. information (MSG_INF)
   4. warning (MSG_WRN)
   5. error (MSG_ERR)
   6. fatal (MSG_FTL)

Each **CARPC** log message contains user message and some additional message header. By default message header has:

MICROSECONDS | PID TID |   [CLASS_ABBR:FUNCTION:LINE] ->

Where:
   - MICROSECONDS - timestamp represented in nanoseconds since the Epoch
   - PID - OS process ID where log message has been emmited
   - TID - OS thread ID where log message has been emmited
   - CLASS_ABBR - user defined class abbreviation
   - FUNCTION - function name where log message has been emmited
   - LINE - line number in the code where log message has been emmited


The syntax of mentioned macros is the same as for usual **C printf** function.

To use **CARPC** tracing there must be included "carpc/trace/Trace.hpp", defined "CLASS_ABBR" and linked libtracing.so library.

In this case code will look like:

-----
<pre>
#include "carpc/trace/Trace.hpp"
#define CLASS_ABBR "OnOff"
...
void OnOffComponent::init( const size_t id )
{
   MSG_DBG( "id = %zu", id );
}
...
</pre>
-----

As a result there will be a message similar to:

-----
<pre>
1653925858116797 | 0x881a 0x7f32e1bcb700 |   [OnOff:init:45] -> id = 12345
</pre>
-----

**CARPC** tracing system has log buffer with predefined size what could be set in application configuration file. Inc ase if message size is more then message buffer size only part of this massage will be sent to corresponding server or console.

In case of error becomes during sending message or other system error (memory buffer can't be allocated) next message will be emmited:

-----
<pre>
----- build message error -----
</pre>
-----
