#!/bin/bash -xe

TARGET_ARCH=arm64
TARGET_ARCH_VARIANT=armv8-a
TARGET_CPU_ABI=arm64-v8a
TARGET_CPU_VARIANT=cortex-a53

CURRENT_DIR=$(pwd)
ANDROID_CONFIGURATION=debug
ANDROID_API=26
IB_CONFIG="android_${TARGET_ARCH}_${TARGET_CPU_ABI}_${ANDROID_CONFIGURATION}"
IB_TARGET=jni/libnative-activity
mkdir -p ../out/${IB_CONFIG}/${IB_TARGET}
IB_OUTPUT=../out/${IB_CONFIG}/${IB_TARGET}
IB_OUTDIR=$(dirname $IB_OUTPUT)
ANDROID_TOOLCHAIN_DIR=${IB_OUTDIR}/native-toolchain-${TARGET_ARCH}
ANDROID_PLATFORM=28.0.1
