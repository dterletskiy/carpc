#!/bin/sh



PROJECT_DIR="/mnt/p4v/Source/RPC"
DELIVERI_DIR=${PROJECT_DIR}/".delivery"



function create_delivery( )
{
   rm -R ${DELIVERI_DIR}

   mkdir -p ${DELIVERI_DIR}/{bin,cfg,include,lib}

   cp -R ./framework/api/ ${DELIVERI_DIR}/include/
   cp ./framework_product/lib/libframework.so ${DELIVERI_DIR}/lib

   cp -R ./hooks/api/ ${DELIVERI_DIR}/include/
   cp ./hooks_product/lib/libhooks.so ${DELIVERI_DIR}/lib

   cp ./servicebrocker/servicebrocker.cfg ${DELIVERI_DIR}/cfg
   cp ./servicebrocker_product/exe/servicebrocker ${DELIVERI_DIR}/bin

   cp ./core/core.cfg ${DELIVERI_DIR}/cfg
   cp ./core_product/exe/core ${DELIVERI_DIR}/bin

   cp ./controller/controller.cfg ${DELIVERI_DIR}/cfg
   cp ./controller_product/exe/controller ${DELIVERI_DIR}/bin

   cp ./hmi/hmi.cfg ${DELIVERI_DIR}/cfg
   cp ./hmi_product/exe/hmi ${DELIVERI_DIR}/bin
}

function clear_delivery( )
{
   rm -R ${DELIVERI_DIR}
}

function start_delivery( )
{
   cd ${DELIVERI_DIR}

   LD_PATH=${LD_LIBRARY_PATH}:${DELIVERI_DIR}/lib:/usr/lib/:/usr/local/lib/
   export LD_LIBRARY_PATH=${LD_PATH}

   ${DELIVERI_DIR}/bin/servicebrocker config=${DELIVERI_DIR}/cfg/servicebrocker.cfg &
   ${DELIVERI_DIR}/bin/hmi config=${DELIVERI_DIR}/cfg/hmi.cfg &
   ${DELIVERI_DIR}/bin/controller config=${DELIVERI_DIR}/cfg/controller.cfg &
   ${DELIVERI_DIR}/bin/core config=${DELIVERI_DIR}/cfg/core.cfg &
}

function stop_delivery( )
{
   killall hmi
   killall controller
   killall core
   killall servicebrocker
}

function start_dlt_daemon( )
{
   dlt-daemon -d -c ${DELIVERI_DIR}/.configuration/dlt.conf
}

function stop_dlt_daemon( )
{
   killall dlt-daemon
}



reset

# create_delivery
# start_delivery

# stop_delivery
# clear_delivery

# stop_dlt_daemon
start_dlt_daemon
