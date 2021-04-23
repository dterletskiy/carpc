#!/bin/sh

# ENVIRONMENT_SETUP=/mnt/p4v/Audi_MIB3/NavCtrl/DevDef/packages/sdk/mib3h/installed/environment-setup-aarch64-poky-linux
# ENVIRONMENT_SETUP=/mnt/p4v/Audi_HCP3/NavCtrl/DevDef/packages/sdk/hcp3sim/installed/environment-setup-core2-64-hcp3-linux-musl
# ENVIRONMENT_SETUP=./_build_/android-env.sh



readonly CLEAN="clean"
readonly CONFIGURE="configure"
readonly BUILD="build"
readonly CLEAN_BUILD="clean_build"
readonly INSTALL="install"
readonly OLD="old"

readonly ROOT_DIR=${PWD}
readonly ROOT_DIR_NAME=${PWD##*/}
readonly PRODUCT_DIR=${ROOT_DIR}/../${ROOT_DIR_NAME}"_product"
readonly BUILD_DIR=${PRODUCT_DIR}/build
readonly DELIVERY_DIR=${PRODUCT_DIR}/delivery
readonly DOCUMENTATION_DIR=${PRODUCT_DIR}/documentation

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

   echo "CXX:" ${CXX}
   echo "CC:" ${CC}
}

function clean( )
{
   local DIR=${1}

   rm -R ${DIR}
}

function configure( )
{
   local SOURCE_DIR=${1}
   local BUILD_DIR=${2}

   # cd ${BUILD_DIR}
   # cmake ${SOURCE_DIR}
   # cd ${SOURCE_DIR}

   cmake -B ${BUILD_DIR} -DCMAKE_INSTALL_PREFIX=${DELIVERY_DIR} -S ${SOURCE_DIR} --graphviz=${DOCUMENTATION_DIR}/graph
}

function build( )
{
   local BUILD_DIR=${1}
   local TARGETS=${2}

   cmake --build ${BUILD_DIR} --target ${TARGETS}
}
   
function install( )
{
   local BUILD_DIR=${1}

   cmake --build ${BUILD_DIR} --target install
   # cmake --install ${BUILD_DIR} --prefix ${DELIVERY_DIR}
}

function main( )
{
   STARTED=$(($(date +%s%N)/1000000))

   setup_sdk ${ENVIRONMENT_SETUP}

   case ${ACTION} in
      ${CLEAN})
         echo "clean"
         clean ${PRODUCT_DIR}
      ;;
      ${CONFIGURE})
         echo "configure"
         configure ${ROOT_DIR} ${BUILD_DIR}
      ;;
      ${BUILD})
         echo "build"
         build ${BUILD_DIR} ${TARGET}
      ;;
      ${CLEAN_BUILD})
         echo "clean build"
         clean ${PRODUCT_DIR}
         configure ${ROOT_DIR} ${BUILD_DIR}
         build ${BUILD_DIR} ${TARGET}
      ;;
      ${INSTALL})
         echo "install"
         install ${BUILD_DIR}
      ;;
      ${OLD})
         echo "using old build system"
         ./_build_/build.py
      ;;
      *)
         echo "clean build and install"
         clean ${PRODUCT_DIR}
         configure ${ROOT_DIR} ${BUILD_DIR}
         build ${BUILD_DIR} ${TARGET}
         install ${BUILD_DIR}
      ;;
   esac

   FINISHED=$(($(date +%s%N)/1000000))
   echo "Build time duration:" $((FINISHED - STARTED)) "miliseconds"
}




main
