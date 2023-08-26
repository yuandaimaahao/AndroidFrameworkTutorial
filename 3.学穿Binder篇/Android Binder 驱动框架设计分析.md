# Android Binder 驱动框架设计与分析

## 1. Binder 应用层框架

![](https://cdn.jsdelivr.net/gh/zzh0838/MyImages@main/img/20230718234054.png)

在应用层，Binder 是一个 CS 架构，涉及了 Client 与 Server 两端。特殊的是，Binder 中还有一个 ServiceManager。那 ServiceManager 是干什么用的？ 

首先我们要明白 Binder 是一个 **RPC**（Remote Procedure Call） 框架，翻译成中文就是**远程过程调用**。也就是说在 Client 进程中可以访问 Server 进程中的函数。

* Server 进程中的这些等待着被远程调用的函数的集合，我们称其为**Binder 服务（Binder Service）**
* 通常，**服务（Service）** 需要事先注册到**服务管家（ServiceManager）**。
* **服务管家（ServiceManager）** 是 Android 系统启动时，启动的一个用于管理 **Binder 服务（Binder Service）** 的进程，其内部通过一个链表记录服务的相关信息。
* 服务（Service）注册到服务管家（ServiceManager）后，Client 才能从服务管家（ServiceManager）获取到服务的必要信息，
* Binder 的跨进程数据传输能力是通过 Binder 驱动实现的


## 2. 跨进程数据传输（IPC）基本原理

Binder 是一个 RPC 框架，RPC 通常基于 IPC（跨进程数据传输） 实现。接下来我们看看 Binder 的 IPC 实现。

在 Linux 中，每个进程都有自己的**虚拟内存地址空间**。虚拟内存地址空间又分为了用户地址空间和内核地址空间。

![](https://cdn.jsdelivr.net/gh/zzh0838/MyImages@main/img/20230717114411.png)

不同进程之间用户地址空间映射到不同的物理地址，所以不同进程之间用户地址空间的变量和函数是不能相互访问的。

虽然用户地址空间是不能互相访问的，但是不同进程的内核地址空间是映射到相同物理地址的，它们是相同和共享的，我们可以借助内核地址空间作为中转站来实现进程间数据的传输。

具体地，我们在 B 进程使用 copy_from_user 内核函数将用户态数据 int a 拷贝到内核态，这样就可以在 A 进程的内核态中访问到 int a

![](https://cdn.jsdelivr.net/gh/zzh0838/MyImages@main/img/20230717121708.png)

更进一步，可以在 A 进程中调用 copy_to_user 可以将 int a 从内核地址空间拷贝到用户地址空间。至此，我们的进程 A 用户态程序就可以访问到进程 B 中的用户地址空间数据 int a 了

![](https://cdn.jsdelivr.net/gh/zzh0838/MyImages@main/img/20230703172958.png)

为了访问 `int a` ，需要拷贝两次数据。能不能优化一下？我们可以通过 mmap 将进程 A 的用户地址空间与内核地址空间进行映射，让他们指向相同的物理地址空间,同时，B 进程对应的内核地址空间同样指向这块物理内存：

![](https://cdn.jsdelivr.net/gh/zzh0838/MyImages@main/img/20230703173006.png)

完成映射后，B 进程只需调用一次 copy_from_user，A 进程的用户空间中就可以访问到 `int a`了。这里就优化到了一次拷贝。

## 3. RPC 原理

Binder 是一个 RPC 框架，也就是说，基于 Binder，Client 进程可以访问 Server 进程中定义的函数。

那 Binder 的 RPC 是如何实现的？一般来说，Client 进程访问 Server 进程函数，我们需要：

* 在 Client 进程中按照固定的规则打包数据，这些数据包含了：
  * 数据发给哪个进程，Binder 中是一个整型变量 Handle
  * 要调用目标进程中的那个函数，Binder 中用一个整型变量 Code 表示
  * 目标函数的参数
  * 要执行具体什么操作，也就是 Binder 协议
* Client 进程通过 IPC 机制将数据传输给 Server 进程 
* Server 进程收到数据，按照固定的格式解析出数据，调用函数，并使用相同的格式将函数的返回值传递给 Client 进程。

![](https://cdn.jsdelivr.net/gh/zzh0838/MyImages@main/img/20230718095800.png)

Binder 要实现的效果就是，整体上看过去，Client 进程执行 Server 进程中的函数就和执行当前进程中的函数是一样的。


## 4. Binder 中各角色之间关系

![](https://cdn.jsdelivr.net/gh/zzh0838/MyImages@main/img/d5ea2a513c65065bc92076f6aa9c2c4.jpg)

图片来自 https://wangkuiwu.github.io/2014/09/01/Binder-Introduce/  稍作修改


### 4.1 binder_node（Binder 实体）

binder_node 是应用层的 service 在内核中的存在形式，是内核中对应用层 service 的描述，在内核中具体表现为 binder_node 结构体。

在上图中，ServiceManager 在 Binder 驱动中有对应的的一个 binder_node(Binder 实体)。每个 Server 在 Binder 驱动中也有对应的的一个 binder_node(Binder 实体)。这里假设每个 Server 内部仅有一个 service，内核中就只有一个对应的 binder_node(Binder 实体)，实际可能存在多个。

binder_node 结构体中存在一个指针 `struct binder_proc *proc;`，指向 binder_node 对应的 binder_proc 结构体。

```cpp
// binder_node 是内核中对应用层 binder 服务的描述
struct binder_node {
	//......
	struct binder_proc *proc;
	//......
}
```

### 4.2 binder_proc

binder_proc 是内核中对应用层进程的描述,内部有众多重要数据结构。

```cpp
// binder_proc 是内核中对应用层进程的描述
struct binder_proc {
	//......
	struct binder_context *context;
	//......
}
```

### 4.3 binder_ref（Binder 引用）

所谓 binder_ref（Binder 引用），实际上是内核中 binder_ref 结构体的对象，它的作用是在表示 binder_node(Binder 实体) 的引用。换句话说，每一个 binder_ref（Binder 引用）都是某一个 binder_node (Binder实体)的引用，通过 binder_ref（Binder 引用） 可以在内核中找到它对应的 binder_node(Binder 实体)。

## 5. Binder 寻址

Binder 是一个 RPC 框架，最少会涉及两个进程。那么就涉及到寻址问题，所谓寻址就是当 A 进程需要调用 B 进程中的函数时，怎么找到 B 进程。

Binder 中寻址分为两种情况：

* ServiceManager 寻址，即 Client 怎么找到 ServiceManager，对应于内核，就是找到 ServiceManager 对应的 binder_proc 结构体实例
* Server 寻址，即 Client 怎么找到 Server，对应于内核，就是找到 Server 对应的 binder_proc 结构体实例

### 5.1 ServiceManager 寻址

每个使用 binder 的进程，在初始化时，会在内核中将 `binder_device` 的 `context` 成员赋值给 `binder_proc->context`。`binder_device`是全局唯一变量，这样的话，所有进程的 `binder_proc->context` 都指向同一个结构体实例。


当 ServiceManager 调用 binder_become_context_manager 后，会陷入内核，在内核中会构建一个 binder_node 结构体实例，构建好以后，会将他保存到 `binder_proc->context->binder_context_mgr_node`
中。

也就是说，任何时候我们都可以通过 `binder_proc->context->binder_context_mgr_node` 获得 ServiceManager 对应的 binder_node 结构体实例。binder_node 结构体中有一个成员 `struct binder_proc *proc;`，通过这个成员我们就能找到 `ServiceManager` 对应的 `binder_proc`

下图展示了 ServiceManager 的寻址过程：

![](https://cdn.jsdelivr.net/gh/zzh0838/MyImages@main/img/20230717215813.png)


### 5.2 Server 寻址

* 服务注册阶段

	* Server 端向 ServiceManager 发起注册服务请求时(svcmgr_publish)，会陷入内核，首先通过 ServiceManager 寻址方式找到 ServiceManager 对应的 binder_proc 结构体，然后在内核中构建一个代表待注册服务的 binder_node 结构体实例，并插入服务端对应的 `binder_proc->nodes` 红黑树中。
	* 接着会构建一个 binder_ref 结构体，binder_ref 会引用到上一阶段构建的 binder_node，并插入到 ServiceManager 对应的 `binder_proc->refs_by_desc` 红黑树中，同时会计算出一个 desc 值（1，2，3 ....依次赋值）保存在 binder_ref 中。
	* 最后服务相关的信息（主要是名字和 desc 值）会传递给 ServiceManager 应用层，应用层通过一个链表将这些信息保存起来

	![](https://cdn.jsdelivr.net/gh/zzh0838/MyImages@main/img/20230718172325.png)


* 服务获取阶段
  
  * Client 端向 ServiceManager 发起获取服务请求时(svcmgr_publish，请求的数据中包含服务的名字)，会陷入内核， 通过 `binder_proc->context->binder_context_mgr_node` 寻址到 ServiceManager，接着通过分配映射内存，拷贝数据后，将"获取服务请求"的数据发送给 ServiceManager， ServiceManager 应用层收到数据后，会遍历内部的链表，通过传递过来的 name 参数，找到对应的 handle，然后将数据返回给 Client 端，接着陷入内核，通过 handle 值在 ServiceManager 对应的 `binder_proc->refs_by_desc` 红黑树中查找到服务对应 binder_ref，接着通过 binder_ref 内部指针找到服务对应的 binder_node 结构。
  * 接着会创建出一个新的 binder_ref 结构体实例，内部 node 指针指向刚刚找到的 binder_node，接着在将 binder_ref 插入到 Client 端的 `binder_proc->refs_by_desc`，并计算出一个 desc 值（1，2，3 ....依次赋值），保存到 binder_ref 中。desc 值也会返回给 Client 的应用层。
  * Client 应用层收到内核返回的 desc 值，改名为 handle，接着向 Server 发起远程调用，远程调用的数据包中包含有 handle 值，接着陷入内核，在内核中首先根据 handle 值在 Client 的 `binder_proc->refs_by_desc`获取到 binder_ref，通过 binder_ref 内部 node 指针找到目标服务对应的 binder_node，然后通过 binder_node 内部的 proc 指针找到目标进程的 binder_proc，这样就完成了整个寻址过程。

![](https://cdn.jsdelivr.net/gh/zzh0838/MyImages@main/img/20230718192920.png)


## 参考资料
* [Android Binder机制(一) Binder的设计和框架](http://wangkuiwu.github.io/2014/09/01/Binder-Introduce/)
* [Android Binder框架实现之Binder的设计思想](https://blog.csdn.net/tkwxty/article/details/102824924)


## 关于

我叫阿豪，2015 年本科毕业于国防科学技术大学指挥信息系统专业，毕业后从事信息化装备的研发工作，主要研究方向是 Android Framework 与 Linux Kernel。

如果你对 Android Framework 感兴趣或者正在学习 Android Framework，可以关注我的微信公众号和抖音，我会持续分享我的学习经验，帮助正在学习的你少走一些弯路。学习过程中如果你有疑问或者你的经验想要分享给大家可以添加我的微信，我拉你进技术交流群。

![](https://cdn.jsdelivr.net/gh/zzh0838/MyImages@main/img/%E4%BA%8C%E7%BB%B4%E7%A0%81.jpg)