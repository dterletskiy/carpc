#!/bin/bash

# used commands:
# ./build.sh --sfw=/mnt/dev/TDA/shell_fw --pfw=/mnt/dev/TDA/python_fw --arch=x86_64 --os=linux --action=clean_build
# ./build.sh --sfw=/mnt/dev/TDA/shell_fw --pfw=/mnt/dev/TDA/python_fw --arch=x86_64 --os=linux --action=start --target=application
# ./build.sh --sfw=/mnt/dev/TDA/shell_fw --pfw=/mnt/dev/TDA/python_fw --arch=x86_64 --os=linux --action=clean_build --carpc=/mnt/dev/TDA/carpc_product/linux-x86_64/delivery
# ./build.sh --sfw=/mnt/dev/TDA/shell_fw --pfw=/mnt/dev/TDA/python_fw --arch=x86_64 --os=linux --action=start --target=application --carpc=/mnt/dev/TDA/carpc_product/linux-x86_64/delivery



declare -A PROJECT_CONFIG
declare -A TARGET_CONFIG
declare -A ACTION



# Processing parameter "--sfw" and source its value for using shell framework scripts in current build system
function setup_build_system( )
{
   echo "----- Setup build system -----"

   for option in "$@"; do
      case ${option} in
         --sfw=*)
            PROJECT_CONFIG[SFW]="${option#*=}"
            shift # past argument=value
         ;;
         --pfw=*)
            PROJECT_CONFIG[PFW]="${option#*=}"
            shift # past argument=value
         ;;
         --carpc=*)
            PROJECT_CONFIG[CARPC]="${option#*=}"
            shift # past argument=value
         ;;
      esac
   done

   if [ -z ${PROJECT_CONFIG[SFW]+x} ]; then
      echo --- "Shell framework is unset";
      exit 1
   elif [ -z ${PROJECT_CONFIG[SFW]} ]; then
      echo --- "Shell framework is empty";
      exit 1
   elif [ ! -d ${PROJECT_CONFIG[SFW]} ]; then
      echo "Shell framework does not exists '${PROJECT_CONFIG[SFW]}'";
      exit 1
   else
      echo "Shell framework is set to '${PROJECT_CONFIG[SFW]}'";
   fi

   source ${PROJECT_CONFIG[SFW]}/constants/console.sh
   source ${PROJECT_CONFIG[SFW]}/constants/constants.sh
   source ${PROJECT_CONFIG[SFW]}/base.sh
   source ${PROJECT_CONFIG[SFW]}/print.sh

   if [ -z ${PROJECT_CONFIG[PFW]+x} ]; then
      echo --- "Python framework is unset";
      exit 1
   elif [ -z ${PROJECT_CONFIG[PFW]} ]; then
      echo --- "Python framework is empty";
      exit 1
   elif [ ! -d ${PROJECT_CONFIG[PFW]} ]; then
      echo "Shell framework does not exists '${PROJECT_CONFIG[PFW]}'";
      exit 1
   else
      echo "Python framework is set to '${PROJECT_CONFIG[PFW]}'";
   fi
}

reset
setup_build_system $@



readonly CARPC_FW=./build
source ${CARPC_FW}/constants.sh
source ${CARPC_FW}/build.sh
source ${CARPC_FW}/run.sh
source ${CARPC_FW}/tools.sh



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
         --compiler=*)
            print_info "compiler"
            set_compiler ${option#*=}
            shift # past argument=value
         ;;
         --sdk=*)
            print_info "sdk"
            set_compiler ${option#*=}
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



main $@
