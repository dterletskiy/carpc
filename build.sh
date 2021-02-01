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


echo ${CXX}
echo ${CC}
./_build_/build.py
