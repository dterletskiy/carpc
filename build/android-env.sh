#!/bin/sh

# links:
# https://developer.android.com/ndk/guides/other_build_systems



NDK_ROOT=/home/dmytro_terletskyi/Soft/android/ndk
NDK_VERSION=r23b
NDK=${NDK_ROOT}/${NDK_VERSION}

HOST_ARCH=x86_64
HOST_OS=linux
HOST_TAG=${HOST_OS}-${HOST_ARCH}

export TOOLCHAIN=${NDK}/toolchains/llvm/prebuilt/${HOST_TAG}
export SYSROOT=${TOOLCHAIN}/sysroot

export ARCH=x86_64
export OS=linux
export API=31
export TARGET=${ARCH}-${OS}-android
export SESSION=${TARGET}${API}

ADDITIONAL_PARAMETERS="-mstackrealign -fno-addrsig"
PARAMETERS="--sysroot=${SYSROOT} --target=${SESSION} ${ADDITIONAL_PARAMETERS}"



export AR="${TOOLCHAIN}/bin/llvm-ar ${PARAMETERS}"
export AS="${TOOLCHAIN}/bin/llvm-as ${PARAMETERS}"
export CC="${TOOLCHAIN}/bin/${TARGET}${API}-clang ${PARAMETERS}"
export CXX="${TOOLCHAIN}/bin/${TARGET}${API}-clang++ ${PARAMETERS}"
# export AS="$CC"
export LD="${TOOLCHAIN}/bin/ld ${PARAMETERS}"
export RANLIB="${TOOLCHAIN}/bin/llvm-ranlib ${PARAMETERS}"
export STRIP="${TOOLCHAIN}/bin/llvm-strip ${PARAMETERS}"

export ABD_TOOL=adb
