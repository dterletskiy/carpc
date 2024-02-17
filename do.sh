#!/usr/bin/env bash

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
   export LD_LIBRARY_PATH="${INSTALL_DIR}/lib/"
   ${INSTALL_DIR}/bin/application config=${INSTALL_DIR}/etc/application.cfg
}

function validate_parameters( )
{
   if [ -z ${CMD_ACTION+x} ]; then
      echo "'--action' is not set"
      exit 1
   fi

   if [ -z ${CMD_TARGET+x} ]; then
      echo "'--target' is not set"
   fi
}

function parse_arguments( )
{
   echo "Parsing arguments..."

   for option in "$@"; do
      echo "Processing option '${option}'"
      case ${option} in
         --action=*)
            if [ -z ${CMD_ACTION+x} ]; then
               CMD_ACTION="${option#*=}"
               shift # past argument=value
               echo "CMD_ACTION: ${CMD_ACTION}"
            else
               echo "'--action' is already set to '${CMD_ACTION}'"
               exit 1
            fi
         ;;
         --target=*)
            if [ -z ${CMD_TARGET+x} ]; then
               CMD_TARGET="${option#*=}"
               shift # past argument=value
               echo "CMD_TARGET: ${CMD_TARGET}"
            else
               echo "'--target' is already set to '${CMD_TARGET}'"
               exit 1
            fi
         ;;
         --debug)
            CMD_DEBUG_FLAG=
            echo "CMD_DEBUG_FLAG: defined"
         ;;
         *)
            echo "undefined option: '${option}'"
            shift # past argument=value
         ;;
      esac
   done

   validate_parameters
}

function main( )
{
   parse_arguments $@

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
         run
      ;;
      *)
         echo "undefined action: '${CMD_ACTION}'"
      ;;
   esac
}




cd ${ROOT_DIR}
main $@








# LD_LIBRARY_PATH="/mnt/host/tda/carpc/out/deploy/lib/" ./stream

# export LD_LIBRARY_PATH="/mnt/host/tda/carpc/out/deploy/lib/"
# /mnt/host/tda/carpc/out/deploy/bin/servicebrocker config=/mnt/host/tda/carpc/out/deploy/etc/servicebrocker.cfg

# export LD_LIBRARY_PATH="/mnt/host/tda/carpc/out/deploy/lib/;/mnt/host/tda/carpc-examples/out/deploy/lib/"
# /mnt/host/tda/carpc-examples/out/deploy/bin/core config=/mnt/host/tda/carpc-examples/out/deploy/etc/core.cfg
# /mnt/host/tda/carpc-examples/out/deploy/bin/controller config=/mnt/host/tda/carpc-examples/out/deploy/etc/controller.cfg
# /mnt/host/tda/carpc-examples/out/deploy/bin/hmi config=/mnt/host/tda/carpc-examples/out/deploy/etc/hmi.cfg
# /mnt/host/tda/carpc-examples/out/deploy/bin/application config=/mnt/host/tda/carpc-examples/out/deploy/etc/application.cfg
