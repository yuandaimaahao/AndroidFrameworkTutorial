# Android Binder 驱动情景分析之服务注册过程

本文系统源码版本：

* AOSP 分支：android-10.0.0_r41
* Kernel 分支：android-goldfish-4.14-gchips

本文依托于[Binder 程序示例之 C 语言篇](https://juejin.cn/post/7210245482861264955)中介绍的应用层示例程序来对驱动的实现做情景化分析。

上文说到，ServiceManager 进入循环，开始读数据后，就进入休眠状态，直到有其他进程写入数据并唤醒他为止。接下来我们就来看看 ServiceManager 是怎么被 Server 端唤醒的。

## 1. Server 主函数

```c
int main(int argc, char **argv)
{
    struct binder_state *bs;
    uint32_t svcmgr = BINDER_SERVICE_MANAGER;
    uint32_t handle;
	int ret;
  
    //Binder 初始化
    bs = binder_open("/dev/binder", 128*1024);
    if (!bs) {
        fprintf(stderr, "failed to open binder driver\n");
        return -1;
    }
	//svcmgr 的值是 0，表示发送信息给 servicemanager
	//注册服务
	ret = svcmgr_publish(bs, svcmgr, "hello", hello_service_handler);
    if (ret) {
        fprintf(stderr, "failed to publish hello service\n");
        return -1;
    }
    //进入 loop， 等待 client 请求服务
    binder_loop(bs, test_server_handler);

    return 0;
}

```

binder_open 的调用流程与 ServiceManager 一致，这里不再重复，总结一下就是：binder_open 主要是初始化了一个 binder_proc 结构体，并插入到全局的链表 procs 中了。

## 2. svcmgr_publish 实现分析

svcmgr_publish 用于发布一个服务,其具体实现如下：

```c
int svcmgr_publish(struct binder_state *bs, uint32_t target, const char *name, void *ptr)
{
    int status;

    unsigned iodata[512/4];
    struct binder_io msg, reply;
    bio_init(&msg, iodata, sizeof(iodata), 4);
    bio_put_uint32(&msg, 0);  // strict mode header
    bio_put_uint32(&msg, 0);
    //SVC_MGR_NAME 值为 "android.os.IServiceManager"
    bio_put_string16_x(&msg, SVC_MGR_NAME);
    //name 的值是 hello
    bio_put_string16_x(&msg, name);
    //ptr 是一个函数指针指向 hello_service_handler 
    bio_put_obj(&msg, ptr);
    bio_put_uint32(&msg, 0);
    bio_put_uint32(&msg, 0);

    //通过 binder_call 发起远程函数调用
    if (binder_call(bs, &msg, &reply, target, SVC_MGR_ADD_SERVICE)) {
        //fprintf(stderr, "svcmgr_public 远程调用失败\n");
        return -1;
    }
  
    //解析返回值
    status = bio_get_uint32(&reply); //调用成功返回0
    //远程调用结束，通知驱动清理内存
    binder_done(bs, &msg, &reply);

    return status;
}
```

binder_io 可以理解为一个数据集合，数据发送端将数据按照一定的顺序写入集合，数据接受端按照相同的顺序读取数据。


svcmgr_publish 中使用如下代码构建了一个 binder_io 结构体：

```c
unsigned iodata[512/4];
struct binder_io msg, reply;
bio_init(&msg, iodata, sizeof(iodata), 4);
bio_put_uint32(&msg, 0);  // strict mode header
bio_put_uint32(&msg, 0);
//SVC_MGR_NAME 值为 "android.os.IServiceManager"
bio_put_string16_x(&msg, SVC_MGR_NAME);
//name 的值是 hello
bio_put_string16_x(&msg, name);
//ptr 是一个函数指针指向 hello_service_handler 
bio_put_obj(&msg, ptr);
bio_put_uint32(&msg, 0);
bio_put_uint32(&msg, 0);
```

接下来我们看看 binder_io 的具体实现：

```c
struct binder_io
{
    char *data;            /* pointer to read/write from */
    binder_size_t *offs;   /* array of offsets */
    size_t data_avail;     /* bytes available in data buffer */
    size_t offs_avail;     /* entries available in offsets array */

    char *data0;           /* start of data buffer */
    binder_size_t *offs0;  /* start of offsets buffer */
    uint32_t flags;
    uint32_t unused;
};
```

binder_io 的初始化过程：

```c
//初始化过程
unsigned iodata[512/4];
struct binder_io msg;
//初始化 binder_io 
bio_init(&msg, iodata, sizeof(iodata), 4);

void bio_init(struct binder_io *bio, void *data,
              size_t maxdata, size_t maxoffs)
{
    size_t n = maxoffs * sizeof(size_t);

    //溢出处理
    if (n > maxdata) {
        bio->flags = BIO_F_OVERFLOW;
        bio->data_avail = 0;
        bio->offs_avail = 0;
        return;
    }

    //将 bio 一分为二
    bio->data = bio->data0 = (char *) data + n;
    bio->offs = bio->offs0 = data;
    bio->data_avail = maxdata - n;
    bio->offs_avail = maxoffs;
    bio->flags = 0;
}
```

从 binder_io 的定义和初始化过程中可以看出，binder_io 用于管理一块内存，同时将内存分为了两部分管理：

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20221204180346.png)

为方便叙述，本文称这两个区为偏移区和数据区。

maxdata 是这块内存总的字节数，偏移区的大小为 n 字节，其中 n = maxoffs * sizeof(size_t)，数据区的大小为 maxdata - n 

两块内存如何使用？

接下来我们看看如何将一个 unit32_t 数据存入 binder_io：

```c
void bio_put_uint32(struct binder_io *bio, uint32_t n)
{
    //分配内存
    uint32_t *ptr = bio_alloc(bio, sizeof(n));
    if (ptr)
        *ptr = n;
}


//在 binder_io 的第二部分分配 size 大小的内存
static void *bio_alloc(struct binder_io *bio, size_t size)
{   
    //size 最终等于 4,8,12,16,20 ......
    //size 的值比原始的值大
    size = (size + 3) & (~3);
    //溢出操作
    if (size > bio->data_avail) {
        bio->flags |= BIO_F_OVERFLOW;
        return NULL;
    } else {
        //分配位置
        void *ptr = bio->data;
        bio->data += size;
        bio->data_avail -= size;
        return ptr;
    }
}
```

写入一个 32 位整数的过程如下：

* 在数据区分配 4 字节倍数的数据
* 将整数值写入已分配的内存

写入后一个 `uint32_t n` 后，内存结构如下图所示：

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230202164604.png)

字符串的写入稍微复杂一点，但是本质原理和写入 unit_32 相同，有兴趣的同学可以自行分析下面的代码：

```c
void bio_put_string16_x(struct binder_io *bio, const char *_str)
{
    unsigned char *str = (unsigned char*) _str;
    size_t len;
    uint16_t *ptr;
    
    //写入一个标记位
    if (!str) {
        bio_put_uint32(bio, 0xffffffff);
        return;
    }

    len = strlen(_str);

    if (len >= (MAX_BIO_SIZE / sizeof(uint16_t))) {
        bio_put_uint32(bio, 0xffffffff);
        return;
    }

    /* Note: The payload will carry 32bit size instead of size_t */
    //写入字符串长度
    bio_put_uint32(bio, len);
    //写入字符串内容
    ptr = bio_alloc(bio, (len + 1) * sizeof(uint16_t));
    if (!ptr)
        return;

    while (*str)
        *ptr++ = *str++;
    *ptr++ = 0;
}
```

接下来我们看看，如何在 binder_io 中存一个指针数据。（在服务注册场景下，这个指针指向一个函数，是 server 端收到调用后的回调函数）

```c
void bio_put_obj(struct binder_io *bio, void *ptr)
{
    struct flat_binder_object *obj;

    //分配内存
    obj = bio_alloc_obj(bio);
    if (!obj)
        return;

    obj->flags = 0x7f | FLAT_BINDER_FLAG_ACCEPTS_FDS;
    obj->hdr.type = BINDER_TYPE_BINDER;
    //ptr 保存在 flat_binder_object 的 binder 成员中
    obj->binder = (uintptr_t)ptr;
    obj->cookie = 0;
}


static struct flat_binder_object *bio_alloc_obj(struct binder_io *bio)
{
    struct flat_binder_object *obj;
    //在数据区分配内存
    obj = bio_alloc(bio, sizeof(*obj));

    //在第一部分保存偏移量
    if (obj && bio->offs_avail) {
        bio->offs_avail--;
        //保存 flat_binder_object 结构在偏移区的偏移值
        *bio->offs++ = ((char*) obj) - ((char*) bio->data0);
        return obj;
    }

    bio->flags |= BIO_F_OVERFLOW;
    return NULL;
}

struct flat_binder_object {
	struct binder_object_header	hdr;
	__u32				flags;

	/* 8 bytes of data. */
	union {
		binder_uintptr_t	binder;	/* local object */
		__u32			handle;	/* remote object */
	};

	/* extra data associated with local object */
	binder_uintptr_t	cookie;
};

struct binder_object_header {
	__u32        type;
};


struct flat_binder_object {
    //hdr.type = BINDER_TYPE_BINDER
	struct binder_object_header	hdr;
    //flags = 0x7f | FLAT_BINDER_FLAG_ACCEPTS_FDS;
	__u32				flags;

	union {
        //binder = (uintptr_t)ptr
        //ptr 就是传入的函数指针  hello_service_handler
		binder_uintptr_t	binder;	
		__u32			handle;	
	};
    //cookie = 0
	binder_uintptr_t	cookie;
};

```

对于指针类型，会在数据区分配一个 flat_binder_object 结构体的数据，将指针数据保存在结构体的 binder 成员中。在偏移区将 flat_binder_object 相对 data0 的偏移值保存在 offs 指向的内存，offs 再加 1。完成数据保存后，其内存结构如下图所示：

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20221204201122.png)

通过上面的分析我们可以画出 svcmgr_publish 中构建的 binder_io 的结构：

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230530112033.png)



构造好 binder_io 以后，接着就通过 binder_call 发起远程过程调用：

```c
// target 的值为 0 ，用于指示数据发送给哪个进程
// msg 就是我们上面构造的数据结构
// SVC_MGR_ADD_SERVICE 用于指示需要调用的远程函数
binder_call(bs, &msg, &reply, target,SVC_MGR_ADD_SERVICE)

int binder_call(struct binder_state *bs,
                struct binder_io *msg, struct binder_io *reply,
                uint32_t target, uint32_t code)
{
    int res;

    //关注点1 binder_io *msg 转为 binder_write_read
    //声明数据
    struct binder_write_read bwr;

    //binder_write_read 内部成员 write_buffer 的结构
    struct {
        uint32_t cmd;
        struct binder_transaction_data txn;
    } __attribute__((packed)) writebuf;
  
    unsigned readbuf[32];

    //...... 省略非核心代码

    //构造 binder_write_read 内部结构 writebuf
    // BC_TRANSACTION 表示当前数据是用于发起远程调用
    writebuf.cmd = BC_TRANSACTION;
    // target 用于找到远程进程，即我们要调用哪个进程的函数
    writebuf.txn.target.handle = target;
    // code 表示调用哪个函数
    writebuf.txn.code = code;
    writebuf.txn.flags = 0;
    writebuf.txn.data_size = msg->data - msg->data0;
    writebuf.txn.offsets_size = ((char*) msg->offs) - ((char*) msg->offs0);
    //data 是数据区，指向一个 binder_io 结构体
    writebuf.txn.data.ptr.buffer = (uintptr_t)msg->data0;
    writebuf.txn.data.ptr.offsets = (uintptr_t)msg->offs0;

    //给 write 相关变量赋值
    //表示当前进程是写入数据，即发送数据
    bwr.write_size = sizeof(writebuf);
    bwr.write_consumed = 0;
    bwr.write_buffer = (uintptr_t) &writebuf;

    hexdump(msg->data0, msg->data - msg->data0);
    for (;;) {
        //关注点2 写的同时也要读数据
        //给 read 相关变量赋值
        //同时，我们也要读取返回的结果值
        bwr.read_size = sizeof(readbuf);
        bwr.read_consumed = 0;
        bwr.read_buffer = (uintptr_t) readbuf;

        //关注点3 发起读写操作
        //发送 binder_write_read 数据
        res = ioctl(bs->fd, BINDER_WRITE_READ, &bwr);

        if (res < 0) {
            fprintf(stderr,"binder: ioctl failed (%s)\n", strerror(errno));
            goto fail;
        }

        //省略部分代码 ......
        //关注点4 解析收到的数据 readbuf
        // res = binder_parse(bs, reply, (uintptr_t) readbuf, bwr.read_consumed, 0);
        // if (res == 0) return 0;
        // if (res < 0) goto fail;
    }

fail:
    memset(reply, 0, sizeof(*reply));
    reply->flags |= BIO_F_IOERROR;
    return -1;
}
```

首先构建好需要发送的数据，其格式如下图所示：

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230530114222.png)

接着调用：

```c
ioctl(bs->fd, BINDER_WRITE_READ, &bwr)
```

软中断陷入内核，经过 vfs 的处理后，最终会调用到 binder 驱动的 binder_ioctl 函数：

```c
static long binder_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	int ret;
	struct binder_proc *proc = filp->private_data;
	struct binder_thread *thread;
	unsigned int size = _IOC_SIZE(cmd);
	void __user *ubuf = (void __user *)arg;

    //获得当前进程的 binder_thread 结构体
    //没有就新建一个，并插入 binder_proc 的 threads 链表中
	thread = binder_get_thread(proc);
	if (thread == NULL) {
		ret = -ENOMEM;
		goto err;
	}

	switch (cmd) {
	case BINDER_WRITE_READ: //进 binder_ioctl_write_read 函数 
		ret = binder_ioctl_write_read(filp, cmd, arg, thread);
		if (ret)
			goto err;
		break;
	//省略不相关case
	default:
		ret = -EINVAL;
		goto err;
	}
	ret = 0;
err:
	if (thread)
		thread->looper_need_return = false;
	return ret;
}

```

接着我们看看 binder_ioctl_write_read 的实现：

```c
static int binder_ioctl_write_read(struct file *filp,
				unsigned int cmd, unsigned long arg,
				struct binder_thread *thread)
{
	int ret = 0;
	struct binder_proc *proc = filp->private_data;
	unsigned int size = _IOC_SIZE(cmd);
	void __user *ubuf = (void __user *)arg;
	struct binder_write_read bwr;

    //......

    //将应用层的 binder_write_read 拷贝到内核层
	if (copy_from_user(&bwr, ubuf, sizeof(bwr))) {
		ret = -EFAULT;
		goto out;
	}

	if (bwr.write_size > 0) {
        //走这里
		ret = binder_thread_write(proc, thread,
					  bwr.write_buffer,
					  bwr.write_size,
					  &bwr.write_consumed);

		if (ret < 0) {
			bwr.read_consumed = 0;
			if (copy_to_user(ubuf, &bwr, sizeof(bwr)))
				ret = -EFAULT;
			goto out;
		}
	}

	if (bwr.read_size > 0) {
		ret = binder_thread_read(proc, thread, bwr.read_buffer,
					 bwr.read_size,
					 &bwr.read_consumed,
					 filp->f_flags & O_NONBLOCK);
		trace_binder_read_done(ret);
		binder_inner_proc_lock(proc);
		if (!binder_worklist_empty_ilocked(&proc->todo))
			binder_wakeup_proc_ilocked(proc);
		binder_inner_proc_unlock(proc);
		if (ret < 0) {
			if (copy_to_user(ubuf, &bwr, sizeof(bwr)))
				ret = -EFAULT;
			goto out;
		}
	}
	
	if (copy_to_user(ubuf, &bwr, sizeof(bwr))) {
		ret = -EFAULT;
		goto out;
	}
out:
	return ret;
}
```

接着看看 binder_thread_write 的实现：

```c
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

        //获取到第一个数据 cmd
		if (get_user(cmd, (uint32_t __user *)ptr))
			return -EFAULT;
		ptr += sizeof(uint32_t);
		trace_binder_command(cmd);
		if (_IOC_NR(cmd) < ARRAY_SIZE(binder_stats.bc)) {
			atomic_inc(&binder_stats.bc[_IOC_NR(cmd)]);
			atomic_inc(&proc->stats.bc[_IOC_NR(cmd)]);
			atomic_inc(&thread->stats.bc[_IOC_NR(cmd)]);
		}
		switch (cmd) {
		//省略不相关的 case
		case BC_TRANSACTION:
		case BC_REPLY: {
			struct binder_transaction_data tr;

            //将 binder_transaction_data 数据读到内核
			if (copy_from_user(&tr, ptr, sizeof(tr)))
				return -EFAULT;
			ptr += sizeof(tr);
            //binder_transaction 发起远程调用
			binder_transaction(proc, thread, &tr,
					   cmd == BC_REPLY, 0);
			break;
		}
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

接着看 binder_transaction，这个函数巨长，看源码之前我们先看几个结构体：

**binder_transaction**

binder_transaction 结构体用来描述进程间通信过程（事务）:

```c
struct binder_transaction {
    int debug_id;
    //用来描述需要处理的工作事项
    struct binder_work work;
    //发起事务的线程
    struct binder_thread *from;
    //事务所依赖的另一个事务
    struct binder_transaction *from_parent;
    //处理该事务的进程
    struct binder_proc *to_proc;
    //处理该事务的线程
    struct binder_thread *to_thread;
    //目标线程下一个需要处理的事务
    struct binder_transaction *to_parent;
    //1: 表示同步事务，需要等待对方回复
    //0: 表示异步事务
    unsigned need_reply:1;
    
    //指向为该事务分配的内核缓冲区
    struct binder_buffer *buffer;
    unsigned int	code;
    unsigned int	flags;
    //发起事务线程的优先级
    struct binder_priority	priority;
    //线程在处理事务时，驱动会修改它的优先级以满足源线程和目标Service组建的要求
    //在修改之前，会将它原来的线程优先级保存在该成员中，以便线程处理完该事务后可以恢复原来的优先级
    struct binder_priority	saved_priority;
    bool    set_priority_called;
    kuid_t	sender_euid;
    binder_uintptr_t security_ctx;

    spinlock_t lock;
};

```

**binder_work**
binder_work 结构体用来描述需要处理的工作事项：

```c
struct binder_work {
    //双向链表中的一个节点，这个链表储存了所有的binder_work
    struct list_head entry;

    //工作项类型
    enum binder_work_type {
        BINDER_WORK_TRANSACTION = 1,
        BINDER_WORK_TRANSACTION_COMPLETE,
        BINDER_WORK_RETURN_ERROR,
        BINDER_WORK_NODE,
        BINDER_WORK_DEAD_BINDER,
        BINDER_WORK_DEAD_BINDER_AND_CLEAR,
        BINDER_WORK_CLEAR_DEATH_NOTIFICATION,
    } type;
};
```

binder_transaction 函数可分为以下三个流程：

* 获得目标进程/线程信息
* 将数据拷贝到目标进程所映射的内存中（此时会建立实际的映射关系）
* 将待处理的任务加入 todo 队列，唤醒目标线程

```c
static void binder_transaction(struct binder_proc *proc,
			       struct binder_thread *thread,
			       struct binder_transaction_data *tr, int reply,
			       binder_size_t extra_buffers_size)
{
	int ret;
	struct binder_transaction *t;
	struct binder_work *tcomplete;
	binder_size_t buffer_offset = 0;
	binder_size_t off_start_offset, off_end_offset;
	binder_size_t off_min;
	binder_size_t sg_buf_offset, sg_buf_end_offset;
	struct binder_proc *target_proc = NULL;
	struct binder_thread *target_thread = NULL;
	struct binder_node *target_node = NULL;
	struct binder_transaction *in_reply_to = NULL;
	struct binder_transaction_log_entry *e;
	uint32_t return_error = 0;
	uint32_t return_error_param = 0;
	uint32_t return_error_line = 0;
	binder_size_t last_fixup_obj_off = 0;
	binder_size_t last_fixup_min_off = 0;
	struct binder_context *context = proc->context;
	int t_debug_id = atomic_inc_return(&binder_last_id);
	char *secctx = NULL;
	u32 secctx_sz = 0;

	e = binder_transaction_log_add(&binder_transaction_log);
	e->debug_id = t_debug_id;
	e->call_type = reply ? 2 : !!(tr->flags & TF_ONE_WAY);
	e->from_proc = proc->pid;
	e->from_thread = thread->pid;
	e->target_handle = tr->target.handle;
	e->data_size = tr->data_size;
	e->offsets_size = tr->offsets_size;
	e->context_name = proc->context->name;

	if (reply) {
		//......
	} else { //传入的 handle 值为 0
		if (tr->target.handle) {
			//......
		} else {
			mutex_lock(&context->context_mgr_node_lock);
            // 从 binder_proc context->binder_context_mgr_node
            // 拿到 ServiceManager 对应的 binder_node
			target_node = context->binder_context_mgr_node;
            //通过 target_node 获得 target_proc
            if (target_node)
				target_node = binder_get_node_refs_for_txn(
						target_node, &target_proc,
						&return_error);
			else
				return_error = BR_DEAD_REPLY;
			//......
		}
		if (!target_node) {
			//......
		}
		

        //此时，thread->transaction_stack 为空，不会进 if
		if (!(tr->flags & TF_ONE_WAY) && thread->transaction_stack) {
            //......
		}
		binder_inner_proc_unlock(proc);
	}

    //......
	
	//struct binder_transaction *t;
	t = kzalloc(sizeof(*t), GFP_KERNEL);
	
    //.....

	binder_stats_created(BINDER_STAT_TRANSACTION);
	spin_lock_init(&t->lock);

	tcomplete = kzalloc(sizeof(*tcomplete), GFP_KERNEL);
	
	binder_stats_created(BINDER_STAT_TRANSACTION_COMPLETE);

	t->debug_id = t_debug_id;

	if (!reply && !(tr->flags & TF_ONE_WAY))
		t->from = thread;
	else
		t->from = NULL;
	t->sender_euid = task_euid(proc->tsk);
	t->to_proc = target_proc;
	t->to_thread = target_thread;
	t->code = tr->code;
	t->flags = tr->flags;
    
    //......

	//分配内存并完成映射
	t->buffer = binder_alloc_new_buf(&target_proc->alloc, tr->data_size,
		tr->offsets_size, extra_buffers_size,
		!reply && (t->flags & TF_ONE_WAY));
	
	
    //......

	t->buffer->debug_id = t->debug_id;
	t->buffer->transaction = t;
	t->buffer->target_node = target_node;
    //......
```

这里我们先看一下 binder_alloc_new_buf 是如何分配内存的：

```c
//加锁后调用 binder_alloc_new_buf_locked
struct binder_buffer *binder_alloc_new_buf(struct binder_alloc *alloc,
					   size_t data_size,
					   size_t offsets_size,
					   size_t extra_buffers_size,
					   int is_async)
{
	struct binder_buffer *buffer;

	mutex_lock(&alloc->mutex);
	buffer = binder_alloc_new_buf_locked(alloc, data_size, offsets_size,extra_buffers_size, is_async);
	mutex_unlock(&alloc->mutex);
	return buffer;
}

static struct binder_buffer *binder_alloc_new_buf_locked(
                struct binder_alloc *alloc,
                size_t data_size,
                size_t offsets_size,
                size_t extra_buffers_size,
                int is_async)
{
    struct rb_node *n = alloc->free_buffers.rb_node;
    struct binder_buffer *buffer;
    size_t buffer_size;
    struct rb_node *best_fit = NULL;
    void *has_page_addr;
    void *end_page_addr;
    size_t size, data_offsets_size;
    int ret;

    if (alloc->vma == NULL) {
        ... //error
    }

    //计算需要的缓冲区大小
    //这里需要将size对齐void *（32位下占用4字节，64位下占用8字节）
    data_offsets_size = ALIGN(data_size, sizeof(void *)) +
        ALIGN(offsets_size, sizeof(void *));
    size = data_offsets_size + ALIGN(extra_buffers_size, sizeof(void *));
    
    
     = max(size, sizeof(void *));

    //从binder_alloc的空闲缓冲红黑树中找到一个大小最合适的binder_buffer
    while (n) {
        //当找到一个需求大小和缓存区大小刚好相同的空闲缓存区时
        //此时buffer就正好指向这个空闲缓存区
        buffer = rb_entry(n, struct binder_buffer, rb_node);
        BUG_ON(!buffer->free);
        buffer_size = binder_alloc_buffer_size(alloc, buffer);

        //当只找到一个比需求大小稍大一点的空闲缓存区时
        //此时buffer指向的是这个空闲缓存区所在节点的父节点
        //然后n指向NULL
        if (size < buffer_size) {
            best_fit = n;
            n = n->rb_left;
        } else if (size > buffer_size)
            n = n->rb_right;
        else {
            best_fit = n;
            break;
        }
    }
    if (best_fit == NULL) {
        ... //error
    }
    //此时buffer指向的是所需求的空闲缓存区所在红黑树节点的父节点
    //需要让其指向真正需求的那个空闲缓存区
    if (n == NULL) {
        buffer = rb_entry(best_fit, struct binder_buffer, rb_node);
        buffer_size = binder_alloc_buffer_size(alloc, buffer);
    }

    //计算出buffer的终点，向下对齐（不能超过可用的buffer_size）
    has_page_addr =
        (void *)(((uintptr_t)buffer->data + buffer_size) & PAGE_MASK);
    WARN_ON(n && buffer_size != size);
    //计算出实际上我们接收数据需要的空间的终点，向上映射
    end_page_addr =
        (void *)PAGE_ALIGN((uintptr_t)buffer->data + size);
    //如果超出了可用的buffer_size，恢复到正常可用的结束地址
    if (end_page_addr > has_page_addr)
        end_page_addr = has_page_addr;
    //分配物理页，建立映射
    ret = binder_update_page_range(alloc, 1,
        (void *)PAGE_ALIGN((uintptr_t)buffer->data), end_page_addr);
    if (ret)
        return ERR_PTR(ret);

    //有空余空间的话，分隔这个buffer，剩余的buffer加入到空闲缓存区红黑树中（合理利用空间）
    if (buffer_size != size) {
        struct binder_buffer *new_buffer;

        new_buffer = kzalloc(sizeof(*buffer), GFP_KERNEL);
        new_buffer->data = (u8 *)buffer->data + size;
        list_add(&new_buffer->entry, &buffer->entry);
        new_buffer->free = 1;
        binder_insert_free_buffer(alloc, new_buffer);
    }

    //我们已经使用了这个buffer，要将其从空闲缓存区红黑树中移除
    rb_erase(best_fit, &alloc->free_buffers);
    //标记为非空闲
    buffer->free = 0;
    buffer->allow_user_free = 0;
    //插入到已分配缓存区红黑树中
    binder_insert_allocated_buffer_locked(alloc, buffer);
    buffer->data_size = data_size;
    buffer->offsets_size = offsets_size;
    buffer->async_transaction = is_async;
    buffer->extra_buffers_size = extra_buffers_size;
    //如果是异步事件, 那么更新binder_alloc的异步事件空闲buffer
    if (is_async) {
        alloc->free_async_space -= size + sizeof(struct binder_buffer);
    }
    return buffer;
    ...
}

```

```c
    //将用户态的数据（data部分）拷贝到目标进程的 binder_buffer中
	if (binder_alloc_copy_user_to_buffer(
				&target_proc->alloc,
				t->buffer, 0,
				(const void __user *)
					(uintptr_t)tr->data.ptr.buffer,
				tr->data_size)) {
		binder_user_error("%d:%d got transaction with invalid data ptr\n",
				proc->pid, thread->pid);
		return_error = BR_FAILED_REPLY;
		return_error_param = -EFAULT;
		return_error_line = __LINE__;
		goto err_copy_data_failed;
	}

    //将用户态的数据（off部分）拷贝到目标进程的 binder_buffer中
	if (binder_alloc_copy_user_to_buffer(
				&target_proc->alloc,
				t->buffer,
				ALIGN(tr->data_size, sizeof(void *)),
				(const void __user *)
					(uintptr_t)tr->data.ptr.offsets,
				tr->offsets_size)) {
		binder_user_error("%d:%d got transaction with invalid offsets ptr\n",
				proc->pid, thread->pid);
		return_error = BR_FAILED_REPLY;
		return_error_param = -EFAULT;
		return_error_line = __LINE__;
		goto err_copy_data_failed;
	}
	

	off_start_offset = ALIGN(tr->data_size, sizeof(void *));
	buffer_offset = off_start_offset;
	off_end_offset = off_start_offset + tr->offsets_size;
	sg_buf_offset = ALIGN(off_end_offset, sizeof(void *));
	sg_buf_end_offset = sg_buf_offset + extra_buffers_size;
	off_min = 0;

    //
	for (buffer_offset = off_start_offset; buffer_offset < off_end_offset;
	     buffer_offset += sizeof(binder_size_t)) {
		struct binder_object_header *hdr;
		size_t object_size;
		struct binder_object object;
		binder_size_t object_offset;

		binder_alloc_copy_from_buffer(&target_proc->alloc,
					      &object_offset,
					      t->buffer,
					      buffer_offset,
					      sizeof(object_offset));
		object_size = binder_get_object(target_proc, t->buffer,
						object_offset, &object);
		if (object_size == 0 || object_offset < off_min) {
			binder_user_error("%d:%d got transaction with invalid offset (%lld, min %lld max %lld) or object.\n",
					  proc->pid, thread->pid,
					  (u64)object_offset,
					  (u64)off_min,
					  (u64)t->buffer->data_size);
			return_error = BR_FAILED_REPLY;
			return_error_param = -EINVAL;
			return_error_line = __LINE__;
			goto err_bad_offset;
		}

		hdr = &object.hdr;
		off_min = object_offset + object_size;
		switch (hdr->type) {
		case BINDER_TYPE_BINDER:
		case BINDER_TYPE_WEAK_BINDER: {
			struct flat_binder_object *fp;

			fp = to_flat_binder_object(hdr);
			ret = binder_translate_binder(fp, t, thread);
			if (ret < 0) {
				return_error = BR_FAILED_REPLY;
				return_error_param = ret;
				return_error_line = __LINE__;
				goto err_translate_failed;
			}
			binder_alloc_copy_to_buffer(&target_proc->alloc,
						    t->buffer, object_offset,
						    fp, sizeof(*fp));
		} break;
		
		default:
			binder_user_error("%d:%d got transaction with invalid object type, %x\n",
				proc->pid, thread->pid, hdr->type);
			return_error = BR_FAILED_REPLY;
			return_error_param = -EINVAL;
			return_error_line = __LINE__;
			goto err_bad_object_type;
		}
	}
	tcomplete->type = BINDER_WORK_TRANSACTION_COMPLETE;
	t->work.type = BINDER_WORK_TRANSACTION;

	if (reply) {
		//......
	} else if (!(t->flags & TF_ONE_WAY)) {
		BUG_ON(t->buffer->async_transaction != 0);
		binder_inner_proc_lock(proc);
		/*
		 * Defer the TRANSACTION_COMPLETE, so we don't return to
		 * userspace immediately; this allows the target process to
		 * immediately start processing this transaction, reducing
		 * latency. We will then return the TRANSACTION_COMPLETE when
		 * the target replies (or there is an error).
		 */
		binder_enqueue_deferred_thread_work_ilocked(thread, tcomplete);
		t->need_reply = 1;
		t->from_parent = thread->transaction_stack;
		thread->transaction_stack = t;
		binder_inner_proc_unlock(proc);
		if (!binder_proc_transaction(t, target_proc, target_thread)) {
			binder_inner_proc_lock(proc);
			binder_pop_transaction_ilocked(thread, t);
			binder_inner_proc_unlock(proc);
			goto err_dead_proc_or_thread;
		}
	} else {
		//......
	}
	if (target_thread)
		binder_thread_dec_tmpref(target_thread);
	binder_proc_dec_tmpref(target_proc);
	if (target_node)
		binder_dec_node_tmpref(target_node);
	/*
	 * write barrier to synchronize with initialization
	 * of log entry
	 */
	smp_wmb();
	WRITE_ONCE(e->debug_id_done, t_debug_id);
	return;

err_dead_proc_or_thread:
	return_error = BR_DEAD_REPLY;
	return_error_line = __LINE__;
	binder_dequeue_work(proc, tcomplete);
err_translate_failed:
err_bad_object_type:
err_bad_offset:
err_bad_parent:
err_copy_data_failed:
	trace_binder_transaction_failed_buffer_release(t->buffer);
	binder_transaction_buffer_release(target_proc, t->buffer,
					  buffer_offset, true);
	if (target_node)
		binder_dec_node_tmpref(target_node);
	target_node = NULL;
	t->buffer->transaction = NULL;
	binder_alloc_free_buf(&target_proc->alloc, t->buffer);
err_binder_alloc_buf_failed:
	if (secctx)
		security_release_secctx(secctx, secctx_sz);
err_get_secctx_failed:
	kfree(tcomplete);
	binder_stats_deleted(BINDER_STAT_TRANSACTION_COMPLETE);
err_alloc_tcomplete_failed:
	kfree(t);
	binder_stats_deleted(BINDER_STAT_TRANSACTION);
err_alloc_t_failed:
err_bad_call_stack:
err_empty_call_stack:
err_dead_binder:
err_invalid_target_handle:
	if (target_thread)
		binder_thread_dec_tmpref(target_thread);
	if (target_proc)
		binder_proc_dec_tmpref(target_proc);
	if (target_node) {
		binder_dec_node(target_node, 1, 0);
		binder_dec_node_tmpref(target_node);
	}

	binder_debug(BINDER_DEBUG_FAILED_TRANSACTION,
		     "%d:%d transaction failed %d/%d, size %lld-%lld line %d\n",
		     proc->pid, thread->pid, return_error, return_error_param,
		     (u64)tr->data_size, (u64)tr->offsets_size,
		     return_error_line);

	{
		struct binder_transaction_log_entry *fe;

		e->return_error = return_error;
		e->return_error_param = return_error_param;
		e->return_error_line = return_error_line;
		fe = binder_transaction_log_add(&binder_transaction_log_failed);
		*fe = *e;
		/*
		 * write barrier to synchronize with initialization
		 * of log entry
		 */
		smp_wmb();
		WRITE_ONCE(e->debug_id_done, t_debug_id);
		WRITE_ONCE(fe->debug_id_done, t_debug_id);
	}

	BUG_ON(thread->return_error.cmd != BR_OK);
	if (in_reply_to) {
		binder_restore_priority(current, in_reply_to->saved_priority);
		thread->return_error.cmd = BR_TRANSACTION_COMPLETE;
		binder_enqueue_thread_work(thread, &thread->return_error.work);
		binder_send_failed_reply(in_reply_to, return_error);
	} else {
		thread->return_error.cmd = return_error;
		binder_enqueue_thread_work(thread, &thread->return_error.work);
	}
}
```

## 参考资料

* [Android源码分析 - Binder驱动（中）](https://juejin.cn/post/7069675794028560391)