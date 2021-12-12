INSTALL_PATH=/data/local/tmp/tda/
DELIVERY_PATH=/home/dmytro_terletskyi/Sources/TDA//RPC_product/delivery/
SYSROOT_PATH=/home/dmytro_terletskyi/Soft/android/ndk/r23b/toolchains/llvm/prebuilt/linux-x86_64/sysroot/
TARGET=x86_64-linux-android

adb root

adb shell mkdir -p ${INSTALL_PATH}/bin/
adb push ${DELIVERY_PATH}/bin/* ${INSTALL_PATH}/bin/
adb shell chmod +x ${INSTALL_PATH}/*

adb shell mkdir -p ${INSTALL_PATH}/lib/
adb push ${DELIVERY_PATH}/lib/* ${INSTALL_PATH}/lib/
adb push ${SYSROOT_PATH}/usr/lib/${TARGET}/libc++_shared.so ${INSTALL_PATH}/lib/

adb shell mkdir -p ${INSTALL_PATH}/etc/
adb push ${DELIVERY_PATH}/etc/* ${INSTALL_PATH}/etc/





adb shell
INSTALL_PATH=/data/local/tmp/tda/
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${INSTALL_PATH}/lib/
cd ${INSTALL_PATH}
${INSTALL_PATH}/bin/servicebrocker config=${INSTALL_PATH}/etc/servicebrocker.cfg trace=CONSOLE
${INSTALL_PATH}/bin/application config=${INSTALL_PATH}/etc/application.cfg trace=CONSOLE
