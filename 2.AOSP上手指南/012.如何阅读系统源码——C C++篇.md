# 如何阅读 Android 系统源码 —— C/C++ 篇 

## 1. 工具篇

对于 Android 系统源码中的 C/C++ 代码，CLion 是一个不错的工具。

较新版本的 Android 源码支持使用 AIDEgen 调用 Clion 查看 C/C++ 代码。但是，对于我们学习使用的 Android10 是不支持的。不过我们可以通过其他办法实现 Clion 查看 C/C++ 代码：

```bash
# 准备工作
source build/envsetup.sh
lunch aosp_x86_64-eng #选择一个合适的 Product
export SOONG_GEN_CMAKEFILES=1
export SOONG_GEN_CMAKEFILES_DEBUG=1
make -j16
```

接着我们就可以使用 Clion 打开我们的代码了。

假设我们需要看 SurfaceFlinger 相关代码:

```bash
#系统源码目录下搜索
find . -name "SurfaceFlinger*"
./frameworks/native/services/surfaceflinger/SurfaceFlinger.cpp
./frameworks/native/services/surfaceflinger/SurfaceFlingerProperties.cpp
./frameworks/native/services/surfaceflinger/SurfaceFlinger.h
./frameworks/native/services/surfaceflinger/SurfaceFlingerProperties.h
./frameworks/native/services/surfaceflinger/SurfaceFlingerFactory.h
.......
```

这里我们知道 SurfaceFlinger 定义在 `frameworks/native/services` 目录:


接着我们打开 Clion，点击 Open：

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230512043810.png)

选择 `out/development/ide/clion/frameworks/native` 目录

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230512044216.png)

这样我们就可以使用 CLion 查看系统源码了，需要注意的是我们的源码需要在 External Libraries 中查看：

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230512044745.png)

我们也可以通过点击 Change Project Root 按钮调整目录结构：

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230512054649.png)

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230512054745.png)


## 2. 手段篇

阅读源码主要两个手段：

* 打印 Log + 打印调用堆栈
* 使用 CLion 调试

这里我们修改 SurfaceFlinger 的主函数 main_surfaceflinger.cpp 来演示打印 Log 和打印调用堆栈：

```c++
//log的头文件
#include "log/log.h"
//直接 define LOG_TAG 会报已定义错误，因为 SurFaceFlinger 模块的 Android.bp 已经定义了 LOG_Tag
//下面这样定义就不会出错了
#ifdef LOG_TAG
#undef LOG_TAG
#define LOG_TAG "yuandaima_sf"
#endif

//打印堆栈的头文件
#include <utils/CallStack.h>

//在 main 函数中打印信息

int main(int, char**) {

    //打印日志
    ALOGD("surfaceflinger is starting");

    //打印堆栈
    android::CallStack callStack(LOG_TAG, 1);

    //省略后面的代码
    //......
}
```

修改 /frameworks/native/services/surfaceflinger/Android.bp,添加 CallStack 的库依赖：

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230512060159.png)


接着我们重新编译代码，启动模拟器，进入 adb shell，查看 log：

```bash
rice14:/ # logcat | grep yuandaima_sf
05-11 09:54:10.291  1531  1531 D yuandaima_sf: surfaceflinger is starting

05-11 09:54:10.296  1531  1531 D yuandaima_sf: #00 pc 00000000000030a1  /system/bin/surfaceflinger (main+65)
05-11 09:54:10.296  1531  1531 D yuandaima_sf: #01 pc 000000000008a985  /apex/com.android.runtime/lib64/bionic/libc.so (__libc_init+117)
```

通过 log 信息我们可以知道程序的运行状态和运行过程中的关键参数。通过调用栈我们可以知道函数的执行流程。


我们还可以通过 Clion 来调试 C/C++ 代码:

这里以调试 service_manager.c 为例：

我们在如下位置打印好断点：

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230512054931.png)

接着配置远程调试：

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230512055029.png)

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230512055121.png)

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230512055156.png)

这样我们的 Clion 就配置好了。

接着我们查看 servicemanage 进程的 pid：

```bash
adb shell ps -A | grep servicemanager 
system        1406     1   14116   5532 binder_ioctl        0 S servicemanager
system        1407     1   21764   9772 SyS_epoll_wait      0 S hwservicemanager
system        1408     1   14816   2584 binder_ioctl        0 S vndservicemanage
```

servicemanager 的 pid 为 1406，接着在模拟器上开启 gdbserver：

```bash
adb forward tcp:1235 tcp:1235
adb shell gdbserver64 :1235 --attach 1406
```

接着点击 Clion 右上角的 debug 按钮就进入 debug 环境了：

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230512055546.png)


这样我们就可以开始调试 C/C++ 代码了。

## 关于


如果你对 Framework 感兴趣或者正在学习 Framework，可以参考我总结的[Android Framework 学习路线指南](https://github.com/yuandaimaahao/AndroidFrameworkTutorial),也可关注我的微信公众号，我会在公众号上持续分享我的经验，帮助正在学习的你少走一些弯路。学习过程中如果你有疑问或者你的经验想要分享给大家可以添加我的微信，我拉你进技术交流群。

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/4e7348e352774883ecb19ab021d6cee.jpg)
