export ANDROID_NDK=/home/zzh0838/android-ndk-r25b

rm -r build
mkdir build && cd build 

# cmake -DCMAKE_SYSTEM_NAME=Android \
# 	-DCMAKE_SYSTEM_VERSION=29 \
# 	-DCMAKE_ANDROID_ARCH_ABI=x86_64 \
# 	-DANDROID_NDK=$ANDROID_NDK \
# 	-DCMAKE_ANDROID_STL_TYPE=c++_shared \
# 	..

cmake \
    -DCMAKE_TOOLCHAIN_FILE=$ANDROID_NDK/build/cmake/android.toolchain.cmake \
    -DANDROID_ABI=x86_64 \
    -DANDROID_PLATFORM=android-29 \
	-DANDROID_STL=c++_shared \
	..

cmake --build .