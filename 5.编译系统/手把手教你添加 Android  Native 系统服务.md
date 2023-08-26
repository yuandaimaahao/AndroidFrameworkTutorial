# 手把手教你添加 Android Native 系统服务

工作学习过程中，我们可能需要去阅读不同类型的 Native 系统服务，也有可能会自己去完成一个 Native 系统服务。无论哪种情况都需要我们了解基本的 Native 如何去添加。就像我们写 Android App 得先了解一下四大组件才行。接着我们就来看看如何添加一个 Android Native 系统服务。使用的源码版本是 AOSP android_r41。 

文章配套源码：https://github.com/yuandaimaahao/HelloNativeService

## 开机自启动 Native 程序

首先，我们先来完成一个开启自动动的 Native 程序:

首先我们在我们的自定义 Product `device/jelly/rice14`  下创建如下的文件与文件夹：

关于自定义 Product，请查看 https://yuandaimaahao.github.io/AndroidFrameworkTutorialPages/0 02.%E7%8E%A9%E8%BD%ACAOSP%E7%AF%87/003.%20%E6%B7%BB%E5%8A%A0%20Product.html

```bash
HelloNativeService/
├── Android.bp
├── HelloServer.cpp
└── HelloServer.rc
```

其中 HelloServer.cpp：

```cpp
#define LOG_TAG "helloserver"
#include <log/log.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
    
    ALOGD("Hello Server is runing");
    
    while(1) 
    {
        sleep(1);
    }
    
    return 0;
}
```

这是我们的主程序，打印一个 Log，然后进入无线循环。

init.rc 脚本 HelloServer.rc:

```rc
service HelloServer /system/bin/HelloServer
    class core
    user system
    group system
```

当启动启动的时候，init 程序会解析我们的 init.rc 教程，并启动我们的程序。

接着，我们需要编写我们的 Android.bp 文件：

```json
cc_binary {
    name: "HelloServer",
    srcs: ["HelloServer.cpp"],
    shared_libs: [
        "liblog",
    ],
    init_rc: ["HelloServer.rc"],
}
```

接着，改编译文件 rice14.mk ：

```Makefile
PRODUCT_ARTIFACT_PATH_REQUIREMENT_WHITELIST +=\
    /system/bin/HelloClient

PRODUCT_PACKAGES += \
    HelloServer
```

最后我们，编译运行我们的程序：

```bash
source build/envsetup.sh
lunch rice14-eng
make -j32
# 进入 Android 模拟器
adb shell 
logcat | grep Hello
```

接着我们就可以看到打印的 Log 了：

```bash
07-16 16:25:06.670  1530  1530 D helloserver: Hello Server is runing
```

说明，我们的开机自启动程序就启动成功了

## 添加 Native 服务

接着我们在 `device/jelly/rice14/HelloNativeService` 目录下创建包目录 `com/yuandaima`

接着在包目录下创建：

```java
package com.yuandaima;

interface IHello {
    void hello();
    int sum(int x, int y);
}
```

接着在项目目录下执行下面的命令，生产源文件：

```bash
aidl-cpp com/yuandaima/IHello.aidl ./ ./IHello.cpp
```

接着我们完善 HelloServer 程序

```cpp
#define LOG_TAG "helloserver"
#include <log/log.h>

#include <unistd.h>
#include <stdlib.h>
#include <utils/RefBase.h>
#include <utils/Log.h>
#include <binder/TextOutput.h>
#include <binder/IInterface.h>
#include <binder/IBinder.h>
#include <binder/ProcessState.h>
#include <binder/IServiceManager.h>
#include <binder/IPCThreadState.h>

#include "com/yuandaima/IHello.h"
#include "com/yuandaima/BnHello.h"

using namespace android;

class MyHelloService : public com::yuandaima::BnHello
{
    public:
    binder::Status hello() 
    {
        ALOGI("server hello function is running");
        return binder::Status();
    }


    binder::Status sum(int32_t x, int32_t y, int32_t* _aidl_return) 
    {
         ALOGI("server sum function is running");
         *_aidl_return = x + y;
         return binder::Status();
    }
    
};

int main(int argc, char const *argv[])
{
    
    ALOGD("Hello Server is runing");
    
    defaultServiceManager()->addService(String16("MyHelloService"), new MyHelloService());
    ProcessState::self()->startThreadPool();
    IPCThreadState::self()->joinThreadPool();
    
    return 0;
}
```

接着我们写一个 HelloClient 来测试我们的服务程序：

```java
#define LOG_TAG "aidl_cpp"
#include <log/log.h>

#include <stdlib.h>
#include <utils/RefBase.h>
#include <utils/Log.h>
#include <binder/TextOutput.h>
#include <binder/IInterface.h>
#include <binder/IBinder.h>
#include <binder/ProcessState.h>
#include <binder/IServiceManager.h>
#include <binder/IPCThreadState.h>

#include "com/yuandaima/IHello.h"
#include "com/yuandaima/BpHello.h"

using namespace android;

int main(int argc, char const *argv[])
{
    sp<IServiceManager> sm = defaultServiceManager();
    sp<IBinder> binder = sm->getService(String16("MyHelloService"));
    sp<com::yuandaima::IHello> hello = interface_cast<com::yuandaima::IHello>(binder);

    hello->hello();
    int ret = 0;
    hello->sum(1, 2, &ret);
    return 0;
}
```

然后，完善 Android.bp

```bash
cc_binary {
    name: "HelloServer",
    srcs: ["HelloServer.cpp", "IHello.cpp"],
    shared_libs: [
        "liblog",
        "libcutils",
        "libutils",
        "libbinder",
    ],
    init_rc: ["HelloServer.rc"],
}


cc_binary {
    name: "HelloClient",
    srcs: ["HelloClient.cpp", "IHello.cpp"],
    shared_libs: [
        "liblog",
        "libcutils",
        "libutils",
        "libbinder",
    ],
}
```


## Selinux 配置

我们需要修改系统的 sepolicy 文件，不能在自定义 Product 的 sepolicy 中添加 selinux 配置，因为会被系统的 seplicy 给覆盖掉。

在 `system/sepolicy/private` 和`system/sepolicy/prebuilts/api/29.0/private` 中添加：

helloserver.te：

```bash
type helloserver_dt, domain, coredomain;
type helloserver_dt_exec, exec_type, file_type, system_file_type;

init_daemon_domain(helloserver_dt)

allow helloserver_dt servicemanager:binder { call transfer };
allow helloserver_dt HelloServer_service:service_manager { add find };

binder_use(helloserver_dt)
add_service(helloserver_dt,HelloServer_service)
```

编译时，编译系统会同时检查这两个目录，如果不同就会报错，所以我们要同时修改两个地方。

在 `system/sepolicy/private/file_contexts` 和 `system/sepolicy/prebuilts/api/29.0/private/file_contexts` 中添加：

```bash
/system/bin/HelloServer     u:object_r:helloserver_dt_exec:s0

```
注意 file_contexts 最后一行必须是空行，不然无法编译过。

在 `system/sepolicy/private/service_contexts` 和 `system/sepolicy/prebuilts/api/29.0/private/service_contexts`  中倒数第二行添加

```bash
MyHelloService                            u:object_r:HelloServer_service:s0

```
注意 service_contexts 最后一行必须是空行，不然无法编译过。

在 `system/sepolicy/private/service.te ` 和 `system/sepolicy/prebuilts/api/29.0/private/service.te`
最后一样中添加：

```bash
type HelloServer_service,           service_manager_type;
```


最后编译启动模拟器：


```bash
source build/envsetup.sh
lunch rice14-eng
make -j32
# 进入 Android 模拟器
adb shell 
logcat | grep hello
```
然后就可以看到 Log 了：

```bash
07-16 16:42:11.616  1534  1534 D helloserver: Hello Server is runing
```

接着我们运行我们的客户端程序，再查看 Log：

```bash
logcat | grep "hello"                                                           
07-16 16:57:46.794  1531  1531 D helloserver: Hello Server is runing
07-16 16:58:52.638  1531  1577 I helloserver: server hello function is running
07-16 16:58:52.638  1531  1577 I helloserver: server sum function is running
```

这样，我们的远程调用就成功了。



最后给同学们留一个问题，HelloClient 为什么没有配 Selinux 权限也能执行，如果要配置，应该怎么配置？


## 关于

我叫阿豪，2015 年本科毕业于国防科学技术大学指挥信息系统专业，毕业后从事信息化装备的研发工作，主要研究方向是 Android Framework 与 Linux Kernel。

如果你对 Android Framework 感兴趣或者正在学习 Android Framework，可以关注我的微信公众号，我会持续分享我的学习经验，帮助正在学习的你少走一些弯路。学习过程中如果你有疑问或者你的经验想要分享给大家可以添加我的微信，我拉你进技术交流群。

如果你想系统学习 Anroid Framework 也可以关注我的抖音账号，在主页店铺即可购买付费课程。

![](https://cdn.jsdelivr.net/gh/zzh0838/MyImages@main/img/%E4%BA%8C%E7%BB%B4%E7%A0%81.jpg)
