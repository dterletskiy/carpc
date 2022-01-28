#!/bin/bash

SHELL_FW=../shell_fw
source ${SHELL_FW}/constants/console.sh
source ${SHELL_FW}/constants/constants.sh
source ${SHELL_FW}/base.sh
source ${SHELL_FW}/print.sh



ENVIRONMENT_SETUP_ANDROID=./.configuration/android-env.sh



ANDROID_INSTALL_PATH=/data/local/tmp/tda



declare -A COMMAND=(
   [INFO]="info"
   [CLEAN]="clean"
   [CONFIGURE]="configure"
   [BUILD]="build"
   [CLEAN_BUILD]="clean_build"
   [INSTALL]="install"
   [ARCHIVE]="archive"
   [START]="start"
   [STOP]="stop"
   [ADB_INSTALL]="adb_install"
)

function info( )
{
   echo "   ${COMMAND[INFO]}         - print project info"
   echo "   ${COMMAND[CLEAN]}        - clean product folder"
   echo "   ${COMMAND[CONFIGURE]}    - configure build"
   echo "   ${COMMAND[BUILD]}        - build project"
   echo "   ${COMMAND[CLEAN_BUILD]}  - clean build project"
   echo "   ${COMMAND[INSTALL]}      - install built project"
   echo "   ${COMMAND[ARCHIVE]}      - archive project"
   echo "   ${COMMAND[START]}        - start installed project"
   echo "   ${COMMAND[STOP]}         - stop installed project"
   echo "   ${COMMAND[ADB_INSTALL]}  - install to device via adb"
}

readonly CORES=$( grep -c ^processor /proc/cpuinfo )

declare -A ARCH_MAP=(
      [x86]=0
      [x86_64]=1
      [arm]=2
      [aarch64]=3
   )

declare -A OS_MAP=(
      [linux]=0
      [android]=1
   )

declare -A PROJECT_CONFIG
declare -A TARGET_CONFIG
declare -A ACTION



function setup_sdk( )
{
   local SDK=${1}

   # Check if 'SDK' is set
   # https://stackoverflow.com/questions/3601515/how-to-check-if-a-variable-is-set-in-bash
   if [ -z ${SDK+x} ]; then
      print_info "SDK is unset";
   elif [ -z ${SDK} ]; then
      print_info "SDK is empty";
   else
      print_info "SDK is set to '${SDK}'";
      if [ ! -f "${SDK}" ]; then
         print_error "'${SDK}' file does not exist"
         exit
      fi
      source ${SDK}
   fi
}

function setup( )
{
   PROJECT_CONFIG[ROOT_DIR]=${PWD}
   PROJECT_CONFIG[ROOT_DIR_NAME]=${PWD##*/}
   PROJECT_CONFIG[SOURCE_DIR]=${PROJECT_CONFIG[ROOT_DIR]}
   PROJECT_CONFIG[PRODUCT_DIR]=${PROJECT_CONFIG[ROOT_DIR]}/../${PROJECT_CONFIG[ROOT_DIR_NAME]}_product/${TARGET_CONFIG[OS]}-${TARGET_CONFIG[ARCH]}
   PROJECT_CONFIG[BUILD_DIR]=${PROJECT_CONFIG[PRODUCT_DIR]}/build
   PROJECT_CONFIG[DELIVERY_DIR]=${PROJECT_CONFIG[PRODUCT_DIR]}/delivery
   PROJECT_CONFIG[DOCUMENTATION_DIR]=${PROJECT_CONFIG[PRODUCT_DIR]}/documentation
   PROJECT_CONFIG[CMAKE_PARAMETERS]=" \
         -DOS_ANDROID:STRING=${OS_MAP[android]} \
         -DOS_LINUX:STRING=${OS_MAP[linux]} \
         -DOS_TARGET:STRING=${OS_MAP[${TARGET_CONFIG[OS]}]} \
      "
   if [ ${TARGET_CONFIG[OS]} == "android" ]; then
      PROJECT_CONFIG[ENVIRONMENT_SETUP]=${ENVIRONMENT_SETUP_ANDROID}
   fi
}

function print_project_info( )
{
   echo "TARGET ARCH:         ${TARGET_CONFIG[ARCH]}"
   echo "TARGET OS:           ${TARGET_CONFIG[OS]}"

   echo "ROOT_DIR:            ${PROJECT_CONFIG[ROOT_DIR]}"
   echo "ROOT_DIR_NAME:       ${PROJECT_CONFIG[ROOT_DIR_NAME]}"
   echo "SOURCE_DIR:          ${PROJECT_CONFIG[SOURCE_DIR]}"
   echo "PRODUCT_DIR:         ${PROJECT_CONFIG[PRODUCT_DIR]}"
   echo "BUILD_DIR:           ${PROJECT_CONFIG[BUILD_DIR]}"
   echo "DELIVERY_DIR:        ${PROJECT_CONFIG[DELIVERY_DIR]}"
   echo "DOCUMENTATION_DIR:   ${PROJECT_CONFIG[DOCUMENTATION_DIR]}"
   echo "CMAKE_PARAMETERS:    ${PROJECT_CONFIG[CMAKE_PARAMETERS]}"
   echo "ENVIRONMENT_SETUP:   ${PROJECT_CONFIG[ENVIRONMENT_SETUP]}"

   echo "AR:                  ${AR}"
   echo "AS:                  ${AS}"
   echo "CC:                  ${CC}"
   echo "CXX:                 ${CXX}"
   echo "LD:                  ${LD}"
   echo "RANLIB:              ${RANLIB}"
   echo "STRIP:               ${STRIP}"
}

function clean( )
{
   rm -R ${PROJECT_CONFIG[PRODUCT_DIR]}
}

function configure( )
{
   cmake -B ${PROJECT_CONFIG[BUILD_DIR]} \
      -DCMAKE_INSTALL_PREFIX=${PROJECT_CONFIG[DELIVERY_DIR]} \
      -S ${PROJECT_CONFIG[SOURCE_DIR]} \
      --graphviz=${PROJECT_CONFIG[DOCUMENTATION_DIR]}/graph \
      ${PROJECT_CONFIG[CMAKE_PARAMETERS]}
}

function build( )
{
   local LOCAL_TARGETS=${1}

   cmake --build ${PROJECT_CONFIG[BUILD_DIR]} --target ${LOCAL_TARGETS} -j ${CORES}
}

function install( )
{
   cmake --build ${PROJECT_CONFIG[BUILD_DIR]} --target install
   # cmake --install ${PROJECT_CONFIG[BUILD_DIR]} --prefix ${PROJECT_CONFIG[DELIVERY_DIR]}
}

function archive( )
{
   ARCHIVE_NAME=${PROJECT_CONFIG[ROOT_DIR_NAME]}_$(date +'%Y-%m-%d_%H-%M-%S')
   zip -r ../${ARCHIVE_NAME} ../${PROJECT_CONFIG[ROOT_DIR_NAME]}
   echo ${ARCHIVE_NAME}
}

function gpb_generate( )
{
   local SOURCE_DIR=${1}
   local DESTINATION_DIR=${2}
   local PROTO_FILE=${3}

   protoc -I=${SOURCE_DIR} --cpp_out=${DESTINATION_DIR} ${SOURCE_DIR}/${PROTO_FILE}
}

function adb_install( )
{
   if [[ -z ${2+x} ]]; then
      echo "Set project and installation path"
      return 255
   fi

   local LOCAL_INSTALL_PATH=${1}

   echo ${ABD_TOOL} root
   echo ${ABD_TOOL} shell mkdir -p ${LOCAL_INSTALL_PATH}
   echo ${ABD_TOOL} push ${PROJECT_CONFIG[DELIVERY_DIR]}/bin/* ${LOCAL_INSTALL_PATH}
   echo ${ABD_TOOL} shell chmod +x ${LOCAL_INSTALL_PATH}/*
   echo ${ABD_TOOL} push ${PROJECT_CONFIG[DELIVERY_DIR]}/lib/* ${LOCAL_INSTALL_PATH}
   echo ${ABD_TOOL} push ${PROJECT_CONFIG[DELIVERY_DIR]}/etc/* ${LOCAL_INSTALL_PATH}
   echo ${ABD_TOOL} push ${SYSROOT}/usr/lib/${TARGET}/libc++_shared.so ${LOCAL_INSTALL_PATH}

}



EXPERIMENTAL=experimental
APPLICATION=application
SERVICEBROCKER=servicebrocker
HMI=hmi
CONTROLLER=controller
CORE=core
DLT_DAEMON="dlt-daemon"

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




function pre_main( )
{
   print_header "Pre-Main Menu"

   for option in "$@"; do
      case ${option} in
         --arch=*)
            print_info "arch"
            TARGET_CONFIG[ARCH]="${option#*=}"
            shift # past argument=value
            ;;
         --os=*)
            print_info "os"
            TARGET_CONFIG[OS]="${option#*=}"
            shift # past argument=value
            ;;
         --action=*)
            print_info "action"
            ACTION[NAME]="${option#*=}"
            shift # past argument=value
            ;;
         --target=*)
            print_info "target"
            ACTION[TARGET]="${option#*=}"
            shift # past argument=value
            ;;
      esac
   done

   if [ -z ${TARGET_CONFIG[ARCH]+x} ]; then
      print_error "Target ARCH is unset";
      exit 1
   elif [ -z ${TARGET_CONFIG[ARCH]} ]; then
      print_error "Target ARCH is empty";
      exit 1
   else
      print_info "Target ARCH is set to '${TARGET_CONFIG[ARCH]}'";
      map_find_key ARCH_MAP ${TARGET_CONFIG[ARCH]}
      if [ $? -eq 0 ]; then
         print_error "Unknown target ARCH"
         exit 1
      fi
   fi

   if [ -z ${TARGET_CONFIG[OS]+x} ]; then
      print_error "Target OS is unset";
      exit 1
   elif [ -z ${TARGET_CONFIG[OS]} ]; then
      print_error "Target OS is empty";
      exit 1
   else
      print_info "Target OS is set to '${TARGET_CONFIG[OS]}'";
      map_find_key OS_MAP ${TARGET_CONFIG[OS]}
      if [ $? -eq 0 ]; then
         print_error "Unknown target OS"
         exit 1
      fi
   fi
}

function main( )
{
   print_header "Main Menu"

   STARTED=$(($(date +%s%N)/1000000))

   pre_main $@

   setup
   setup_sdk ${PROJECT_CONFIG[ENVIRONMENT_SETUP]}
   print_project_info

   case ${ACTION[NAME]} in
      ${COMMAND[INFO]})
         print_info "info"
         print_project_info
      ;;
      ${COMMAND[CLEAN]})
         print_info "clean"
         clean
      ;;
      ${COMMAND[CONFIGURE]})
         print_info "configure"
         configure
      ;;
      ${COMMAND[BUILD]})
         print_info "build"
         build
         install
      ;;
      ${COMMAND[CLEAN_BUILD]})
         print_info "clean build"
         clean
         configure
         build
         install
      ;;
      ${COMMAND[INSTALL]})
         print_info "install"
         install
      ;;
      ${COMMAND[ARCHIVE]})
         print_info "archive"
         archive
      ;;
      ${COMMAND[START]})
         print_info "start"
         start
      ;;
      ${COMMAND[STOP]})
         print_info "stop"
         stop
      ;;
      ${COMMAND[ADB_INSTALL]})
         print_info "adb install"
         adb_install ${ANDROID_INSTALL_PATH}
      ;;
      *)
         print_error "Undefined action:" ${ACTION[NAME]}
         info
      ;;
   esac

   FINISHED=$(($(date +%s%N)/1000000))
   echo "Build time duration:" $((FINISHED - STARTED)) "miliseconds"
}



reset
main $@
