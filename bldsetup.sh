#!/bin/sh
echo "doing......"
export CROSS_COMPILE=~/prebuilt/linux-x86/toolchain/arm-eabi-4.4.3/bin/arm-eabi-
#export PATH=$PATH:~/prebuilt/linux-x86/toolchain/arm-eabi-linaro-4.6.2/bin
export ARCH=arm
#linaro.. export CROSS_COMPILE=arm-eabi-
export PATH=$PATH:~/prebuilt/linux-x86/toolchain/arm-eabi-4.4.3/bin
env | grep PATH
env | grep CROSS
echo "done ....."

#make ARCH=arm CROSS_COMPILE=/prebuilt/linux-x86/toolchain/arm-eabi-linaro-4.6.2/bin- defconfig griffin_defconfig