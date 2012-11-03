#!/bin/sh
rm griffinifw/linux
rm griffinifw/griffin.ifw
cp arch/arm/boot/zImage griffinifw/linux
cd griffinifw
./packer griffin.ifw < griffin.conf
adb remount
adb push griffin.ifw /sdcard/
#adb push linux /sdcard/zimage
cd ../