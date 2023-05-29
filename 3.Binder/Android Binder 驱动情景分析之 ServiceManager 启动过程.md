# Android Binder 驱动情景分析之 ServiceManager 启动过程

这是一个 Binder 相关的系列教程,持续更新中：

* 学习 Binder 的预备知识
  * [虚拟内存与 Linux 文件访问接口](https://juejin.cn/post/7232459954581307448)
  * [Linux 内核常用数据结构](https://juejin.cn/post/7234370256746348581)
  * [linux 驱动开发入门](https://juejin.cn/post/7235309338097123388)
* [Binder 基本原理](https://juejin.cn/post/7210175991837392933)
* [Binder 程序示例之 C 语言篇](https://juejin.cn/post/7210245482861264955)
* [Binder 服务注册过程情景分析之C语言篇](https://juejin.cn/post/7214342319347712057)
* [Binder 服务获取与使用过程情景分析之C语言篇](https://juejin.cn/post/7215401973842706491)
* Binder 驱动情景分析之 ServiceManager 启动过程
* Binder 驱动情景分析之服务注册过程
* Binder 驱动情景分析之服务获取与使用过程
* [Binder C++ 程序示例](https://juejin.cn/post/7222109036588810297)
* Binder 程序示例之 aidl-cpp 篇
* [Binder C++ 程序分析之主要类解析](https://juejin.cn/post/7223185420157763641/)
* [Binder 服务注册过程情景分析之 C++ 篇](https://juejin.cn/post/7223185420157943865)
* [Binder 服务获取与使用过程情景分析之 C++ 篇](https://juejin.cn/post/7223410490763771941)
* Binder 程序示例之 Java 篇
* Binder Java 程序分析之主要类解析
* Binder 服务注册过程情景分析之 Java 篇
* Binder 服务获取与使用过程情景分析之 Java 篇
* Binder 多线程分析
* Binder 匿名服务分析
* Binder 死亡通知机制
* Binder 相关疑难 bug 解析
* Binder 面试题解析

本文系统源码版本：

* AOSP 分支：android-10.0.0_r41
* Kernel 分支：android-goldfish-4.14-gchips

本文依托于[Binder 程序示例之 C 语言篇](https://juejin.cn/post/7210245482861264955)中介绍的应用层示例程序来对驱动的实现做情景化分析。

## 1. Binder 驱动的初始化

内核启动时，会加载各类驱动，对于 binder 驱动，会执行 binder_init 函数， 完成驱动的注册与初始化。
   

```c
// drivers/android/binder.c
//为便于理解删减部分非核心代码
static int __init binder_init(void)
{
	int ret;
	char *device_name, *device_names, *device_tmp;
	struct binder_device *device;
	struct hlist_node *tmp;

    // 用于初始化一个 lru 链表
    // 链表成员是 binder_lru_page
    // 用于内存页的回收
	ret = binder_alloc_shrinker_init();
	if (ret)
		return ret;

    //省略部分 debugfs 相关代码，这部分代码用于调试

    //binder_devices_param 的值是 "binder,hwbinder,vndbinder" 
    // 对应三个设备文件 /dev/binder /dev/hwbinder /dev/vndbinder
	device_names = kzalloc(strlen(binder_devices_param) + 1, GFP_KERNEL);
	if (!device_names) {
		ret = -ENOMEM;
		goto err_alloc_device_names_failed;
	}
	strcpy(device_names, binder_devices_param);

	device_tmp = device_names;
	while ((device_name = strsep(&device_tmp, ","))) {
        //注册驱动程序
		ret = init_binder_device(device_name);
		if (ret)
			goto err_init_binder_device_failed;
	}

	return ret;

err_init_binder_device_failed:
	hlist_for_each_entry_safe(device, tmp, &binder_devices, hlist) {
		misc_deregister(&device->miscdev);
		hlist_del(&device->hlist);
		kfree(device);
	}

	kfree(device_names);

err_alloc_device_names_failed:
	debugfs_remove_recursive(binder_debugfs_dir_entry_root);

	return ret;
}

//binder 注册为一个杂项驱动
//杂项驱动通过一个 misc_register 函数注册，需要提供一个 miscdevice 结构体
//和之前介绍的字符驱动类似
static int __init init_binder_device(const char *name)
{
	int ret;
	struct binder_device *binder_device;

	binder_device = kzalloc(sizeof(*binder_device), GFP_KERNEL);
	if (!binder_device)
		return -ENOMEM;

	binder_device->miscdev.fops = &binder_fops;
	binder_device->miscdev.minor = MISC_DYNAMIC_MINOR;
	binder_device->miscdev.name = name;

	binder_device->context.binder_context_mgr_uid = INVALID_UID;
	binder_device->context.name = name;
	mutex_init(&binder_device->context.context_mgr_node_lock);

    //注册驱动
	ret = misc_register(&binder_device->miscdev);
	if (ret < 0) {
		kfree(binder_device);
		return ret;
	}

	hlist_add_head(&binder_device->hlist, &binder_devices);

	return ret;
}
```

binder 驱动在 init 部分主要完成了以下工作：

* 初始化 lru 链表，该链表用于管理内存页
* 注册 binder 驱动

## 2. ServiceManager 启动过程

我们首先看一下，应用层 ServiceManager 对应的源码：

```c
int main(int argc, char** argv)
{
    struct binder_state *bs;
    char *driver;

    if (argc > 1) {
        driver = argv[1];
    } else {
        driver = "/dev/binder";
    }

    //关注点1
    bs = binder_open(driver, 128*1024);
  
    //删减了 VENDORSERVICEMANAGER 相关内容
    if (!bs) {
        ALOGE("failed to open binder driver %s\n", driver);
        return -1;
    }

    //关注点2
    if (binder_become_context_manager(bs)) {
        ALOGE("cannot become context manager (%s)\n", strerror(errno));
        return -1;
    }

    //删减了 selinux 相关内容

	//关注点3
    binder_loop(bs, svcmgr_handler);

    return 0;
}
```

### 2.1 binder_open

关注点1：在这里调用 binder_open 来完成 binder 初始化操作

```c
struct binder_state *binder_open(const char* driver, size_t mapsize)
{
    struct binder_state *bs; //用于存需要返回的值
    struct binder_version vers; 

    bs = malloc(sizeof(*bs)); 
    if (!bs) {
        errno = ENOMEM;
        return NULL;
    }

    //关注点4  打开 /dev/binder，拿到内核返回的句柄
    bs->fd = open(driver, O_RDWR | O_CLOEXEC); 
    if (bs->fd < 0) {
        fprintf(stderr,"binder: cannot open %s (%s)\n",
                driver, strerror(errno));
        goto fail_open;
    }

    //关注点5 查询版本
    if ((ioctl(bs->fd, BINDER_VERSION, &vers) == -1) ||
        (vers.protocol_version != BINDER_CURRENT_PROTOCOL_VERSION)) {
        fprintf(stderr,
                "binder: kernel driver version (%d) differs from user space version (%d)\n",
                vers.protocol_version, BINDER_CURRENT_PROTOCOL_VERSION);
        goto fail_open;
    }

    //关注点6 内存映射相关的初始化
    bs->mapsize = mapsize;
    bs->mapped = mmap(NULL, mapsize, PROT_READ, MAP_PRIVATE, bs->fd, 0);
    if (bs->mapped == MAP_FAILED) {
        fprintf(stderr,"binder: cannot map device (%s)\n",
                strerror(errno));
        goto fail_map;
    }

    return bs;

fail_map:
    close(bs->fd);
fail_open:
    free(bs);
    return NULL;
}
```

在 [虚拟内存与 Linux 文件访问接口](https://juejin.cn/post/7232459954581307448) 中分享了这么一个图：

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230512160048.png)

当我们调用系统调用函数 open 后，会触发一个软中断，处理器切换到内核态，并执行对应的中断处理函数，中断处理程序会执行 vfs 中的代码，做一些初始化工作，对传入的参数做一些处理（了解即可），然后就会调用到 binder 驱动程序中注册的 binder_open 函数了：

```c
//为方便理解，删减部分非核心代码
//binder_open 函数主要功能是初始化一个 binder_proc 结构体
//filp 是 vfs 创建的一个 file 文件，是对 /dev/binder 文件的描述
static int binder_open(struct inode *nodp, struct file *filp)
{
	struct binder_proc *proc;
	struct binder_device *binder_dev;

	proc = kzalloc(sizeof(*proc), GFP_KERNEL);
	if (proc == NULL)
		return -ENOMEM;

	//proc->tsk 赋值为 current->group_leader
	//current->group_leader 是主线程对应的 task_struct 结构体
	get_task_struct(current->group_leader);
	proc->tsk = current->group_leader;

	//初始化 proc->todo 链表，链表节点的类型是 binder_work
	INIT_LIST_HEAD(&proc->todo);
	
	//获得驱动初始化时定义的结构体，并保存上下文信息到 proc->context 中
	binder_dev = container_of(filp->private_data, struct binder_device,
				  miscdev);
	proc->context = &binder_dev->context;

	//初始化 proc->alloc， alloc 类型是 binder_alloc，用于管理 binder_buffer
	// binder_buffer 描述一次 RPC 交互过程中使用到的内存
	binder_alloc_init(&proc->alloc);

	proc->pid = current->group_leader->pid;

	//初始化两个链表
	INIT_LIST_HEAD(&proc->delivered_death);
	INIT_LIST_HEAD(&proc->waiting_threads);
	
	//将初始化好的 binder_proc 保存到 filp->private_data 中
	//方便 binder_ioctl binder_mmap 函数获取到 binder_proc
	filp->private_data = proc;

	//将 binder_proc 添加到全局 hash 表 binder_procs 中
	hlist_add_head(&proc->proc_node, &binder_procs);

	return 0;
}


// binder_proc 是 Binder 驱动中用于描述用户态进程的结构体
struct binder_proc {
	struct hlist_node proc_node;
	struct rb_root threads;
	struct rb_root nodes;
	struct rb_root refs_by_desc;
	struct rb_root refs_by_node;
	struct list_head waiting_threads;
	int pid;
	struct task_struct *tsk;
	struct files_struct *files;
	struct mutex files_lock;
	struct hlist_node deferred_work_node;
	int deferred_work;
	bool is_dead;

	struct list_head todo;
	struct binder_stats stats;
	struct list_head delivered_death;
	int max_threads;
	int requested_threads;
	int requested_threads_started;
	int tmp_ref;
	struct binder_priority default_priority;
	struct dentry *debugfs_entry;
	struct binder_alloc alloc;
	struct binder_context *context;
	spinlock_t inner_lock;
	spinlock_t outer_lock;
};
```

* binder_proc 是 binder 驱动中用于描述用户态进程的结构体
* binder_open 的主要工作是初始化一个 binder_proc 结构体，并将其插入到全局链表 binder_procs 中



关注点 5 处，应用程序调用 ioctl 向内核查询 binder 版本信息：

```c
ioctl(bs->fd, BINDER_VERSION, &vers)
```

和 open 系统调用相同,ioctl 最终会执行到 binder 驱动的 binder_ioctl 函数：

```c 
ioctl() -> vfs -> binder_ioctl
```

binder_ioctl 的实现如下：

```c
//drivers/android/binder.c
//为方便理解，删减部分非核心代码
static long binder_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	int ret;
	//binder_open 时，会把 binder_proc 保存到 filp->private_data
	//这里从 filp->private_data 中取出
	struct binder_proc *proc = filp->private_data;
	struct binder_thread *thread;
	unsigned int size = _IOC_SIZE(cmd);
	void __user *ubuf = (void __user *)arg;

	//binder_get_thread 会构建一个新的 binder_thread, 并插入 binder_proc.threads 红黑树
	thread = binder_get_thread(proc);
	if (thread == NULL) {
		ret = -ENOMEM;
		goto err;
	}

	//应用层  
	switch (cmd) {
	//......
	//省略不相关的 case
	case BINDER_VERSION: {  //走这里

		struct binder_version __user *ver = ubuf;

		if (size != sizeof(struct binder_version)) {
			ret = -EINVAL;
			goto err;
		}
		//binder 中版本定义为一个常量
		//拷贝到应用层的指针
		if (put_user(BINDER_CURRENT_PROTOCOL_VERSION,
			     &ver->protocol_version)) {
			ret = -EINVAL;
			goto err;
		}
		break;
	}

	//......
	//省略不相关的 case
	}

	//......
	return ret;
}
```

在内核中 binder 的驱动版本定义为一个常量 `BINDER_CURRENT_PROTOCOL_VERSION`，驱动通过 put_user 函数将这个常量的值拷贝到用户空间。


关注点 6，这里调用 mmap，来完成内存映射相关的初始化，和 open ioctl 系统调用相同, mmap 最终会执行到 binder 驱动的 binder_mmap 函数：

```c
mmap(NULL, mapsize, PROT_READ, MAP_PRIVATE, bs->fd, 0) -> vfs -> binder_mmap
```

接着我们看下 binder_mmap 的具体实现：

 ```c++
 //filp 是 vfs    中的 filp 是同一个结构体
 //vma 是 vfs 中根据应用层传入的 mapsize 构建的结构体，用于描述一段用户空间虚拟内存
 static int binder_mmap(struct file *filp, struct vm_area_struct *vma)
{
	int ret;
	//binder_open 中将构建好的 binder_proc 保存到了 filp->private_data 中
	struct binder_proc *proc = filp->private_data;
	const char *failure_string;

	if (proc->tsk != current->group_leader)
		return -EINVAL;

	// 映射区最多分配 4m
	if ((vma->vm_end - vma->vm_start) > SZ_4M)
		vma->vm_end = vma->vm_start + SZ_4M;

	//......

	//配置 vma
	if (vma->vm_flags & FORBIDDEN_MMAP_FLAGS) {
		ret = -EPERM;
		failure_string = "bad vm_flags";
		goto err_bad_arg;
	}
	vma->vm_flags |= VM_DONTCOPY | VM_MIXEDMAP;
	vma->vm_flags &= ~VM_MAYWRITE;

	vma->vm_ops = &binder_vm_ops;
	vma->vm_private_data = proc;

	//初始化 binder_proc 中的 binder_alloc
	ret = binder_alloc_mmap_handler(&proc->alloc, vma);
	if (ret)
		return ret;
	
	// 将当前进程 task_struct 中的 files_struct 赋值给 binder_proc 成员的 proc->files
	mutex_lock(&proc->files_lock);
	proc->files = get_files_struct(current);
	mutex_unlock(&proc->files_lock);
	return 0; 

err_bad_arg:
	pr_err("%s: %d %lx-%lx %s failed %d\n", __func__,
	       proc->pid, vma->vm_start, vma->vm_end, failure_string, ret);
	return ret;
}


// 初始化 binder_proc 中的 binder_alloc
// binder_alloc 用于操作 binder_buffer 的结构体
// binder_buffer 描述一次 RPC 交互过程中使用到的内存
int binder_alloc_mmap_handler(struct binder_alloc *alloc,
			      struct vm_area_struct *vma)
{
	int ret;
	const char *failure_string;
	struct binder_buffer *buffer;

	//已经分配就退出
	mutex_lock(&binder_alloc_mmap_lock);
	if (alloc->buffer) {
		ret = -EBUSY;
		failure_string = "already mapped";
		goto err_already_mapped;
	}

	//alloc->buffer 指向用户空间起始位置
	alloc->buffer = (void __user *)vma->vm_start;
	mutex_unlock(&binder_alloc_mmap_lock);

	//pages 类型是 binder_lru_page 用于管理内存中的页
	alloc->pages = kzalloc(sizeof(alloc->pages[0]) *
				   ((vma->vm_end - vma->vm_start) / PAGE_SIZE),
			       GFP_KERNEL);
	if (alloc->pages == NULL) {
		ret = -ENOMEM;
		failure_string = "alloc page array";
		goto err_alloc_pages_failed;
	}
	//记录映射区域的大小
	alloc->buffer_size = vma->vm_end - vma->vm_start;

	//初始化 binder_buffer
	buffer = kzalloc(sizeof(*buffer), GFP_KERNEL);
	if (!buffer) {
		ret = -ENOMEM;
		failure_string = "alloc buffer struct";
		goto err_alloc_buf_struct_failed;
	}

	buffer->user_data = alloc->buffer; //就是 vma->vm_start，用户空间映射区的起始位置
	// 初始化完成的 binder_alloc 保存到 binder_alloc 的 
	list_add(&buffer->entry, &alloc->buffers); 
	buffer->free = 1;
	//将初始化好的 binder_buffer 插入 alloc.free_buffers 红黑树，
	binder_insert_free_buffer(alloc, buffer);
	//free_async_space 异步调用的空间
	alloc->free_async_space = alloc->buffer_size / 2;
	//alloc->vma = vma
	binder_alloc_set_vma(alloc, vma);
	mmgrab(alloc->vma_vm_mm);

	return 0;

err_alloc_buf_struct_failed:
	kfree(alloc->pages);
	alloc->pages = NULL;
err_alloc_pages_failed:
	mutex_lock(&binder_alloc_mmap_lock);
	alloc->buffer = NULL;
err_already_mapped:
	mutex_unlock(&binder_alloc_mmap_lock);
	pr_err("%s: %d %lx-%lx %s failed %d\n", __func__,
	       alloc->pid, vma->vm_start, vma->vm_end, failure_string, ret);
	return ret;
}
```

可以看出，binder_mmap 主要工作是完成 binder_proc 的成员 binder_allc 结构体的初始化，没有分配物理内存与映射。

这里涉及一个常见的面试题，binder 支持传递的最大内存是多少？

```c
// 映射区最多分配 4m
if ((vma->vm_end - vma->vm_start) > SZ_4M)
	vma->vm_end = vma->vm_start + SZ_4M;
```

在驱动层面的代码看，最多可以分配 4m 的映射区，当然具体多少还和应用层传递过来的参数有关。


### 2.2 binder_become_context_manager

关注点 2 处，调用 binder_become_context_manager 函数，将当前进程设置为 context_manager：

```c
int binder_become_context_manager(struct binder_state *bs)
{
    //构建需要发送的数据 flat_binder_object
    struct flat_binder_object obj;
    memset(&obj, 0, sizeof(obj));
    obj.flags = FLAT_BINDER_FLAG_TXN_SECURITY_CTX;

    //向 Binder 驱动发送数据
    int result = ioctl(bs->fd, BINDER_SET_CONTEXT_MGR_EXT, &obj);

    //如果失败，使用原始方式再次调用 ioctl
    // fallback to original method
    if (result != 0) {
        android_errorWriteLog(0x534e4554, "121035042");

        result = ioctl(bs->fd,  , 0);
    }
    return result;
}
```

ioctl 最终会调用到 binder 驱动中的 binder_ioctl：

```c
ioctl(bs->fd, BINDER_SET_CONTEXT_MGR_EXT, &obj) -> vfs -> binder_ioctl
```

```c
static long binder_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	int ret;
	struct binder_proc *proc = filp->private_data;
	struct binder_thread *thread;
	unsigned int size = _IOC_SIZE(cmd);
	void __user *ubuf = (void __user *)arg;

	/*pr_info("binder_ioctl: %d:%d %x %lx\n",
			proc->pid, current->pid, cmd, arg);*/

	binder_selftest_alloc(&proc->alloc);

	trace_binder_ioctl(cmd, arg);

	ret = wait_event_interruptible(binder_user_error_wait, binder_stop_on_user_error < 2);
	if (ret)
		goto err_unlocked;

	thread = binder_get_thread(proc);
	if (thread == NULL) {
		ret = -ENOMEM;
		goto err;
	}

	switch (cmd) {
	//省略不相关的 case
	case BINDER_SET_CONTEXT_MGR_EXT: { //代码走这里
		struct flat_binder_object fbo;

		//数据拷贝到内核区
		if (copy_from_user(&fbo, ubuf, sizeof(fbo))) {
			ret = -EINVAL;
			goto err;
		}
		//binder_ioctl_set_ctx_mgr
		ret = binder_ioctl_set_ctx_mgr(filp, &fbo);
		if (ret)
			goto err;
		break;
	}
	//省略不相关的 case
	default:
		ret = -EINVAL;
		goto err;
	}
	ret = 0;
err:
	if (thread)
		thread->looper_need_return = false;
	wait_event_interruptible(binder_user_error_wait, binder_stop_on_user_error < 2);
	if (ret && ret != -ERESTARTSYS)
		pr_info("%d:%d ioctl %x %lx returned %d\n", proc->pid, current->pid, cmd, arg, ret);
err_unlocked:
	trace_binder_ioctl_done(ret);
	return ret;
}


接着会调用到 binder_ioctl_set_ctx_mgr：

```c
//为方便理解，省略部分非核心代码
static int binder_ioctl_set_ctx_mgr(struct file *filp,
				    struct flat_binder_object *fbo)
{
	int ret = 0;
	struct binder_proc *proc = filp->private_data;
	struct binder_context *context = proc->context;
	struct binder_node *new_node;

	mutex_lock(&context->context_mgr_node_lock);
	
	// binder_node 结构体用于描述一个应用层 bidner 服务
	//binder_new_node 函数会初始化一个 binder_node，并插入 binder_proc 的 nodes 红黑树中 
	new_node = binder_new_node(proc, fbo);
	if (!new_node) {
		ret = -ENOMEM;
		goto out;
	}
	
	// 将新构建的 binder_node 赋值给 proc->context 的 binder_context_mgr_node 成员
	context->binder_context_mgr_node = new_node;
	binder_node_unlock(new_node);
	binder_put_node(new_node);
out:
	mutex_unlock(&context->context_mgr_node_lock);
	return ret;
}
```

binder_ioctl_set_ctx_mgr 用于将当前进程设置为ServiceManager, binder_node 结构体用于描述一个应用层 bidner 服务，binder_new_node 函数会构建一个新的 binder_node 结构体，并插入到 binder_proc 的 nodes 红黑树中。 接着会把 `proc->context->binder_context_mgr_node` 指针指向新创建的 node。


### 2.3 binder_loop

接着 ServiceManager 调用 binder_loop 进入读数据，处理数据的循环：

```c
void binder_loop(struct binder_state *bs, binder_handler func)
{
    int res;
    //ioctl 读写数据类型
    struct binder_write_read bwr;
    uint32_t readbuf[32];

    bwr.write_size = 0;
    bwr.write_consumed = 0;
    bwr.write_buffer = 0;

    //告诉驱动，应用程序要进入循环了
    readbuf[0] = BC_ENTER_LOOPER;
    //ioctl 的基本封装
    binder_write(bs, readbuf, sizeof(uint32_t));

    for (;;) {
        //结合上面 bwr 的赋值，这里是要读数据
        bwr.read_size = sizeof(readbuf);
        bwr.read_consumed = 0;
        bwr.read_buffer = (uintptr_t) readbuf;
        //向驱动发起读操作
        res = ioctl(bs->fd, BINDER_WRITE_READ, &bwr);

        if (res < 0) {
            ALOGE("binder_loop: ioctl failed (%s)\n", strerror(errno));
            break;
        }

      //省略后续代码......
    }
}

//binder_write 是对 ioctl + BINDER_WRITE_READ 写操作的简单封装
int binder_write(struct binder_state *bs, void *data, size_t len)
{
    struct binder_write_read bwr;
    int res;

    bwr.write_size = len;
    bwr.write_consumed = 0;
    bwr.write_buffer = (uintptr_t) data;
    bwr.read_size = 0;
    bwr.read_consumed = 0;
    bwr.read_buffer = 0;
    res = ioctl(bs->fd, BINDER_WRITE_READ, &bwr);
    if (res < 0) {
        fprintf(stderr,"binder_write: ioctl failed (%s)\n",
                strerror(errno));
    }
    return res;
} 

```

首先是调用 binder_write，告诉驱动应用程序进入循环，调用路线如下:

```c
binder_write -> ioctl(bs->fd, BINDER_WRITE_READ, &bwr) -> binder_ioctl

//readbuf[0] = BC_ENTER_LOOPER;
//ioctl 的基本封装
//binder_write(bs, readbuf, sizeof(uint32_t));
//res = ioctl(bs->fd, BINDER_WRITE_READ, &bwr);

static long binder_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{

	//......
	switch (cmd) {
	case BINDER_WRITE_READ: //代码走这里
		ret = binder_ioctl_write_read(filp, cmd, arg, thread);
		if (ret)
			goto err;
		break;
	//省略不相关 case
	default:
		ret = -EINVAL;
		goto err;
	}

	//......
}

//接着调用 binder_ioctl_write_read
static int binder_ioctl_write_read(struct file *filp,
				unsigned int cmd, unsigned long arg,
				struct binder_thread *thread)
{
	int ret = 0;
	struct binder_proc *proc = filp->private_data;
	unsigned int size = _IOC_SIZE(cmd);
	void __user *ubuf = (void __user *)arg;
	struct binder_write_read bwr;

	if (size != sizeof(struct binder_write_read)) {
		ret = -EINVAL;
		goto out;
	}
	//将应用层的数据拷贝到内核中
	if (copy_from_user(&bwr, ubuf, sizeof(bwr))) {
		ret = -EFAULT;
		goto out;
	}
	
	//......
	if (bwr.write_size > 0) {
		//接着调用 binder_thread_write 写数据
		ret = binder_thread_write(proc, thread,
					  bwr.write_buffer,
					  bwr.write_size,
					  &bwr.write_consumed);
		trace_binder_write_done(ret);
		if (ret < 0) {
			bwr.read_consumed = 0;
			if (copy_to_user(ubuf, &bwr, sizeof(bwr)))
				ret = -EFAULT;
			goto out;
		}
	}
	
	//......

	if (copy_to_user(ubuf, &bwr, sizeof(bwr))) {
		ret = -EFAULT;
		goto out;
	}
out:
	return ret;
}

//binder_thread_write
static int binder_thread_write(struct binder_proc *proc,
			struct binder_thread *thread,
			binder_uintptr_t binder_buffer, size_t size,
			binder_size_t *consumed)
{
	uint32_t cmd;
	struct binder_context *context = proc->context;
	void __user *buffer = (void __user *)(uintptr_t)binder_buffer;
	void __user *ptr = buffer + *consumed;
	void __user *end = buffer + size;

	while (ptr < end && thread->return_error.cmd == BR_OK) {
		int ret;

		//cmd 就是应用层传入的 BC_ENTER_LOOPER
		if (get_user(cmd, (uint32_t __user *)ptr))
			return -EFAULT;
		ptr += sizeof(uint32_t);
		
		//......

		switch (cmd) {
		
		case BC_ENTER_LOOPER:
			if (thread->looper & BINDER_LOOPER_STATE_REGISTERED) {
				thread->looper |= BINDER_LOOPER_STATE_INVALID;
			}
			//操作很简单，给 thread->looper 或一个 BINDER_LOOPER_STATE_ENTERED
			thread->looper |= BINDER_LOOPER_STATE_ENTERED;
			break;
		
		default:
			pr_err("%d:%d unknown command %d\n",
			       proc->pid, thread->pid, cmd);
			return -EINVAL;
		}
		*consumed = ptr - buffer;
	}
	return 0;
}
```

接着就会进入循环，开始读数据：

```c
 for (;;) {
        //结合上面 bwr 的赋值，这里是要读数据
        bwr.read_size = sizeof(readbuf);
        bwr.read_consumed = 0;
        bwr.read_buffer = (uintptr_t) readbuf;
        //向驱动发起读操作
        res = ioctl(bs->fd, BINDER_WRITE_READ, &bwr);

        if (res < 0) {
            ALOGE("binder_loop: ioctl failed (%s)\n", strerror(errno));
            break;
        }

      //省略后续代码......
    }

//ioctl 最终会调用到驱动中的 binder_ioctl
static long binder_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	//......
	switch (cmd) {
	//......
	case BINDER_WRITE_READ: //代码走这里
		ret = binder_ioctl_write_read(filp, cmd, arg, thread);
		if (ret)
			goto err;
		break;
		//......
	}
}

static int binder_ioctl_write_read(struct file *filp,
				unsigned int cmd, unsigned long arg,
				struct binder_thread *thread)
{
	int ret = 0;
	struct binder_proc *proc = filp->private_data;
	unsigned int size = _IOC_SIZE(cmd);
	void __user *ubuf = (void __user *)arg;
	struct binder_write_read bwr;

	if (size != sizeof(struct binder_write_read)) {
		ret = -EINVAL;
		goto out;
	}
	if (copy_from_user(&bwr, ubuf, sizeof(bwr))) {
		ret = -EFAULT;
		goto out;
	}
	
	//......
	if (bwr.read_size > 0) {
		//开始读数据
		ret = binder_thread_read(proc, thread, bwr.read_buffer,
					 bwr.read_size,
					 &bwr.read_consumed,
					 filp->f_flags & O_NONBLOCK);
	//......
	}
	
	//拷贝给用户空间
	if (copy_to_user(ubuf, &bwr, sizeof(bwr))) {
		ret = -EFAULT;
		goto out;
	}
out:
	return ret;
}

static int binder_thread_read(struct binder_proc *proc,
			      struct binder_thread *thread,
			      binder_uintptr_t binder_buffer, size_t size,
			      binder_size_t *consumed, int non_block)
{
	void __user *buffer = (void __user *)(uintptr_t)binder_buffer;
	void __user *ptr = buffer + *consumed;
	void __user *end = buffer + size;

	int ret = 0;
	int wait_for_proc_work;

	if (*consumed == 0) {
		if (put_user(BR_NOOP, (uint32_t __user *)ptr))
			return -EFAULT;
		ptr += sizeof(uint32_t);
	}

retry:
	binder_inner_proc_lock(proc);

	//使用 proc->todo 而不是 thread->todo 
	//这里会返回 true
	wait_for_proc_work = binder_available_for_proc_work_ilocked(thread);
	binder_inner_proc_unlock(proc);

	//配置 looper 的值
	thread->looper |= BINDER_LOOPER_STATE_WAITING;

	
	if (wait_for_proc_work) {
		//不会进入 if
		if (!(thread->looper & (BINDER_LOOPER_STATE_REGISTERED |
					BINDER_LOOPER_STATE_ENTERED))) {
			binder_user_error("%d:%d ERROR: Thread waiting for process work before calling BC_REGISTER_LOOPER or BC_ENTER_LOOPER (state %x)\n",
				proc->pid, thread->pid, thread->looper);
			wait_event_interruptible(binder_user_error_wait,
						 binder_stop_on_user_error < 2);
		}
		binder_restore_priority(current, proc->default_priority);
	}

	if (non_block) {
		if (!binder_has_work(thread, wait_for_proc_work))
			ret = -EAGAIN;
	} else { //代码走这里，等待binder工作到来，当有远程访问时，会从这里唤醒
		ret =  binder_wait_for_work(thread, wait_for_proc_work);
	}

	//......
}

static int binder_wait_for_work(struct binder_thread *thread,
				bool do_proc_work)
{
	//定义一个等待队列项
	DEFINE_WAIT(wait);
	struct binder_proc *proc = thread->proc;
	int ret = 0;

	freezer_do_not_count();
	binder_inner_proc_lock(proc);
	for (;;) {
		//准备睡眠等待
		prepare_to_wait(&thread->wait, &wait, TASK_INTERRUPTIBLE);
		if (binder_has_work_ilocked(thread, do_proc_work))
			break;
		if (do_proc_work)
			list_add(&thread->waiting_thread_node,
				 &proc->waiting_threads);
		binder_inner_proc_unlock(proc);
		schedule(); //发起进程调度，进程进入休眠
		binder_inner_proc_lock(proc);
		list_del_init(&thread->waiting_thread_node);
		if (signal_pending(current)) {
			ret = -ERESTARTSYS;
			break;
		}
	}
	finish_wait(&thread->wait, &wait);
	binder_inner_proc_unlock(proc);
	freezer_count();

	return ret;
}
```

这里涉及了 Linux 内核中进程调度相关的 API，这里简单介绍一下：


**DEFINE_WAIT**

这是一个宏，它定义了一个 wait_queue_t 结构体

```c
#define DEFINE_WAIT_FUNC(name, function)				\
	wait_queue_t name = {						\
		.private	= current,				\
		.func		= function,				\
		.task_list	= LIST_HEAD_INIT((name).task_list),	\
	}

#define DEFINE_WAIT(name) DEFINE_WAIT_FUNC(name, autoremove_wake_function)
```
这个结构体中的private域指向的即是当前执行系统调用所在进程的描述结构体，func域指向唤醒这个队列项进程所执行的函数

**prepare_to_wait**
这个函数将我们刚刚定义的wait队列项加入到一个等待队列中（在binder中即是加入到thread->wait中），然后将进程的状态设置为我们指定的状态，在这里为TASK_INTERRUPTIBLE（可中断的睡眠状态）

**schedule**
schedule是真正执行进程调度的地方，由于之前进程状态已经被设置成TASK_INTERRUPTIBLE状态，在调用schedule后，该进程就会让出CPU，不再调度运行，直到该进程恢复TASK_RUNNING状态

**wake_up_interruptible**
这个函数会唤醒TASK_INTERRUPTIBLE状态下的进程，它会循环遍历等待队列中的每个元素，分别执行其唤醒函数，也就对应着我们DEFINE_WAIT定义出来的结构体中的func域，即autoremove_wake_function，它最终会调用try_to_wake_up函数将进程置为TASK_RUNNING状态，这样后面的进程调度便会调度到该进程，从而唤醒该进程继续执行

**signal_pending**
这个函数是用来检查当前系统调用进程是否有信号需要处理的，当一个进程陷入系统调用并处于等待状态时，如果此时产生了一个信号，仅仅是在该进程的thread_info中标识了一下，所以我们唤醒进程后需要检查一下是否有信号需要处理，如果有的话，返回-ERESTARTSYS，先处理信号，后续Linux上层库函数会根据-ERESTARTSYS此返回值重新执行这个系统调用

**finish_wait**
最后一步，当进程被唤醒后，调用finish_wait函数执行清理工作，将当前进程置为TASK_RUNNING状态，并把当前wait队列项从等待队列中移除


## 总结

最后我们对上述的分析做一个总结：

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230529160635.png)

## 参考资料
* 《Android 框架解密》
* [Binder系列1—Binder Driver初探](http://gityuan.com/2015/11/01/binder-driver/)
* [Android源码分析 - Binder驱动（下）](https://juejin.cn/post/7073783503791325214)
* [理解Android Binder机制(1/3)：驱动篇](https://paul.pub/android-binder-driver/)

## 关于

我叫阿豪，2015 年毕业于国防科技大学，毕业后，在某单位从事信息化装备的研发工作。主要研究方向为 Android Framework 与 Linux Kernel，2023年春节后开始做 Android Framework 相关的技术分享。

如果你对 Framework 感兴趣或者正在学习 Framework，可以参考我总结的[Android Framework 学习路线指南](https://github.com/yuandaimaahao/AndroidFrameworkTutorial),也可关注我的微信公众号，我会在公众号上持续分享我的经验，帮助正在学习的你少走一些弯路。学习过程中如果你有疑问或者你的经验想要分享给大家可以添加我的微信，我拉你进技术交流群。

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/4e7348e352774883ecb19ab021d6cee.jpg)


