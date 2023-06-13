# Binder 程序示例之 java 篇

这是一个 Binder 相关的系列教程,持续更新中：

* 学习 Binder 的预备知识
  * [虚拟内存与 Linux 文件访问接口](https://juejin.cn/post/7232459954581307448)
  * [Linux 内核常用数据结构](https://juejin.cn/post/7234370256746348581)
  * [linux 驱动开发入门](https://juejin.cn/post/7235309338097123388)
* [Binder 基本原理](https://juejin.cn/post/7210175991837392933)
* [Binder 程序示例之 C 语言篇](https://juejin.cn/post/7210245482861264955)
* [Binder 服务注册过程情景分析之C语言篇](https://juejin.cn/post/7214342319347712057)
* [Binder 服务获取与使用过程情景分析之C语言篇](https://juejin.cn/post/7215401973842706491)
* [Binder 驱动情景分析之 ServiceManager 启动过程](https://juejin.cn/post/7238491755446878267)
* [Binder 驱动情景分析之服务注册过程](https://juejin.cn/post/7241452728747262013)
* [Binder 驱动情景分析之服务获取与使用过程](https://juejin.cn/post/7241884241615831098)
* [Binder C++ 程序示例](https://juejin.cn/post/7222109036588810297)
* [Binder 程序示例之 aidl-cpp 篇](https://juejin.cn/post/7242258285159546936)
* [Binder C++ 程序分析之主要类解析](https://juejin.cn/post/7223185420157763641/)
* [Binder 服务注册过程情景分析之 C++ 篇](https://juejin.cn/post/7223185420157943865)
* [Binder 服务获取与使用过程情景分析之 C++ 篇](https://juejin.cn/post/7223410490763771941)
* Binder 程序示例之 Java 篇【本文】
* Binder Java 程序分析之主要类解析
* Binder 服务注册过程情景分析之 Java 篇
* Binder 服务获取与使用过程情景分析之 Java 篇
* Binder 多线程分析
* Binder 匿名服务分析
* Binder 死亡通知机制
* Binder 相关疑难 bug 解析
* Binder 面试题解析

本文基于 `AOSP Android10 r41` 源码环境

本文示例程序可以在 https://github.com/yuandaimaahao/AndroidFrameworkTutorial/tree/main/3.Binder/src/BinderJavaDemo 下载到

本文给出一个简单的 binder java 示例程序，后文的分析会基于该示例程序：

## 1. 定义 aidl 协议文件

在 `device/jelly/rice14` 目录下创建 `BinderJavaDemo/com/yuandaima` 三个文件夹，然后在 `device/jelly/rice14/BinderJavaDemo/com/yuandaima` 目录下创建 IHelloService.aidl

```java
//IHelloService.aidl
package com.yuandaima;

interface IHelloService
{
	void sayhello();
	int sayhello_to(String name);
}
```

将 aidl 编译为 java 文件：

在系统源码下：

```bash
source build/envsetup.sh
aidl IHelloService.aidl
```
即可生成对应的 Java 文件 `IHelloService.java`


## 2. 实现 Hello 服务

在 `device/jelly/rice14/BinderJavaDemo/com/yuandaima` 目录下创建 `HelloService.java`

```java
//HelloService.java
package com.yuandaima;

import android.util.Log;

public class HelloService extends IHelloService.Stub {
    private static final String TAG = "HelloService";
    private int cnt1 = 0;
    private int cnt2 = 0;

    public void sayhello() throws android.os.RemoteException {
        cnt1++;
        Log.i(TAG, "sayhello : cnt = "+cnt1);
    }
    
    public int sayhello_to(java.lang.String name) throws android.os.RemoteException {
        cnt2++;
        Log.i(TAG, "sayhello_to "+name+" : cnt = "+cnt2);
        return cnt2;
    }
}
```

## 3.实现服务端

在 `device/jelly/rice14/BinderJavaDemo/com/yuandaima` 目录下创建 `Server.java`

```java
//Server.java
package com.yuandaima;

import android.util.Log;
import android.os.ServiceManager;

public class Server {

    private static final String TAG = "BinderServer";

    public static void main(String args[]) {
        /* add Service */
        Log.i(TAG, "add hello service");
        ServiceManager.addService("hello", new HelloService());

        //app_process 启动时，会启动 binder 线程用于获取和解析 binder 消息，应用程序无需关心
        while (true) {
            try {
            	Thread.sleep(100);
          	} catch (Exception e){}
        }   
    }
}
```

## 4.实现客户端

在 `device/jelly/rice14/BinderJavaDemo/com/yuandaima` 目录下创建 `Client.java`

```java
//Client.java
package com.yuandaima;

import android.util.Log;
import android.os.ServiceManager;
import android.os.IBinder;

public class Client {

    private static final String TAG = "BinderClient";

    public static void main(String args[])
    {
        
        /* 1. getService */
        IBinder binder = ServiceManager.getService("hello");
        
        if (binder == null)
        {
            Log.i(TAG, "can not get hello service");
            return;
        }

        IHelloService svr = IHelloService.Stub.asInterface(binder);

        try {
	        svr.sayhello();
	        Log.i(TAG, "call sayhello");
        } catch (Exception e) {

        }
           
        try {
	        int cnt = svr.sayhello_to("hello");
	    
	        Log.i(TAG, "call sayhello_to " + " : cnt = " + cnt);
        } catch (Exception e) {
            System.out.println("call sayhello_to , err :"+e);
            Log.i(TAG, "call sayhello_to , err : "+e);
        }
    }
}
```


## 5. 编译运行

编写 Android.bp 文件：

```json
java_library {
    name: "BinderClient",
    installable: true,
    srcs: [ "Client.java", 
            "HelloService.java", 
            "IHelloService.java" ],
}

java_library {
    name: "BinderServer",
    installable: true,
    srcs: [ "Server.java", 
            "HelloService.java", 
            "IHelloService.java" ],
}
```


接着把 jar 包 push 到模拟器上并执行：

```bash
# 将项目源码放到系统源码下
# 在项目目录下执行
mm
# 将 jar 包 push 到虚拟机上
cd out/target/product/generic_x86_64/system/framework
adb push BinderClient.jar /data/local/tmp
adb push BinderServer.jar /data/local/tmp

# 执行 java 程序
export CLASSPATH=/data/local/tmp/BinderClient.jar:/data/local/tmp/BinderServer.jar
# 启动服务端
app_process /data/local/tmp  com.yuandaima.Server
# 启动客户端
app_process /data/local/tmp com.yuandaima.Client 
```

查看 log：

```bash
logcat | grep "BinderServer"

02-15 12:20:20.493  4171  4171 I BinderServer: add hello service

logcat | grep "BinderClient"

02-15 12:20:43.169  4183  4183 I BinderClient: call sayhello
02-15 12:20:43.169  4183  4183 I BinderClient: call sayhello_to  : cnt = 1
```

## 关于

我叫阿豪，2015 年本科毕业于国防科技大学指挥自动化专业，毕业后，在某单位从事信息化装备的研发工作。主要研究方向为 Android Framework 与 Linux Kernel，2023年春节后开始做 Android Framework 相关的技术分享。

如果你对 Framework 感兴趣或者正在学习 Framework，可以参考我总结的[Android Framework 学习路线指南](https://github.com/yuandaimaahao/AndroidFrameworkTutorial),也可关注我的微信公众号，我会在公众号上持续分享我的经验，帮助正在学习的你少走一些弯路。学习过程中如果你有疑问或者你的经验想要分享给大家可以添加我的微信，我拉你进技术交流群。

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/4e7348e352774883ecb19ab021d6cee.jpg)