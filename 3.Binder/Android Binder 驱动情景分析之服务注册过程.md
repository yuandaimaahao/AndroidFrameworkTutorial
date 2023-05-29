# Android Binder 驱动情景分析之服务注册过程

本文系统源码版本：

* AOSP 分支：android-10.0.0_r41
* Kernel 分支：android-goldfish-4.14-gchips

本文依托于[Binder 程序示例之 C 语言篇](https://juejin.cn/post/7210245482861264955)中介绍的应用层示例程序来对驱动的实现做情景化分析。

上文说到，ServiceManager 进入循环，开始读数据后，就进入休眠状态，直到有其他进程写入数据并唤醒他为止。接下来我们就来看看 ServiceManager 是怎么被唤醒的。


## Service 主函数

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

binder_open 的调用流程与 ServiceManager 一致，这里不再重复

svcmgr_publish

```c
int svcmgr_publish(struct binder_state *bs, uint32_t target, const char *name, void *ptr)
{
    int status;
    unsigned iodata[512/4];
    struct binder_io msg, reply;

    bio_init(&msg, iodata, sizeof(iodata), 4);
    bio_put_uint32(&msg, 0);  // strict mode header
    bio_put_uint32(&msg, 0);
    bio_put_string16_x(&msg, SVC_MGR_NAME);
    bio_put_string16_x(&msg, name);
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

//binder_call 中的构造过程
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

```c
//svcmgr 的值为 0，是驱动中 serivemanager 对应的 binder_ref 红黑树中的索引值
svcmgr_publish(bs, svcmgr, "hello", hello_service_handler)

binder_call(bs, &msg, &reply, target, SVC_MGR_ADD_SERVICE)


```