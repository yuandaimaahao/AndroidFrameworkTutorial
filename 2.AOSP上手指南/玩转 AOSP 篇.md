[TOC]
# 写给应用开发的 Android Framework 教程——玩转 AOSP 篇

## 第一章 AOSP 极速上手

### 1. 硬件要求

用于 Android Framework 开发的电脑需要较强的 CPU，大内存，大存储，一般来说需要满足以下要求：

* CPU 不低于 6 核心，建议 8 核及以上
* 内存不低于 32G，建议 64G
* 存储空间不低于 500G，建议 1TB SSD

### 2. 虚拟机安装

这里推荐在 Windows 下使用 VMware 虚拟机软件安装 Ubuntu20.04 进行开发（AOSP 官网推荐的的是 Ubuntu18.04，但 Ubuntu20.04 在 VMware下兼容性更好，实测也没有任何兼容性问题），安装系统的过程如下：

**在[官网](https://releases.ubuntu.com/focal/)下载好 Ubuntu20.04 桌面版系统镜像**

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230220211821.png)


**VMware 新建虚拟机**

打开 VMware，菜单栏选择 `文件 -> 新建虚拟机` 后，出现以下界面：

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230220212516.png)

选择 `自定义高级`，然后点击 `下一步`，出现以下界面：

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230220212614.png)

这里不用改，点击 `下一步`，出现以下界面：

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230220212726.png)

选择 `稍候安装操作系统`，点击 `下一步`，出现以下界面：

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230220212836.png)

操作系统选择 `Linux`，版本选择 `Ubuntu 64位`，然后点击 `下一步`，出现以下界面：

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230220213434.png)

`虚拟机名称`处取一个你喜欢的名字，`位置`选择剩余空间超过 500G 的路径，然后点击 `下一步`，出现以下界面：

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230220213715.png)

`处理器数量`选择 1，`每个处理器的内核数量`选择你 CPU 的线程数，比如我的 CPU 是 amd ryzen 3800x，8核心16线程，那么选择 16,然后点击 `下一步`，出现以下界面：

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230220213928.png)

虚拟机内存的设置建议尽可能的大一些，内存小了，全编译系统会出错，这里选择 32G：

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230220214329.png)

网络类型选择 `使用桥接网络`，然后点击 `下一步`，出现以下界面：

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230220214415.png)

这里不改动，直接点击下一步，出现以下界面：

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230220214521.png)

这里不改动，直接点击 `下一步`，出现以下界面：

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230220214715.png)


这里不改动，直接点击 `下一步`，出现以下界面：

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230220214944.png)


根据你磁盘的空间，`最大磁盘大小`尽可能设置大一点，我这里设置为 1024 G，然后点击 `下一步`，出现以下界面：

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230220215050.png)

这里不改动，直接点击 `下一步`，出现以下界面：

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230220215149.png)

选择 `自定义硬件`，出现以下界面：

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230220215550.png)

选择 `新 CD/DVD(SATA)`，出现以下界面：

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230220215722.png)

右侧选择 `使用 ISO 映像文件`，点击浏览，选择我们刚才下载好的系统镜像，然后点击 `关闭`，出现以下界面：

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230220215919.png)

然后点击完成，至此，虚拟机新建完毕。

**系统安装**

打开 VMware 虚拟机软件：

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230220220138.png)

点击左侧 `Ubuntu 64位` 即我们刚才新建的虚拟机，点击右侧 `开启此虚拟机`，然后就进入了 Ubuntu 的系统安装流程：

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230220224245.png)

左侧选择 `English`，右侧点击 `install Ubuntu`：

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230220224849.png)

点击 `continue`：

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230220225027.png)

点击 `continue`：

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230220225244.png)

点击 `install now`：

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230220225737.png)

点击 `continue`：

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230220225838.png)


点击以后会显示一张地图，这里就不贴图了（有地图审核不过），就是一张地图，点击一下中国即可。

然后点一下中国地图，下面显示 `Shanghai`，点击 `continue`：


![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230220225949.png)

依次输入：
* 名字
* 电脑名字
* 用户名
* 密码

然后点击 `continue`，然后等待安装完成，重启虚拟机即可。

系统安装完成后，关闭虚拟机，点击 `编辑虚拟机设置`：

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230322121536.png)

选择`处理器`，并勾选上虚拟化引擎两个选项（如果是 intel 处理器勾选第一个即可）：

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230322121711.png)


### 3. 开发环境搭建

安装好系统后需要安装必要的软件：

```bash
sudo apt-get install git-core gnupg flex bison build-essential zip curl zlib1g-dev gcc-multilib g++-multilib libc6-dev-i386 libncurses5 lib32ncurses5-dev x11proto-core-dev libx11-dev lib32z1-dev libgl1-mesa-dev libxml2-utils xsltproc unzip fontconfig python
```

至此，开发环境就搭建好了。


### 4. 下载编译源码

#### 4.1 下载 repo 工具

```bash
mkdir ~/bin
curl https://mirrors.tuna.tsinghua.edu.cn/git/git-repo -o ~/bin/repo
chmod +x ~/bin/repo
```

repo 的运行过程中会尝试访问官方的 git 源更新自己，如果想使用 tuna 的镜像源进行更新，可以将如下内容复制到你的 ~/.bashrc 或者 ~/.zshrc 里。

```bash
export REPO_URL='https://mirrors.tuna.tsinghua.edu.cn/git/git-repo'
PATH=~/bin:$PATH
```

然后 source 一下：

```bash
source ~/.bashrc
#如果使用的是 zsh
#source ~/.zshrc
```

#### 4.2 初始化仓库并同步远程代码

```bash
git config --global user.email "you@example.com"
git config --global user.name "Your Name"
mkdir aosp 
cd asop
#初始化仓库,-b 指示分支，这里使用 android10
repo init -u https://mirrors.tuna.tsinghua.edu.cn/git/AOSP/platform/manifest -b android-10.0.0_r41
#同步远程代码
repo sync
```

-b 后面的值参考[源代码标记和 build](https://source.android.com/docs/setup/start/build-numbers?hl=zh-cn#source-code-tags-and-builds)。这里选用了 android-10.0.0_r41 版本用于学习。Android 每年都会更新一个大版本，学习的角度来说，选择一个不太老的版本即可，不必追新。

#### 4.3 编译源码

```bash
source build/envsetup.sh
lunch aosp_x86_64-eng
make -j16
```

#### 4.4 运行模拟器

```bash
emulator -verbose -cores 4 -show-kernel
```

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230220231917.png)

### 5. 从一个简单的实际开发需求体验 Framework 开发

需求：去掉原生 Launcher 中的 google 搜索栏。

很多产品，比如用于仓库，电商，物流的 pda 等，更多的是考虑功能性和稳定性，UI 的美观和易用性是其次的。这些产品一般是不会重新开发和定制 Launcher 的，但是会对 Launcher 做一些修改，比如一个常见的需求是：**去掉原生 Launcher 中的 google 搜索栏**。

由于众所周知的原因 google 搜索栏在中国是没有办法使用的。在中国销售的产品，肯定是要把它去掉的。


第一步：修改文件 `packages/apps/Launcher3/res/layout/search_container_workspace.xml`，将以下内容注释掉：

```xml
    <!-- <fragment
        android:name="com.android.launcher3.qsb.QsbContainerView$QsbFragment"
        android:layout_width="match_parent"
        android:tag="qsb_view"
        android:layout_height="match_parent"/> -->
```

第二步：修改文件：`packages/apps/Launcher3/src/com/android/launcher3/Workspace.java` 将以下内容注释掉

```java
        // CellLayout.LayoutParams lp = new CellLayout.LayoutParams(0, 0, firstPage.getCountX(), 1);
        // lp.canReorder = false;
        // if (!firstPage.addViewToCellLayout(qsb, 0, R.id.search_container_workspace, lp, true)) {
        //     Log.e(TAG, "Failed to add to item at (0, 0) to CellLayout");
        // }
```

第三步：重新编译源码，启动模拟器

```
make -j16
emulator
```

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230221095253.png)

可以看到 Google 搜索框没有了。

很多同学可能有疑问，你怎么知道这么改就可以了？其实办法就一个——**读源码**。Launcher 也是一个 Android 应用，和我们平时开发的 App 没有本质的区别。就像我们接手别人开发的项目，去解决项目中的 bug，添加新的需求是一样的。

Android Framework 不同的是，代码量大，涉及知识广泛，App 开发同学如果没有人指导，阅读起来比较吃力。后续内容我们带着大家由浅入深，步步分解，为 App 开发的同学解开 Android Framework 的神秘面纱。


### 6. 总结

本文主要介绍了以下几个方面的内容：

* 虚拟机安装
* 开发环境搭建
* 下载编译源码
* 从一个简单的实际开发需求体验 Framework 开发

基本是偏向实际操作的内容，感兴趣的同学可以按照上面介绍的步骤体验一番

## 第二章 系统开发工具推荐

### 1. SSH 服务与 Tabby Terminal

SSH 服务使得我们在其他平台上通过 SSH 客户端程序即可访问到我们的 linux 服务器。方便我们的开发工作。

接下来我们分为两步来介绍 ssh 服务的搭建以及客户端的使用：

#### 1.1 配置 Ubuntu ssh 服务

**安装 openssh-server**

```bash
sudo apt install openssh-server
# 开机自启动
sudo systemctl enable ssh
# 重启 ssh 服务
sudo systemctl restart ssh
```

**配置固定 IP 地址**

```bash
sudo apt install net-tools -y  
cd /etc/netplan
# 备份旧的配置文件
sudo cp 00-installer-config.yaml  00-installer-config.yaml_before
```

```bash
# 修改配置文件：
sudo vim 00-installer-config.yaml

network:
  version: 2
  renderer: NetworkManager
  ethernets:
    enp0s5:   # 网卡名称
      dhcp4: no     # 关闭dhcp
      dhcp6: no
      addresses: [10.0.0.89/24]  # 静态ip，根据自己网络情况配置
      gateway4: 10.0.0.1     # 网关，根据自己网络情况配置
      nameservers:
        addresses: [10.0.0.1, 114.114.114.114] #dns，根据自己网络情况配置
```

使配置生效：

```bash
sudo netplan apply
```
至此，固定 IP 就配置好了。

#### 1.2 ssh 客户端 Tabby Terminal

Tabby 是一款高颜值，开源，免费，多平台的命令行工具，ssh 客户端是它的功能之一。

我们在 [Tabby官网](https://tabby.sh/) 下载安装好后，配置好 ssh 连接配置信息即可使用 ssh 服务。

配置的过程如下：

1. 点击配置
![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230330183830.png)

2. 点击配置与连接
![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230330183955.png)

3. 点击新配置
![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230330190200.png)

4. 下滑菜单，选择 `SSH 连接`
![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230330190312.png)

5. 输入 ssh 服务信息
   * 名称，随意
   * 主机填写 linux 的固定 IP 地址，端口 22
   * 用户名填写实际用户名
   * 点击设置密码，添加用户名对应的密码
   * 点击保存

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230330190602.png)

6. 鼠标移动至 UbuntuForAndroid 选项，点击三角形
![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230330191231.png)

7. 进入远程 ssh 终端界面
![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230330191401.png)


至此，我们就可以远程管理我们的 linux 了。

### 2. vim

vim 主要用于文件的简单修改，需要掌握：

* 文件的简单修改
* 文件内容搜索


**一般模式**

通过 `vim + 文件名`，进入 vim 环境（test.txt 可以是当前目录已存在的文件，也可以是当前目录不存在的文件。）：

```bash
vim test.txt
```

此时，我们在 vim 的一般模式下，如果 test.txt 文件有内容，我们可以通过：

* j 下移光标
* k 上移光标
* h 左移光标
* l 右移光标

如果用的不习惯，上下左右方向键也可以移动光标。

**编辑模式**

在一般模式下，输入 a i o 可以进入编辑模式，编辑模式下，我们可以输入文本修改文件内容。

a i o 的区别如下:

* a：在当前光标下一位插入
* i：当前光标处插入
* o：换行并创建全新空白行，并在行首插入


在编辑模式下，我们点击 esc 键即可回到一般模式。


**命令行模式**

在一般模式下，输入分号，就会进入命令行模式。在命令行中，可以输入不同的指令或者字符串以实现不同的功能。其中涉及的最常用的功能包括了两类：

* 退出 vim
* 当前文本中，搜索字符串


进入命令行模式后，有三种方式来退出 vim：

* 当前文件没有做任何改变时，输入 `q` 指令，退出 vim 环境
* 当前文件做了一些修改，如果想放弃已修改的内容，输入 `q!` 指令，退出 vim 环境
* 当前文件做了一些修改，如果想保存已修改的内容，输入 `wq` 指令，退出 vim 环境

进入命令行模式后，我们还可以输入 `/` + 字符串，点击回车键，进入搜索模式，点击 N 键，光标就会在匹配处依次跳动。

在一般模式下，我们也可以直接输入 `/` + 字符串进行搜索。 

**vim 基本使用小结**

vim 编辑器的三种模式间切换如下图所示：

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230224141446.png)

### 3. find + grep 命令

Android 系统源码繁杂，通常我们通过 find 命令来查找我们关心的文件。使用 find + grep 命令查找我们关心的文件内容。

* 找文件，比如 service_manager.c

```bash
find . -name "service_manager.c"
./frameworks/native/cmds/servicemanager/service_manager.c
```

* 找文件内容，找 recyclerview 库

```bash
find . -name "Android.bp" | xargs grep "name: \".*recyclerview.*\""
./prebuilts/sdk/current/androidx/Android.bp:    name: "androidx.recyclerview_recyclerview-selection-nodeps",
./prebuilts/sdk/current/androidx/Android.bp:    name: "androidx.recyclerview_recyclerview-selection",
./prebuilts/sdk/current/androidx/Android.bp:    name: "androidx.recyclerview_recyclerview-nodeps",
./prebuilts/sdk/current/androidx/Android.bp:    name: "androidx.recyclerview_recyclerview",
./prebuilts/sdk/current/support/Android.bp:    name: "android-support-recyclerview-selection-nodeps",
./prebuilts/sdk/current/support/Android.bp:    name: "android-support-recyclerview-selection",
./prebuilts/sdk/current/support/Android.bp:    name: "android-support-v7-recyclerview-nodeps",
./prebuilts/sdk/current/support/Android.bp:    name: "android-support-v7-recyclerview",
```

### 4. Samba 服务器搭建

Samba 是一款数据共享的软件，可用于 Ubuntu 与 Windows 之间共享源代码，传输文件。


Ubuntu 安装 samba 服务端：

```bash
sudo apt install samba
```
配置 samba，修改 /etc/samba/smb.conf，添加如下内容：

```bash
[Project]
        comment = project
        path = /home/zzh0838/Project #你自己需要共享的目录
        browseable = yes
        writable = yes
```

后续操作：

```bash
chmod 777 /home/zzh0838/Project
sudo smbpasswd -a username #这里是当前用户名
sudo systemctl enable smbd #samba 开机自启动
sudo systemctl restart smbd #重启 samba 
```

windows 访问 Ubuntu 共享的文件键：

win 键加 R 键，跳出运行窗口，输入 `\\10.0.0.89`，其中 `10.0.0.89` 是我的 ubuntu 的 IP 地址，你需要根据你自己的情况修改为自己的 Ubuntu 的 IP 地址。

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230330224915.png)

点击确定，即可访问到 Ubuntu 中共享的文件了：

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230330225704.png)

双击 Project，进入到下面的文件夹中：

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230330225744.png)


###  5. VSCode 

VSCode 是一款微软推出的文本编辑器，同个它的插件系统可以实现类 ide 的体验，在功能性和适应性角度，非常适合用于 Android 系统开发。

我们可以通过两种方式来使用 VSCode

* Ubuntu 本机使用
* Windows 通过 remote-ssh 插件远程使用 VSCode 编辑linux 上的代码


#### 5.1 Ubuntu 本机使用

首先是安装 VScode，在[官网](https://code.visualstudio.com/)下载好 deb 格式的安装包，接着安装：

```bash
sudo dpkg -i code_1.76.2-1678817801_amd64.deb
```

这样我们就可以使用 vscode 了，我们打开 vscode，然后通过 file -> open folder 选择我们的系统源码，即可打开整个系统源码。

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230331003748.png)

当然，我们根据需要只打开我们关心的模块。

另外为了使我们的 vscode 更好用更好看，通常我们会安装一下一些插件：

* C/C++ Extension Pack
* Extension Pack for Java
* Makefile Tools
* RC Script language
* Android System Tools
* Android Studio Color Theme


#### 5.2 远程使用

在 Windows 下，我们也可以通过 VSCode 的 remote-ssh 插件远程修改我们的系统源码。

首先我们需要在[官网](https://code.visualstudio.com/)下载好 VSCode 的安装包，然后打开安装包狂点下一步即可安装好 VSCode。

接着在插件市场中安装好 Remote-SSH 插件。

接着就可以配置 SSH 远程连接了：

点击左侧 remote-explorer 图标，点击右侧加号：

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230331084430.png)

在弹出的输入框中，输入 `用户名@IP地址`，点击回车。

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230331084625.png)

接着，选择第一个配置文件，点击回车。

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230331084721.png)

接着，左侧的清单中就会出现我们配置的 10.0.0.89,点击右侧有个加号的文件图标：

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230331092230.png)

弹出新的 VSCode 窗口，我们点击 File -> Open Folder，然后选择我们的源码路径即可：

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230331092351.png)

打开的过程中，需要我们输入 Ubuntu 的登录密码。我们也可以通过配置，实现免密登录的效果：

打开 win 上的 PowerShell：

```bash
# 生成公钥
ssh-keygen -t rsa
cat ~/.ssh/id_rsa.pub
```
将公钥文件的内容拷贝到 ubuntu 的 ~/.ssh/authorized_keys 中 

ubuntu 重启 ssh 服务

```bash
sudo systemctl restart ssh
```

### 6. adb

最后，我们在介绍 adb 的两个常用功能

* adb push：将开发机上的文件上传到 Android 机器上

```bash
touch testfile
# 一些临时文件
adb push ./testfile /data/local/tmp
```

* adb shell：用于进入 Android 的 shell 终端

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230331093004.png)


## 第三章 添加 Product

### 1. 什么是 Product

在 App 的开发中，我们要去打渠道包，根据不同应用市场的要求打包出不同的 apk 包。

同样的道理，Android 的系统源码，经过简单的配置，可以打包出不同的系统镜像，用于不同的产品。例如小米 12s，小米12s pro，小米12s ultra 均源于骁龙8+平台。

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230223201246.png)

Android 系统源码是通过配置文件来实现 “分包” 的，配置文件将我们的源码配置为了不同的 Product，每一个 Product 适用于特定的硬件产品，这一系列的配置文件我们称为 **Product**。


###  2. AOSP 中预制的 Product


我们在编译源码之前需要执行 lunch ：

```bash
lunch

You're building on Linux

Lunch menu... pick a combo:
     1. aosp_arm-eng
     2. aosp_arm64-eng
     3. aosp_blueline-userdebug
     4. aosp_bonito-userdebug
     5. aosp_car_arm-userdebug
     6. aosp_car_arm64-userdebug
     7. aosp_car_x86-userdebug
     8. aosp_car_x86_64-userdebug
     9. aosp_cf_arm64_phone-userdebug
     10. aosp_cf_x86_64_phone-userdebug
     11. aosp_cf_x86_auto-userdebug
     12. aosp_cf_x86_phone-userdebug
     13. aosp_cf_x86_tv-userdebug
     14. aosp_coral-userdebug
     15. aosp_coral_car-userdebug
     16. aosp_crosshatch-userdebug
     17. aosp_crosshatch_car-userdebug
     18. aosp_flame-userdebug
     19. aosp_marlin-userdebug
     20. aosp_sailfish-userdebug
     21. aosp_sargo-userdebug
     22. aosp_taimen-userdebug
     23. aosp_walleye-userdebug
     24. aosp_walleye_test-userdebug
     25. aosp_x86-eng
     26. aosp_x86_64-eng
     27. beagle_x15-userdebug
     28. car_x86_64-userdebug
     29. fuchsia_arm64-eng
     30. fuchsia_x86_64-eng
     31. hikey-userdebug
     32. hikey64_only-userdebug
     33. hikey960-userdebug
     34. hikey960_tv-userdebug
     35. hikey_tv-userdebug
     36. m_e_arm-userdebug
     37. mini_emulator_arm64-userdebug
     38. mini_emulator_x86-userdebug
     39. mini_emulator_x86_64-userdebug
     40. poplar-eng
     41. poplar-user
     42. poplar-userdebug
     43. qemu_trusty_arm64-userdebug
     44. uml-userdebug

Which would you like? [aosp_arm-eng]
```

这里的每一个选项就是一个 Product， AOSP 预制了很多 Product。对应于 x86_64 模拟器，我们选择的是 aosp_x86_64-eng。

AOSP 中 Product 配置文件保存在以下两个目录：

* build/target：aosp 提供的 product 配置文件保存在这个目录下，我们选择的 aosp_x86_64-eng Product 就配置在这个目录下。
* device：芯片及方案厂商提供的 product 配置文件保存在这个目录下


先使用 tree 命令查看 build/target 基本的目录结构:

```bash
tree . -L 2
.
├── board
│   ├── Android.mk
│   ├── BoardConfigEmuCommon.mk
│   ├── BoardConfigGsiCommon.mk
│   ├── BoardConfigMainlineCommon.mk
│   ├── BoardConfigModuleCommon.mk
│   ├── BoardConfigPixelCommon.mk
│   ├── emulator_arm
│   ├── emulator_arm64
│   ├── emulator_x86
│   ├── emulator_x86_64
│   ├── emulator_x86_64_arm64
│   ├── emulator_x86_arm
│   ├── generic
│   ├── generic_64bitonly_x86_64
│   ├── generic_arm64
│   ├── generic_x86
│   ├── generic_x86_64
│   ├── generic_x86_64_arm64
│   ├── generic_x86_arm
│   ├── go_defaults_512.prop
│   ├── go_defaults_common.prop
│   ├── go_defaults.prop
│   ├── gsi_arm64
│   ├── gsi_system_ext.prop
│   ├── gsi_system_ext_user.prop
│   ├── mainline_arm64
│   ├── mainline_sdk
│   ├── mainline_x86
│   ├── mainline_x86_64
│   ├── mainline_x86_arm
│   ├── module_arm
│   ├── module_arm64
│   ├── module_x86
│   ├── module_x86_64
│   └── ndk
├── OWNERS
└── product
    ├── AndroidProducts.mk
    ├── aosp_64bitonly_x86_64.mk
    ├── aosp_arm64.mk
    ├── aosp_arm.mk
    ├── aosp_base.mk
    ├── aosp_base_telephony.mk
    ├── aosp_product.mk
    ├── aosp_x86_64.mk
    ├── aosp_x86_arm.mk
    ├── aosp_x86.mk
    ├── base.mk
    ├── base_product.mk
    ├── base_system_ext.mk
    ├── base_system.mk
    ├── base_vendor.mk
    ├── cfi-common.mk
    ├── core_64_bit.mk
    ├── core_64_bit_only.mk
    ├── core_minimal.mk
    ├── core_no_zygote.mk
    ├── default_art_config.mk
    ├── developer_gsi_keys.mk
    ├── empty-preloaded-classes
    ├── empty-profile
    ├── emulated_storage.mk
    ├── emulator.mk
    ├── emulator_system.mk
    ├── emulator_vendor.mk
    ├── full_base.mk
    ├── full_base_telephony.mk
    ├── full.mk
    ├── full_x86.mk
    ├── generic.mk
    ├── generic_no_telephony.mk
    ├── generic_ramdisk.mk
    ├── generic_system_arm64.mk
    ├── generic_system.mk
    ├── generic_system_x86_64.mk
    ├── generic_system_x86_arm.mk
    ├── generic_system_x86.mk
    ├── generic_x86.mk
    ├── go_defaults_512.mk
    ├── go_defaults_common.mk
    ├── go_defaults.mk
    ├── gsi
    ├── gsi_release.mk
    ├── handheld_product.mk
    ├── handheld_system_ext.mk
    ├── handheld_system.mk
    ├── handheld_vendor.mk
    ├── languages_default.mk
    ├── languages_full.mk
    ├── mainline_sdk.mk
    ├── mainline_system_arm64.mk
    ├── mainline_system.mk -> generic_system.mk
    ├── mainline_system_x86_64.mk
    ├── mainline_system_x86_arm.mk
    ├── mainline_system_x86.mk
    ├── media_product.mk
    ├── media_system_ext.mk
    ├── media_system.mk
    ├── media_vendor.mk
    ├── module_arm64.mk
    ├── module_arm.mk
    ├── module_common.mk
    ├── module_x86_64.mk
    ├── module_x86.mk
    ├── ndk.mk
    ├── non_ab_device.mk
    ├── OWNERS
    ├── product_launched_with_k.mk
    ├── product_launched_with_l.mk
    ├── product_launched_with_l_mr1.mk
    ├── product_launched_with_m.mk
    ├── product_launched_with_n.mk
    ├── product_launched_with_n_mr1.mk
    ├── product_launched_with_o.mk
    ├── product_launched_with_o_mr1.mk
    ├── product_launched_with_p.mk
    ├── profile_boot_common.mk
    ├── runtime_libart.mk
    ├── sdk_arm64.mk
    ├── sdk.mk
    ├── sdk_phone_arm64.mk
    ├── sdk_phone_armv7.mk
    ├── sdk_phone_x86_64.mk
    ├── sdk_phone_x86.mk
    ├── sdk_x86_64.mk
    ├── sdk_x86.mk
    ├── security
    ├── sysconfig
    ├── telephony.mk
    ├── telephony_product.mk
    ├── telephony_system_ext.mk
    ├── telephony_system.mk
    ├── telephony_vendor.mk
    ├── updatable_apex.mk
    ├── userspace_reboot.mk
    ├── vboot.mk
    ├── verity.mk
    ├── virtual_ab_ota
    ├── virtual_ab_ota.mk -> virtual_ab_ota/launch.mk
    ├── virtual_ab_ota_plus_non_ab.mk -> virtual_ab_ota/plus_non_ab.mk
    └── virtual_ab_ota_retrofit.mk -> virtual_ab_ota/retrofit.mk
```

board目录下主要是一些硬件相关的配置， product 目录主要是产品相关的配置

针对我们选择的 aosp_x86_64-eng，我们主要关注以下几个文件：

* `/board/generic_x86_64/BoardConfig.mk` ： 用于硬件相关配置
* `/product/AndroidProducts.mk`   `/product/aosp_x86_64.mk`：用于配置 Product
 

接下来我们来看看这三个配置文件：


**BoardConfig.mk** 用于定义和硬件相关的底层特性和变量，比如当前源码支持的 cpu 位数(64/32位)，bootloader 和 kernel, 是否支持摄像头，GPS导航等一些板级特性。

```bash
# x86_64 emulator specific definitions
TARGET_CPU_ABI := x86_64
TARGET_ARCH := x86_64
TARGET_ARCH_VARIANT := x86_64

TARGET_2ND_CPU_ABI := x86
TARGET_2ND_ARCH := x86
TARGET_2ND_ARCH_VARIANT := x86_64

TARGET_PRELINK_MODULE := false
include build/make/target/board/BoardConfigGsiCommon.mk
include build/make/target/board/BoardConfigEmuCommon.mk

BOARD_USERDATAIMAGE_PARTITION_SIZE := 576716800

BOARD_SEPOLICY_DIRS += device/generic/goldfish/sepolicy/x86

# Wifi.
BOARD_WLAN_DEVICE           := emulator
BOARD_HOSTAPD_DRIVER        := NL80211
BOARD_WPA_SUPPLICANT_DRIVER := NL80211
BOARD_HOSTAPD_PRIVATE_LIB   := lib_driver_cmd_simulated
BOARD_WPA_SUPPLICANT_PRIVATE_LIB := lib_driver_cmd_simulated
WPA_SUPPLICANT_VERSION      := VER_0_8_X
WIFI_DRIVER_FW_PATH_PARAM   := "/dev/null"
WIFI_DRIVER_FW_PATH_STA     := "/dev/null"
WIFI_DRIVER_FW_PATH_AP      := "/dev/null"
```

其中还通过 include 包含了 BoardConfigGsiCommon.mk 和 BoardConfigEmuCommon.mk 两个配置文件，前者用于通用系统映像的配置，后者用于模拟器的配置

主要和硬件相关，有一个基本的了解即可。一般很少改动。

**AndroidProducts.mk** 定义我们执行 lunch 命令时，打印的列表以及每个选项对应的配置文件

```bash
# Unbundled apps will be built with the most generic product config.
# TARGET_BUILD_APPS 编译Android系统时，这个值为空，编译单模块时，这个值为所编译模块的路径

#编译单模块
ifneq ($(TARGET_BUILD_APPS),)
PRODUCT_MAKEFILES := \
    $(LOCAL_DIR)/aosp_arm64.mk \
    $(LOCAL_DIR)/aosp_arm.mk \
    $(LOCAL_DIR)/aosp_x86_64.mk \
    $(LOCAL_DIR)/aosp_x86.mk \
    $(LOCAL_DIR)/full.mk \
    $(LOCAL_DIR)/full_x86.mk \
#编译系统
else
PRODUCT_MAKEFILES := \
    $(LOCAL_DIR)/aosp_64bitonly_x86_64.mk \
    $(LOCAL_DIR)/aosp_arm64.mk \
    $(LOCAL_DIR)/aosp_arm.mk \
    $(LOCAL_DIR)/aosp_x86_64.mk \
    $(LOCAL_DIR)/aosp_x86_arm.mk \
    $(LOCAL_DIR)/aosp_x86.mk \
    $(LOCAL_DIR)/full.mk \
    $(LOCAL_DIR)/full_x86.mk \
    $(LOCAL_DIR)/generic.mk \
    $(LOCAL_DIR)/generic_system_arm64.mk \
    $(LOCAL_DIR)/generic_system_x86.mk \
    $(LOCAL_DIR)/generic_system_x86_64.mk \
    $(LOCAL_DIR)/generic_system_x86_arm.mk \
    $(LOCAL_DIR)/generic_x86.mk \
    $(LOCAL_DIR)/mainline_system_arm64.mk \
    $(LOCAL_DIR)/mainline_system_x86.mk \
    $(LOCAL_DIR)/mainline_system_x86_64.mk \
    $(LOCAL_DIR)/mainline_system_x86_arm.mk \
    $(LOCAL_DIR)/ndk.mk \
    $(LOCAL_DIR)/sdk_arm64.mk \
    $(LOCAL_DIR)/sdk.mk \
    $(LOCAL_DIR)/sdk_phone_arm64.mk \
    $(LOCAL_DIR)/sdk_phone_armv7.mk \
    $(LOCAL_DIR)/sdk_phone_x86_64.mk \
    $(LOCAL_DIR)/sdk_phone_x86.mk \
    $(LOCAL_DIR)/sdk_x86_64.mk \
    $(LOCAL_DIR)/sdk_x86.mk \

endif

PRODUCT_MAKEFILES += \
    $(LOCAL_DIR)/mainline_sdk.mk \
    $(LOCAL_DIR)/module_arm.mk \
    $(LOCAL_DIR)/module_arm64.mk \
    $(LOCAL_DIR)/module_x86.mk \
    $(LOCAL_DIR)/module_x86_64.mk \

# 配置我们在 lunch 时的选项
COMMON_LUNCH_CHOICES := \
    aosp_arm64-eng \
    aosp_arm-eng \
    aosp_x86_64-eng \
    aosp_x86-eng \
```

PRODUCT_MAKEFILES 用于引入产品的配置文件

COMMON_LUNCH_CHOICES 用于添加 lunch 时的选项，选项的名字由两部分过程 `产品名 + 构建模式`：

* 产品名就是 PRODUCT_MAKEFILES 中引入的产品配置文件名去掉 `.mk` 后缀，例如 aosp_x86_64
* 构建模式有三种：用户模式 user、用户调试模式 userdebug 和工程模式 eng

主要区别如下：、

**用户模式 user** 
 
* 仅安装标签为 user 的模块
* 设定属性 ro.secure=1，打开安全检查功能
* 设定属性 ro.debuggable=0，关闭应用调试功能
* 默认关闭 adb 功能
* 打开 Proguard 混淆器
* 打开 DEXPREOPT 预先编译优化

**用户调试模式 userdebug** 
 
* 安装标签为 user、debug 的模块
* 设定属性 ro.secure=1，打开安全检查功能
* 设定属性 ro.debuggable=1，启用应用调试功能
* 默认打开 adb 功能
* 打开 Proguard 混淆器
* 打开 DEXPREOPT 预先编译优化

**工程模式 eng** 
 
* 安装标签为 user、debug、eng 的模块
* 设定属性 ro.secure=0，关闭安全检查功能
* 设定属性 ro.debuggable=1，启用应用调试功能
* 设定属性 ro.kernel.android.checkjni=1，启用 JNI 调用检查
* 默认打开 adb 功能
* 关闭 Proguard 混淆器
* 关闭 DEXPREOPT 预先编译优化

**aosp_x86_64.mk**：这个文件就是我们产品配置的主基地。

```bash
PRODUCT_USE_DYNAMIC_PARTITIONS := true

# The system image of aosp_x86_64-userdebug is a GSI for the devices with:
# - x86 64 bits user space
# - 64 bits binder interface
# - system-as-root
# - VNDK enforcement
# - compatible property override enabled

# This is a build configuration for a full-featured build of the
# Open-Source part of the tree. It's geared toward a US-centric
# build quite specifically for the emulator, and might not be
# entirely appropriate to inherit from for on-device configurations.

# GSI for system/product
$(call inherit-product, $(SRC_TARGET_DIR)/product/core_64_bit.mk)
$(call inherit-product, $(SRC_TARGET_DIR)/product/gsi_common.mk)

# Emulator for vendor
$(call inherit-product-if-exists, device/generic/goldfish/x86_64-vendor.mk)
$(call inherit-product, $(SRC_TARGET_DIR)/product/emulator_vendor.mk)
$(call inherit-product, $(SRC_TARGET_DIR)/board/generic_x86_64/device.mk)

# Enable mainline checking for excat this product name
ifeq (aosp_x86_64,$(TARGET_PRODUCT))
PRODUCT_ENFORCE_ARTIFACT_PATH_REQUIREMENTS := relaxed
endif

PRODUCT_ARTIFACT_PATH_REQUIREMENT_WHITELIST += \
    root/init.zygote32_64.rc \
    root/init.zygote64_32.rc \

# Copy different zygote settings for vendor.img to select by setting property
# ro.zygote=zygote64_32 or ro.zygote=zygote32_64:
#   1. 64-bit primary, 32-bit secondary OR
#   2. 32-bit primary, 64-bit secondary
# init.zygote64_32.rc is in the core_64_bit.mk below
PRODUCT_COPY_FILES += \
    system/core/rootdir/init.zygote32_64.rc:root/init.zygote32_64.rc

# Product 基本信息
PRODUCT_NAME := aosp_x86_64
PRODUCT_DEVICE := generic_x86_64
PRODUCT_BRAND := Android
PRODUCT_MODEL := AOSP on x86_64
```


**inherit-product** 函数表示继承另外一个文件

```bash
$(call inherit-product, $(SRC_TARGET_DIR)/product/emulator_vendor.mk)
$(call inherit-product-if-exists, device/generic/goldfish/x86_64-vendor.mk)
```

在 Makefile 中可使用 **“-include”** 来代替 “include”，来忽略由于包含文件不存在或者无法创建时的错误提示（“-”的意思是告诉make，忽略此操作的错误。make继续执行）,如果不加-,当 include 的文件出错或者不存在的时候， make 会报错并退出。

```bash
-include $(TARGET_DEVICE_DIR)/AndroidBoard.mk
```

**include 和 inherit-product 的区别**：

* 假设 `PRODUCT_VAR := a` 在 A.mk 中, `PRODUCT_VAR := b` 在 B.mk 中。
* 如果你在 A.mk 中 include B.mk，你最终会得到 `PRODUCT_VAR := b`。
* 但是如果你在 A.mk inherit-product B.mk，你会得到 `PRODUCT_VAR := a b`。并 inherit-product 确保您不会两次包含同一个 makefile 。


**Product 配置文件中涉及的变量**可分为一下几类：

* 通用变量
* 路径变量
* 自定义变量
* 功能变量

**通用变量**

```makefile
PRODUCT_BRAND := Android

PRODUCT_NAME := sdk_phone_x86_64

PRODUCT_DEVICE := generic_x86_64

PRODUCT_MODEL := Android SDK built for x86_64
```

常用的**路径变量**

* SRC_TARGET_DIR 其值为 build/target
* LOCAL_DIR 代表当前目录

**自定义变量**，表示该变量如何使用， 取决于自己，如：

```makefile
BOARD_DDR_VAR_ENABLED := true
```

**功能变量**：表示改变量有特殊功能

* PRODUCT_COPY_FILES： 用于完成拷贝，可以将源码中的文件拷贝到编译好的分区文件中

```makefile
PRODUCT_COPY_FILES += vendor/rockchip/common/phone/etc/spn-conf.xml:system/etc/spn-conf.xml
```

* PRODUCT_PROPERTY_OVERRIDES： 用于设置系统属性(覆盖)

```makefile
PRODUCT_PROPERTY_OVERRIDES += \
    ro.product.version = 1.0.0 \
```


### 3. 添加自己的 Product

这里假设我们的公司名叫果冻（Jelly），我们准备开发一款对标苹果 14 的手机，取名叫 大米14（Rice14）。

接下来我们在源码中添加我们自己的产品（Product）

在 device 目录下添加如下的目录与文件：

```bash
Jelly/
└── Rice14
    ├── AndroidProducts.mk
    ├── BoardConfig.mk
    └── Rice14.mk
```

BoardConfig.mk 包含了硬件芯片架构配置，分区大小配置等信息这里我们直接使用 aosp_x86_64 的 BoardConfig.mk 就行。BoardConfig.mk 拷贝自 build/target/board/generic_x86_64/BoardConfig.mk

```bash
# x86_64 emulator specific definitions
TARGET_CPU_ABI := x86_64
TARGET_ARCH := x86_64
TARGET_ARCH_VARIANT := x86_64

TARGET_2ND_CPU_ABI := x86
TARGET_2ND_ARCH := x86
TARGET_2ND_ARCH_VARIANT := x86_64

TARGET_PRELINK_MODULE := false
include build/make/target/board/BoardConfigGsiCommon.mk
include build/make/target/board/BoardConfigEmuCommon.mk

BOARD_USERDATAIMAGE_PARTITION_SIZE := 576716800

BOARD_SEPOLICY_DIRS += device/generic/goldfish/sepolicy/x86

# Wifi.
BOARD_WLAN_DEVICE           := emulator
BOARD_HOSTAPD_DRIVER        := NL80211
BOARD_WPA_SUPPLICANT_DRIVER := NL80211
BOARD_HOSTAPD_PRIVATE_LIB   := lib_driver_cmd_simulated
BOARD_WPA_SUPPLICANT_PRIVATE_LIB := lib_driver_cmd_simulated
WPA_SUPPLICANT_VERSION      := VER_0_8_X
WIFI_DRIVER_FW_PATH_PARAM   := "/dev/null"
WIFI_DRIVER_FW_PATH_STA     := "/dev/null"
WIFI_DRIVER_FW_PATH_AP      := "/dev/null"
```

Rice14.mk 拷贝自 build/target/product/aosp_x86_64.mk

其中的 if 语句需要注释掉，同时需要修改最后四行

```makefile
PRODUCT_USE_DYNAMIC_PARTITIONS := true

# The system image of aosp_x86_64-userdebug is a GSI for the devices with:
# - x86 64 bits user space
# - 64 bits binder interface
# - system-as-root
# - VNDK enforcement
# - compatible property override enabled

# This is a build configuration for a full-featured build of the
# Open-Source part of the tree. It's geared toward a US-centric
# build quite specifically for the emulator, and might not be
# entirely appropriate to inherit from for on-device configurations.

# GSI for system/product
$(call inherit-product, $(SRC_TARGET_DIR)/product/core_64_bit.mk)
$(call inherit-product, $(SRC_TARGET_DIR)/product/gsi_common.mk)

# Emulator for vendor
$(call inherit-product-if-exists, device/generic/goldfish/x86_64-vendor.mk)
$(call inherit-product, $(SRC_TARGET_DIR)/product/emulator_vendor.mk)
$(call inherit-product, $(SRC_TARGET_DIR)/board/generic_x86_64/device.mk)

# Enable mainline checking for excat this product name
#ifeq (aosp_x86_64,$(TARGET_PRODUCT))
PRODUCT_ENFORCE_ARTIFACT_PATH_REQUIREMENTS := relaxed
#endif

PRODUCT_ARTIFACT_PATH_REQUIREMENT_WHITELIST += \
    root/init.zygote32_64.rc \
    root/init.zygote64_32.rc \

# Copy different zygote settings for vendor.img to select by setting property
# ro.zygote=zygote64_32 or ro.zygote=zygote32_64:
#   1. 64-bit primary, 32-bit secondary OR
#   2. 32-bit primary, 64-bit secondary
# init.zygote64_32.rc is in the core_64_bit.mk below
PRODUCT_COPY_FILES += \
    system/core/rootdir/init.zygote32_64.rc:root/init.zygote32_64.rc

# Overrides
PRODUCT_BRAND := Jelly
PRODUCT_NAME := Rice14
PRODUCT_DEVICE := Rice14
PRODUCT_MODEL := Android SDK built for x86_64 Rice14
```

AndroidProducts.mk 内容如下：

```makefile
PRODUCT_MAKEFILES := \
    $(LOCAL_DIR)/Rice14.mk

COMMON_LUNCH_CHOICES := \
    Rice14-eng \
    Rice14-userdebug \
    Rice14-user \
```

最后验证：

```bash
source build/envsetup.sh
lunch Rice14-eng
make -j16
emulator
```

以上示例给出了最简单的模拟器 Product 的添加，主要是用于我们的学习。


## 第四章 添加可执行程序

AOSP 添加的可执行程序，可以分为两类：

* C/C++ 可执行程序
* Java 可执行程序

在了解如何给 AOSP 添加可执行程序前，我们需要了解一下ARM + Android 行业流程与 Android 常用的四个分区：

* System 分区
* Vender 分区
* Odm 分区
* Product 分区

### 1. ARM + Android 行业流程与 Android 分区

ARM + Android 这个行业，一个简化的普遍流程：

1. Google 开发迭代 AOSP + Kernel
2. 芯片厂商，针对自己的芯片特点，移植 AOSP 和 Kernel，使其可以在自己的芯片上跑起来。
3. 方案厂商（很多芯片厂商也扮演了方案厂商的角色），设计电路板，给芯片添加外设，在芯片厂商源码基础上开发外设相关软件，主要是驱动和 hal，改进性能和稳定性。
4. 产品厂商，主要是系统软件开发，UI 定制以及硬件上的定制(添加自己的外设)，改进性能和稳定性.

Google 开发的通用 Android 系统组件编译后会被存放到 System 分区，原则上不同厂商、不同型号的设备都通用。

芯片厂商和方案厂商针对硬件相关的平台通用的可执行程序、库、系统服务和 app 等一般放到 Vender 分区。（开发的驱动程序是放在 boot 分区的 kernel 部分）

到了产品厂商这里，情况稍微复杂一点，通常针对同一套软硬件平台，可能会开发多个产品。比如：小米 12s，小米12s pro，小米12s ultra 均源于骁龙8+平台。

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230223201246.png)


每一个产品，我们称之为一个 Variant（变体）。

通常情况下，做产品的厂商在同一个硬件平台上针对不同的产品会从硬件和软件两个维度来做定制。

硬件上，产品 A 可能用的是京东方的屏，产品 B 可能用的是三星的屏；差异硬件相关的软件部分都会放在 Odm 分区。这样，产品 A 和产品 B 之间 Odm 以外的分区都是一样的，便于统一维护与升级。(硬件相关的软件共用部分放在 vendor 分区)

软件上，产品 A 可能是带广告的版本，产品 B 可能是不带广告的版本。这些有差异的软件部分都放在 Product 分区，这样产品 A 和产品 B 之间 Product 以外的分区都是一样的，便于统一维护与升级。(软件共用部分都放在 System分区)

总结一下，不同产品之间公共的部分放在 System 和 Vender 分区，差异的部分放在 Odm 和 Product 分区。


### 2. 动手在系统源码中添加一个 C/C++ 可执行程序

#### 2.1 源码添加

我们先看看如何以源码的方式来添加一个可执行程序。

在 `device/Jelly/Rice14` 目录下创建如下的文件结构：

```bash
hello
├── Android.bp
└── hello.cpp
```

其中 hello.cpp 的内容如下

```c++
#include <cstdio>

int main()
{
    printf("Hello Android\n");
    return 0;
}
```

Android.bp是程序的编译配置文件，作用类似于 App 开发中的 gradle.build 文件，其格式为 json。Android.bp 的内容如下：

```json
cc_binary {                 //模块类型为可执行文件
    name: "hello",          //模块名hello
    srcs: ["hello.cpp"],    //源文件列表
    cflags: ["-Werror"],    //添加编译选项
}
```

在 `device/Jelly/Rice14/Rice14.mk` 中添加：

```bash
PRODUCT_PACKAGES += hello
```

接下来编译系统：

```bash
source build/envsetup.sh
lunch Rice14-eng
make -j16
```

你会发现报错了：

```bash
Offending entries:
system/bin/helloworld
build/make/core/main.mk:1414: error: Build failed.
```

默认情况下，我们的模块会被安装到 System 分区，编译系统限制了我们在 System 分区添加东西，理论上来说， System 分区应该只能由 Google 来添加和修改内容。

这种错误一般都能搜到解决办法，通过搜索引擎我找到了 Android [官方论坛的回复](https://groups.google.com/g/android-building/c/KE-Sfavd4Ds/m/GDqP5XGMAwAJ)

大概意思说，我们得改下编译系统的某个文件，具体咋改他也没说，要么就写到 product 分区。

如果现在的情况是，我就想把它预制到 system 分区，咋整？那我们就看看 google 自己是怎么干的。

首先思路梳理清楚：

* 找个原生系统中预制的 app，看下它的 Android.mk 或者 Android.bp
* build/target 中搜一下这个 app 是怎么添加的

app 一般定义在 packages/apps 中，我看下这个目录中的 Messaging，看下它的 Android.mk

```Makefile
LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE_TAGS := optional

LOCAL_SRC_FILES := $(call all-java-files-under, src)

LOCAL_RESOURCE_DIR := $(LOCAL_PATH)/res
LOCAL_USE_AAPT2 := true

LOCAL_STATIC_ANDROID_LIBRARIES := \
    androidx.core_core \
    androidx.media_media \
    androidx.legacy_legacy-support-core-utils \
    androidx.legacy_legacy-support-core-ui \
    androidx.fragment_fragment \
    androidx.appcompat_appcompat \
    androidx.palette_palette \
    androidx.recyclerview_recyclerview \
    androidx.legacy_legacy-support-v13 \
    colorpicker \
    libchips \
    libphotoviewer

LOCAL_STATIC_JAVA_LIBRARIES := \
    androidx.annotation_annotation \
    android-common \
    android-common-framesequence \
    com.android.vcard \
    guava \
    libphonenumber

include $(LOCAL_PATH)/version.mk

LOCAL_AAPT_FLAGS += --version-name "$(version_name_package)"
LOCAL_AAPT_FLAGS += --version-code $(version_code_package)

ifdef TARGET_BUILD_APPS
    LOCAL_JNI_SHARED_LIBRARIES := libframesequence libgiftranscode
else
    LOCAL_REQUIRED_MODULES:= libframesequence libgiftranscode
endif

LOCAL_PROGUARD_ENABLED := obfuscation optimization

LOCAL_PROGUARD_FLAG_FILES := proguard.flags
ifeq (eng,$(TARGET_BUILD_VARIANT))
    LOCAL_PROGUARD_FLAG_FILES += proguard-test.flags
else
    LOCAL_PROGUARD_FLAG_FILES += proguard-release.flags
endif

LOCAL_PACKAGE_NAME := messaging

LOCAL_CERTIFICATE := platform

LOCAL_SDK_VERSION := current

include $(BUILD_PACKAGE)

include $(call all-makefiles-under, $(LOCAL_PATH))
```

没什么特别的，对我们有用的信息就是模块名是 messaging，那打包出来的 apk 名就叫 messaging.apk

我们接着在 build/target 目录下搜一下：

```bash
grep -r "messaging.apk" .

./product/gsi_common.mk:    system/app/messaging/messaging.apk \
```

看看 `gsi_common.mk`：

```Makefile
PRODUCT_ARTIFACT_PATH_REQUIREMENT_WHITELIST += \
    system/app/messaging/messaging.apk \
    system/app/WAPPushManager/WAPPushManager.apk \
    system/bin/healthd \
    system/etc/init/healthd.rc \
    system/etc/seccomp_policy/crash_dump.%.policy \
    system/etc/seccomp_policy/mediacodec.policy \
    system/etc/vintf/manifest/manifest_healthd.xml \
    system/lib/libframesequence.so \
    system/lib/libgiftranscode.so \
    system/lib64/libframesequence.so \
    system/lib64/libgiftranscode.so \
```

答案就出来了，我们需要添加到 System 的模块，添加到 PRODUCT_ARTIFACT_PATH_REQUIREMENT_WHITELIST 变量即可。

修改 device/Jelly/Rice14/Rice14.mk，添加以下内容 ：

```
PRODUCT_ARTIFACT_PATH_REQUIREMENT_WHITELIST += \
    system/bin/helloworld \
```

再次编译执行即可。

以上的方法是可行的，但是是不推荐的，对于软件相关的定制，我们应该安装[官方论坛的回复](https://groups.google.com/g/android-building/c/KE-Sfavd4Ds/m/GDqP5XGMAwAJ)的要求将其放到 product 分区。


要把 helloworld 模块放到 product 分区也很简单，在其 Android.bp 中添加 product_specific: true 即可：

```json
cc_binary {              //模块类型为可执行文件
    name: "helloworld",       //模块名hellobp
    srcs: ["helloworld.cpp"], //源文件列表
    product_specific: true,        //编译出来放在/product目录下(默认是放在/system目录下)
    cflags: ["-Werror"], //添加编译选项
}
```

再删除 device/Jelly/Rice14/Rice14.mk 中的以下内容 ：

```
PRODUCT_ARTIFACT_PATH_REQUIREMENT_WHITELIST += \
    system/bin/helloworld \
```

再次编译执行即可。


这里给出一个安装位置配置的总结：

* System 分区
  * Android.mk 默认就是输出到 system 分区，不用指定
  * Android.bp 默认就是输出到 system 分区，不用指定
* Vendor 
  * Android.mk LOCAL_VENDOR_MODULE := true
  * Android.bp vendor: true
* Odm 分区
  * Android.mk LOCAL_ODM_MODULE := true
  * Android.bp device_specific: true
* product 分区
  * Android.mk LOCAL_PRODUCT_MODULE := true
  * Android.bp product_specific: true  

#### 2.2 可执行文件添加

有的时候，我们需要添加到系统的程序只有编译好的可执行文件，没有源码。接下来我们看看如何在系统源码中添加一个可执行文件：

BusyBox 是打包为单个二进制文件的核心 Unix 实用程序的集合。常用于嵌入式设备。

适用于 x86 架构的 busybox 可通过以下命令下载：

```bash
wget https://busybox.net/downloads/binaries/1.30.0-i686/busybox
```

接下来我们把它添加到我们的 aosp 中：

在 `device/Jelly/Rice14/` 目录下创建如下的目录结构：

```bash
prebuilt/
└── busybox
    ├── Android.bp
    └── busybox
```

busybox 就是我们之前的下载的文件。

其中 Android.bp 的内容如下：

```json
cc_prebuilt_binary {
    name: "busybox",
    srcs: ["busybox"],
    product_specific: true,
}
```

接下来在 `device/Jelly/Rice14/Rice14.mk` 中添加该模块

```json
PRODUCT_PACKAGES += busybox
```

编译源代码，启动模拟器：

```bash
source build/envsetup.sh
lunch Rice14-eng
make -j16
emulator
```

进入 adb shell，执行 busybox 命令

```bash
adb shell
busybox
```

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/1674125200821.png)


### 3. 添加 Java 可执行程序

#### 3.1 源码添加

在 `device/Jelly/Rice14/` 目录下创建以下的目录和文件：

```bash
hellojava
├── Android.bp
└── com
    └── ahaoyuandaima
        └── main
            └── HelloJava.java
```

其中 Android.bp 的内容如下：

```json
java_library {
    name: "hellojava",
    installable: true,
    product_specific: true,
    srcs: ["**/*.java"],
    sdk_version: "current"
}
```

这里着重解释一下 installable 这个选项，如果不指定 installable: true, 则编译出来的 jar 包里面是 .class 文件。这种包是没法安装到系统上的，只能给其他 java 模块作为 static_libs 依赖。

指定 installable: true, 则编译出来的 jar 包里面是 classes.dex 文件。这种才是 Android 虚拟机可以加载的格式。

HelloJava.java 内容如下：

```cpp
package com.ahaoyuandaima.main;

public class HelloJava
{
	public static void main(String[] args) 
	{
		System.out.println("Hello Java");
	}
}
```

接着在 `device/Jelly/Rice14/Rice14.mk` 中添加：

```bash
PRODUCT_PACKAGES += \
    hellojava
```

接下来编译系统：

```bash
source build/envsetup.sh
lunch Rice14-eng
make -j16
```

编译完成启动虚拟机后，进入 adb shell 执行程序：

```bash
# 进入模拟器shell
adb shell
# 配置 classpath
export CLASSPATH=/product/framework/hellojava.jar 
app_process /product/framework/ com.ahaoyuandaima.main.HelloJava
```


执行结果如下图所示：

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230331180651.png)

#### 3.2 可执行 jar 包添加

有的时候我们可能需要在源码中添加别人编译好的可执行 jar 包，接着我们看看具体怎么操作：

在 `device/Jelly/Rice14/` 目录下创建以下的目录和文件：

```bash
hellojavajar
├── Android.bp
└── hellojava.jar
```

其中 hellojava.jar，是从 `out/target/product/Rice14/system/product/framework/hellojava.jar` 移动过来的。



Android.bp 的内容如下：

```json
java_import {
    name: "hellojavajar",
    installable: true,
    jars: ["hellojava.jar"],
    product_specific: true,   
}
```

为了避免冲突，我们把 hellojava 文件夹删除。在 `device/Jelly/Rice14/Rice14.mk` 中删除已添加的 hellojava 模块。并重新添加 javahellojar 模块

```Makefile
PRODUCT_PACKAGES += \
    hellojavajar
```

接下来编译系统：

```bash
source build/envsetup.sh
lunch Rice14-eng
make -j16
```

编译完成启动虚拟机后，进入 adb shell 执行程序：

```bash
# 进入模拟器shell
adb shell
# 配置 classpath
export CLASSPATH=/product/framework/hellojava.jar
# 执行可执行程序 
app_process /product/framework/ com.ahaoyuandaima.main.HelloJava
```

### 总结

本文主要讲解了如何将

* C/C++ 可执行程序源码
* Java 可执行程序源码

两类模块添加到源码中，以实际操作为主，同学们可以自己实践体验。


## 第五章 添加 C/C++、Java 库

### 1.添加 C/C++ 程序库

#### 1.1 源码方式添加

在 `device/Jelly/Rice14/` 目录下创建以下的目录和文件

```bash
libmymath
├── Android.bp
├── my_math.cpp
└── my_math.h
```

**libmymath** 是一个动态库。其 `Android.bp` 内容如下：

```json
cc_library_shared {
    name: "libmymath",

    srcs: ["my_math.cpp"],

    export_include_dirs: ["."],

    product_specific: true,

}
```

my_math.h 内容如下：

```c++
#ifndef __MY_MATH_H__
#define __MY_MATH_H__

int my_add(int a, int b);
int my_sub(int a, int b);

#endif
```

my_math.cpp 内容如下：

```c++
#include "my_math.h"

int my_add(int a, int b)
{
	return a + b;
}

int my_sub(int a, int b)
{
	return a - b;
}
```

接着修改我们之前添加的 hello 项目：

修改 `hello.cpp`

```c++
#include <cstdio>
#include "my_math.h" //添加头文件

int main()
{
    printf("Hello Android %d \n", my_add(1,2)); //添加函数调用
    return 0;
}
```

修改 Android.bp：

```json
cc_binary {              
    name: "hello",       
    srcs: ["hello.cpp"], 
    cflags: ["-Werror"], 
    product_specific: true,        
    shared_libs: ["libmymath"]    //添加动态库依赖
}
```

接下来编译系统：

```bash
source build/envsetup.sh
lunch Rice14-eng
make -j16
```

编译完成启动虚拟机后，就可以通过 adb shell 运行我们的 hello 程序了

```bash
emulator
adb shell hello
```

执行结果如下图所示：

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230402121506.png)


#### 1.2 so 包方式添加


在 `device/Jelly/Rice14` 目录下创建如下的文件与文件夹：

```bash
libmymathprebuild
├── Android.bp
├── include
│   └── my_math.h
└── lib
    ├── x86
    │   └── libmymath.so
    └── x86_64
        └── libmymath.so
```

* 其中 libmymath.so 是上一节中的编译产物，x86/libmymath.so 拷贝自 out/target/product/Rice14/system/product/lib/libmymath.so
* x86_64/libmymath.so 拷贝自 out/target/product/Rice14/system/product/lib64/libmymath.so
* my_math.h 是上一节中 libmymath 模块中拷贝过来的头文件

Android.bp 的内容如下：

```json
cc_prebuilt_library_shared {
    name: "libmymathprebuild",

     arch: {
        x86: {
            srcs: ["lib/x86/libmymath.so"],
        },
        x86_64: {
            srcs: ["lib/x86_64/libmymath.so"],
        }
    },

    export_include_dirs: ["include"],

    product_specific: true,

}
```

为了避免冲突，我们把上一节添加的 `libmymath` 移出系统源码目录或者删除。

接下来，编译整个系统，开启虚拟机

```bash
source build/envsetup.sh
lunch Rice14-eng
make -j16
emulator
```

接下来执行 hello 程序 

```bash
adb shell hello
```

执行结果如下图所示：

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230402132353.png)


### 2. 添加 Java 库

#### 2.1 源码方式添加

在 `device/Jelly/Rice14/` 目录下创建以下的目录和文件：

```bash
libmymathjava
├── Android.bp
└── com
    └── yuandaima
        └── mymath
            └── MyMath.java
```

libmymathjava 是一个 java 库。其 Android.bp 内容如下：

```json
java_library {
    name: "libmymathjava",
    installable: false,
    product_specific: true,
    srcs: ["**/*.java"],
    sdk_version: "current"
}
```

如果不指定 installable: true, 则编译出来的 jar 包里面是 .class 文件。这种包是没法安装到系统上的，只能给其他 java 模块作为 static_libs 依赖。最终生成的 jar 包不会被直接存放到 Android 的文件系统中，而是打包进依赖于当前模块的其他模块中。


MyMath.java 内容如下：

```cpp
package com.yuandaima.mymath;

public class MyMath
{

    public MyMath() {

    }

	public int add(int a, int b)
	{
		return (a+b);
	}

}
```

接着我们把 helllojavajar 删除，将之前写好的 hellojava 模块移动到 `device/Jelly/Rice14` 目录下，并做以下修改：

修改 helljava/Android.bp:

```json
java_library {
    name: "hellojava",
    installable: true,
    product_specific: true,
    srcs: ["**/*.java"],
    sdk_version: "current",
    static_libs: ["libmymathjavajar"]
}
```

在 `device/Jelly/Rice14/Rice14.mk` 中添加：

```bash
PRODUCT_PACKAGES += hellojava
```

接下来编译系统：

```bash
source build/envsetup.sh
lunch Rice14-eng
make -j16
```

编译完成启动虚拟机后：

```bash
# 进入模拟器shell
adb shell
# 配置 classpath
export CLASSPATH=/product/framework/hellojava.jar
app_process /product/framework/ com.ahaoyuandaima.main.HelloJava 
```

执行结果如下图所示：

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230402150426.png)


#### 2.2 Jar 包方式添加

在系统源码目录下执行：

```bash
source build/envsetup.sh
lunch Rice14-eng
```
接下来编译获得 jar 包：

```bash
cd device/Jelly/Rice14/libmymathjava
mm
#编译完成后，会打印出编译产物路径 out/target/product/Rice14/obj/JAVA_LIBRARIES/libmymathjava_intermediates/javalib.jar
```

为避免冲突我们把  `device/Jelly/Rice14/libmymathjava` 移动到源码以外的目录或者删除

在 `device/Jelly/Rice14/` 下重新创建如下的目录结构：

```bash
libmymathjavajar
├── Android.bp
└── libmymathjava.jar
```

其中 `libmymathjava.jar` 是拷贝自编译产物 `out/target/product/Rice14/obj/JAVA_LIBRARIES/libmymathjava_intermediates/javalib.jar`，并改名为 libmymathjava.jar 

Android.bp 的内容如下：

```json
java_import {
    name: "libmymathjavajar",
    installable: false,
    jars: ["libmymathjava.jar"],
    product_specific: true,   
}
```

接着修改 hellojava 模块的依赖：

```json
java_library {
    name: "hellojava",
    installable: true,
    product_specific: true,
    srcs: ["**/*.java"],
    sdk_version: "current",
    static_libs: ["libmymathjavajar"]  //修改这里
}
```

编译系统，并启动模拟器：

```bash
source build/envsetup.sh
lunch Rice14-eng
make -j16
emulator
```

验证 libmymathjavajar 模块是否被正确编译到 hellojava 模块中。

```bash
# 进入模拟器shell
adb shell
# 配置 classpath
export CLASSPATH=/product/framework/hellojava.jar
app_process /product/framework/ com.ahaoyuandaima.main.HelloJava 
```

执行结果如下：

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230402153537.png)

## 第六章 添加配置文件与删除已有模块

### 1. 添加配置文件

PRODUCT_COPY_FILES 常用于产品的配置文件中，在本文中就是 Rice14.mk 文件，用于将源码的文件拷贝到 Android 文件系统中。

这里看一个源码中的示例：

`aosp/build/target/product/core_64_bit.mk` 中有如下内容：

```makefile
PRODUCT_COPY_FILES += system/core/rootdir/init.zygote64_32.rc:system/etc/init/hw/init.zygote64_32.rc
```

这一行表示将源码中的 `system/core/rootdir/init.zygote64_32.rc` 拷贝到 Android 文件系统的 system/etc/init/hw/init.zygote64_32.rc 文件中。

init.zygote64_32.rc 是 init 程序使用的一个配置文件，当我们的程序需要配置文件时，也可以参考以上的方式来完成。

### 2. 删除已有模块

有的时候，我们需要删除系统中已有的模块，比如我们系统的应用场景是广告牌、电视，那我们就需要删除电话，通信录等应用。下面以删除通信录（Contacts）为例，演示如何操作：

```bash
grep -r "Contacts" .
./make/target/product/handheld_product.mk:    Contacts \
./make/target/product/mainline_arm64.mk:  system/priv-app/Contacts/Contacts.apk \
./make/target/product/base_system.mk:    ContactsProvider \
./make/core/build-system.html:Dialer, Contacts, etc.  This will probably change or go away when we switch
```

在 `./make/target/product/handheld_product.mk` 中删除 PRODUCT_PACKAGES 变量的 Contacts

接着重新编译启动模拟器：

```bash
source build/envsetup.sh
lunch Rice14-eng
make clean
make -j16
emulator
```
可以看到，Contacts 应用已经没有了：

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230403115334.png)


## 第七章 添加系统 App 源码

### 1. 如何新建一个系统 App 项目

使用 Android Studio 新建一个空项目 FirstSystemApp，包名设置为 `com.yuandaima.firstsystemapp`，语言选择 Java。后面为叙述方便称该项目为 as 项目。

接着在 `jelly/rice14` 目录下创建如下的目录和文件：

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230227131236.png)

接着将 as 项目中的 res 文件下的资源文件拷贝到 `Jelly/Rice14/FirstSystemApp/res` 中，把 as 项目中的 MainActivity.java 拷贝到 `Jelly/Rice14/FirstSystemApp/src/com/yuandaima/firstsystemapp` 中。

接着修改已添加的 AndroidManifest.xml 文件：

```xml
<?xml version="1.0" encoding="utf-8"?>
<manifest xmlns:android="http://schemas.android.com/apk/res/android"
    package="com.yuandaima.firstsystemapp">

    <application
        android:icon="@mipmap/ic_launcher"
        android:label="@string/app_name"
        android:supportsRtl="true"
        android:theme="@style/Theme.FirstSystemApp">
        <activity
            android:name=".MainActivity"
            android:exported="true">
            <intent-filter>
                <action android:name="android.intent.action.MAIN" />

                <category android:name="android.intent.category.LAUNCHER" />
            </intent-filter>

            <meta-data
                android:name="android.app.lib_name"
                android:value="" />
        </activity>
    </application>

</manifest>
```

接着修改已添加的 Android.bp 文件：

```json
android_app {
    name: "FirstSystemApp",

    srcs: ["src/**/*.java"],

    resource_dirs: ["res"],

    manifest: "AndroidManifest.xml",

    platform_apis: true,
    
    sdk_version: "",

    certificate: "platform",

    product_specific: true,

    //依赖
    static_libs: ["androidx.appcompat_appcompat",
                 "com.google.android.material_material",
                 "androidx-constraintlayout_constraintlayout"],

}
```

至此我们的系统 App 就创建好了。

接着在我们的 Product 中添加这个App，修改 `device/Jelly/Rice14/Rice14.mk`：

```Makefile
# 添加以下内容
PRODUCT_PACKAGES += FirstSystemApp
```

接着编译系统，启动虚拟机，打开 app：

```bash
source build/envsetup.sh
lunch Rice14-eng
make -j16
emulator 
```

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230227135955.png)


### 2. 系统 App 与 普通 App 的差异

#### 2.1 系统 App 可以使用更多的 api

当我们在 Android.bp 中配置了：

```
platform_apis: true,
sdk_version: "",
```

当 platform_apis 为 true 时，sdk_version 必须为空。这种情况下我们的 app 会使用平台 API 进行编译而不是 SDK，这样我们的 App 就能访问到非 SDK API 了。关于 SDK API 和非 SDK API 的内容可以参考[官方文档](https://developer.android.com/guide/app-compatibility/restrictions-non-sdk-interfaces?hl=zh-cn)


#### 2.2 系统 App 的签名

AOSP 内置了 apk 签名文件，我们可以在 Android.bp 中通过 certificate 配置系统 app 的签名文件，certificate 的值主要有一下几个选项：

* testkey：普通 apk，默认情况下使用
* platform：该 apk 完成一些系统的核心功能。经过对系统中存在的文件夹的访问测试，这种方式编译出来的 APK 所在进程的 UID 为system
* shared：该 apk 需要和 home/contacts 进程共享数据
* media：该 apk 是 media/download 系统中的一环
* PRESIGNED：表示 这个 apk 已经签过名了，系统不需要再次签名;

#### 2.3 系统 App 能使用更多的权限

当 Android.bp 中的 privileged 被配置为 true 时，我们的系统 App 在添加特许权限许可名单后，能使用 signatureOrSystem 级别的权限，而普通 App 是不能使用这些权限的。

#### 2.4 系统 App 能更轻松地实现进程保活

三方 App 为了不被杀掉，可以说是用尽了千方百计。保活对于系统 App 其实是非常简单的：

在 AndroidManifest.xml 中添加如下参数即可：

```xml
<application
    android:persistent="true">
```

### 3. 系统 App 添加依赖

#### 3.1 添加 AOSP 中已有的库

在 FirstSystemApp 的 Android.bp 中我们添加了很多依赖：

```json
    static_libs: ["androidx.appcompat_appcompat",
                 "com.google.android.material_material",
                 "androidx-constraintlayout_constraintlayout"],

```

在 AOSP 中， 很多常用的库均以预编译模块的方式添加到系统源码中。比如常用的 AndroidX 库定义在 `prebuilts/sdk/current/androidx` 目录下。这些库通过 `prebuilts/sdk/current/androidx/Android.bp` 引入。比如 recyclerview 库的引入方式如下：

```json
android_library {
    name: "androidx.recyclerview_recyclerview",
    sdk_version: "31",
    apex_available: [
        "//apex_available:platform",
        "//apex_available:anyapex",
    ],
    min_sdk_version: "14",
    manifest: "manifests/androidx.recyclerview_recyclerview/AndroidManifest.xml",
    static_libs: [
        "androidx.recyclerview_recyclerview-nodeps",
        "androidx.annotation_annotation",
        "androidx.collection_collection",
        "androidx.core_core",
        "androidx.customview_customview",
    ],
    java_version: "1.7",
}
```

可以看到引入的是一个 `android_library`，名字叫 `androidx.recyclerview_recyclerview`。maifest 文件在 `manifests/androidx.recyclerview_recyclerview/` 目录下，进入这个目录只有一个 `AndroidManifest.xml` 文件，其内容如下：

```xml
<?xml version="1.0" encoding="utf-8"?>

<manifest xmlns:android="http://schemas.android.com/apk/res/android"
    package="androidx.recyclerview" >

    <uses-sdk
        android:minSdkVersion="14"
        android:targetSdkVersion="28" />

</manifest>
```

很奇怪，并没有看到 RecyclerView 库的源码，也没有看到 aar 库文件。我们接着看 Android.bp 中的依赖，其中一项是 `androidx.recyclerview_recyclerview-nodeps`，我们在 `Android.bp` 中看一下它的引入方式：

```json
android_library_import {
    name: "androidx.recyclerview_recyclerview-nodeps",
    aars: ["m2repository/androidx/recyclerview/recyclerview/1.1.0-alpha07/recyclerview-1.1.0-alpha07.aar"],
    sdk_version: "current",
    min_sdk_version: "14",
    static_libs: [
        "androidx.annotation_annotation",
        "androidx.collection_collection",
        "androidx.core_core",
        "androidx.customview_customview",
    ],
}
```

这里看到了，它的 aar 库在这里： `m2repository/androidx/recyclerview/recyclerview/1.1.0-alpha07/recyclerview-1.1.0-alpha07.aar`

继续查阅我们可以发现，`prebuilts/tools/common/m2` 目录下引入了大量的三方库。

总结一下，当我们的系统 App 需要引入一个库的时候，通常会在 prebuilds 目录下查找：

* androidx 相关库引入，先在 prebuilts/sdk/current/androidx 下寻找配置好的 bp 文件
* 其他库引入，先在 prebuilts/tools/common/m2 下寻找寻找配置好的 bp 文件

都没有，就得自己引入了


#### 3.2 自己给 AOSP 添加库

##### 3.2.1 java 库源码引入

这部分参考之前的 添加 C/C++、Java 库

##### 3.2.2 java 库以 jar 包形式引入

这部分参考之前的添加 C/C++、Java 库


##### 3.2.3 Android 库源码引入

在 `device/Jelly/Rice14` 目录下创建如下的文件和文件夹

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230228155617.png)

其中 `MyCustomView.java` 是一个用于演示的没有具体功能的自定义 View：

```java
package com.yuandaima.firstsystemandroidlibrary;

import android.content.Context;
import android.util.AttributeSet;
import android.view.View;

import androidx.annotation.Nullable;

public class MyCustomView extends View {
    public MyCustomView(Context context) {
        super(context);
    }

    public MyCustomView(Context context, @Nullable AttributeSet attrs) {
        super(context, attrs);
    }

    public MyCustomView(Context context, @Nullable AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
    }

    public MyCustomView(Context context, @Nullable AttributeSet attrs, int defStyleAttr, int defStyleRes) {
        super(context, attrs, defStyleAttr, defStyleRes);
    }
}
```

`AndroidManifest.xml` 的内容如下：

```xml
<?xml version="1.0" encoding="utf-8"?>
<manifest xmlns:android="http://schemas.android.com/apk/res/android"
 package="com.yuandaima.firstsystemandroidlibrary">

</manifest>
```

`Android.bp` 的内容如下：

```json
android_library  {
    name: "FirstSystemAndroidLibrary",

    srcs: ["src/**/*.java"],

    resource_dirs: ["res"],

    manifest: "AndroidManifest.xml",

    sdk_version: "current",

    product_specific: true,
    
    //依赖
    static_libs: ["androidx.appcompat_appcompat",],

    java_version: "1.7",

    installable: true,

}
```

接着修改我们的 FirstSystemApp 项目

`Android.bp` 添加依赖如下：

```json
android_library  {

    //......
    
    //依赖
    static_libs: ["androidx.appcompat_appcompat",
                 "com.google.android.material_material",
                 "androidx-constraintlayout_constraintlayout",
                 "FirstSystemAndroidLibrary"],

}
```

修改一下 `MainActivity`，在 App 里使用我们的自定义 View：

```java
package com.yuandaima.firstsystemapp;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;

import com.yuandaima.firstsystemandroidlibrary.MyCustomView;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        MyCustomView myView = new MyCustomView(this);
    }
}
```

接着编译系统，启动虚拟机，打开 app：

```bash
source build/envsetup.sh
lunch Rice14-eng
make -j16
emulator 
```

这样我们的库就算引入完毕了。

##### 3.2.4 Android 库以 aar 包形式引入

更多的时候 Android 库是以 aar 包的形式引入。

假设我们的 FirstSystemApp 需要引入 lottie 这个动画库。

首先我们[这里](https://repo1.maven.org/maven2/com/airbnb/android/lottie/5.2.0/)下载好 lottie 库的 aar 打包文件。

在 `device/Jelly/Rice14` 目录下创建如下的目录结构：

```bash
liblottie/
├── Android.bp
└── lottie-5.2.0.aar
```

其中 Android.bp 的内容如下：

```bash
android_library_import {
    name: "lib-lottie",
    aars: ["lottie-5.2.0.aar"],
    sdk_version: "current",
}
```

然后我们修改 FirstSystemApp 中的 Android.bp 引入这个库：

```json

    static_libs: ["androidx.appcompat_appcompat",
                 "com.google.android.material_material",
                 "androidx-constraintlayout_constraintlayout",
                 "FirstSystemAndroidLibrary",
                  "lib-lottie"],
```

这样就可以在 App 中使用 lottie 库了

### 4. JNI 项目

#### 4.1 创建 JNI 项目

Android 10 下，Android.bp(soong) 方式对 JNI 的支持有点问题，所以我们只有用 Android.mk 来演示了。Android 13 下 Android.bp (soong) 是完美支持 JNI 的。

在 `device/Jelly/Rice14` 目录下添加如下的文件与文件夹：

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230306191505.png)

jni/Android.mk 内容如下：

```Makefile
LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE_TAGS := optional

# This is the target being built.
LOCAL_MODULE:= myjnilib


# All of the source files that we will compile.
LOCAL_SRC_FILES:= \
    native.cpp

# All of the shared libraries we link against.
LOCAL_LDLIBS := -llog

# No static libraries.
LOCAL_STATIC_LIBRARIES :=

LOCAL_CFLAGS := -Wall -Werror

LOCAL_NDK_STL_VARIANT := none

LOCAL_SDK_VERSION := current

LOCAL_PRODUCT_MODULE := true

include $(BUILD_SHARED_LIBRARY)

```

jni/native.cpp 的内容如下：

```c++
/*
 * Copyright (C) 2008 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define LOG_TAG "simplejni native.cpp"
#include <android/log.h>

#include <stdio.h>

#include "jni.h"

#define ALOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)
#define ALOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define ALOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define ALOGW(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)
#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

static jint
add(JNIEnv* /*env*/, jobject /*thiz*/, jint a, jint b) {
int result = a + b;
    ALOGI("%d + %d = %d", a, b, result);
    return result;
}

static const char *classPathName = "com/example/android/simplejni/Native";

static JNINativeMethod methods[] = {
  {"add", "(II)I", (void*)add },
};

/*
 * Register several native methods for one class.
 */
static int registerNativeMethods(JNIEnv* env, const char* className,
    JNINativeMethod* gMethods, int numMethods)
{
    jclass clazz;

    clazz = env->FindClass(className);
    if (clazz == NULL) {
        ALOGE("Native registration unable to find class '%s'", className);
        return JNI_FALSE;
    }
    if (env->RegisterNatives(clazz, gMethods, numMethods) < 0) {
        ALOGE("RegisterNatives failed for '%s'", className);
        return JNI_FALSE;
    }

    return JNI_TRUE;
}

/*
 * Register native methods for all classes we know about.
 *
 * returns JNI_TRUE on success.
 */
static int registerNatives(JNIEnv* env)
{
  if (!registerNativeMethods(env, classPathName,
                 methods, sizeof(methods) / sizeof(methods[0]))) {
    return JNI_FALSE;
  }

  return JNI_TRUE;
}


// ----------------------------------------------------------------------------

/*
 * This is called by the VM when the shared library is first loaded.
 */
 
typedef union {
    JNIEnv* env;
    void* venv;
} UnionJNIEnvToVoid;

jint JNI_OnLoad(JavaVM* vm, void* /*reserved*/)
{
    UnionJNIEnvToVoid uenv;
    uenv.venv = NULL;
    jint result = -1;
    JNIEnv* env = NULL;
    
    ALOGI("JNI_OnLoad");

    if (vm->GetEnv(&uenv.venv, JNI_VERSION_1_4) != JNI_OK) {
        ALOGE("ERROR: GetEnv failed");
        goto bail;
    }
    env = uenv.env;

    if (registerNatives(env) != JNI_TRUE) {
        ALOGE("ERROR: registerNatives failed");
        goto bail;
    }
    
    result = JNI_VERSION_1_4;
    
bail:
    return result;
}
```

SimpleJNI.java 的内容如下：

```java

package com.example.android.simplejni;

import android.app.Activity;
import android.os.Bundle;
import android.widget.TextView;

public class SimpleJNI extends Activity {
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        TextView tv = new TextView(this);
        int sum = Native.add(2, 3);
        tv.setText("2 + 3 = " + Integer.toString(sum));
        setContentView(tv);
    }
}

class Native {
    static {
    	// The runtime will add "lib" on the front and ".o" on the end of
    	// the name supplied to loadLibrary.
        System.loadLibrary("simplejni");
    }

    static native int add(int a, int b);
}

```

最外面的 Android.mk 的内容如下：

```Makefile
TOP_LOCAL_PATH:= $(call my-dir)

# Build activity

LOCAL_PATH:= $(TOP_LOCAL_PATH)
include $(CLEAR_VARS)

LOCAL_MODULE_TAGS := optional

LOCAL_SRC_FILES := $(call all-subdir-java-files)

LOCAL_PACKAGE_NAME := JNIApp

LOCAL_JNI_SHARED_LIBRARIES := myjnilib

LOCAL_PROGUARD_ENABLED := disabled

LOCAL_SDK_VERSION := current

LOCAL_DEX_PREOPT := false

LOCAL_PRODUCT_MODULE := true

include $(BUILD_PACKAGE)

# ============================================================

# Also build all of the sub-targets under this one: the shared library.
include $(call all-makefiles-under,$(LOCAL_PATH))
```

AndroidManifest.xml 的内容如下：

```xml
<manifest xmlns:android="http://schemas.android.com/apk/res/android"
      package="com.example.android.simplejni">
    <application android:label="Simple JNI">
        <activity android:name="SimpleJNI">
            <intent-filter>
                <action android:name="android.intent.action.MAIN" />
                <category android:name="android.intent.category.LAUNCHER" />
            </intent-filter>
        </activity>
    </application>
</manifest> 
```

最后在 `device/Jelly/Rice14/Rice14.mk` 中添加：

```Makefile
PRODUCT_PACKAGES += helloworld \
    JNIApp \
```

编译并运行虚拟机就可以看到 JNIApp 了：

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230306200043.png)


#### 4.2 JNIApp 链接自定义库

我们这里尝试修改 JNIApp，让其引用到我们的 libmymath 库。

修改 JNIApp/jni/Android.mk：

```Makefile
# 添加以下内容
LOCAL_SHARED_LIBRARIES := libmymath
```

修改 JNIApp/jni/native.cpp：

```c++
#include "my_math.h"

static jint
add(JNIEnv* /*env*/, jobject /*thiz*/, jint a, jint b) {
    int result = a + b;
    result = my_add(result, result);
    ALOGI("%d + %d = %d", a, b, result);
    return result;
}

```

然后编译系统，发现报以下错误：

```bash
error: myjnilib (native:ndk:none:none) should not link to libmymath (native:platform)
```

可以看出是编译平台不一致导致的，修改 JNIApp/jni/Android.mk：

```Makefile
# 下面这行注释掉即可
# LOCAL_SDK_VERSION := current
```

最后重新编译，执行虚拟机即可

## 第八章 使用 Android Studio 开发系统 App

### 1. 编译 framework 模块

系统应用可以调用隐藏的API，这需要我们引入包含被隐藏 API 的 jar 包。

为了得到这个 jar 包，我们需要在源码下编译 Framework 模块：

```bash
source 
lunch rice14-eng
# Android10 及以前
make framework
# Android11 及以后
#make framework-minus-apex
```

编译完成后，我们在 `out/target/common/obj/JAVA_LIBRARIES/framework_intermediates` 目录下找到 `classes.jar` 文件，为方便识别，我们将该文件拷贝到其他地方，并将文件名修改为 `framework.jar` 。

### 2. 创建系统 App 项目

使用 Android Studio 创建一个 Empty Activity 空项目。接着把之前准备好的 framework.jar 拷贝到项目的 `app/libs` 文件夹中。

接着修改项目根目录下的 build.gradle，添加如下内容：

```groovy
allprojects{
    gradle.projectsEvaluated {
        tasks.withType(JavaCompile) {
            Set<File> fileSet = options.bootstrapClasspath.getFiles()
            List<File> newFileList = new ArrayList<>();
            newFileList.add(new File("./app/framework/framework.jar"))
            newFileList.addAll(fileSet)
            options.bootstrapClasspath = files(
                    newFileList.toArray()
            )
        }
    }
}
```

接着修改 app/build.gradle：

```groovy
dependencies {
    compileOnly files('libs/framework.jar')
    //.......
}
```

然后在项目的 AndroidManifest.xml 中添加：

```xml
<manifest xmlns:android="http://schemas.android.com/apk/res/android"
    xmlns:tools="http://schemas.android.com/tools"
    android:sharedUserId="android.uid.system">
```

接着我们需要制作系统签名，这里使用 [keytool-importkeypair](https://github.com/getfatday/keytool-importkeypair) 签名工具。

将 [keytool-importkeypair](https://github.com/getfatday/keytool-importkeypair) clone 到本地，并将其中的 keytool-importkeypair 文件添加到 PATH 路径。

接着进入系统源码下的 `build/target/product/security` 路径，接着执行：

```bash
keytool-importkeypair -k ./platform.keystore -p android -pk8 platform.pk8 -cert platform.x509.pem -alias platform
```
k 表示要生成的签名文件的名字，这里命名为 platform.keystore
-p 表示要生成的 keystore 的密码，这里是 android
-pk8 表示要导入的 platform.pk8 文件
-cert 表示要导入的platform.x509.pem
-alias 表示给生成的 platform.keystore 取一个别名，这是命名为 platform

接着，把生成的签名文件 platform.keystore 拷贝到 Android Studio 项目的 app 目录下，然后在 app/build.gradle 中添加签名的配置信息：

```groovy
android {
    signingConfigs {
        sign {
            storeFile file('platform.keystore')
            storePassword 'android'
            keyAlias = 'platform'
            keyPassword 'android'
        }
    }

    buildTypes {
        release {
            minifyEnabled false
            proguardFiles getDefaultProguardFile('proguard-android-optimize.txt'), 'proguard-rules.pro'
            signingConfig signingConfigs.sign
        }

        debug {
            minifyEnabled false
            signingConfig signingConfigs.sign
        }
    }
}
```

至此，我们在 AS 中就搭建好了我们的系统 App

### 3. 系统 APP 的编译运行

在开发过程中，大部分情况下，我们可以直接点击 Android Stuido 中的运行按钮来运行我们的配置好的 App。当我们的 App 开发完成，我们需要将其预制到系统中：

我们在系统源码下的 `device/Jelly/Rice14`  目录下，创建如下的文件与文件夹：

```bash
AsSystemApp
├── Android.mk
└── app.apk
```

其中 app.apk 是我们用 Android Studio 打包好的 apk 安装包。Android.mk 的内容如下(Android10 下，Android.bp 貌似还不支持引入 apk，实测 Android13 是没问题的，这里就使用 Android.mk 了)：

```Makefile
LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := AsSystemApp
LOCAL_CERTIFICATE := PRESIGNED
LOCAL_SRC_FILES := app.apk
LOCAL_MODULE_CLASS := APPS
LOCAL_PRODUCT_MODULE := true
include $(BUILD_PREBUILT)
```

接着修改 `device/Jelly/Rice14/Rice14.mk`

```bash
PRODUCT_PACKAGES += \
    AsSystemApp
```

然后编译系统，启动虚拟机，就可以看到我们的 App 了。

```bash
source build/envsetup.sh
lunch Rice14-eng
make -j16
emulator 
```

### 4. 系统 App 特点

#### 4.1 系统 app 可以执行三方 App 不能执行的 Api

一些 API 只能又系统 App 调用，比如：

```kotlin
SystemClock.setCurrentTimeMillis(0)
```

如果在普通 App 中使用，就会报以下的错误：

```bash
E/SystemClock: Unable to set RTC
    java.lang.SecurityException: setTime: Neither user 10099 nor current process has android.permission.SET_TIME.
```


网络上很多文章教你在 App 如何调用 Hide 的 Api，在系统 App 中可以直接使用这些 Hide Api：

```kotlin
import android.os.SystemClock

class MainActivity : AppCompatActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        SystemClock.setCurrentTimeMillis(0)

        intent = Intent(this, TestService::class.java)
        startService(intent)

        //在系统 App 中可以正常使用
        //AudioSystem 整个类被标记为 hide
        Log.d("MainActivity", "" + AudioSystem.STREAM_ACCESSIBILITY)

    }
}
```

#### 4.2 进程保活

为了保活，开发者不知道加了多少班，掉了多少头发。在系统 App 这里，可以说保活不要太简单了：

在 AndroidManifest.xml 中添加：

```xml
 <application
        android:persistent="true"
```

添加一个 Service：

```kotlin
class TestService : Service() {

    override fun onBind(p0: Intent?): IBinder? {
        return null
    }

    override fun onCreate() {
        super.onCreate()
        Thread() {
            while (true) {
                Log.d("TestService", "this is TestService")
                Thread.sleep(2000)
            }
        }.start()
    }
}
```

在 MainActivity 中启动 Service：

```kotlin
intent = Intent(this, TestService::class.java)
startService(intent)
```

最后不要忘了在 AndroidManifest.xml 中添加 Service 的声明：

```xml
        <service
            android:name=".TestService"
            android:enabled="true"
            android:exported="true">
        </service>
```

最后直接点击运行按钮启动我们的 App，可以看到 Service 一直再打 Log。

我们使用 `ps -ef | grep "AsSystemApp"` 查找到我们 App 对应的 pid，然后使用 kill 命令将其强杀掉，经过短暂的等待后，Log 窗口又开始打 Log 了，说明我们的 App 在强杀后，被系统重新拉起运行。

#### 4.3 系统 App 权限

相比三方 App，系统 App 可以使用更多的权限。因为这部分内容涉及较多系统权限相关的基础知识。我们就在系统权限部分再来讲解系统 App 权限相关的内容吧。

## 第九章 SeAndroid 使用极速上手

### 1. 基本概念

SEAndroid 是一种安全系统，相关的概念和术语对于初学者来说都相对晦涩难懂。我们可以简单地理解：

* 在 Android 系统里面有很多资源（资源主要包括了根文件系统下的文件，属性系统中的属性，binder 服务，进程，用户等）
* 为了方便描述，我们需要给这些资源取名字
* 为方便管理，我们需要给资源进行分类管理
* 另外，我们需要定义一些规则，来规范资源的使用

#### 1.1 资源的名字——安全上下文（security context）

资源的名字在 SEAndroid 中称之为安全上下文（security context），我们看一个例子：

```bash
# 这里的意思是 /dev/myse_dev 的名字（security context）是 u:object_r:myse_testdev_t:s0
/dev/myse_dev u:object_r:myse_testdev_t:s0
```

`u:object_r:myse_testdev_t:s0` 就是一个安全上下文（security context） 由4部分组成：

* u 是 selinux 的用户名（user），在 Android 中只定义了一个用户，固定为 u 
* object_r 是 selinux 中的角色（role），类似于 linux 中的用户组，一个用户可以拥有多个角色，不同的
* myse_testdev_t 是资源的类型，对于文件来说叫 type，对于进程叫 domain，内容为用户自定义
* S0 和 SELinux 为了满足军用和教育行业而设计的 Multi-Level Security（MLS）机制有关。在 Android 中固定为 s0


上面说到，资源的类型可以自定义，接下来我们看看资源类型具体如何定义：
```bash
# 定义一个类型 myse_testdev_t，后面的 dev_type 表示 myse_testdev_t 是一个设备类型
type myse_testdev_t, dev_type;
```

这里的 dev_type 可以理解为**安全上下文类型的类型**，通过 attribute 定义：

```bash
# 定义一个类型 dev_type，表示设备类型  来自 system/sepolicy/public/attributes
attribute dev_type
```

当定义好安全上下文后，我们需要将安全上下文（资源的名字）与具体的资源相关联：

```bash
# 这里的意思是 /dev/myse_dev 的名字（security context）是 u:object_r:myse_testdev_t:s0
/dev/myse_dev u:object_r:myse_testdev_t:s0
```

#### 1.2 定义资源的使用规则

以上的准备工作完成后，我们就可以添加相应的规则来限制资源的访问：

```bash
# 示例来自于 system/sepolicy/private/adbd.te 
# 允许 adbd （安全上下文）， 对安全上下文为 anr_data_file 的目录(dir)有读目录权限
allow adbd anr_data_file:dir r_dir_perms;
```

上面这条规则的意思是：允许 adbd （安全上下文）， 对安全上下文为 anr_data_file 的目录(dir)有读目录的权限。其中 adbd 称之为主体，anr_data_file 称为客体，dir 表示客体的类型是目录，类型的定义位于：`system/sepolicy/private/security_classes` :

```bash
class security
class process
class system
class capability

# file-related classes
class filesystem
class file
class dir
class fd
class lnk_file
class chr_file
class blk_file
class sock_file
class fifo_file

# network-related classes
class socket
class tcp_socket
class udp_socket
class rawip_socket
class node
class netif
class netlink_socket
class packet_socket
class key_socket
class unix_stream_socket
class unix_dgram_socket

# 省略
#.......
```

r_dir_perms 表示目录的读权限，定义在 `system/sepolicy/public/global_macros`：

```bash
# ......
define(`x_file_perms', `{ getattr execute execute_no_trans map }')
define(`r_file_perms', `{ getattr open read ioctl lock map }')
define(`w_file_perms', `{ open append write lock map }')
define(`rx_file_perms', `{ r_file_perms x_file_perms }')
define(`ra_file_perms', `{ r_file_perms append }')
define(`rw_file_perms', `{ r_file_perms w_file_perms }')
define(`rwx_file_perms', `{ rw_file_perms x_file_perms }')
define(`create_file_perms', `{ create rename setattr unlink rw_file_perms }')
#......
```

#### 1.3 类型转换（Domain/Type Transition）

系统在运行的过程中，资源的名字（安全上下文）是会变化的，一个常见的例子：init 进程的名字为 u:r:init:s0，而 init fork 的子进程显然不会也不应该拥有和 init 进程一样的名字，否则这些子进程就有了和 init 一样的权限，这不是我们需要的结果。这样的问题称之为**类型转换（Domain/Type Transition）** 。

接着我们来看一个类型转换的例子(例子来自这里[深入理解SELinux SEAndroid（第一部分）](https://blog.csdn.net/innost/article/details/19299937))：

```
type_transition init_t apache_exec_t : process apache_t;
```

type_transition 用于定义类型转换，上述语句的意思是：init_t 在执行（fork 并 execv）类型为 apache_exec_t 的可执行文件时，对应的进程（process）的类型要切换到 apache_t

上面的切换并不完整，需要完成切换，还需要下面的规则配合：

```
# init_t 进程能够执行 type 为 apache_exec_t 的文件
allow init_t apache_exec_t : file execute;
# 允许 init_t 切换进入 apache_t
allow init_t apache_t : process transition;
# 切换入口（对应为entrypoint权限）为 apache_exec_t
allow apache_t apache_exec_t : file entrypoint;
```

每次都这样写稍显麻烦，SeAndroid 定义了一个宏 domain_auto_trans 来帮我们完成上述所有功能：

```
domain_auto_trans(init_t, apache_exec_t, apache_t)
```

针对文件，也需要做类型转换，比如：

```
file_type_auto_trans(appdomain, download_file, download_file)
```

上述规则的意思是，当资源名（安全上下文）为 appdomain 的进程在资源名（安全上下文）为 download_file 的文件夹中创建新的文件时，新文件的资源名（安全上下文）为 download_file


### 2. Hello SeAndroid 示例

SEAndroid 相关的内容繁多，要完全掌握其细节费神费力，一般通过示例来学习和积累。这里演示一个访问设备文件的 CPP 可执行程序。

在 `device/Jelly/Rice14` 目录下添加如下的文件和文件夹：

```bash
hello_seandroid
├── Android.bp
└── hello_seandroid.c

sepolicy
├── device.te
├── file_contexts
└── hello_se.te
```

hello_seandroid 目录下是一个读取文件的可执行程序：

hello_seandroid.c:

```c
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#define LOG_TAG "helloseandroid"
#include <log/log.h>

int main(int argc, char *argv[])
{
	
	int fd  = -1;
	int ret = -1;
	char *content = "hello test for selinux";
	char *dev_name = "/dev/hello_seandroid_dev";
	fd = open(dev_name, O_RDWR);
	if(fd < 0)
	{
		ALOGE("open %s error: %s", dev_name, strerror(errno));
		return -1;
	}

	ret = write(fd, content, strlen(content));	
	if(ret < 0)
	{
		ALOGE("write testfile error: %s", strerror(errno));
		return -1;
	}else
	{
		ALOGD("write testfile ok: %d",  ret);
	}
	
	while(1);

	close(fd);

	return 0;
}
```

这段程序的主要作用就是向 `/dev/hello_seandroid_dev` 文件写入一段字符串。

Android.bp：

```json
cc_binary {                 
    name: "helloseandroid",         
    srcs: ["hello_seandroid.c"],    
    cflags: [
        "-Werror",
        "-Wno-unused-parameter"
    ],    
	//Android10 上貌似不支持配置 product 分区的 sepolicy（Android 11 及以后是支持的）
	//所以只能选择 vendor 分区了
    vendor: true,
    shared_libs: [
        "libcutils",
        "liblog"
    ]
}
```

hello_se.te：

```bash
# 进程对应的类型
type  hello_se_dt, domain;
# 可执行文件对应的类型
type  hello_se_dt_exec, exec_type, vendor_file_type, file_type;
#表示该程序如果从 init 进程启动 hello_seandroid_dt_exec，其安全上下文的 domain 部分从 init 转化为 hello_seandroid_dt
init_daemon_domain(hello_se_dt);
#从 shell 启动 type 为 hello_seandroid_dt_exec 的可执行程序，其对应进程的 domain 为 hello_seandroid_dt
domain_auto_trans(shell, hello_se_dt_exec, hello_se_dt);
```

device.te：

```bash
# 定义设备 /dev/hello_seandroid_dev 的类型
type hello_se_dev_t, dev_type;
```

file_contexts：

```bash
/vendor/bin/helloseandroid     u:object_r:hello_se_dt_exec:s0
/dev/hello_seandroid_dev        u:object_r:hello_se_dev_t:s0
```

编译运行：

```bash
source build/envsetup.sh
# 注意这里的版本和前面不一样了，选择了 userdebug
lunch Rice14-userdebug
make -j16
```

准备工作：

```bash
#进入Android shell 环境
adb shell
 
# 创建待访问的设备文件
su #使用 root 
touch /dev/hello_seandroid_dev
ls -Z /dev/hello_seandroid_dev                     
u:object_r:device:s0 /dev/hello_seandroid_dev
# 加载 file_contexts
restorecon /dev/hello_seandroid_dev
# 查看文件的安全上下文
ls -Z /dev/hello_seandroid_dev           
u:object_r:hello_se_dev_t:s0 /dev/hello_seandroid_dev
#放宽权限
chmod 777 /dev/hello_seandroid_dev 
# 查看可执行文件的安全上下文
ls -Z /vendor/bin/helloseandroid
u:object_r:hello_se_dt_exec:s0 /vendor/bin/helloseandroid
```

我们没有配置 selinux 权限，所以我们的程序是不能成功执行的，但是我们可以通过 `setenforce 0` 命令切换到 selinux permissive 模式，该模式下不会阻止进程的行为，只会打印权限缺失信息，这样我们就可以执行我们的程序，并得到缺失的权限信息，然后通过源码提供的 audit2allow 工具帮我们生成对于的 selinux 权限信息：

```bash
setenforce 0
exit #退出 root
# 执行程序
helloseandroid &
```

接着查看 log：

```bash
logcat | grep "helloseandroid" --line-buffered | grep "avc"               

04-08 15:11:05.290  6595  6595 I helloseandroid: type=1400 audit(0.0:36): avc: denied { use } for path="/dev/pts/0" dev="devpts" ino=3 scontext=u:r:hello_se_dt:s0 tcontext=u:r:adbd:s0 tclass=fd permissive=1
04-08 15:11:05.290  6595  6595 I helloseandroid: type=1400 audit(0.0:37): avc: denied { read write } for path="/dev/pts/0" dev="devpts" ino=3 scontext=u:r:hello_se_dt:s0 tcontext=u:object_r:devpts:s0 tclass=chr_file permissive=1
04-08 15:11:05.290  6595  6595 I helloseandroid: type=1400 audit(0.0:38): avc: denied { read write } for path="socket:[10425]" dev="sockfs" ino=10425 scontext=u:r:hello_se_dt:s0 tcontext=u:r:adbd:s0 tclass=unix_stream_socket permissive=1
04-08 15:11:05.290  6595  6595 I helloseandroid: type=1400 audit(0.0:39): avc: denied { use } for path="/vendor/bin/helloseandroid" dev="dm-1" ino=71 scontext=u:r:hello_se_dt:s0 tcontext=u:r:shell:s0 tclass=fd permissive=1
04-08 15:11:05.300  6595  6595 I helloseandroid: type=1400 audit(0.0:40): avc: denied { read write } for name="hello_seandroid_dev" dev="tmpfs" ino=23235 scontext=u:r:hello_se_dt:s0 tcontext=u:object_r:hello_se_dev_t:s0 tclass=file permissive=1
```

我们把权限相关的 log 复制下来，在源码目录下，保存到 avc_log.txt 文件中，并执行一下命令：

```bash
source build/envsetup.sh
audit2allow -i avc_log.txt

allow hello_se_dt adbd:unix_stream_socket { read write };
allow hello_se_dt devpts:chr_file { read write };
allow hello_se_dt hello_se_dev_t:file { read write };
allow hello_se_dt shell:fd use;
```

这里就会输出相应的权限规则,我们将其添加到源码中 hello_se.te 后面即可：

```bash
# 进程对应的类型
type  hello_se_dt, domain;
# 可执行文件对应的类型
type  hello_se_dt_exec, exec_type, vendor_file_type, file_type;
#表示该程序如果从 init 进程启动 hello_seandroid_dt_exec，其安全上下文的 domain 部分从 init 转化为 hello_seandroid_dt
init_daemon_domain(hello_se_dt);
#从 shell 启动 type 为 hello_seandroid_dt_exec 的可执行程序，其对应进程的 domain 为 hello_seandroid_dt
domain_auto_trans(shell, hello_se_dt_exec, hello_se_dt);

allow hello_se_dt adbd:unix_stream_socket { read write };
allow hello_se_dt devpts:chr_file { read write };
allow hello_se_dt hello_se_dev_t:file { read write };
allow hello_se_dt shell:fd use;
```

再次编译运行系统，即可正常使用 helloseandroid 程序


## 第十章 添加开机自启动 Shell 脚本


很多时候，我们想在系统启动的时候干一些“私活”，这个时候，我们就可以添加开机自启动的脚本来完成。下面我们介绍一个简单的示例：

在 `device/Jelly/Rice14` 目录下添加如下的文件与文件夹：

```bash
initscript
├── Android.bp
├── initscript.rc
└── initscript.sh

sepolicy    #部分文件为 seandroid 入门添加的内容
├── device.te      
├── file_contexts
├── hello_se.te
└── initscript.te
```

initscript.sh 是一个简单的 shell 脚本：

```bash
#!/vendor/bin/sh

echo "this is init script"
log -t initscript "this is initscript!" #打 log
```

需要注意的是 shebang 的内容是 `#!/vendor/bin/sh`。


initscript.rc 的内容如下：

```rc
service initscript /vendor/bin/initscript
    class main
    user root
    group root system
    oneshot
```

* class main 指明当前服务时系统的基本服务,保证了系统启动时，会启动这个服务
* oneshot 表示服务只执行一次

Android.bp 的内容如下：

```json
cc_prebuilt_binary {
    name: "initscript",
    srcs: ["initscript.sh"],
    init_rc: ["initscript.rc"], 
    strip: {
        none: true,
    },
    vendor: true
}

```

接着是配置 selinux：

initscript.te 的内容如下：

```te
type initscript_dt, domain;
type initscript_dt_exec, exec_type, vendor_file_type, file_type;

init_daemon_domain(initscript_dt)
domain_auto_trans(shell, initscript_dt_exec, initscript_dt);
```

file_contexts 中添加如下内容：

```te
/vendor/bin/initscript          u:object_r:initscript_dt_exec:s0
```

最后修改 `device/Jelly/Rice14/Rice14.mk`：

```Makefile
PRODUCT_PACKAGES += \
    helloseandroid \
    initscript

BOARD_SEPOLICY_DIRS += \
    device/Jelly/Rice14/sepolicy
```


接着编译系统，启动模拟器：

```Makefile
source build/envsetup.sh
lunch Rice14-userdebug
make -j16
emulator
```


接着我们查看 log：

```bash
logcat | grep initscript

04-08 23:34:06.250  1600  1600 W initscript: type=1400 audit(0.0:6): avc: denied { execute_no_trans } for path="/vendor/bin/toybox_vendor" dev="dm-1" ino=205 scontext=u:r:initscript_dt:s0 tcontext=u:object_r:vendor_toolbox_exec:s0 tclass=file permissive=0
```

错误信息，提示我们缺少权限，按照之前介绍的方法使用 audit2allow 命令，发现并没有生成缺失的权限。那怎么办？看报错信息喽：

报错信息的意思是：当 initscript_dt 执行安全上下文为 `u:object_r:vendor_toolbox_exec:s0` 的 `/vendor/bin/toybox_vendor` 时，缺少 execute_no_trans 权限。

什么意思呢？

我们先看下 `/vendor/bin/toybox_vendor` 文件：

```bash
#切换为 root
su 
#带 x 权限，是一个可执行文件
-rwxr-xr-x 1 root shell 503304 2023-04-08 23:04 /vendor/bin/toybox_vendor
#不带参数执行一下
/vendor/bin/toybox_vendor                                                                                            
acpi base64 basename bc blkid blockdev cal cat chattr chcon chgrp
chmod chown chroot chrt cksum clear cmp comm cp cpio cut date dd devmem
df diff dirname dmesg dos2unix du echo egrep env expand expr fallocate
false fgrep file find flock fmt free freeramdisk fsfreeze fsync getconf
getenforce getfattr grep groups gunzip gzip head help hostname hwclock
i2cdetect i2cdump i2cget i2cset iconv id ifconfig inotifyd insmod
install ionice iorenice iotop kill killall ln load_policy log logname
losetup ls lsattr lsmod lsof lspci lsusb makedevs md5sum microcom
mkdir mkfifo mknod mkswap mktemp modinfo modprobe more mount mountpoint
mv nbd-client nc netcat netstat nice nl nohup nproc nsenter od partprobe
paste patch pgrep pidof ping ping6 pivot_root pkill pmap printenv
printf prlimit ps pwd pwdx readlink realpath renice restorecon rev
rfkill rm rmdir rmmod runcon sed sendevent seq setenforce setfattr
setprop setsid sha1sum sha224sum sha256sum sha384sum sha512sum sleep
sort split start stat stop strings stty swapoff swapon sync sysctl
tac tail tar taskset tee time timeout top touch tr traceroute traceroute6
true truncate tty tunctl ulimit umount uname uniq unix2dos unlink
unshare uptime usleep uudecode uuencode uuidgen vconfig vmstat watch
```

从以上的操作可以看出 toybox_vendor 是一个命令集合，我们常用的 shell 命令均会通过 toybox_vendor 来执行。

再回到权限那里，我们的脚本调用了 echo log 两个命令，这两个命令会通过执行 toybox_vendor 来实现，当执行 toybox_vendor 时，我们就需要 toybox_vendor 的打开，读取，执行权限，以及配置 domain 转换（A 程序到 B 程序都需要配置域转换）。 domain 转换可以简单配置执行时不转换 execute_no_trans 即可，综上，我们在 initscript.te 中添加如下权限：

```bash
allow initscript_dt vendor_toolbox_exec:file { read open execute execute_no_trans };
```

接着再次编译系统，启动模拟器：

```Makefile
source build/envsetup.sh
lunch Rice14-userdebug
make -j16
emulator
```

进入 adb shell 查看信息：

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230409095814.png)

启动时打的 log，以及启动相关的属性值均正常，证明我们添加的脚本执行成功了。


## 第十一章 如何阅读系统源码 C/C++ 篇

### 1. 工具篇

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


### 2. 手段篇

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


## 第十二章 如何阅读系统源码 Java 篇

### 1. 工具篇 —— AIDEGen + Android Studio 

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


### 2. 手段篇 —— Java 源码的阅读

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