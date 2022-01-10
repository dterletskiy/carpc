#!/bin/bash

ROOT_DIR=/mnt/dev/TDA/
PROJECT_NAME=rpc
PROJECT_DIR=${ROOT_DIR}/${PROJECT_NAME}/
PRODUCT_DIR=${ROOT_DIR}/${PROJECT_NAME}_product/
DELIVERY_DIR=${PRODUCT_DIR}/delivery/
DELIVERY_BIN_DIR=${DELIVERY_DIR}/bin/
DELIVERY_LIB_DIR=${DELIVERY_DIR}/lib/
DELIVERY_ETC_DIR=${DELIVERY_DIR}/etc/
NAME=${1}



clear

# export LD_PRELOAD=/mnt/p4v/Source/RPC/../RPC_product/delivery/lib/libhooks.so
export LD_LIBRARY_PATH=:${DELIVERY_LIB_DIR}:/usr/lib/:/usr/local/lib/
# export MALLOC_TRACE=/tmp/experimental_mtrace

${DELIVERY_BIN_DIR}/${NAME} config=${DELIVERY_ETC_DIR}/${NAME}.cfg trace_log=CONSOLE

# unset LD_PRELOAD
# mtrace /mnt/p4v/Source/RPC/../RPC_product/delivery/bin/experimental ${MALLOC_TRACE}
