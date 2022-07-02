[Go to contents](../README.md#table-of_contents)

# ***Configuration Parameters***


## ***IPC configuration***

-----

**ipc** - boolean flag what identifies will application use IPC/RPC or not.

Value type: boolean

Available value: true, false

-----

**ipc_servicebrocker_domain** - socket domain which specifies the protocol family which will be used for communication to connect to servicebrocker.

Value type: string

Available value: AF_UNIX, AF_INET

-----

**ipc_servicebrocker_type** - socket type which specifies the communication semantics to connect to servicebrocker.

Value type: string

Available value: SOCK_STREAM, SOCK_DGRAM, SOCK_SEQPACKET, SOCK_RAW, SOCK_RDM

-----

**ipc_servicebrocker_protocole** - specifies a particular protocol to be used with the socket to connect to servicebrocker.

Value type: integer

Available value: 0

-----

**ipc_servicebrocker_address** - servicebrocker ip address or path to Unix socket.

Value type:
   - string if ipc_servicebrocker_type=AF_UNIX
   - network address data if ipc_servicebrocker_type=AF_INET

Available value: according to network rules

-----

**ipc_servicebrocker_port** - port used to establish connection to servicebrocker.

Value type: integer

Available value: according to network rules

-----

**ipc_application_domain** - application socket type which specifies the communication semantics.

Value type: string

Available value: AF_UNIX, AF_INET

-----

**ipc_application_type** - application socket type which specifies the communication semantics.

Value type: string

Available value: SOCK_STREAM, SOCK_DGRAM, SOCK_SEQPACKET, SOCK_RAW, SOCK_RDM

-----

**ipc_application_protocole** - application specifies a particular protocol to be used with the socket.

Value type: integer

Available value: 0

-----

**ipc_application_address** - application ip address or path to Unix socket.

Value type:
   - string if ipc_application_type=AF_UNIX
   - network address data if ipc_application_type=AF_INET

Available value: according to network rules

-----

**ipc_application_port** - application port used to establish connection to.

Value type: integer

Available value: according to network rules

-----



## ***Tracing configuration***

-----

**trace_log** - trace log strategy:

Value type: string

Available value: CONSOLE, DLT, ANDROID

-----

**trace_buffer** - trace buffer size

Value type: integer

Available value: 0 - any

-----

**trace_app_name** - application name which will be used in tracing system

Value type: string

Available value: any

-----



## ***Watchdog configuration***

**application_wd_timout** -
