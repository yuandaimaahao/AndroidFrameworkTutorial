# 如何阅读 Android 系统源码 —— Java 篇

## 1. 工具篇 —— AIDEGen + Android Studio 

AIDEGen，我叫它 “爱得跟”，大概是 Android10 中，google 推出的一个工具，用于方便开发者使用 IDE 来查看和修改系统源码。AIDEGen + Android Studio 非常适合用于阅读与修改 Android 系统 Java 代码。

使用 “爱得跟” 之前我们需要做一些准备工作，编译 sdk：

```bash
source build/envsetup.sh
lunch sdk-eng
# or
#lunch sdk-userdebug
# or
#lunch sdk-user
make sdk
```

接着我们需要去 google 官方下载 Android Studio，并将其放到 /opt 目录下。这推荐使用 3.6.3 版本，与我们的 Android10_r41 更为匹配。

接下来，选择我们的目标 Product：

```bash
source build/envsetup.sh
lunch aosp_x86_64-eng  #也可以是其他 product
```

做好以上准备工作后，我们就可以打开系统模块了，：

```bash
# Settings 是系统应用 设置
aidegen Settings -i s  # -i 表示使用ide，s 表示 Android Studio

# 也可以打开一个路径，确保这个路径下有 Android.bp 或者 Android.mk 即可
aidegen frameworks/base/services -i s
```

稍作等待，Android Studio 就打开了，不过现在还不能直接使用，我们还需要做一些基本的配置:


Android Studio 打开后，右下角会出现一个提示框（如果没有出现，点击 file -> Invalidate Caches -> Invalidate and Restart）：

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230330232504.png)

点击 Configure，出现以下界面：

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230330232744.png)

点击 ok 即可。

接着配置 jdk 和 sdk，点击 file -> Project Structure：

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230330233341.png)

接着点击 + 号：

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230330233429.png)

点击 `Add JDK`：

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230330233429.png)

选择源码下的 `prebuilts/jdk/jdk9/linux-x86`,然后点击 ok:

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230330233925.png)

接着将 name 修改为 aosp10-jdk9，然后点击右下角 apply:

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230330234053.png)

接着点击 + 号，添加 Android SDK：

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230330234337.png)

选择系统源码目录下的 `out/host/linux-x86/sdk/sdk/android-sdk_eng.zzh0838_linux-x86` (最后一个文件夹的名字和你的用户名相关，你的和我的可能不一样)，然后点击 ok：

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230330235615.png)

接着 Java SDK 选择刚刚添加的 aosp10-jdk9，点击 ok：

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230330235743.png)

接着，修改 skd 的名字为 aosp10-android-sdk29:

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230330235943.png)

点击 ok 即可。


接着我们需要配置项目的 sdk，点击 file -> Project Structure，点击左侧 Project，右侧 SDK 选项选择 aosp10-android-sdk：

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230331000336.png)

点击左侧 Modules，中间选择 Settings，右侧 Module SDK 选择 Project SDK：

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230331000444.png)

接着，中间选择 dependencies-Settings，右侧 Module SDK 选择 Project SDK：

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230331000540.png)

最后点击 apply，项目开始重新索引，项目内绝大部分类方法变量均可正确跳转。

至此，配置完成。


## 2. 手段篇 —— Java 源码的阅读

Java 源码我们一般通过以下两种方法阅读：

1. Log + 打印调用栈
2. AIDEGen + Android Studio + 断点调试

针对启动相关的源码，一般是使用第一种方法阅读，接下来我们来看看如何阅读系统启动过程中的重要进程 SystemServer：

首先找到 SystemServer：

```bash
#源码根目录下
cd frameworks

find . -name "SystemServer*"
./base/cmds/svc/src/com/android/commands/svc/SystemServerCommand.java
./base/services/java/com/android/server/SystemServer.java
./base/services/core/java/com/android/server/SystemServerInitThreadPool.java

# aidengen 打开
aidegen frameworks/base/services
```

在 Android Studio 中我们打开 SystemServer.java，找到 main 方法，并添加下面的代码：


```java
public static void main(String[] args) {

    //通过打印 log 了解程序的运行状况
    Log.d("yuandaima", "SystemServer is starting");
    //通过打印堆栈了解方法的调用路径
    printStackTrace(Thread.currentThread());
        
    new SystemServer().run();
}

 

//打印调用堆栈的方法
public static void printStackTrace(Thread thread) {
        for (StackTraceElement e : thread.getStackTrace()) {
            Log.d("yuandaima", e.toString());
        }
}
  
```

接着我们编译系统，开启模拟器，然后获取日志信息：

```bash
rice14:/ # logcat | grep "yuandaima"
05-10 21:37:04.719  1641  1641 D yuandaima: SystemServer is starting
05-10 21:37:04.720  1641  1641 D yuandaima: dalvik.system.VMStack.getThreadStackTrace(Native Method)
05-10 21:37:04.721  1641  1641 D yuandaima: java.lang.Thread.getStackTrace(Thread.java:1720)
05-10 21:37:04.721  1641  1641 D yuandaima: com.android.server.SystemServer.printStackTrace(SystemServer.java:350)
05-10 21:37:04.721  1641  1641 D yuandaima: com.android.server.SystemServer.main(SystemServer.java:362)
05-10 21:37:04.721  1641  1641 D yuandaima: java.lang.reflect.Method.invoke(Native Method)
05-10 21:37:04.721  1641  1641 D yuandaima: com.android.internal.os.RuntimeInit$MethodAndArgsCaller.run(RuntimeInit.java:492)
05-10 21:37:04.721  1641  1641 D yuandaima: com.android.internal.os.ZygoteInit.main(ZygoteInit.java:908)
```

通过打日志我们可以了解系统程序的运行状况以及一些关键参数，通过调用堆栈我们可以了解程序运行过程中方法的一个调用流程。


非启动相关的源码，通常结合两种方法来阅读。接下来我们看看如何断点调试系统 App：

首先，我们开一个新的终端，打开模拟器：

```bash
source build/envsetup.sh
lunch aosp_x86_64-eng
emulator
```

接着我们在 Settings/AndroidManifest.xml 查找到 Settings App 的首 Activity 为 SettingsHomepageActivity，我们在其 onCreate 出打一个断点：

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230331001249.png)


接着在 Android 模拟器中打开 Settings 应用。

接着点击 android studio 的 debug 按钮：

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230331001845.png)

在弹出的窗口中，依次选择 Settings 和 com.android.settings,然后点击 ok：

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230331002002.png)

在模拟器中，点击返回键，再次进入 Settings 应用，即可进入调试界面：

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230331002134.png)

接着我们就可以顺利的进行调试了。


## 关于


如果你对 Framework 感兴趣或者正在学习 Framework，可以参考我总结的[Android Framework 学习路线指南](https://github.com/yuandaimaahao/AndroidFrameworkTutorial),也可关注我的微信公众号，我会在公众号上持续分享我的经验，帮助正在学习的你少走一些弯路。学习过程中如果你有疑问或者你的经验想要分享给大家可以添加我的微信，我拉你进技术交流群。

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/4e7348e352774883ecb19ab021d6cee.jpg)




