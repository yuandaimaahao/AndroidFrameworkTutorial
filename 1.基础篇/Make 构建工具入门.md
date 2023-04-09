# Make 构建工具入门

本文是在[Make 命令教程](https://www.ruanyifeng.com/blog/2015/02/make.html)的基础上进行的演绎和补充。

## 1. 什么是 Make

代码变成可执行文件，叫做编译（compile）；先编译这个，还是先编译那个（即编译的安排），叫做构建（build）。

Make 是最常用的构建工具，诞生于 1977 年，主要用于 C 语言的项目。但是实际上 ，任何只要某个文件有变化，就要重新构建的项目，都可以用 Make 构建。

Make 这个词，英语的意思是"制作"。Make 命令直接用了这个意思，就是要做出某个文件。比如，要做出文件 a.txt，就可以执行下面的命令。

```make
make a.txt
```

但是，如果你真的输入这条命令，它并不会起作用。因为 Make 命令本身并不知道，如何做出 a.txt，需要有人告诉它，如何调用其他命令完成这个目标。

比如，假设文件 a.txt 依赖于 b.txt 和 c.txt ，是后面两个文件连接（cat命令）的产物。那么，make 需要知道下面的规则。


```make
a.txt: b.txt c.txt
    cat b.txt c.txt > a.txt
```

也就是说，make a.txt 这条命令的背后，实际上分成两步：

* 第一步，确认 b.txt 和 c.txt 必须已经存在
* 第二步使用 cat 命令将这个两个文件合并，输出为新文件。

像这样的规则，都写在一个叫做 Makefile 的文件中，Make 命令依赖这个文件进行构建。

总之，make 只是一个根据指定的 Shell 命令进行构建的工具。它的规则很简单，你规定要构建哪个文件、它依赖哪些源文件，当那些文件有变动时，如何重新构建它。

## 2. Makefile 文件的格式

Makefile文件由一系列规则（rules）构成。每条规则的形式如下

```make
<target> : <prerequisites> 
[tab]  <commands> 
```

上面第一行冒号前面的部分，叫做"目标"（target），冒号后面的部分叫做"前置条件"（prerequisites）；第二行必须由一个tab键起首，后面跟着"命令"（commands）。

"目标"是必需的，不可省略；"前置条件"和"命令"都是可选的，但是两者之中必须至少存在一个。

每条规则就明确两件事：构建目标的前置条件是什么，以及如何构建。

我们看个最简单的例子：

```bash
test : main.c sub.c sub.h
    gcc -o test main.c sub.c
```

test 是我们的目标，它依赖于前置条件 main.c sub.c sub.h ，有一下两种情况会执行第二行的命令：

* test 文件不存在
* main.c sub.c sub.h 比 test 更新，即修改过

下面就详细讲解，每条规则的这三个组成部分。

### 2.1 目标（target）

一个目标（target）就构成一条规则。目标通常是文件名，指明Make命令所要构建的对象，比如上文的 a.txt 。目标可以是一个文件名，也可以是多个文件名，之间用空格分隔。

除了文件名，目标还可以是某个操作的名字，这称为"伪目标"（phony target）。

```make
clean:
      rm *.o
```

上面代码的目标是 clean，它不是文件名，而是一个操作的名字，属于"伪目标 "，作用是删除对象文件。

```make
make  clean
```

但是，如果当前目录中，正好有一个文件叫做 clean，那么这个命令不会执行。因为 Make 发现 clean 文件已经存在，就认为没有必要重新构建了，就不会执行指定的 rm 命令。

为了避免这种情况，可以明确声明 clean 是"伪目标"，写法如下。

```make
.PHONY: clean
clean:
        rm *.o temp
```

声明 clean 是"伪目标"之后，make就不会去检查是否存在一个叫做 clean 的文件，而是每次运行都执行对应的命令。


### 2.2 前置条件（prerequisites）

前置条件通常是一组文件名，之间用空格分隔。它指定了"目标"是否重新构建的判断标准：前置条件修改过，或者说是前置文件比目标文件更新，具体的，前置文件比前置文件的 last-modification 时间戳比目标的时间戳新，"目标"就需要重新构建。（通过 `ls -l` 可以查看到文件的 last-modification 时间）

我们接下来看一个例子：

```make
result.txt: source.txt
    cp source.txt result.txt
```
上面代码中，构建 result.txt 的前置条件是 source.txt 。如果当前目录中，source.txt 已经存在，那么 make result.txt 可以正常运行，否则必须再写一条规则，来生成 source.txt 。

```make
source.txt:
    echo "this is the source" > source.txt
```

上面代码中，source.txt 后面没有前置条件，就意味着它跟其他文件都无关，只要这个文件还不存在，每次调用 make source.txt，它都会生成。

```make
$ make result.txt
$ make result.txt
```

上面命令连续执行两次 make result.txt。第一次执行会先新建  source.txt，然后再新建 result.txt。第二次执行，Make 发现  source.txt 没有变动（时间戳晚于 result.txt），就不会执行任何操作，result.txt 也不会重新生成。


### 2.3 命令（commands）

命令（commands）表示如何更新目标文件，由一行或多行的 Shell 命令组成。它是构建"目标"的具体指令，它的运行结果通常就是生成目标文件。每行命令之前必须有一个 tab 键。

需要注意的是，每行命令在一个单独的 shell 中执行。这些 Shell 之间没有继承关系。

```make
var-lost:
    export foo=bar
    echo "foo=[$$foo]"
```

上面代码执行后（make var-lost），取不到 foo 的值。因为两行命令在两个不同的进程执行。一个解决办法是将两行命令写在一行，中间用分号分隔。

```make
var-kept:
    export foo=bar; echo "foo=[$$foo]"
```

另一个解决办法是在换行符前加反斜杠转义。

```make
var-kept:
    export foo=bar; \
    echo "foo=[$$foo]"
```

最后一个方法是加上 .ONESHELL:命令。

```make
.ONESHELL:
var-kept:
    export foo=bar; 
    echo "foo=[$$foo]"
```

## 3. Makefile 基础语法

### 3.1 注释

井号（#）在 Makefile 中表示注释

```make
# 这是注释
result.txt: source.txt
    # 这是注释
    cp source.txt result.txt # 这也是注释
```

### 3.2 回声（echoing）

正常情况下，make会打印每条命令，然后再执行，这就叫做回声（echoing）

在命令的前面加上@，就可以关闭回声。

```make
test:
    @echo TODO
```

### 3.3 通配符

通配符（wildcard）用来指定一组符合条件的文件名。Makefile 的通配符与 Bash 一致，主要有星号（*）、问号（？）和 []：

* \* 匹配0个或者是任意个字符
* ?  匹配任意一个字符
* [] 我们可以指定匹配的字符放在 "[]" 中


比较常用的就是 \* 号

```make
.PHONY:clean
clean:
        rm -f *.o
```

### 3.4 模式匹配

Make 命令允许对文件名，进行类似正则运算的匹配，主要用到的匹配符是 %。比如，假定当前目录下有 f1.c 和 f2.c 两个源码文件，需要将它们编译为对应的对象文件。

```make
%.o: %.c
```

等同于下面的写法。

```make
f1.o: f1.c
f2.o: f2.c
```

使用匹配符 %，可以将大量同类型的文件，只用一条规则就完成构建。

### 3.5 变量和赋值符

Makefile 中允许自定义变量。

```makefile
txt = Hello World
test:
    @echo $(txt)
```

上面代码中，变量 txt 等于 Hello World。在使用时，需要给在变量名前加上 $ 符号，但最好用小括号 () 或是大括号 {} 把变量给包括起来。小括号的用法比较常见。

调用 Shell 变量，需要在美元符号前，再加一个美元符号：

```makefile
test:
    @echo $$HOME
```

#### 3.5.1 递归展开(Recursively Expanded)

使用 `=` 来定义的变量是递归展开的 (Recursively Expanded)，直到该变量被使用时等号右边的内容才会被展开。而且每次使用该变量时，等号右边的内容都会被重新展开。

概念比较有点拗口，看个例子：

```make
foo = $(bar)
bar = $(ugh)
ugh = Huh?

all:
	echo $(foo)
```

执行 make all 时，(foo) 被展开成 (bar)，(bar) 被展开成 (ugh)，(ugh) 被展开成 Huh?，于是最后输出为 Huh? 最终将会打印出变量 (foo)的值为 `Huh?`

使用这种方法的一个好处是，我们可以把变量的真实值推到后面来定义。

```make
CFLAGS = $(include_dirs) -O
include_dirs = -Ifoo -Ibar
```

当 CFLAGS 在命令中被展开时，会是-Ifoo -Ibar -O

当然最主要的缺点就是递归定义可能导致出现无限循环展开，尽管 make 能检测出这样的无限循环展开并报错。

```make
CFLAGS = $(CFLAGS) -O
```

另一个问题就是如果在变量中使用函数，每次展开变量时都要重新执行函数，这种方式会使make运行得非常慢。更糟糕的是，这种用法会使得“wildcard”和“shell”发生不可预知的错误，因为你不知道这两个函数会被调用多少次。

#### 3.5.2 简单展开 (Simply Expanded)

使用 := 来定义的变量是简单展开的 (Simply Expanded)

使用这种方法，读到变量定义这一行时 等号右边立即被展开，引用的所有变量也会被立即展开。

前面的变量不能使用后面的变量，只能使用前面已定义好了的变量。

```bash
x := foo
y := $(x) bar
x := later
```

等价于：

```bash
y := foo bar
x := later
```

使用这种方法可以在变量中引入开头空格。见下面的示例:

```make
nullstring :=
space := $(nullstring) # end of the line
```

nullstring 是一个 Empty 变量，其中什么也没有，而 space 的值是一个空格。因为在操作符的右边是很难描述一个空格的，这里采用的技术很管用。先用一个 Empty 变量来标明变量的值开始了，而后面采用 `#` 注释符来表示变量定义的终止，这样，我们可以定义出其值是一个空格的变量。


#### 3.5.3 条件变量赋值

使用 ?= 操作符给变量赋值称为条件变量赋值

```make
FOO ?= bar
```
如果 FOO 没有被定义过，那么变量 FOO 的值被定义为 bar，如果 FOO 先前被定义过，那么这条语将什么也不做。

**注意**将变量定义为空字符也是定义的一种。

?= 是递归展开的

```Makefile
x := foo
y ?= $(x) bar
x := later

echo $y
```
这里输出 later bar

#### 3.5.4 追加变量值

使用 += 操作符给变量赋值，称为追加变量值。有以下几种情况：


**对未定义变量使用追加**：如果变量之前没有定义过，那么，+= 会自动变成 =，追加变量直接变成递归展开。

**对使用 := 方式定义的变量使用追加**：如果前面是以简单展开方式 (:=) 定义的变量，那么 += 在将新的值追加到已有变量的值的后面之前，会以简单展开 (:=) 的方式将原来的内容先展开

**对使用 = 方式定义的变量使用追加**：如果前面是以递归展开方式 (=) 定义的变量，那么 += 在将新的值追加到已有变量的值的后面之后，不会展开原来的内容

```make
CFLAGS = $(includes) -O
…
CFLAGS += -pg   # CFLAGS = $(includes) -O -pg  不会展开
```

这样我们就可以保留对 includes 的引用，当之后的某个节点完成对 includes 的定义时，当 CFLAGS 被使用时（即 $(CFLAGS)) ，includes 的值才会被展开


##  4. 内置变量（Implicit Variables）

Make命令提供一系列内置变量，比如，\$(CC) 指向当前使用的编译器，\$(MAKE) 指向当前使用的 Make 工具。这主要是为了跨平台的兼容性，详细的[内置变量清单见手册](https://www.gnu.org/software/make/manual/html_node/Implicit-Variables.html)。


## 5. 自动变量（Automatic Variables）

Make 命令还提供一些自动变量，它们的值与当前规则有关。主要有以下几个。

**\$@**： 指代当前目标，就是 Make 命令当前构建的那个目标。比如，make foo 的 \$@ 就指代 foo。

```Makefile
a.txt b.txt: 
    touch $@
```

等同于下面的写法。

```Makefile
a.txt b.txt: 
    touch a.txt b.txt
```

**\$<**： 指代第一个前置条件。比如，规则为 t: p1 p2，那么 \$< 就指代p1

```Makefile
a.txt: b.txt c.txt
    cp $< $@
```

等同于下面的写法：

```Makefile
a.txt: b.txt c.txt
    cp b.txt a.txt 
```

**\$?** 指代比目标更新的所有前置条件，简单的说就是修改过的前置条件，之间以空格分隔。比如，规则为 t: p1 p2，其中 p2 的时间戳比 t 新，$?就指代p2。 看个例子：

```Makefile
result.txt : a.txt b.txt
	cat $?
```
接下来同时创建三个文件：

```bash
touch a.txt b.txt result.txt
```

执行 `make result.txt`,什么都不会做：

```bash
make result.txt
make: 'result.txt' is up to date.
```

接着修改 a.txt 的内容如下：

```bash
this is a.txt
```

再次执行 `make result.txt`：

```bash
make result.txt
cat a.txt
this is a.txt
```

这里的 `$?` 就代表了 a.txt ,因为 a.txt 比 result.txt 更新



**\$^** 指代所有前置条件，之间以空格分隔。比如，规则为 t: p1 p2，那么 \$^ 就指代 p1 p2 。看个例子：

```bash
result.txt : a.txt b.txt
	cat $^
```

准备工作：

```bash
touch result.txt
#修改 a.txt 的内容如下：
this is a.txt
#修改 b.txt 的内容如下：
this is b.txt
```

执行 `make result.txt`：

```bash
make result.txt
cat a.txt b.txt
this is a.txt
this i b.txt
```

**\$*** 指代匹配符 % 匹配的部分， 比如% 匹配 f1.txt 中的f1 ，$* 就表示 f1。

## 6. 判断和循环

Makefile 使用 Bash 语法，完成判断和循环。

```Makefile
#判断当前编译器是否 gcc ，然后指定不同的库文件
ifeq ($(CC),gcc)
  libs=$(libs_for_gcc)
else
  libs=$(normal_libs)
endif

# 循环
LIST = one two three
all:
    for i in $(LIST); do \
        echo $$i; \
    done

# 等同于
all:
    for i in one two three; do \
        echo $$i; \
    done  
```


## 7. 函数

<!-- 函数的使用和变量引用的展开方式相同：

```Makefile
$(function arguments)
${function arguments}
```
关于函数的使用格式，有以下需要注意的地方： -->

<!-- * 函数主要分为两类：make 内嵌函数和用户自定义函数。对于 make 内嵌的函数，直接引用就可以了；对于用户自定义的函数，要通过 make 的 call 函数来间接调用。 
* 函数和参数列表之间要用空格隔开，多个参数之间使用逗号隔开。
* 如果在参数中引用了变量，变量的引用建议和函数引用使用统一格式：要么是一对小括号，要么是一对大括号。 -->

<!-- 函数使用示例：
如果我们想要获取某个目录下所有的 C 文件列表，可以使用扩展通配符函数：wildcard -->

Makefile 中，函数主要分为两类：Make 内嵌函数和用户自定义函数

接下来我们来看一下 Make 内嵌函数的使用。

如果我们想要获取某个目录下所有的 C 文件列表，可以使用扩展通配符函数：wildcard

```Makefile
SRC  = $(wildcard *.c)
HEAD = $(wildcard *.h)
all:
    @echo "SRC = $(SRC)"
    @echo "HEAD = $(HEAD)"
```

在当前目录下，我们新建一些C文件和H文件，然后使用 make 命令：

```Makefile
# ls
add.c  add.h  hello.c  main.c  makefile  sub.c  sub.h
# make
SRC = hello.c main.c add.c sub.c
HEAD = add.h sub.h
```

wildcard 还可用于判断文件是否存在：

```Makefile
#判断文件是否存在
files := main.o
# main.o 存在 files 的值为 main.o
# main.o 不存在 files 的值为空
files := $(wildcard $(files))
```

foreach 函数用于循环，使用方法如下：

```Makefile
names := a b c d
# $(name)中的单词会被挨个取出，并存到变量“n”中，“$(n).o”每次根据“$(n)”计算出一个值，这些值以空格分隔，最后作为foreach函数的返回，所以，$(files)的值是“a.o b.o c.o d.o”。
files := $(foreach n,$(names),$(n).o)
```

Make 提供了大量的内嵌函数，大大方便了用户 Makefile 的编写。但有时候根据需要，用户也可以自定义一些函数，然后在 Makefile 中引用它们：

```Makefile
PHONY: all

define func
    @echo "pram1 = $(0)"
    @echo "pram2 = $(1)"
endef
all:
    $(call func, hello world)
```

* 用户自定义函以 define 开头，endef 结束，给函数传递的参数在函数中使用 $(0)、$(1) 引用，分别表示第1个参数、第2个参数。更多参数，规则类似。

* 对于用户自定义函数，在 Makefile 中要使用 call 函数间接调用，各个参数之间使用空格隔开


## 参考资料

* [Make 命令教程](https://www.ruanyifeng.com/blog/2015/02/make.html)
* 《嵌入式 Linux 应用开发完全手册 韦东山》
* [Makefile学习笔记之变量定义与赋值](https://www.cnblogs.com/phillee/p/15031245.html)
* [Makefile 函数](https://www.zhaixue.cc/makefile/makefile-function.html)
