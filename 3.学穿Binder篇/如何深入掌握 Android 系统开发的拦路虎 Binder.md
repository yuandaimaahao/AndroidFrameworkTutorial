# 如何深入掌握 Android 系统开发的拦路虎 Binder

## 0. 为什么要深入学习 Binder

* Binder 是整个 Android 的基石
  * 所有的系统服务都是基于 Binder，比如 AMS WMS PMS SurfaceFlinger Audiofilinger 以及硬件操作服务等等
  * Android 四大组件的底层实现离不开 Binder
* 做系统开发需要自定义一些系统服务，这些工作需要我们了解 Binder
* Android O 以后的 Treble 计划,基于 Binder 魔改出了 HwBinder VndBinder。
* ANR 冻屏 卡顿 卡死等偶现 BUG 可能与 Binder 相关 

## 1. 学习 Binder 的四个阶段

* 会用，能添加 Java Native 系统服务
* 熟读应用层各种情景下的源码
* 熟读内核里面的数据结构和流程
* 能解决各种奇奇怪怪的 bug


## 2. 准备工作

下载编译好 AOSP + Kernel，能通过自定义内核的方式启动虚拟机。

这部分内容比较简单，可以参考：

* [AOSP 极速上手](https://mp.weixin.qq.com/s/TEKIz4UEyc4MfPtnDmLv7g)
* [Linux 驱动开发入门第一节](https://mp.weixin.qq.com/s/yc1H7Sh1d8PQo3xH9u6rKw)


## 3. 预备基础知识

预备基础知识快速过一遍，忘了再回头再看

* [Linux 驱动开发入门](https://mp.weixin.qq.com/s/yc1H7Sh1d8PQo3xH9u6rKw)
* [Linux 内核常用数据结构](https://mp.weixin.qq.com/s/P8SwE-dNJotUPYHQWBoKgw)
* [虚拟内存与 Linux 文件访问接口](https://mp.weixin.qq.com/s/sDecEIzkVJmTKEByoG_clg)
* [JNI 编程入门](https://mp.weixin.qq.com/mp/appmsgalbum?__biz=MzIyMDg5MzEyMA==&action=getalbum&album_id=2969089821740597249&scene=173&from_msgid=2247484766&from_itemidx=1&count=3&nolastread=1#wechat_redirect)


## 4. Binder 基本原理

首先要明确一点 Binder 是一个 RPC（Remote Procedure Call） 框架，也就是说借助于 Binder，我们可以在 A 进程中访问 B 进程中的函数。

### 4.1 IPC 原理

RPC 一般基于 IPC（Inter-Process Communication） 来实现的，IPC 就是跨进程数据传输，大白话就是在 A 进程可以访问到 B 进程中的数据，或者说 B 进程中的数据可以传递给 A 进程，都是一个意思。

在 Linux 中，每个进程都有自己的**虚拟内存地址空间**。虚拟内存地址空间又分为了用户地址空间和内核地址空间。

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20221117212450.png)

不同进程之间用户地址空间的变量和函数是不能直接相互访问的。

虽然用户地址空间是不能互相访问的，但是不同进程的内核地址空间是相同和共享的，它们映射的物理地址是相同的，我们可以借助内核地址空间作为中转站来实现进程间数据的传输。

具体的我们在 B 进程使用 copy_from_user 将用户态数据 `int a` 拷贝到内核态，这样就可以在 A 进程的内核态中访问到 `int a`


![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20221117214847.png)

更进一步，可以在 A 进程中调用 copy_to_user 可以将 `int a` 从内核地址空间拷贝到用户地址空间。至此，我们的进程 A 用户态程序就可以访问到进程 B 中的用户地址空间数据 `int a` 了

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20221117215145.png)

为了访问 `int a` ，需要拷贝两次数据。能不能优化一下？我们可以通过 mmap 将进程 A 的用户地址空间与内核地址空间进行映射，让他们指向相同的物理地址空间：

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20221117220739.png)

完成映射后，B 进程只需调用一次 copy_from_user，A 进程的用户空间中就可以访问到 `int a`了。这里就优化到了一次拷贝。

### 4.2 RPC 原理

接着我们来看以下，Binder 的 RPC 是如何实现的：

一般来说，A 进程访问 B 进程函数，我们需要：

* 在 A 进程中按照固定的规则打包数据，这些数据包含了：
  * 数据发给那个进程，Binder 中是一个整型变量 Handle
  * 要调用目标进程中的那个函数，Binder 中用一个整型变量 Code 表示
  * 目标函数的参数
  * 要执行具体什么操作，也就是 Binder 协议 
* 进程 B 收到数据，按照固定的格式解析出数据，调用函数，并使用相同的格式将函数的返回值传递给进程 A。

![](https://gitee.com/androidframeworkahao/pic-bed/raw/master/img/20230625231109.png)

Binder 要实现的效果就是，整体上看过去，进程 A 执行进程 B 中的函数就和执行当前进程中的函数是一样的。


## 5. Binder 应用层工作流程

Binder 是一个 **RPC**（Remote Procedure Call） 框架，翻译成中文就是**远程过程调用**。也就是说通过 Binder：

* 可以在 A 进程中访问 B 进程中定义的函数
* 进程 B 中的这些等待着被远程调用的函数的集合，我们称其为 **Binder 服务（Binder Service）**
* 进程 A 称之为 **Binder 客户端（Binder Client）**，进程 B 称之为 **Binder 服务端（Binder Server）**
* 通常，系统中的服务很多，我们需要一个管家来管理它们，**服务
登录管家（ServiceManager）** 是 Android 系统启动时，启动的一个用于管理 **Binder 服务（Binder Service）** 的进程。通常，**服务（Service）** 需要事先注册到**服务管家（ServiceManager）**，其他进程向**服务管家（ServiceManager）** 查询服务后才能使用服务。
* Binder 的 RPC 能力通过 **Binder 驱动**实现


通常一个完整的 Binder 程序涉及 4 个流程：
1. 在 Binder Server 端定义好服务
2. 然后向 ServiceManager 注册服务
3. 在 Binder Client 中向 ServiceManager 获取到服务
4. 发起远程调用，调用 Binder Server 中定义好的服务

整个流程都是建立在 Binder 驱动提供的跨进程调用能力之上：

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230201090305.png)

## 6. Android Binder 整体架构

从源码实现角度来说，Binder 整体架构实现如下：

![](https://gitee.com/androidframeworkahao/pic-bed/raw/master/img/20230626095048.png)

有点复杂，我们一点点说：

* VFS 是内核中的一个中间层，向上对应用层提供统一的系统调用函数，这些系统调用函数主要是 `open mmap ioctl write read ` 等，向下封装不同的外设（字符设备，块设备），特殊系统文件，文件系统的操作。Binder 是一个字符驱动，当应用层调用到 binder 的 `open mmap ioctl release` 系统调用时，经过 vfs 的一层包装后，就会调用到 Binder 驱动中的 `binder_open bider_mmap binder_ioctl binder_release` 函数。

* 不同于一般的驱动，Binder 应用层的使用要复杂不少，如果直接使用 `open mmap ioctl release` 系统调用会使得应用程序非常复杂且难以复用相同功能的代码，刚开始 google 的工程师做了一套简单的封装，把常用的操作封装为一系列的函数，这些函数都在 binder.c 中，ServiceManger 的就是通过 binder.c 中封装的函数实现的（Android10及以前）。源码中还存在一个 bctest.c 的程序，这个是 binder.c 的一个测试程序。C 语言级别的封装虽然简单，但使用起来还是稍显麻烦，很多细节也没有考虑进去，所以 google 的工程师又封装了一个叫 libbinder 的库，我们 native 层的 binder 服务端与客户端都是基于这个库来实现的，Java 层的 binder 服务端与客户端都是通过 JNI 间接使用 libbinder 库实现的，从使用上来说 libbinder 更为简单，但是 libbinder 本身比 binder.c 复杂了不少。

## 7. C 层实现分析

AOSP 源码中有一个 binder.c 源文件，对 binder 驱动的基本操作 `open ioctl mmap close` 做了封装以适应和简化应用层程序的编写。

很多博客教程会忽略这一层的分析，相比 libbinder 库 C++ 的封装，binder.c 会简单不少 ，方便初学者理解 binder 应用层工作流程。

我们可以模仿 bctest.c service_manager.c 写一个完整的 Binder 应用层 demo。

这个工作已经有大佬完成了:

https://github.com/weidongshan/APP_0003_Binder_C_App

但是也有一些问题，这个代码是基于 Android5 的，稍微有点老了，我在以上实现的基础上做了一些修改和适配工作，使得代码可以在 Android10 上跑起来：

https://github.com/yuandaimaahao/AndroidFrameworkTutorial/tree/main/3.Binder/src/BinderCDemo

关于这个示例程序的分析，可以参考以下几篇文章：

* [Binder 程序示例之 C 语言篇](https://mp.weixin.qq.com/s/g8sEXI6gPx2zVinKMjWP5w)
* [Binder 服务注册过程情景分析之 C 语言篇](https://mp.weixin.qq.com/s/FAVuQLawn4WpvAi7Ni4RNw)
* [Binder 服务获取与使用过程情景分析之 C 语言篇](https://mp.weixin.qq.com/s/X5qv4faWMSSEOeppWLcl6A)

## 8. 驱动分析

驱动分析这部分结合 C 层应用的实现来分析驱动的实现，主要搞清楚：

* 三个情景的流程：注册，获取，使用
* 三个情景下内核中各种数据结构的变化

这部分内容可以参考之前分享的：

* [驱动情景分析之 ServiceManager 启动过程](https://mp.weixin.qq.com/s/aVv0w2kMZCEc4KuMocE-Bw)
* [驱动情景分析之服务注册过程](https://mp.weixin.qq.com/s/gK8labbQBHI5ZFK6g9oiVA)
* [驱动情景分析之服务获取与使用过程](https://mp.weixin.qq.com/s/IiOUGJhlJuo4eLXTH8l4Vg)


## 9. C++ 层分析

首先我们要写一个基于 libbinder 库的 Demo，能跑起来就行：

* [Binder 程序示例之 C++ 篇](https://mp.weixin.qq.com/s/OHIi8xNEp0kSKWSQMPAf9g)
* [Binder 程序示例之 aidl-cpp 篇](https://mp.weixin.qq.com/s/nflAjyWtqme45aRjR9VibA)

接着分析三个情景下的执行过程与各个类与函数的功能：

* [Binder C++ 程序分析之主要类解析](https://mp.weixin.qq.com/s/UGcg7xK82nBI0fiK_efKjg)
* [Binder 服务注册过程情景分析之 C++ 篇](https://mp.weixin.qq.com/s/2_07Dk_oMc46OtN8CpMIwg)
* [Binder 服务获取与使用过程情景分析之 C++ 篇](https://mp.weixin.qq.com/s/2_07Dk_oMc46OtN8CpMIwg)


当然还有几个特殊的场景也需要进行分析：

* 死亡通知
* 多线程
* 匿名服务

这部分内容会陆续在公众号和掘金平台推送。


## 10. Java 层分析

学习这部分的前提是了解 JNI 编程。这个可以参考系列文章：

* [JNI 编程上手指南](https://mp.weixin.qq.com/s/lE-VjZrhHLsIsthWt1h92w)

我们先写一个 Demo，能跑起来就行：

* [Binder 程序示例之 java 篇](https://mp.weixin.qq.com/s/gUjeYLoi99h0Cwx_UalYng)


接着我们分析三个情景下的执行过程与各个函数的功能：

* [Binder Java 层初始化](https://mp.weixin.qq.com/s/X1-k1nA8njl51YfakSriYw)
* [Binder Java 层服务注册过程分析](https://mp.weixin.qq.com/s/3dOH03ViHEA2onkwStK6gA)
* Binder Java 层服务获取与使用过程分析

当然还有一些其他高级特性也需要我们分析，这部分内容会在后续推送：

* AIDL 中 in out inout oneway 的分析
* Parcel 数据结构分析
* Java 层死亡通知
* Java 层多线程分析
* 匿名服务


## 11. 疑难问题

不论是应用开发还是系统开发我们都会遇到一些棘手的 bug，很多时候这些 bug 都和 binder 有关，总结起来，大概可以分为几类：

* 死锁
* 线程池满了
* 代理对象内存泄露
* 传输数据过大
* 关键方法内发起 Binder 同步调用导致卡顿
* Android O 异步远程调用无限阻塞冻屏 bug

这类 bug 很多都难以复现，很多时候都不了了之了，导致拥有这部分经验的同学很少。

这部分内容工作量巨大，我会在接下来的时间陆续在公众号和掘金推送相关的文章。

# 关于

我叫阿豪，2015 年本科毕业于国防科技大学指挥自动化专业，毕业后，从事信息化装备的研发工作。主要研究方向为 Android Framework 与 Linux Kernel，2023年春节后开始做 Android Framework 相关的技术分享。

如果你对 Framework 感兴趣或者正在学习 Framework，可以参考我总结的[Android Framework 学习路线指南](https://mp.weixin.qq.com/s/cPQpyqeuOCRp1lpa3HN5Gg),也可关注我的微信公众号，我会在公众号上持续分享我的经验，帮助正在学习的你少走一些弯路。学习过程中如果你有疑问或者你的经验想要分享给大家可以添加我的微信，我拉你进技术交流群。

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/4e7348e352774883ecb19ab021d6cee.jpg)