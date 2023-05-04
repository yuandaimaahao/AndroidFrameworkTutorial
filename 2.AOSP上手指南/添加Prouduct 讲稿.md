# Product 添加 讲稿

## 什么是 Product

在编译系统的时候，需要执行 lunch 命令，lunch 命令的每一个选项就是一个 Product。

为什么需要 Product？
Product 类似于 app 开发中的渠道包，我们在 gradle 中通过 flavorDimensions 和 productFlavors 的配置，使得一份源码可以打包出满足不同渠道需求的 apk 包。

Product 就是一些配置文件， 一份源码通过 Product 的配置用于不同的产品。不同的 Product 配置文件就可以编译出适用于不同产品的安装镜像文件。

举个例子，我用的手机是 小米12s pro，小米12s，小米12 ultra，一般来说，这三款手机使用的是同一份源码，不同的是他们的 Product 配置文件，选择 小米12s pro 的配置文件就能编译出小米12s pro 的安装镜像（或者叫刷机包）

比如我们的 aosp 通过 Product 配置，源码可用于 qemu 模拟器和多款 pixel 手机。

## Product 在哪里？

Product 主要集中在两个目录：

```
build/target
device
```

我们选择的 aosp_x86_64-eng product 适用于模拟器，其 Product 配置文件比较分散，主要包含了以下几个：


* `build/target/board/generic_x86_64/BoardConfig.mk`： 用于硬件相关配置
* `build/target/product/AndroidProducts.mk` 和 `build/target/product/aosp_x86_64.mk`：用于软件相关的配置

BoardConfig.mk 用于定义和硬件相关的底层特性和变量，比如当前源码支持的 cpu 位数(64/32位)，bootloader 和 kernel, 是否支持摄像头，GPS导航等一些板级特性。

```Makefile
# x86_64 emulator specific definitions
TARGET_CPU_ABI := x86_64
TARGET_ARCH := x86_64
TARGET_ARCH_VARIANT := x86_64

TARGET_2ND_CPU_ABI := x86
TARGET_2ND_ARCH := x86
TARGET_2ND_ARCH_VARIANT := x86_64

TARGET_PRELINK_MODULE := false
include build/make/target/board/BoardConfigGsiCommon.mk
include build/make/target/board/BoardConfigEmuCommon.mk

BOARD_USERDATAIMAGE_PARTITION_SIZE := 576716800

BOARD_SEPOLICY_DIRS += device/generic/goldfish/sepolicy/x86

# Wifi.
BOARD_WLAN_DEVICE           := emulator
BOARD_HOSTAPD_DRIVER        := NL80211
BOARD_WPA_SUPPLICANT_DRIVER := NL80211
BOARD_HOSTAPD_PRIVATE_LIB   := lib_driver_cmd_simulated
BOARD_WPA_SUPPLICANT_PRIVATE_LIB := lib_driver_cmd_simulated
WPA_SUPPLICANT_VERSION      := VER_0_8_X
WIFI_DRIVER_FW_PATH_PARAM   := "/dev/null"
WIFI_DRIVER_FW_PATH_STA     := "/dev/null"
WIFI_DRIVER_FW_PATH_AP      := "/dev/null"
```

其中还通过 include 包含了 BoardConfigGsiCommon.mk 和 BoardConfigEmuCommon.mk 两个配置文件，前者用于通用系统映像的配置，后者用于模拟器的配置

主要和硬件相关，有一个基本的了解即可。一般很少改动。