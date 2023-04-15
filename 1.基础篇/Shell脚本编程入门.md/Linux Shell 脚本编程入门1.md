# Linux Shell 脚本编程入门1

什么是 Shell 脚本？我们把多个 linux 命令写进一个文件，这个文件就叫 shell 脚本。shell 脚本可以重复使用，相比手动输入大大提高了效率。

## 1. Hello World

`hello.sh`：

```bash
#!/bin/bash
<<COMMENT
这是 shell 脚本中的
多行注释
COMMENT 

#这是单行注释
echo "hello world"
```

Hello World 示例中三个需要注意的点：
* 文件开头的 `!/bin/bash` 叫 shebang，用于指定执行脚本的 shell
* \# 开头的行是单行注释，<<COMMETN 与 COMMENT 之间的内容是多行注释，其中两个 COMMENT 可以更换为其他字符，但是要保持一直
* echo 是输出命令，用于输出字符串到标准输出

## 2. 如何执行脚本文件

```bash
#没有可执行权限
bash hello.sh #开启新的进程执行 hello.sh
sh hello.sh #开启新的进程执行 hello.sh
source hello.sh #当前进程下执行 hello.sh
. hello.sh #当前进程下执行 hello.sh

#有可执行权限
chmod +x hello.sh
./hello.sh  #开启新的进程执行 hello.sh
```

## 3. 输入与输出

### 3.1 echo 命令

echo 命令的作用是在屏幕输出一行文本，可以将该命令的参数原样输出。

```bash
echo hello world
hello world

#为避免歧义，建议使用双引号包裹待输出内容
echo "hello world"
```

输出多行数据时，需要使用双引号将数据包裹。另外，echo 会保留多行数据的格式：

```bash
echo \ 
"<HTML>
    <HEAD>
          <TITLE>Page Title</TITLE>
    </HEAD>
    <BODY>
          Page body.
    </BODY>
</HTML>"
```

默认情况下，echo 会在输出的文本末尾添加一个回车符，我们可以通过 `-n` 选项去除结尾的回车符：

```bash
echo a
echo b
echo -n c
echo -n d
```

-e 参数会解释引号（双引号和单引号）里面的特殊字符：

```bash
# \n 代表换行
echo -e "Hello\nWorld"
Hello
World

echo -e 'Hello\nWorld'
Hello
World

# \033 或 \e 可以设置终端的字体颜色、背景颜色、光标定位等
# [1m 是让终端粗体显示字符串，[0m 关闭终端的属性设置,如果最后没有使用 0m 关闭属性设置，则之后终端中所有的字符串都使用粗体显示
echo -e "\033[1mOK\033[0m"
```

### 3.2 printf

printf 命令用于格式化输出数据：

```bash
printf "%d" 12  #输出整数
printf "%5d" 12 #设置打印宽度为 5
printf "%-5d" 12 #-设置左对齐，默认是右对齐

printf "%f\n" 3.88 #打印小数，\n 表示换行
printf "%.3f\n" 3.88 #保留小数点后 3 位
printf "%8.3f\n" 3.88 #占用8位宽度，保留小数点后 3 位

printf "%s\n" "hello" #打印字符串
```

### 3.3 read

read 命令用于从标准输入读取一行数据：

```bash
read key1
read key1 key2 key3  #从标准输入读取 3 组字符串

read -p "请输入用户名:" user #设置一个提示信息

read -s -p "请输入密码:" pass # -s 让用户输入的任何数据都不显示
```

### 3.4 管道

管道可以将前一个命令的的输出作为后一个命令的输入：

```bash
who | wc -l  #wc -l 用于统计文本的行数
```



## 参考资料
* 《Linux Shell 核心编程指南》 丁明一
* [Bash 脚本教程](https://wangdoc.com/bash/intro)
