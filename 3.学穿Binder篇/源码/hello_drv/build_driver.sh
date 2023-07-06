#!/bin/bash
export ARCH=x86_64
export SUBARCH=x86_64
export CROSS_COMPILE=x86_64-linux-android-
export PATH=/home/android/Project/android10_r41/prebuilts/gcc/linux-x86/x86/x86_64-linux-android-4.9/bin:$PATH

make  -C ..