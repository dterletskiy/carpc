function start_process( )
{
   local LOCAL_PROCESS_NAME=${1}
   local LOCAL_TRACE=${2}

   local LOCAL_PROCESS_PID=$(pgrep -x ${LOCAL_PROCESS_NAME})
   if [ -z "${LOCAL_PROCESS_PID}" ]; then
      echo "starting" ${LOCAL_PROCESS_NAME}
      echo ${PROJECT_CONFIG[DELIVERY_DIR]}/bin/${LOCAL_PROCESS_NAME} \
         config=${PROJECT_CONFIG[DELIVERY_DIR]}/etc/${LOCAL_PROCESS_NAME}.cfg \
         trace=${LOCAL_TRACE} &
      ${PROJECT_CONFIG[DELIVERY_DIR]}/bin/${LOCAL_PROCESS_NAME} \
         config=${PROJECT_CONFIG[DELIVERY_DIR]}/etc/${LOCAL_PROCESS_NAME}.cfg \
         trace=${LOCAL_TRACE} &
      echo ${LOCAL_PROCESS_NAME} "started successfully with PID" $!
   else
      echo ${LOCAL_PROCESS_NAME} "has been started with PID" ${LOCAL_PROCESS_PID}
   fi
}

function stop_process( )
{
   local LOCAL_PROCESS_NAME=${1}

   echo "stopping" ${LOCAL_PROCESS_NAME}
   killall ${LOCAL_PROCESS_NAME}
}

function start_delivery( )
{
   start_dlt_daemon
   start_process ${SERVICEBROCKER} "DLT"
   start_process ${APPLICATION} "DLT"
   # start_process ${HMI} "DLT"
   # start_process ${CONTROLLER} "DLT"
   # start_process ${CORE} "DLT"
}

function stop_delivery( )
{
   # stop_process ${CORE}
   # stop_process ${CONTROLLER}
   # stop_process ${HMI}
   stop_process ${APPLICATION}
   stop_process ${SERVICEBROCKER}
   stop_dlt_daemon
}

function start_test( )
{
   start_process ${SERVICEBROCKER} "CONSOLE"
   start_process ${EXPERIMENTAL} "CONSOLE"
}

function stop_test( )
{
   stop_process ${EXPERIMENTAL}
   stop_process ${SERVICEBROCKER}
}

function start_dlt_daemon( )
{
   local LOCAL_PROCESS_NAME=${DLT_DAEMON}

   local LOCAL_PROCESS_PID=$(pgrep -x ${LOCAL_PROCESS_NAME})
   if [ -z "${LOCAL_PROCESS_PID}" ]; then
      echo "starting" ${LOCAL_PROCESS_NAME}
      echo ${LOCAL_PROCESS_NAME} -d -c ${PROJECT_CONFIG[DELIVERI_DIR]}/etc/dlt.conf
      ${LOCAL_PROCESS_NAME} -d -c ${PROJECT_CONFIG[DELIVERI_DIR]}/etc/dlt.conf
      echo ${LOCAL_PROCESS_NAME} "started successfully with PID" $!
   else
      echo ${LOCAL_PROCESS_NAME} "has been started with PID" ${LOCAL_PROCESS_PID}
   fi
}

function stop_dlt_daemon( )
{
   local LOCAL_PROCESS_NAME=${DLT_DAEMON}

   stop_process ${LOCAL_PROCESS_NAME}
}

function start( )
{
   _LD_LIBRARY_PATH_=${LD_LIBRARY_PATH}:${PROJECT_CONFIG[DELIVERY_DIR]}/lib:/usr/lib/:/usr/local/lib/
   echo export LD_LIBRARY_PATH=${_LD_LIBRARY_PATH_}
   export LD_LIBRARY_PATH=${_LD_LIBRARY_PATH_}

   # _LD_PRELOAD_=${LD_PRELOAD}:${PROJECT_CONFIG[DELIVERY_DIR]}/lib/libhooks.so
   # echo export LD_PRELOAD=${_LD_PRELOAD_}
   # export LD_PRELOAD=${_LD_PRELOAD_}

   case ${ACTION[TARGET]} in
      delivery)
         print_info "delivery"
         start_delivery
         ;;
      test)
         print_info "test"
         start_test
         ;;
      dlt)
         print_info "dlt"
         start_dlt_daemon
         ;;
      *)
         print_info "target"
         start_process ${ACTION[TARGET]}
         ;;
   esac
}

function stop( )
{
   case ${ACTION[TARGET]} in
      delivery)
         print_info "delivery"
         stop_delivery
         ;;
      test)
         print_info "test"
         stop_test
         ;;
      dlt)
         print_info "dlt"
         stop_dlt_daemon
         ;;
      *)
         print_info "target"
         stop_process ${ACTION[TARGET]}
         ;;
   esac
}
