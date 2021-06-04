#!/bin/sh

# ENVIRONMENT_SETUP=/mnt/p4v/Audi_MIB3/NavCtrl/DevDef/packages/sdk/mib3h/installed/environment-setup-aarch64-poky-linux
# ENVIRONMENT_SETUP=/mnt/p4v/Audi_HCP3/NavCtrl/DevDef/packages/sdk/hcp3sim/installed/environment-setup-core2-64-hcp3-linux-musl
# ENVIRONMENT_SETUP=./.configuration/android-env.sh



declare -A COMMAND=(
   [INFO]="--info"                  [INFO_S]="-i"
   [CLEAN]="--clean"                [CLEAN_S]="-c"
   [CONFIGURE]="--configure"        [CONFIGURE_S]="-s"
   [BUILD]="--build"                [BUILD_S]="-b"
   [CLEAN_BUILD]="--clean_build"    [CLEAN_BUILD_S]="-x"
   [INSTALL]="--install"            [INSTALL_S]="-d"
   [ARCHIVE]="--archive"            [ARCHIVE_S]="-a"
   [START]="--start"                [START_S]="-r"
   [STOP]="--stop"                  [STOP_S]="-f"
   [OLD]="old"
)

readonly PROJECT_ROOT_DIR=${PWD}
readonly PROJECT_ROOT_DIR_NAME=${PWD##*/}
readonly PROJECT_PRODUCT_DIR=${PROJECT_ROOT_DIR}/../${PROJECT_ROOT_DIR_NAME}"_product"
readonly PROJECT_BUILD_DIR=${PROJECT_PRODUCT_DIR}/build
readonly PROJECT_DELIVERY_DIR=${PROJECT_PRODUCT_DIR}/delivery
readonly PROJECT_DOCUMENTATION_DIR=${PROJECT_PRODUCT_DIR}/documentation

declare -A PROJECT



ACTION=${1}
TARGET=${2}



function setup_sdk( )
{
   local SDK=${1}

   # Check if 'SDK' is set
   # https://stackoverflow.com/questions/3601515/how-to-check-if-a-variable-is-set-in-bash
   if [ -z ${SDK+x} ]; then
      echo "SDK is unset";
   elif [ -z ${SDK} ]; then
      echo "SDK is empty";
   else
      echo "SDK is set to '${SDK}'";
      if [ ! -f "${SDK}" ]; then
         echo "'${SDK}' file does not exist"
         exit
      fi
      source ${SDK}
   fi
}

function print( )
{
   local -n LOCAL_PRINT__PROJECT=${1}

   echo "ROOT_DIR:           ${LOCAL_PRINT__PROJECT[ROOT_DIR]}"
   echo "ROOT_DIR_NAME:      ${LOCAL_PRINT__PROJECT[ROOT_DIR_NAME]}"
   echo "SOURCE_DIR:         ${LOCAL_PRINT__PROJECT[SOURCE_DIR]}"
   echo "PRODUCT_DIR:        ${LOCAL_PRINT__PROJECT[PRODUCT_DIR]}"
   echo "BUILD_DIR:          ${LOCAL_PRINT__PROJECT[BUILD_DIR]}"
   echo "DELIVERY_DIR:       ${LOCAL_PRINT__PROJECT[DELIVERY_DIR]}"
   echo "DOCUMENTATION_DIR:  ${LOCAL_PRINT__PROJECT[DOCUMENTATION_DIR]}"

   echo "CXX:" ${CXX}
   echo "CC:" ${CC}
}

function setup( )
{
   local -n LOCAL_SETUP__PROJECT=${1}

   LOCAL_SETUP__PROJECT[ROOT_DIR]=${PWD}
   LOCAL_SETUP__PROJECT[ROOT_DIR_NAME]=${PWD##*/}
   LOCAL_SETUP__PROJECT[SOURCE_DIR]=${LOCAL_SETUP__PROJECT[ROOT_DIR]}
   LOCAL_SETUP__PROJECT[PRODUCT_DIR]=${LOCAL_SETUP__PROJECT[ROOT_DIR]}/../${LOCAL_SETUP__PROJECT[ROOT_DIR_NAME]}"_product"
   LOCAL_SETUP__PROJECT[BUILD_DIR]=${LOCAL_SETUP__PROJECT[PRODUCT_DIR]}/build
   LOCAL_SETUP__PROJECT[DELIVERY_DIR]=${LOCAL_SETUP__PROJECT[PRODUCT_DIR]}/delivery
   LOCAL_SETUP__PROJECT[DOCUMENTATION_DIR]=${LOCAL_SETUP__PROJECT[PRODUCT_DIR]}/documentation
}

function clean( )
{
   local DIR=${1}

   rm -R ${DIR}
}

function configure( )
{
   local -n LOCAL_CONFIGURE__PROJECT=${1}

   cmake -B ${LOCAL_CONFIGURE__PROJECT[BUILD_DIR]} -DCMAKE_INSTALL_PREFIX=${LOCAL_CONFIGURE__PROJECT[DELIVERY_DIR]} -S ${LOCAL_CONFIGURE__PROJECT[SOURCE_DIR]} --graphviz=${LOCAL_CONFIGURE__PROJECT[DOCUMENTATION_DIR]}/graph
}

function build( )
{
   local -n LOCAL_BUILD__PROJECT=${1}
   local LOCAL_TARGETS=${2}

   cmake --build ${LOCAL_BUILD__PROJECT[BUILD_DIR]} --target ${LOCAL_TARGETS}
}

function install( )
{
   local -n LOCAL_INSTALL__PROJECT=${1}

   cmake --build ${LOCAL_INSTALL__PROJECT[BUILD_DIR]} --target install
   # cmake --install ${LOCAL_INSTALL__PROJECT[BUILD_DIR]} --prefix ${LOCAL_INSTALL__PROJECT[DELIVERY_DIR]}
}

function archive( )
{
   local -n LOCAL_ARCHIVE__PROJECT=${1}

   ARCHIVE_NAME=${LOCAL_ARCHIVE__PROJECT[ROOT_DIR_NAME]}_$(date +'%Y-%m-%d_%H-%M-%S')
   zip -r ../${ARCHIVE_NAME} ../${LOCAL_ARCHIVE__PROJECT[ROOT_DIR_NAME]}
   echo ${ARCHIVE_NAME}
}

function gpb_generate( )
{
   local SOURCE_DIR=${1}
   local DESTINATION_DIR=${2}
   local PROTO_FILE=${3}

   protoc -I=${SOURCE_DIR} --cpp_out=${DESTINATION_DIR} ${SOURCE_DIR}/${PROTO_FILE}
}


APPLICATION=application
SERVICEBROCKER=servicebrocker
HMI=hmi
CONTROLLER=controller
CORE=core
DLT_DAEMON="dlt-daemon"

function start_delivery_process( )
{
   local -n LOCAL_SDP__PROJECT=${1}
   local LOCAL_PROCESS_NAME=${2}
   local LOCAL_TRACE=${3}

   local LOCAL_PROCESS_PID=$(pgrep -x ${LOCAL_PROCESS_NAME})
   if [ -z "${LOCAL_PROCESS_PID}" ]; then
      echo "starting" ${LOCAL_PROCESS_NAME}
      echo ${LOCAL_SDP__PROJECT[DELIVERY_DIR]}/bin/${LOCAL_PROCESS_NAME} config=${LOCAL_SDP__PROJECT[DELIVERY_DIR]}/etc/${LOCAL_PROCESS_NAME}.cfg trace=${LOCAL_TRACE} &
      ${LOCAL_SDP__PROJECT[DELIVERY_DIR]}/bin/${LOCAL_PROCESS_NAME} config=${LOCAL_SDP__PROJECT[DELIVERY_DIR]}/etc/${LOCAL_PROCESS_NAME}.cfg trace=${LOCAL_TRACE} &
      echo ${LOCAL_PROCESS_NAME} "started successfully with PID" $!
   else
      echo ${LOCAL_PROCESS_NAME} "has been started with PID" ${LOCAL_PROCESS_PID}
   fi
}

function start( )
{
   # start_dlt_daemon PROJECT
   # start_delivery PROJECT
   start_test PROJECT
}

function stop( )
{
   # stop_delivery PROJECT
   # stop_dlt_daemon
   stop_test PROJECT
}

function start_delivery( )
{
   local -n LOCAL_SD__PROJECT=${1}

   LD_PATH=${LD_LIBRARY_PATH}:${LOCAL_SD__PROJECT[DELIVERY_DIR]}/lib:/usr/lib/:/usr/local/lib/
   export LD_LIBRARY_PATH=${LD_PATH}

   start_delivery_process LOCAL_SD__PROJECT ${SERVICEBROCKER} "DLT"
   start_delivery_process LOCAL_SD__PROJECT ${HMI} "DLT"
   start_delivery_process LOCAL_SD__PROJECT ${CONTROLLER} "DLT"
   start_delivery_process LOCAL_SD__PROJECT ${CORE} "DLT"
}

function stop_delivery( )
{
   killall ${CORE}
   killall ${CONTROLLER}
   killall ${HMI}
   killall ${SERVICEBROCKER}
}

function start_test( )
{
   local -n LOCAL_SD__PROJECT=${1}

   LD_PATH=${LD_LIBRARY_PATH}:${LOCAL_SD__PROJECT[DELIVERY_DIR]}/lib:/usr/lib/:/usr/local/lib/
   echo export LD_LIBRARY_PATH=${LD_PATH}
   export LD_LIBRARY_PATH=${LD_PATH}

   start_delivery_process LOCAL_SD__PROJECT ${APPLICATION} "CONSOLE"
}

function stop_test( )
{
   killall ${APPLICATION}
}

function start_dlt_daemon( )
{
   local -n LOCAL_SDLT__PROJECT=${1}
   local LOCAL_PROCESS_NAME=${DLT_DAEMON}

   local LOCAL_PROCESS_PID=$(pgrep -x ${LOCAL_PROCESS_NAME})
   if [ -z "${LOCAL_PROCESS_PID}" ]; then
      echo "starting" ${LOCAL_PROCESS_NAME}
      ${LOCAL_PROCESS_NAME} -d -c ${LOCAL_SDLT__PROJECT[DELIVERI_DIR]}/etc/dlt.conf
      echo ${LOCAL_PROCESS_NAME} "started successfully with PID" $!
   else
      echo ${LOCAL_PROCESS_NAME} "has been started with PID" ${LOCAL_PROCESS_PID}
   fi
}

function stop_dlt_daemon( )
{
   local LOCAL_PROCESS_NAME=${DLT_DAEMON}

   killall ${LOCAL_PROCESS_NAME}
}



function main( )
{
   STARTED=$(($(date +%s%N)/1000000))

   setup PROJECT
   setup_sdk ${ENVIRONMENT_SETUP}

   case ${ACTION} in
      ${COMMAND[INFO]} | ${COMMAND[INFO_S]})
         print PROJECT
      ;;
      ${COMMAND[CLEAN]} | ${COMMAND[CLEAN_S]})
         echo "clean"
         clean ${PROJECT[PRODUCT_DIR]}
      ;;
      ${COMMAND[CONFIGURE]} | ${COMMAND[CONFIGURE_S]})
         echo "configure"
         configure PROJECT
      ;;
      ${COMMAND[BUILD]} | ${COMMAND[BUILD_S]})
         echo "build"
         build PROJECT
         install PROJECT
      ;;
      ${COMMAND[CLEAN_BUILD]} | ${COMMAND[CLEAN_BUILD_S]})
         echo "clean build"
         clean ${PROJECT[PRODUCT_DIR]}
         configure PROJECT
         build PROJECT
         install PROJECT
      ;;
      ${COMMAND[INSTALL]} | ${COMMAND[INSTALL_S]})
         echo "install"
         install PROJECT
      ;;
      ${COMMAND[ARCHIVE]} | ${COMMAND[ARCHIVE_S]})
         echo "archive"
         archive PROJECT
      ;;
      ${COMMAND[START]} | ${COMMAND[START_S]})
         echo "start"
         start
      ;;
      ${COMMAND[STOP]} | ${COMMAND[STOP_S]})
         echo "stop"
         stop
      ;;
      ${COMMAND[OLD]})
         echo "using old build system"
         ./_build_/build.py
      ;;
      *)
         echo "   --info         | -i     print info"
         echo "   --clean        | -c     clean product folder"
         echo "   --configure    | -s     configure build"
         echo "   --build        | -b     build project"
         echo "   --clean_build  | -x     clean build project"
         echo "   --install      | -d     install built project"
         echo "   --archive      | -a     archive project"
         echo "   --start        | -r     start installed project"
         echo "   --stop         | -f     stop installed project"
      ;;
   esac

   FINISHED=$(($(date +%s%N)/1000000))
   echo "Build time duration:" $((FINISHED - STARTED)) "miliseconds"
}



reset
main
