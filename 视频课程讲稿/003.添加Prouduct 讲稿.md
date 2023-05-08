# Product 添加 讲稿

## 什么是 Product


在编译系统的时候，需要执行 lunch 命令，(手动演示)lunch 命令的每一个选项就是一个 Product。

这样说呢，有点抽象，具体一点，什么是 Product 呢？

我们先看一个熟悉的例子，在 app 开发中的，经常需要打渠道包，我们在修改 gradle 文件和部分源码后，一份源码就可以打包出满足不同渠道需求的 apk 包。

Product 实质上就是一些配置文件， 一份源码通过配置不同的 Product 就可以打包出不同的镜像文件，可用于不同的产品。

举个例子，我用的手机是 小米12s pro，同系列的手机还有 小米12s，小米12 ultra，

如果三款手机使用三份源码，同系列的手机功能特性基本相同，如果需要添加新的软件，我们就需要修改三份源码，编译三次，操作管理起来稍显麻烦。

一般来说，这三款手机使用的是同一份源码，不同的是他们的 Product 配置，选择 小米12s pro 的配置文件就能编译出小米12s pro 的系统镜像文件,同理，。。。。。

比如我们的 aosp 通过不同的 Product 编译，生成的系统镜像可用于 qemu 模拟器和多款 pixel 手机，

## Product 在哪里？

Product 主要集中在两个目录：

```
build/target : 模拟器相关的 product 文件
device  ： 实际硬件设备相关的 product 文件
```

我们选择的 aosp_x86_64-eng product 适用于模拟器，其 Product 配置文件比较分散，主要包含了以下几个：

* `build/target/board/generic_x86_64/BoardConfig.mk`： 用于硬件相关配置
* `build/target/product/AndroidProducts.mk` 和 `build/target/product/aosp_x86_64.mk`：用于软件相关的配置



我们来看一下学习使用的 aosp_x86_64-eng product

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


接着看下 AndroidProducts.mk：

```Makefile
# Unbundled apps will be built with the most generic product config.
# TARGET_BUILD_APPS 编译Android系统时，这个值为空，编译单模块时，这个值为所编译模块的路径

#编译单模块
ifneq ($(TARGET_BUILD_APPS),)
PRODUCT_MAKEFILES := \
    $(LOCAL_DIR)/aosp_arm64.mk \
    $(LOCAL_DIR)/aosp_arm.mk \
    $(LOCAL_DIR)/aosp_x86_64.mk \
    $(LOCAL_DIR)/aosp_x86.mk \
    $(LOCAL_DIR)/full.mk \
    $(LOCAL_DIR)/full_x86.mk \
#编译系统
else
PRODUCT_MAKEFILES := \
    $(LOCAL_DIR)/aosp_64bitonly_x86_64.mk \
    $(LOCAL_DIR)/aosp_arm64.mk \
    $(LOCAL_DIR)/aosp_arm.mk \
    $(LOCAL_DIR)/aosp_x86_64.mk \
    $(LOCAL_DIR)/aosp_x86_arm.mk \
    $(LOCAL_DIR)/aosp_x86.mk \
    $(LOCAL_DIR)/full.mk \
    $(LOCAL_DIR)/full_x86.mk \
    $(LOCAL_DIR)/generic.mk \
    $(LOCAL_DIR)/generic_system_arm64.mk \
    $(LOCAL_DIR)/generic_system_x86.mk \
    $(LOCAL_DIR)/generic_system_x86_64.mk \
    $(LOCAL_DIR)/generic_system_x86_arm.mk \
    $(LOCAL_DIR)/generic_x86.mk \
    $(LOCAL_DIR)/mainline_system_arm64.mk \
    $(LOCAL_DIR)/mainline_system_x86.mk \
    $(LOCAL_DIR)/mainline_system_x86_64.mk \
    $(LOCAL_DIR)/mainline_system_x86_arm.mk \
    $(LOCAL_DIR)/ndk.mk \
    $(LOCAL_DIR)/sdk_arm64.mk \
    $(LOCAL_DIR)/sdk.mk \
    $(LOCAL_DIR)/sdk_phone_arm64.mk \
    $(LOCAL_DIR)/sdk_phone_armv7.mk \
    $(LOCAL_DIR)/sdk_phone_x86_64.mk \
    $(LOCAL_DIR)/sdk_phone_x86.mk \
    $(LOCAL_DIR)/sdk_x86_64.mk \
    $(LOCAL_DIR)/sdk_x86.mk \

endif

PRODUCT_MAKEFILES += \
    $(LOCAL_DIR)/mainline_sdk.mk \
    $(LOCAL_DIR)/module_arm.mk \
    $(LOCAL_DIR)/module_arm64.mk \
    $(LOCAL_DIR)/module_x86.mk \
    $(LOCAL_DIR)/module_x86_64.mk \

# 配置我们在 lunch 时的选项
COMMON_LUNCH_CHOICES := \
    aosp_arm64-eng \
    aosp_arm-eng \
    aosp_x86_64-eng \
    aosp_x86-eng \

```

AndroidProducts.mk 定义我们执行 lunch 命令时，打印的列表以及每个选项对应的配置文件

PRODUCT_MAKEFILES 用于引入产品的配置文件

COMMON_LUNCH_CHOICES 用于添加 lunch 时的选项，选项的名字由两部分过程 产品名 + 构建模式：

产品名就是 PRODUCT_MAKEFILES 中引入的产品配置文件名去掉 .mk 后缀，例如 aosp_x86_64
构建模式有三种：用户模式 user、用户调试模式 userdebug 和工程模式 eng


接着我们在看下 aosp_x86_64.mk，这个文件就是我们产品配置的主基地：


```Makefile
PRODUCT_USE_DYNAMIC_PARTITIONS := true

# The system image of aosp_x86_64-userdebug is a GSI for the devices with:
# - x86 64 bits user space
# - 64 bits binder interface
# - system-as-root
# - VNDK enforcement
# - compatible property override enabled

# This is a build configuration for a full-featured build of the
# Open-Source part of the tree. It's geared toward a US-centric
# build quite specifically for the emulator, and might not be
# entirely appropriate to inherit from for on-device configurations.

# GSI for system/product
$(call inherit-product, $(SRC_TARGET_DIR)/product/core_64_bit.mk)
$(call inherit-product, $(SRC_TARGET_DIR)/product/gsi_common.mk)

# Emulator for vendor
$(call inherit-product-if-exists, device/generic/goldfish/x86_64-vendor.mk)
$(call inherit-product, $(SRC_TARGET_DIR)/product/emulator_vendor.mk)
$(call inherit-product, $(SRC_TARGET_DIR)/board/generic_x86_64/device.mk)

# Enable mainline checking for excat this product name
ifeq (aosp_x86_64,$(TARGET_PRODUCT))
PRODUCT_ENFORCE_ARTIFACT_PATH_REQUIREMENTS := relaxed
endif

PRODUCT_ARTIFACT_PATH_REQUIREMENT_WHITELIST += \
    root/init.zygote32_64.rc \
    root/init.zygote64_32.rc \

# Copy different zygote settings for vendor.img to select by setting property
# ro.zygote=zygote64_32 or ro.zygote=zygote32_64:
#   1. 64-bit primary, 32-bit secondary OR
#   2. 32-bit primary, 64-bit secondary
# init.zygote64_32.rc is in the core_64_bit.mk below
PRODUCT_COPY_FILES += \
    system/core/rootdir/init.zygote32_64.rc:root/init.zygote32_64.rc

# Product 基本信息
PRODUCT_NAME := aosp_x86_64
PRODUCT_DEVICE := generic_x86_64
PRODUCT_BRAND := Android
PRODUCT_MODEL := AOSP on x86_64
```

**inherit-product** 函数表示继承另外一个文件

```bash
$(call inherit-product, $(SRC_TARGET_DIR)/product/emulator_vendor.mk)
$(call inherit-product-if-exists, device/generic/goldfish/x86_64-vendor.mk)
```

在 Makefile 中可使用 **“-include”** 来代替 “include”，来忽略由于包含文件不存在或者无法创建时的错误提示（“-”的意思是告诉make，忽略此操作的错误。make继续执行）,如果不加-,当 include 的文件出错或者不存在的时候， make 会报错并退出。

```bash
-include $(TARGET_DEVICE_DIR)/AndroidBoard.mk
```

**include 和 inherit-product 的区别**：

* 假设 `PRODUCT_VAR := a` 在 A.mk 中, `PRODUCT_VAR := b` 在 B.mk 中。
* 如果你在 A.mk 中 include B.mk，你最终会得到 `PRODUCT_VAR := b`。
* 但是如果你在 A.mk inherit-product B.mk，你会得到 `PRODUCT_VAR := a b`。并 inherit-product 确保您不会两次包含同一个 makefile 。


pixel4 的开发代号是 flame，pixel4 xl 的开发代号是 coral，这两款手机是同系列同时开发的两款手机，他们的 product 配置文件一起放在了 `device/google/coral`，`device/google/coral-kernel` 和 `device/google/coral-sepolicy` 几个目录下。



## 3. 添加自己的 Product

这里假设我们的公司名叫果冻（Jelly），我们准备开发一款对标苹果 14 的手机，取名叫 大米14（Rice14）。

接下来我们在源码中基于 Aosp_x86_64-eng Product 来添加我们自己的产品（Product）

在 device 目录下添加如下的目录与文件：

```bash
jelly/
└── rice14
    ├── AndroidProducts.mk
    ├── BoardConfig.mk
    └── rice14.mk
```

BoardConfig.mk 包含了硬件芯片架构配置，分区大小配置等信息这里我们直接使用 aosp_x86_64 的 BoardConfig.mk 就行。BoardConfig.mk 拷贝自 build/target/board/generic_x86_64/BoardConfig.mk

```bash
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

Rice14.mk 拷贝自 build/target/product/aosp_x86_64.mk

其中的 if 语句需要注释掉，同时需要修改最后四行

```makefile
PRODUCT_USE_DYNAMIC_PARTITIONS := true

# The system image of aosp_x86_64-userdebug is a GSI for the devices with:
# - x86 64 bits user space
# - 64 bits binder interface
# - system-as-root
# - VNDK enforcement
# - compatible property override enabled

# This is a build configuration for a full-featured build of the
# Open-Source part of the tree. It's geared toward a US-centric
# build quite specifically for the emulator, and might not be
# entirely appropriate to inherit from for on-device configurations.

# GSI for system/product
$(call inherit-product, $(SRC_TARGET_DIR)/product/core_64_bit.mk)
$(call inherit-product, $(SRC_TARGET_DIR)/product/gsi_common.mk)

# Emulator for vendor
$(call inherit-product-if-exists, device/generic/goldfish/x86_64-vendor.mk)
$(call inherit-product, $(SRC_TARGET_DIR)/product/emulator_vendor.mk)
$(call inherit-product, $(SRC_TARGET_DIR)/board/generic_x86_64/device.mk)

# Enable mainline checking for excat this product name
#ifeq (aosp_x86_64,$(TARGET_PRODUCT))
PRODUCT_ENFORCE_ARTIFACT_PATH_REQUIREMENTS := relaxed
#endif

PRODUCT_ARTIFACT_PATH_REQUIREMENT_WHITELIST += \
    root/init.zygote32_64.rc \
    root/init.zygote64_32.rc \

# Copy different zygote settings for vendor.img to select by setting property
# ro.zygote=zygote64_32 or ro.zygote=zygote32_64:
#   1. 64-bit primary, 32-bit secondary OR
#   2. 32-bit primary, 64-bit secondary
# init.zygote64_32.rc is in the core_64_bit.mk below
PRODUCT_COPY_FILES += \
    system/core/rootdir/init.zygote32_64.rc:root/init.zygote32_64.rc

# Overrides
PRODUCT_BRAND := Jelly
PRODUCT_NAME := Rice14
PRODUCT_DEVICE := Rice14
PRODUCT_MODEL := Android SDK built for x86_64 Rice14
```

AndroidProducts.mk 内容如下：

```makefile
PRODUCT_MAKEFILES := \
    $(LOCAL_DIR)/Rice14.mk

COMMON_LUNCH_CHOICES := \
    Rice14-eng \
    Rice14-userdebug \
    Rice14-user \
```

最后验证：

```bash
source build/envsetup.sh
lunch rice14-eng
make -j16
emulator
```

以上示例给出了最简单的模拟器 Product 的添加，主要是用于我们的学习。