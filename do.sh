#!/usr/bin/env bash

# ./do.sh --action=run --target=lisot --params="--server --bind=192.168.0.100 --port=10000"
# ./do.sh --action=run --target=lisot --params="--client=192.168.0.100 --port=10000 --family=AF_INET --type=SOCK_STREAM"

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"

ROOT_DIR=${SCRIPT_DIR}
SOURCE_DIR=${ROOT_DIR}/
REPOS_DIR=${SOURCE_DIR}/repos/
PRODUCT_DIR=${ROOT_DIR}/_product_/
BUILD_DIR=${PRODUCT_DIR}/build/
GEN_DIR=${PRODUCT_DIR}/gen/
INSTALL_DIR=${PRODUCT_DIR}/deploy/
DOC_DIR=${PRODUCT_DIR}/doc/

export CC=/usr/bin/cc
export CXX=/usr/bin/c++

BUILD_VARIABLES=""
BUILD_VARIABLES+=" -DROOT_GEN_DIR:STRING=${GEN_DIR}"
BUILD_VARIABLES+=" -DTARGET_OS:STRING=linux"
BUILD_VARIABLES+=" -DDLT_TRACE:STRING=yes"
BUILD_VARIABLES+=" -DSYS_TRACE:STRING=yes"
BUILD_VARIABLES+=" -DMSG_TRACE:STRING=yes"
BUILD_VARIABLES+=" -DDEBUG_STREAM:STRING=no"
BUILD_VARIABLES+=" -DCOLORED_TRACE:STRING=yes"
BUILD_VARIABLES+=" -DMEMORY_HOOK:STRING=no"
BUILD_VARIABLES+=" -DINSTRUMENTAL:STRING=no"
BUILD_VARIABLES+=" -DUSE_DEBUG:STRING=no"
BUILD_VARIABLES+=" -DUSE_GPB:STRING=yes"
BUILD_VARIABLES+=" -DUSE_RTTI:STRING=yes"



SHELL_FW=${SCRIPT_DIR}/submodules/dterletskiy/shell_fw/
source ${SHELL_FW}/constants/console.sh
source ${SHELL_FW}/constants/constants.sh
source ${SHELL_FW}/base.sh
source ${SHELL_FW}/print.sh
source ${SHELL_FW}/ui.sh
source ${SHELL_FW}/drive.sh



function fetch( )
{
   declare -A PROJECTS=(
      ["carpc-builder"]="main"
      ["carpc-tracing"]="main"
      ["carpc-base"]="main"
      ["carpc-tools"]="main"
      ["carpc-osw"]="main"
      ["carpc-runtime"]="main"
      ["carpc-hooks"]="main"
      ["carpc-instrumental"]="main"
      ["carpc-servicebrocker"]="main"
      ["carpc-examples"]="scorpius"
      ["carpc-tutorial"]="scorpius"
   )
   

   for PROJECT in ${!PROJECTS[@]}; do
      local BRANCH=${PROJECTS[${PROJECT}]}
      git clone --recursive -b ${BRANCH} "git@github.com:dterletskiy/${PROJECT}.git" "${REPOS_DIR}/${PROJECT}"
   done
}

function config( )
{
   cmake \
      -S ${SOURCE_DIR} \
      -B ${BUILD_DIR} \
      --install-prefix ${INSTALL_DIR} \
      --graphviz=${DOC_DIR}/graph/project \
      ${BUILD_VARIABLES}
}

function build( )
{
   LOCAL_TARGET=${1}
   if [ -z ${LOCAL_TARGET+x} ]; then
      PARAMETER_TARGET=""
   elif [ -z ${LOCAL_TARGET} ]; then
      PARAMETER_TARGET=""
   else
      PARAMETER_TARGET="--target ${LOCAL_TARGET}"
   fi

   cmake --build ${BUILD_DIR} -j8 ${PARAMETER_TARGET}
}

function install( )
{
   build "install"
}

function clean( )
{
   build "clean"
}

function pure( )
{
   rm -rf \
      ${BUILD_DIR}/CMakeFiles \
      ${BUILD_DIR}/CMakeCache.txt \
      ${BUILD_DIR}/Makefile \
      ${BUILD_DIR}/install_manifest.txt \
      ${BUILD_DIR}/cmake_install.cmake
   rm -fr ${PRODUCT_DIR}
}

function run( )
{
   LOCAL_TARGET=${1}
   shift
   LOCAL_OPTIONS=${@}

   export LD_LIBRARY_PATH="${INSTALL_DIR}/lib/"
   ${INSTALL_DIR}/bin/${LOCAL_TARGET} --config=${INSTALL_DIR}/etc/${LOCAL_TARGET}.cfg ${LOCAL_OPTIONS}
}

function validate_parameters( )
{
   if [ -z ${CMD_ACTION+x} ]; then
      print_error "'--action' is not set"
      exit 1
   fi

   if [ -z ${CMD_TARGET+x} ]; then
      print_warning "'--target' is not set"
   fi
}

function parse_arguments( )
{
   print_header "Parsing arguments..."

   for option in "$@"; do
      print_info "Processing option '${option}'"
      case ${option} in
         --action=*)
            if [ -z ${CMD_ACTION+x} ]; then
               CMD_ACTION="${option#*=}"
               shift # past argument=value
               echo "CMD_ACTION: ${CMD_ACTION}"
            else
               print_error "'--action' is already set to '${CMD_ACTION}'"
               exit 1
            fi
         ;;
         --target=*)
            if [ -z ${CMD_TARGET+x} ]; then
               CMD_TARGET="${option#*=}"
               shift # past argument=value
               echo "CMD_TARGET: ${CMD_TARGET}"
            else
               print_error "'--target' is already set to '${CMD_TARGET}'"
               exit 1
            fi
         ;;
         --params=*)
            if [ -z ${CMD_OPTIONS+x} ]; then
               CMD_OPTIONS="${option#*=}"
               shift # past argument=value
               echo "CMD_OPTIONS: ${CMD_OPTIONS}"
            else
               print_error "'--params' is already set to '${CMD_OPTIONS}'"
               exit 1
            fi
         ;;
         --debug)
            CMD_DEBUG_FLAG=
            echo "CMD_DEBUG_FLAG: defined"
         ;;
         *)
            print_error "undefined option: '${option}'"
            shift # past argument=value
            exit 1
         ;;
      esac
   done

   validate_parameters
}

function main( )
{
   parse_arguments "$@"
   print_info "Processing action:" ${CMD_ACTION}

   case ${CMD_ACTION} in
      fetch)
         fetch
      ;;
      clean)
         clean
      ;;
      pure)
         pure
      ;;
      config)
         config
      ;;
      build)
         build ${CMD_TARGET}
      ;;
      install)
         install
      ;;
      world)
         pure
         config
         build
         install
      ;;
      run)
         run ${CMD_TARGET} "${CMD_OPTIONS}"
      ;;
      *)
         print_warning "undefined action: '${CMD_ACTION}'"
         exit 1
      ;;
   esac
}



cd ${ROOT_DIR}
main "$@"
