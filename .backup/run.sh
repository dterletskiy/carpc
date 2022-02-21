#!/bin/bash

ROOT_DIR=/mnt/dev/TDA/
PROJECT_NAME=rpc
PROJECT_DIR=${ROOT_DIR}/${PROJECT_NAME}/
PRODUCT_DIR=${ROOT_DIR}/${PROJECT_NAME}_product/linux-x86_64
DELIVERY_DIR=${PRODUCT_DIR}/delivery/
DELIVERY_BIN_DIR=${DELIVERY_DIR}/bin/
DELIVERY_LIB_DIR=${DELIVERY_DIR}/lib/
DELIVERY_ETC_DIR=${DELIVERY_DIR}/etc/
NAME=${1}



clear

# echo export LD_PRELOAD=/mnt/p4v/Source/RPC/../RPC_product/delivery/lib/libhooks.so
# export LD_PRELOAD=/mnt/p4v/Source/RPC/../RPC_product/delivery/lib/libhooks.so

echo export LD_LIBRARY_PATH=:${DELIVERY_LIB_DIR}:/usr/lib/:/usr/local/lib/
export LD_LIBRARY_PATH=:${DELIVERY_LIB_DIR}:/usr/lib/:/usr/local/lib/

# echo export MALLOC_TRACE=/tmp/experimental_mtrace
# export MALLOC_TRACE=/tmp/experimental_mtrace

echo ${DELIVERY_BIN_DIR}/${NAME} config=${DELIVERY_ETC_DIR}/${NAME}.cfg trace_log=CONSOLE
${DELIVERY_BIN_DIR}/${NAME} config=${DELIVERY_ETC_DIR}/${NAME}.cfg trace_log=CONSOLE

# echo unset LD_PRELOAD
# unset LD_PRELOAD

# echo mtrace /mnt/p4v/Source/RPC/../RPC_product/delivery/bin/experimental ${MALLOC_TRACE}
# mtrace /mnt/p4v/Source/RPC/../RPC_product/delivery/bin/experimental ${MALLOC_TRACE}
