clear
# export LD_PRELOAD=/mnt/p4v/Source/RPC/../RPC_product/delivery/lib/libhooks.so
export LD_LIBRARY_PATH=:/mnt/p4v/Source/RPC/../RPC_product/delivery/lib:/usr/lib/:/usr/local/lib/
# export MALLOC_TRACE=/tmp/experimental_mtrace
/mnt/p4v/Source/RPC/../RPC_product/delivery/bin/experimental config=/mnt/p4v/Source/RPC/../RPC_product/delivery/etc/experimental.cfg trace_log=CONSOLE
# unset LD_PRELOAD
# mtrace /mnt/p4v/Source/RPC/../RPC_product/delivery/bin/experimental ${MALLOC_TRACE}
