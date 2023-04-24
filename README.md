# Android Framework 学习路线指南

## 1. 为什么要学习 Android Framework？

作为一名应用层 App 开发工程师，我们为什么要学习 Android Framework？答案很简单，为了不被淘汰！在 2023 年的当下,不会点 `Binder WMS AMS PMS` 好像都找不到工作了，更过分的是应聘企业甚至希望你会点 `RN Fluter` 等跨平台技术，顺便能掌握点前端技术就更好了，活生生的面试造火箭！对于市面上 99% 的 App 的开发是不需要我们掌握 Framework 的，企业去面试这些，只是为了筛人罢了，因为收到的简历确实太多了，常规的题目已经无法对应聘者做出筛选了。

对于 Android 应用开发工程师，如果想要长期的在这个行业干下去，大的方向上大概有两条相对靠谱的进阶路线可以选择：

* 大前端路线：掌握 Android IOS 双端原生开发，跨平台开发，前端开发
* 系统开发路线：掌握 Linux 系统开发技术，熟悉 Framework 层核心组件原理，了解 Linux 内核

大前端路线出现的很大原因是企业挣不到钱了，养不起人了，希望一个人能干几个人的活，这种岗位主打的是性价比。

不得不承认的是，移动互联网的黄金年代已经过去，大型 App 的开发机会几乎绝迹，更多的是短平快的小项目。对于这些短平快的小项目，5 年以上开发人员的优势非常小，在企业的角度甚至是减分项，为什么这么说？老开发工资高，加班接受度低，不吃老板画的饼了..... 相比之下，2-3 年工作经验的新人更受欢迎，工资低，有冲劲，能加班。最致命的问题是， App 开发，无论是原生，前端，跨平台技术栈，变化非常快。老开发通常有家庭需要照顾，精力有限，学习速度很可能是赶不上新人。以上种种，导致了 35 岁问题！

35 岁问题出现最核心的原因是**你积累的经验没有形成新人入行的门槛**。简单地说，你的工作不吃经验！

Android 系统开发涉及的知识面很广，代码量大，复杂度高，相关的学习资料也非常匮乏。新人上手有较大的门槛，无形中就形成了岗位的护城河。

从 2008 年发布 Android1.0 到现在，Android 迭代了很多版本，但核心基础组件是一直没有大的变化的（比如 Binder，应用层四大组件，Handler 等），也就是说系统开发积累的经验是可以积累并形成壁垒的。

废了这么多话，归根结底一句话——**Framework 是值得应用开发投入精力来学习的**

以下是 Android Framework 核心基础的学习路线图：

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/%E5%86%99%E7%BB%99%E5%BA%94%E7%94%A8%E5%BC%80%E5%8F%91%E7%9A%84%20Android%20Framework%20%E6%95%99%E7%A8%8B.jpg)

清晰的学习路线图可以在 ` 链接：https://pan.baidu.com/s/1F0Ibi8pHcYT39HASy3zvNw 提取码：1234  ` 处下载到。 


学习路线一共分为 11 个专题，接下来看看这 11 个专题都需要学习哪些内容：

## 2. 基础篇

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230417153209.png)

基础知识涉及的面很广，我们应该做好长期学习的准备，学习过程中做好笔记，及时复习和记忆。另外，我们应该一边学习系统源码，一遍学习基础，遇到不会的，再回头看看基础，形成知识记忆到应用的闭环。千万不要基础学完了，再去学习系统源码，效率低，效果差。

基础部分我们主要掌握以下的学习内容：


* Linux的基本使用：推荐学习 《Linux 运维之道 第二版》
* 编程语言
  * C 语言：《一站式学习C编程》、《C语言非常道》、《嵌入式C语言自我修养》
  * C++ ：《21天学通 C++》
  * Java：《Java 核心技术》
  * JNI：这个没有特别合适的书籍推荐，可以关注我的博客和公众号，后续会分享一些我的学习笔记。
* 工具类编程语言和软件构建工具，学习这部分内容，主要是为了学习 Android 的编译系统，以及在日常工作中能完成一些自动化脚本的编写，提升工作效率
  * Makefile：这个没有特别推荐的资料，可以关注我的博客和公众号，后续会分享一些我的学习笔记。
  * Soong：这个主要就是参看源码中的写法以及参考官方提供的[文档](https://ci.android.com/builds/submitted/9155974/linux/latest/view/soong_build.html)
  * Shell 脚本，推荐学习 《Linux Shell 核心编程指南》
  * Python，推荐学习 《Python 编程从入门到实战》
  * Go，推荐学习 [Go 语言从入门到实战](https://time.geekbang.org/course/intro/100024001?tab=intro)

* Linux 系统编程是比较重要的部分
  * [Linux编程技术](https://www.icourse163.org/course/XIYOU-1461794181?from=searchPage&outVendor=zw_mooc_pcssjg_),这是西安邮电大学提供的一套 mooc 课程，可供入门学习使用
  * 《Linux 系统编程 杰克-本尼 佩尔松》
  * 《UNIX 环境高级编程》《Linux/Unix 系统编程手册》：这两本书可以作为编程过程中的字典使用
* Linux 内核入门与操作系统基础
  * [操作系统及Linux内核](https://www.icourse163.org/course/XIYOU-1461809182?from=searchPage&outVendor=zw_mooc_pcssjg_)， 这是西安邮电大学提供的一套 mooc 课程，可供入门学习使用
  * 《深入理解 Linux 内核 第三版》   
  * 《奔跑吧Linux 内核 系列书籍》 
  * Linux 驱动开发入门：推荐韦东山老师 B 站分享的系列课程

基础部分已分享内容：
  * [Ubuntu 使用快速入门](https://juejin.cn/post/7203571284558381117)
  * [Make 构建工具入门](https://juejin.cn/post/7203931072261193787)
  * [理解 Unicode UTF-8 UTF-16 UTF-32](https://juejin.cn/post/7207365636694458425)
  * [Linux Shell 脚本编程入门——核心基础语法](https://juejin.cn/post/7222931700439285817)
  * [SEAndroid 使用极速上手](https://juejin.cn/post/7208472817460248637)
  * [理解 C++ 的 Memory Order](https://juejin.cn/post/7216182763237146681)
  * [Linux Shell 脚本编程入门1——核心基础语法](https://juejin.cn/post/7222931700439285817)
  * [Linux Shell 脚本编程入门2——脚本自动化基础](https://juejin.cn/post/7223669247711887416)
  * [Android 平台如何编译执行 C C++ 可执行程序](https://juejin.cn/post/7224898006842097725)

## 3. AOSP快速上手路线

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230417153046.png)

这部分内容主要是让大家上手 Android 系统源码，对系统开发有一个感性的认知，主要有以下内容：

* [AOSP 极速上手 视频教程](https://juejin.cn/post/7202634945171537977)
* [系统开发工具 视频教程](https://juejin.cn/post/7216495812577427517)
* [添加 Product](https://juejin.cn/post/7203958049983529015)
* pixel4 源码编译与刷机
* 系统开发案例——儿童学习机家长模式初探
* 系统模块添加
    *   [添加 C/C++、Java 可执行程序](https://juejin.cn/post/7216624116337508412)
    *   [添加 C/C++、Java 库](https://juejin.cn/post/7217279252316045372)
    *   [添加配置文件与删除已有模块](https://juejin.cn/post/7217644586868391996)
* 系统 App 开发入门 
  * [系统 App 源码添加](https://juejin.cn/post/7207374216127103033)             
  * [使用 Android Studio 开发系统 App](https://juejin.cn/post/7207358268804579386)      
* [添加开机自启动 Shell 脚本](https://juejin.cn/post/7219712310586474553)
* 添加开机自启动 C++ 程序
* APK 预制详解
* 系统源码管理工具 Gerrit 搭建与使用


## 4.Binder

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230417153146.png)

Binder 是一套 linux 上的 **远程过程调用（RPC）** 编程框架，是整个 Android 系统的核心基础组件，在体验的系统开发的“快乐”以后，我们需要第一时间掌握 Binder 的里里外外。计划分享以下内容：

* [Binder 基本原理](https://juejin.cn/post/7210175991837392933)
* [Binder 程序示例之 C 语言篇](https://juejin.cn/post/7210245482861264955)
* [Binder 服务注册过程情景分析之C语言篇](https://juejin.cn/post/7214342319347712057)
* [Binder 服务获取与使用过程情景分析之C语言篇](https://juejin.cn/post/7215401973842706491)
* [Binder C++ 程序示例](https://juejin.cn/post/7222109036588810297)
* [Binder C++ 程序分析之主要类解析](https://juejin.cn/post/7223185420157763641/)
* [Binder 服务注册过程情景分析之 C++ 篇](https://juejin.cn/post/7223185420157943865)
* Binder 服务获取与使用过程情景分析之 C++ 篇
* Binder 程序示例之 Java 篇
* Binder Java 程序分析之主要类解析
* Binder 服务注册过程情景分析之 Java 篇
* Binder 服务获取与使用过程情景分析之 Java 篇
* Binder 多线程分析
* Binder 匿名服务分析
* Binder 驱动实现分析上
* Binder 驱动实现分析中
* Binder 驱动实现分析下
* Binder 相关疑难 bug 解析
* Binder 面试题解析

## 5.基础组件

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230417153351.png)

所谓基础组件，就是系统源码中使用的较多的模块，需要学习的内容主要包括：

* Android 源码中的智能指针
* Android 属性系统使用与分析
* Android 日志系统使用与分析
* Android 中的权限管理
* Android 匿名共享
* 基础组件面试题解析

## 6.Hal 与硬件服务

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230417153441.png)

Hal 主要是对驱动功能做一些封装，下接驱动，上接 Framework，主要学习以下内容：



* [Kernel 下载与编译](https://juejin.cn/post/7207602567290765373)
* [Linux 驱动开发入门](https://juejin.cn/post/7207607724900810812)
* C 程序访问我们的驱动
* Hal 层添加访问 Hello Dirver 的接口
* 通过 JNI 访问到 Hal 层接口
* Framework 中添加访问 Hello Driver 的硬件服务 
* 编写 App 访问硬件服务 
* HwBinder 分析上
* HwBinder 分析中
* HwBinder 分析下
* Hal 层面试题分析

## 7.构造系统

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230417153458.png)

有的时候我们可能需要修改构造系统的行为，这需要我们对 AOSP 的构造系统有基本的认识，下面是构造系统的学习顺序：

* AOSP 构造系统的演进
* 构造系统初始化
* 编译过程解析
* 打包流程解析
* 构造系统面试题分析
  
## 8. 应用层框架

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230417153554.png)

应用层框架是应用开发接触最频繁的组件，主要掌握以下几个方面：

* AMS/ATMS 与四大组件
  * Activity 启动过程与进程启动
  * Service 启动
  * ContentProvider 实现原理
  * 广播机制
* Handler与Looper消息处理机制
* PackageManagerService 工作流程与原理
* 应用层框架面试题分析

## 9.系统启动过程

系统启动是我们常打交道的部分，其流程如下图所示：

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230417154556.png)

图片来自[这里](https://github.com/jeanboydev/Android-ReadTheFuckingSourceCode/blob/master/article/android/framework/Android-%E7%B3%BB%E7%BB%9F%E5%90%AF%E5%8A%A8%E8%BF%87%E7%A8%8B.md)

主要熟悉以下内容：

* 系统启动过程整体流程
* init 进程解析
* zygote 进程解析
* SystemServer 进程解析
* 系统启动过程面试题分析

## 10.Android 图形系统

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230417154734.png)

Android 图形系统是系统框架中一个非常重要的子系统，我们从以下几个方面来学习和理解图形系统的整体框架：

* 图形系统整体框架
* Hal 层原理分析
* SurfaceFlinger 实现与剖析
* WMS 实现与剖析
* Android View 系统分析
* VSync 原理解析
* Android 图形系统面试题分析
  
## 11.Android 输入系统

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230417154922.png)

输入系统主要分为输入子系统和 IMS，我们重以下几个角度来学习 Android 的输入系统：

* 输入系统整体框架
* 事件的读取
* 事件的分发
* 事件的处理
* Android 输入系统面试题分析

## 12.系统应用

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230417154938.png)

系统开发的大部分内容是做系统应用开发，常修改的系统应用如下：

* Settings
* Launcher
* SystemUI
* 系统应用开发相关面试题分析

## 总结

在掌握了这 11 个专题后，不论是继续研究 Framework 中其他模块，或是参与 App 开发，又或是研究内核与驱动，我相信，你一定早已是是游刃有余了！

## 关于

如果你对 Framework 感兴趣或者正在学习 Framework，可以关注我的微信公众号，我会在公众号上持续分享我的经验，帮助正在学习的你少走一些弯路。学习过程中如果你有疑问或者你的经验想要分享给大家可以添加我的微信，我拉你进技术交流群。

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/4e7348e352774883ecb19ab021d6cee.jpg)