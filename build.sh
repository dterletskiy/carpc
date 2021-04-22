#!/bin/sh

# ENVIRONMENT_SETUP=/mnt/p4v/Audi_MIB3/NavCtrl/DevDef/packages/sdk/mib3h/installed/environment-setup-aarch64-poky-linux
# ENVIRONMENT_SETUP=/mnt/p4v/Audi_HCP3/NavCtrl/DevDef/packages/sdk/hcp3sim/installed/environment-setup-core2-64-hcp3-linux-musl
# ENVIRONMENT_SETUP=./_build_/android-env.sh



# Check if 'ENVIRONMENT_SETUP' is set
# https://stackoverflow.com/questions/3601515/how-to-check-if-a-variable-is-set-in-bash
if [ -z ${ENVIRONMENT_SETUP+x} ]; then
   echo "ENVIRONMENT_SETUP is unset";
else
   echo "ENVIRONMENT_SETUP is set to '${ENVIRONMENT_SETUP}'";
   if [ ! -f "${ENVIRONMENT_SETUP}" ]; then
      echo "'${ENVIRONMENT_SETUP}' file does not exist"
      exit
   fi
   source ${ENVIRONMENT_SETUP}
fi


# echo ${CXX}
# echo ${CC}
# ./_build_/build.py







ROOT_DIR=${PWD}
BUILD_DIR=${PWD}/"build"
TARGET=${1}

function clean( )
{
   local BUILD_DIR=${1}

   rm -R ${BUILD_DIR}
   mkdir ${BUILD_DIR}
}

function configure( )
{
   local SOURCE_DIR=${1}
   local BUILD_DIR=${2}

   # cd ${BUILD_DIR}
   # cmake ${SOURCE_DIR}
   # cd ${SOURCE_DIR}

   cmake -B${BUILD_DIR} -S${SOURCE_DIR}
}

function build( )
{
   local BUILD_DIR=${1}
   local TARGETS=${2}

   cmake --build ${BUILD_DIR} --target ${TARGETS}
}



STARTED=$(($(date +%s%N)/1000000))

clean ${BUILD_DIR}
configure ${ROOT_DIR} ${BUILD_DIR}
build ${BUILD_DIR} ${TARGET}

FINISHED=$(($(date +%s%N)/1000000))

echo "Build time duration:" $((FINISHED - STARTED)) "miliseconds"
