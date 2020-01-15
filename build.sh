#!/bin/sh

PROJECT_DIR=/mnt/p4v/Audi_HCP3/NavCtrl/DevDef
SDK_DIR=${PROJECT_DIR}/packages/sdk/hcp3sim/installed
ENVIRONMENT_SETUP=${SDK_DIR}/environment-setup-core2-64-hcp3-linux-musl

echo ${ENVIRONMENT_SETUP}
if [ ! -f "${ENVIRONMENT_SETUP}" ]; then
   echo "'${ENVIRONMENT_SETUP}' file does not exist"
   exit
fi

export CXX="g++"
# source ${ENVIRONMENT_SETUP}
echo ${CXX}
./_build_/build.py
