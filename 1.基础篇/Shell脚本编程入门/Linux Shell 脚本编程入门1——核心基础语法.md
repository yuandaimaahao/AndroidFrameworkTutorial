# Linux Shell 脚本编程入门1——核心基础语法

学习任何一门编程语言，和小学生学习语文类似，汉字有上万个，但是，小学生在掌握了一到两千字就可以开始阅读了，其中关键就是，小学生有字典。编程技术的学习也类似：找一份薄点的教程，快速上手，官方的文档，相关的书籍教程可以作为我们实践过程中的`字典`。

这份教程就是我总结的`一份薄点的教程` 

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
who | wc -l  #wc -l 用于统计文本的行数，该命令可以统计出当前系统中已登录的用户数
```

使用管道的前提是右侧的命令能够接受标准输入作为参数。但是，大多数命令都不接受标准输入作为参数，只能直接在命令行输入参数，这导致无法用管道命令传递参数。

echo命令就不接受管道传参：

```bash
echo "hello world" | echo
```

上面的代码不会有输出。因为管道右侧的echo不接受管道传来的标准输入作为参数。

xargs命令的作用，是将标准输入转为命令行参数：

```bash
echo "hello world" | xargs echo
```

### 3.5 输入输出重定向

echo 命令本来会将数据输出显示在屏幕上，但如果使用重定向后就可以将输出的信息导出到文件中：

```bash
# > 以覆盖的方式重定向
echo "hello the world" > test.txt
# >> 以覆盖的方式重定向
echo "hello the world" > test.txt
# 错误重定向，覆盖
ls -l /nofiles 2> test.txt
#错误重定向,追加数据
ls -l /oops 2>> test.txt 
```

使用 &> 符号可以同时将标准输出和错误输出都重定向到一个文件（覆盖），也可以使用 &>> 符号实现追加重定向：

```bash
ls -l /etc/hosts /nofile &> test.txt
ls -l /etc/passwd /ooops &>> test.txt
```

Linux 系统中有一个特殊的设备/dev/null，这是一个黑洞。无论往该文件中写入多少数据，都会被系统吞噬、丢弃。如果有些输出信息是我们不再需要的，则可以使用重定向将输出信息导入该设备文件中。

```bash
echo "hello" > /dev/null
ls /nofile 2> /dev/null
ls /etc/hosts /nofile &> /dev/null
```

## 4. 引号的使用

双引号的作用是引用一个整体：

```bash
touch a b c #创建三个文件,分别是 a、b、c
touch "a b c" #创建一个文件,空格是文件名的一部分
rm "a b c"
```

单引号也可用于引用一个整体，同时单引号还可以屏蔽特殊字符：

```bash
touch 'a b c' #引用一个整体
#屏蔽特殊字符
echo '#'
echo '$test RMB'
```

## 5.命令替换

反引号是一个命令替换符号，它可以使用命令的
输出结果替代命令：

```bash
tar -czf /root/log-`date +%Y%m%d`.tar.gz /var/log/
tar -czf x.tar.gz `ls /etc/*.conf`
```

反引号虽然很好用，但也有其自身的缺陷，比如容易跟单引号混淆，不支持嵌套（反引号中再使用反引号），为了解决这些问题，人们又设计了$()组合符号，功能也是命令替换，而且支持嵌套功能：

```bash
echo "当前系统账户登录数量:$(who | wc -l)"
ping -c2 $(hostname)
touch $(date +%Y%m%d).txt
echo "当前系统进程数量: $(ps aux | wc -l)"
echo $(echo 我是 1 级嵌套 $(echo 我是 2 级嵌套))
```

## 6. 变量

变量的定义与引用：

```bash
#变量的定义
hello=123 #等号两边不能有空格
#变量的引用
echo $test
```

系统预设变量：

```bash
#!/bin/bash
echo "当前账户是:$USER,当前账户的 UID 是:$UID"
echo "当前账户的根目录是:$HOME"
echo "当前工作目录是:$PWD"
echo "返回 0~32767 的随机数:$RANDOM"
echo "当前脚本的进程号是:$$"
echo "当前脚本的名称为:$0"
echo "当前脚本的第 1 个参数是:$1"
echo "当前脚本的第 2 个参数是:$2"
echo "当前脚本的第 3 个参数是:$3"
echo "当前脚本的所有参数是:$*"
echo "准备创建一个文件..."
#命令行的所有参数，所有的参数作为一个整体
touch "$*"
echo "准备创建多个文件..."
#命令行的所有参数，所有的参数作为独立的个体
touch "$@"
```

## 7. grep 与正则表达式

### 7.1 基本使用

grep 命令用于查找关键词并打印匹配的行：

```bash
grep th test.txt              #在 test.txt 文件中过滤包含 th 关键词的行
grep -i the test.txt          #过滤包含 the 关键词的行（不区分字母大小写）
grep -w num test.txt          #仅过滤 num 关键词（不会过滤 number 关键词）
grep -v the test.txt          #过滤不包含 the 关键词的行
grep -q root /etc/passwd      #不在屏幕上显示过滤的结果
```

### 7.2 基本正则表达式

```bash
# . 匹配任意单个字符
# 查找 : 与 0: 之间包含任意两个字符的字符串
grep ":..0:" /tmp/passwd

# * 和前一个字符成为一个整体，前一个字符出现零次或多次
# 查找包含至少一个 0 的行（第一个 0 必须出现,第二个 0 可以出现 0 次或多次）
grep "00*" /tmp/passwd

# 查找包含 oot 或 ost 的行
grep "o[os]t" /tmp/passwd

# 查找包含 0~9 数字的行（输出内容较多，这里为部分输出）
grep "[0-9]" /tmp/passwd

# 查找包含 f~q 字母的行
grep "[f-q]" /tmp/passwd

# ^ 匹配字符串的开头
# 查找以 root 开头的行
grep "^root" /tmp/passwd

# $ 匹配字符串的结尾、
# 查找以 bash 结尾的行
grep "bash$" /tmp/passwd

# ^ 匹配字符串的开头
# 查找 sbin/ 后面不跟 n 的行
grep "sbin/[^n] " /tmp/passwd

# 查找数字 0 出现最少 1 次、最多 2 次的行
grep "0\{1,2\}" /tmp/passwd
#查找两个 root 之间可以是任意字符的行。注意，这里使用\(root\）将 root 保留，后面的\1 再次调用 root，类似于前面复制 root，后面粘贴 root
grep "\(root\).*\1" /tmp/passwd

# 过滤文件的空白行
grep "^$" /tmp/passwd

#过滤文件的非空白行
grep -v "^$" /tmp/passwd
```

### 7.3 扩展正则表达式

扩展表达式使用 grep -E 或者 egrep

```bash
# + 匹配前一个字符出现一次或多次
egrep "go+d" test.txt
# ? 匹配前一个字符出现零次或一次
egrep "go?d" test.txt
# 查找数字 0 出现最少 1 次最多 2 次的行
egrep "0{1,2}" /tmp/passwd
# | 匹配逻辑或，即匹配 | 前或后的字串
# 查找包含 root 或者 admin 的行
egrep " (root|admin) " /tmp/passwd
```

## 8. 算术运算

```bash
echo $((2+4))
echo $[x+y]
let 1+2 #无输出
let i=1+2*3;echo $i
# 浮点数运算需要使用 bc 命令
x=$(echo "(1+2)*3" | bc); echo "$x"
# scale=2 保留两位小数
echo "2+3;scale=2;8/19" | bc
5
.42
```

## 参考资料

* 《Linux Shell 核心编程指南》 丁明一
* [Bash 脚本教程](https://wangdoc.com/bash/intro)