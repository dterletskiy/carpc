function start_process_blocking( )
{
   local LOCAL_PROCESS_NAME=${1}
   local LOCAL_TRACE="CONSOLE"

   print_info export VSOMEIP_APPLICATION_NAME=${LOCAL_PROCESS_NAME}
   export VSOMEIP_APPLICATION_NAME=${LOCAL_PROCESS_NAME}
   print_info export VSOMEIP_CONFIGURATION=${PROJECT_CONFIG[DELIVERY_DIR]}/etc/vsomeip-${LOCAL_PROCESS_NAME}.json
   export VSOMEIP_CONFIGURATION=${PROJECT_CONFIG[DELIVERY_DIR]}/etc/vsomeip-${LOCAL_PROCESS_NAME}.json

   local LOCAL_PROCESS_PID=$(pgrep -x ${LOCAL_PROCESS_NAME})
   if [ -z "${LOCAL_PROCESS_PID}" ]; then
      echo "starting" ${LOCAL_PROCESS_NAME}
      echo ${PROJECT_CONFIG[DELIVERY_DIR]}/bin/${LOCAL_PROCESS_NAME} \
         config=${PROJECT_CONFIG[DELIVERY_DIR]}/etc/${LOCAL_PROCESS_NAME}.cfg \
         trace=${LOCAL_TRACE}
      ${PROJECT_CONFIG[DELIVERY_DIR]}/bin/${LOCAL_PROCESS_NAME} \
         config=${PROJECT_CONFIG[DELIVERY_DIR]}/etc/${LOCAL_PROCESS_NAME}.cfg \
         trace=${LOCAL_TRACE}
      echo ${LOCAL_PROCESS_NAME} "started successfully with PID" $!
   else
      echo ${LOCAL_PROCESS_NAME} "has been started with PID" ${LOCAL_PROCESS_PID}
   fi
}

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

function start_test_application( )
{
   start_dlt_daemon
   start_process ${SERVICEBROCKER} "DLT"
   start_process ${APPLICATION} "DLT"
}

function stop_test_application( )
{
   stop_process ${APPLICATION}
   stop_process ${SERVICEBROCKER}
   stop_dlt_daemon
}

function start_test_mvc( )
{
   start_dlt_daemon
   start_process ${SERVICEBROCKER} "DLT"
   start_process ${HMI} "DLT"
   start_process ${CONTROLLER} "DLT"
   start_process ${CORE} "DLT"
}

function stop_test_mvc( )
{
   stop_process ${CORE}
   stop_process ${CONTROLLER}
   stop_process ${HMI}
   stop_process ${SERVICEBROCKER}
   stop_dlt_daemon
}

function start_test_rolename( )
{
   start_dlt_daemon
   start_process ${SERVICEBROCKER} "DLT"
   start_process ${HYBRID} "DLT"
   start_process ${SYMBIONT} "DLT"
}

function stop_test_rolename( )
{
   stop_process ${SYMBIONT}
   stop_process ${HYBRID}
   stop_process ${SERVICEBROCKER}
   stop_dlt_daemon
}

function start_dlt_daemon( )
{
   local LOCAL_PROCESS_NAME=${DLT_DAEMON}

   local LOCAL_PROCESS_PID=$(pgrep -x ${LOCAL_PROCESS_NAME})
   if [ -z "${LOCAL_PROCESS_PID}" ]; then
      echo "starting" ${LOCAL_PROCESS_NAME}
      echo ${LOCAL_PROCESS_NAME} -d -c ${PROJECT_CONFIG[DELIVERY_DIR]}/etc/dlt.conf
      ${LOCAL_PROCESS_NAME} -d -c ${PROJECT_CONFIG[DELIVERY_DIR]}/etc/dlt.conf
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
   print_info export PATH=${PATH}:${PROJECT_CONFIG[DELIVERY_DIR]}/bin:${PROJECT_CONFIG[CARPC]}/bin
   export PATH=${PATH}:${PROJECT_CONFIG[DELIVERY_DIR]}/bin:${PROJECT_CONFIG[CARPC]}/bin

   print_info export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${PROJECT_CONFIG[DELIVERY_DIR]}/lib:${PROJECT_CONFIG[CARPC]}/lib:/usr/lib/:/usr/local/lib/:/mnt/dev/COVESA/build/vsomeip
   export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${PROJECT_CONFIG[DELIVERY_DIR]}/lib:${PROJECT_CONFIG[CARPC]}/lib:/usr/lib/:/usr/local/lib/:/mnt/dev/COVESA/build/vsomeip

   # print_info export LD_PRELOAD=${LD_PRELOAD}:${PROJECT_CONFIG[DELIVERY_DIR]}/lib/libhooks.so
   # export LD_PRELOAD=${LD_PRELOAD}:${PROJECT_CONFIG[DELIVERY_DIR]}/lib/libhooks.so

   case ${ACTION[TARGET]} in
      test_application)
         print_info "test application"
         start_test_application
         ;;
      test_mvc)
         print_info "test mvc"
         start_test_mvc
         ;;
      test_rolename)
         print_info "test rolename"
         start_test_rolename
         ;;
      dlt)
         print_info "dlt"
         start_dlt_daemon
         ;;
      *)
         print_info ${ACTION[TARGET]}
         start_process_blocking ${ACTION[TARGET]}
         ;;
   esac
}

function stop( )
{
   case ${ACTION[TARGET]} in
      test_application)
         print_info "test application"
         stop_test_application
         ;;
      test_mvc)
         print_info "test mvc"
         stop_test_mvc
         ;;
      test_rolename)
         print_info "test rolename"
         stop_test_rolename
         ;;
      dlt)
         print_info "dlt"
         stop_dlt_daemon
         ;;
      *)
         print_info ${ACTION[TARGET]}
         stop_process ${ACTION[TARGET]}
         ;;
   esac
}
