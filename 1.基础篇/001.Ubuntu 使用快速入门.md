# Ubuntu 使用快速入门

## 如何使用命令行

使用命令行的第一步是打开 Terminal 软件：

* 点击 windows 键，进入以下界面
![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230223221456.png)

* 在搜索栏输入 Terminal
![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230223221412.png)
* 双击 Terminal 图标打开
![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230223221702.png) 

这样我们就进入了命令行环境了。

Terminal 是一个图形化程序，我们在 Terminal 中输入一些指令（通常称为命令），Terminal 会将这些指令交由 `/bin/bash` 程序处理，处理完成后，会将输出的结果显示在 Terminal 的窗口中。

和 `/bin/bash` 提供相同功能的程序有很多种，常见的有 `/bin/sh /bin/zsh` 等。这类程序我们统称为 shell 

## 命令行基础

### 1. 提示符

打开 Terminal 后，可以看到提示符，其具体含义如下：

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230224100331.png)


### 2. 路径

在 Linux 中路径分为绝对路径和相对路径。

Linux 下的根目录为 `/`，从根目录下出发可以找到任意目录、任意文件。从根目录开始表示目录或文件的方法称为“绝对路径”。比如：

```bash
/home/zzh0838
/bin
/bin/pwd
/usr
```

有时候使用绝对路径太过麻烦，可以使用相对路径。假设当前正位于 `/home/zzh0838` 家目录下，那么：

```bash
./1.txt #表示当前目录下的 1.txt，即 /home/zzh0838/1.txt； . 表示当前目录

../book/1.txt #表示当前目录的上一级目录里，book 子目录下的 1.txt, .. 表示上一级目录
```


## 文件与目录相关命令

### 1. ls 命令

**ls** 用于查看目录下的文件信息  

```bash
ls
# -l 表示查看文件的详细信息
ls -l

total 52856
drwxrwxr-x 4 zzh0838 zzh0838     4096 1月   7 15:59 android-kernel
drwxrwxrwx 5 root    root        4096 1月   7 16:02 Aosp
drwxrwxr-x 2 zzh0838 zzh0838     4096 1月   7 11:24 bin
drwxr-xr-x 2 zzh0838 zzh0838     4096 1月  10 16:37 Desktop
drwxr-xr-x 2 zzh0838 zzh0838     4096 1月   7 10:11 Documents
drwxr-xr-x 2 zzh0838 zzh0838     4096 1月   7 10:11 Downloads
-rw-r--r-- 1 zzh0838 zzh0838     8980 1月   7 10:06 examples.desktop
drwxr-xr-x 2 zzh0838 zzh0838     4096 1月   7 10:11 Music
drwxr-xr-x 2 zzh0838 zzh0838     4096 1月   7 10:11 Pictures
drwxr-xr-x 2 zzh0838 zzh0838     4096 1月   7 10:11 Public
drwxrwxr-x 2 zzh0838 zzh0838     4096 1月  12 16:50 temp
drwxr-xr-x 2 zzh0838 zzh0838     4096 1月   7 10:11 Templates
drwxr-xr-x 2 zzh0838 zzh0838     4096 1月   7 10:11 Videos
-rw------- 1 zzh0838 zzh0838 54056661 7月  31 08:09 VMwareTools-10.3.25-20206839.tar.gz
drwxr-xr-x 8 zzh0838 zzh0838     4096 7月  31 08:08 vmware-tools-distrib
```

Linux 用户管理

* Linux 是一个多用户操作系统
* Linux 中有很多用户组，每个用户都在一个或多个用户组，

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230224103346.png)

第1个字母代表文件类型：

* -：常规文件
* d：目录文件
* b：block device 即块设备文件
* c：character device 即字符设备文件
* l：symbolic link 即符号链接文件，又称软链接文件
* p：pipe 即命名管道文件
* s：socket 即套接字文件

文件类型后面的 9 个字符表示文件的权限，以 3 个为一组

* 第一组表示 “文件所有者的权限”；
* 第二组表示 “用户组的权限”；
* 第三组表示 “其他非本用户组的权限”。

每组都是 rwx 的组合，其中 r 代表可读，w 代表可写，x 代表可执行；如果没有对应的权限，就会出现减号（-）。比 “rw-r--r--” 表示：文件的所有者对该文件有读权限、写权限，但是没有执行权限；同一个用户组的其他用户对该文件只有读权限；其他用户对该文件也只有读权限。

连接数：表示有多少文件名连接到此节点。

文件所有者：表示这个文件的 “所有者的账号”。

文件所属用户组。

文件大小：表示这个文件的大小，默认单位是 B(字节)。

文件最后被修改的时间：这个文件的创建文件日期或者是最近的修改日期。

文件名：对应文件的文件名。如果文件名之前多了一个“.”,则说明这个文件为“隐藏文件”，执行“ls -a”命令可以列出隐藏文件。


```bash
# -a 表示查看所有文件，包括隐藏文件
ls -a

.               Downloads                     .repoconfig
..              .emulator_console_auth_token  .repo_.gitconfig.json
.android        examples.desktop              .ssh
android-kernel  .gitconfig                    .sudo_as_admin_successful
Aosp            .gnupg                        temp
.bash_history   .ICEauthority                 Templates
.bash_logout    .local                        Videos
.bashrc         .mozilla                      .viminfo
bin             Music                         VMwareTools-10.3.25-20206839.tar.gz
.cache          Pictures                      vmware-tools-distrib
.config         .pki                          .vscode-server
Desktop         .profile                      .wget-hsts
Documents       Public
```

### 2. cd 命令

```bash
cd /home/username
# 进入家目录
cd ~
# 进入上一次的目录
cd -
```


### 3. pwd 命令

**pwd** 用于查看当前所在目录

```bash
pwd
#输出内容
/home/zzh0838
```

### 4. touch 命令

**touch** 用于创建一个文件

```bash
touch test
```

### 5. **ln** 用于链接文件

```bash
# 软链接
ln -s data_file sl_data_file
# 硬链接
ln  data_file l_data_file
```

软链接类似 windows 中的快捷方式，通过软链接我们可以访问到原文件。

硬链接相当于给原来的文件起一个别名。

这里入门只需做一个基本的认识即可，要彻底理解，需要对 linux 的文件系统有基本的认识才行。

### 6. mkdir 用于创建目录

```bash
mkdir dir
mkdir -p dir/dir2/dir3
```

### 7. file 用于查看文件类型

```bash
file test.txt 
test.txt: ASCII text
```

### 8. cat 用于查看文件内容

```bash
cat test
#加上行号
cat -n test
```

### 9. less 

cat 是一次性将文件内容输出到终端，文件内容很多不方便浏览，这时可以使用 less 逐页查看

```bash
# j k 上下移动
# q 退出
less -N test
```

### 10. head tail

这两个命令用于查看文件的开头和结尾

```bash
#最后十行
tail -n 10 log_file
#开始五行
head -5 .bashrc
```

### 11. rm 用于删除文件

```bash
#删除文件
rm file 
#删除文件夹
rm -r dir
#强制删除
rm -f file
## 强制删除文件夹
rm -rf dir
```


### 12. mv 用于移动文件或者重命名文件

```bash
mv file dir
mv file file_new_name
```

### 13. cp 用于复制文件

```bash
cp hello hello_back
```

### 14. tree 用户查看目录树

```bash
#可能需要安装先
sudo apt install tree
# 打印两级目录树
tree -L 2
```


### 15. chmod 用于修改文件权限

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230114170658.png)

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230114170948.png)


```bash
chmod a+r file
chmod a-x file
chmod 777 file
```

### 16. chown 用于设置文件所有者和文件关联组

```bash
chown root /var/run/httpd.pid
chown runoob:runoobgroup file1.txt
```


## 查找搜索命令


### 1. find 用于搜索文件

```bash
find /home/book/dira/ -name "test1.txt"
find /home/book/dira/ -name "*.txt"
find /home/book/dira/ -name "dira"
find . -name "*.txt"
# 默认是当前路径
find -name "*.txt"
```

-name 后面跟的字符串可以使用 Linux 通配符，常见的通配符有：

\*   代表任意字符(0到多个)
？  代表一个字符
[ ]  中间为字符组合，仅匹配其中任一一个字符

```bash
# s a[bcd].txt 找以a开头，且文件名以bcd中任一字符结尾的txt文件
ls a[bcd].txt
```


### 2. grep 查找文件中符合条件的字符串

```bash
#在test1.txt中查找字符串abc,-n 表示显示行号
grep -n "abc" test1.txt
#当前目录下递归查找字符串
grep -R "abc" .
```


## 文件压缩命令

**tar** 用于打包压缩和解压

```bash
-c(create)：表示创建用来生成文件包 。
-x：表示提取，从文件包中提取文件。
-t：可以查看压缩的文件。
-z：使用 gzip 方式进行处理，它与 "c" 结合就表示压缩，与 "x" 结合就表示解压缩。
-j：使用 bzip2 方式进行处理，它与 "c" 结合就表示压缩，与”x“结合就表示解压缩。
-v(verbose)：详细报告 tar 处理的信息。
-f(file)：表示文件，后面接着一个文件名。 
-C <指定目录> 解压到指定目录。

把目录 dira 压缩、打包为 dira.tar.gz 文件
tar -czvf dira.tar.gz dira
#解压到当前目录
tar -xzvf dira.tar.gz
#解压到 /home/book
tar -xzvf dira.tar.gz -C /home/book
```

## 网络管理相关命令

### 1. ping 用于测试主机之间网络的连通性

```bash
ping www.baidu.com
```
### 2. wget 用于在网络上下载文件

```
wget http......
```

### 3.curl 用于在网络上下载文件

```
curl http......
```

### 4.ifconfig 用于查看网络情况

```bash
ifconfig
ens33: flags=4163<UP,BROADCAST,RUNNING,MULTICAST>  mtu 1500
        inet 10.0.0.28  netmask 255.255.255.0  broadcast 10.0.0.255
        inet6 fd49:caa5:57ad::d75  prefixlen 128  scopeid 0x0<global>
        inet6 fd00:f484:8d54:d280:d933:bf40:7740:a341  prefixlen 64  scopeid 0x0<global>
        inet6 fd49:caa5:57ad:0:d933:bf40:7740:a341  prefixlen 64  scopeid 0x0<global>
        inet6 fd49:caa5:57ad:0:c026:65bf:38cc:3943  prefixlen 64  scopeid 0x0<global>
        inet6 fd00:f484:8d54:d280:3e48:f8ff:abb0:a14a  prefixlen 64  scopeid 0x0<global>
        inet6 fe80::d938:3bb3:82a1:5823  prefixlen 64  scopeid 0x20<link>
        ether 00:0c:29:35:84:45  txqueuelen 1000  (Ethernet)
        RX packets 117589  bytes 25633504 (25.6 MB)
        RX errors 0  dropped 954  overruns 0  frame 0
        TX packets 59559  bytes 16040356 (16.0 MB)
        TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0

lo: flags=73<UP,LOOPBACK,RUNNING>  mtu 65536
        inet 127.0.0.1  netmask 255.0.0.0
        inet6 ::1  prefixlen 128  scopeid 0x10<host>
        loop  txqueuelen 1000  (Local Loopback)
        RX packets 57065  bytes 14426471 (14.4 MB)
        RX errors 0  dropped 0  overruns 0  frame 0
        TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0
```

## 软件安装相关命令

**搜索软件**

```bash
sudo apt search 关键字
```

**安装软件**

```bash
sudo apt install 软件包名
```

**卸载软件**

```bash
#卸载软件
sudo apt remove 软件包名
#卸载软件并删除配置文件
sudo apt remove 软件包名 --purge
#卸载软件并移除软件依赖
sudo apt autoremove 软件包名
```

**软件更新**

```bash
sudo apt upgrade 软件包名
# 升级所有可以升级的软件
sudo apt upgrade
```

## 系统管理相关命令

**useradd** 用于添加用户

```bash
useradd hello
```

**passwd** 用于设置或者修改用户密码

```bash
passwd hello
```

**userdel** 用于删除用户

```bash
# -r 选项可以删除用户家目录
sudo userdel username
```


**usermod** 用来修改用户帐号的各项设定

```bash
#最常用于将用户加入指定的组
#将用户myfreax添加到games组
#应该始终使用-a追加选项。如果省略-a选项，则用户将从-G选项之后未列出的组中删除
sudo usermod -a -G games myfreax
```

**free** 用于查看系统内存

```bash
free -m #以 MB 为单位
free -g #以 GB 为单位
```

**whoami** 用于查看当前用户名

**ps** 用于查看进程信息

```bash
ps -a #查看所有进程
ps -e #查看除内核以外的所有进程
ps -f #全格式输出
```

**top** 动态查看进程信息

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230224142757.png)


**kill** 手动关闭
```bash
kill 1234 #后面跟pid
``` 

**shutdown** 用于关机

## vim 的基本使用

很多人不习惯在命令行下编辑文件，实际开发中也不会经常在命令行下编辑文件。但是在 Linux 系统中对文件做些简单修改时，使用 vim 命令的效率非常高。并且在很多时候，比如现场调试时，并没有 GUI 形式的编辑工具，vim 是唯一选择。


vim 编辑器有三种模式,各个模式侧重点不一样：
* 一般模式（光标移动、复制、粘贴、删除）
* 编辑模式（编辑文本）
* 命令行模式（查找和替换）

vim 编辑器的三种模式间切换如下图所示：

![](https://gitee.com/stingerzou/pic-bed/raw/master/img/20230224141446.png)

当不知道处于何种模式时，按 ESC 键返回到一般模式


## 场景实操

### Ubuntu 搭建 Samba 服务器

```bash
sudo apt install samba
```
修改 /etc/samba/smb.conf，添加如下内容：

```bash
[Project]
        comment = project
        path = /home/zzh0838/Project
        browseable = yes
        writable = yes
```

后续操作：

```bash
sudo smbpasswd -a username
sudo systemctl restart smbd
sudo systemctl enable smbd
```

### Window  ssh 免密登录 Ubuntu

```bash
# 生成公钥
ssh-keygen -t rsa
cat ~/.ssh/id_rsa.pub
```
将公钥文件的内容拷贝到 ubuntu 的 ~/.ssh/authorized_keys 中 

重启 ssh 服务

```bash
systemctl restart sshd
```

### Ubuntu 虚拟机挂载新磁盘

```bash
# 查看磁盘情况
sudo fdisk -l

# 分区
sudo fdisk /dev/sdb

Welcome to fdisk (util-linux 2.31.1).
Changes will remain in memory only, until you decide to write them.
Be careful before using the write command.

The old ext4 signature will be removed by a write command.

Device does not contain a recognized partition table.
Created a new DOS disklabel with disk identifier 0x980ef849.

Command (m for help): n
Partition type
   p   primary (0 primary, 0 extended, 4 free)
   e   extended (container for logical partitions)
Select (default p): p
Partition number (1-4, default 1): 1
First sector (2048-2147483647, default 2048): 
Last sector, +sectors or +size{K,M,G,T,P} (2048-2147483647, default 2147483647): 

Created a new partition 1 of type 'Linux' and of size 1024 GiB.

Command (m for help): w
The partition table has been altered.
Calling ioctl() to re-read partition table.
Syncing disks.

# 格式化
sudo mkfs -t ext4 /dev/sdb

# 挂载
sudo mount /dev/sdb /home/zzh0838/Aosp

sudo chmod -R 777 /home/zzh0838/Aosp
```

配置系统启动自动挂载

```bash
# 查询 uuid
sudo blkid /dev/sdb
sudo vim /etc/fstab

UUID=7eb32ab5-4e67-477d-a78c-1be16cf65e48 /home/zzh0838/Aosp ext4 defaults 0 2
```

这里配置的含义如下：
- UUID 为硬盘分区的 UUID 值
- 路径 为挂载的目标路径
- 分区格式 这里一般为 ext4
- 挂载参数 一般为默认 defaults
- 备份 0为不备份， 1为每天备份，2为不定期备份
- 检测 0为不检测，其他为优先级