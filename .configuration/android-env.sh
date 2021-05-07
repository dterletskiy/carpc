#!/bin/sh

NDK=/mnt/sdk/Android/ndk/r20b
HOST_TAG=linux-x86_64

export TOOLCHAIN=${NDK}/toolchains/llvm/prebuilt/${HOST_TAG}
export SYSROOT=${TOOLCHAIN}/sysroot

export ARCH=i686
export OS=linux
export API=android24
export SESSION=${ARCH}-${OS}-${API}
ADDITIONAL_PARAMETERS="-mstackrealign -fno-addrsig"
PARAMETERS="--sysroot=${SYSROOT} --target=${SESSION} ${ADDITIONAL_PARAMETERS}"

export AR="${TOOLCHAIN}/bin/aarch64-linux-android-ar ${PARAMETERS}"
export AS="${TOOLCHAIN}/bin/aarch64-linux-android-as ${PARAMETERS}"
export CC="${TOOLCHAIN}/bin/aarch64-linux-android21-clang ${PARAMETERS}"
export CXX="${TOOLCHAIN}/bin/aarch64-linux-android21-clang++ ${PARAMETERS}"
export LD="${TOOLCHAIN}/bin/aarch64-linux-android-ld ${PARAMETERS}"
export RANLIB="${TOOLCHAIN}/bin/aarch64-linux-android-ranlib ${PARAMETERS}"
export STRIP="${TOOLCHAIN}/bin/aarch64-linux-android-strip ${PARAMETERS}"
