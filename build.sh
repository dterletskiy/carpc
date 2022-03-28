#!/bin/bash

readonly CARPC_FW=./build
source ${CARPC_FW}/base.sh
source ${CARPC_FW}/constants.sh
source ${CARPC_FW}/build.sh
source ${CARPC_FW}/run.sh
source ${CARPC_FW}/tools.sh



COMPILER="clang"

case ${COMPILER} in
   "gcc")
      export CC=/usr/bin/cc
      export CXX=/usr/bin/c++
      # export CXX=/usr/bin/g++
   ;;
   "clang")
      export CC=/usr/bin/clang
      export CXX=/usr/bin/clang++
   ;;
   *)
   ;;
esac




declare -A PROJECT_CONFIG
declare -A TARGET_CONFIG
declare -A ACTION



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
         local ANDROID_INSTALL_PATH=/data/local/tmp/tda
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
