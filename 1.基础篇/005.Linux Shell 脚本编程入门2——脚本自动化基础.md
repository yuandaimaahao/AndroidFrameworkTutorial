# Linux Shell 脚本编程入门2——脚本自动化基础

tftp 0x60003000 uImage;tftp 0x60500000 vexpress-v2p-ca9.dtb;setenv bootargs 'root=/dev/mmcblk0 console=ttyAMA0';bootm 0x60003000 - 0x60500000;

tftp 0x60003000 uImage;tftp 0x60800000 vexpress-v2p-ca9.dtb;setenv bootargs 'root=/dev/nfs rw nfsroot=10.0.0.103:/home/zzh0838/nfs,proto=tcp,nfsvers=3,nolock init=/linuxrc ip=10.0.0.144 console=ttyAMA0';bootm 0x60003000 - 0x60800000;

## 1. 字符串的判断与比较

在 shell 中，可以使用 test 或者 [] 来做判断：

```bash
# 使用 $? 查看上一条命令的退出码，0 代表正确（true），非 0 代表错误（false）
#这里的 a b 就是字符串本身，而不是变量
# 注意 == 两边的空格不能少
test a == b ; echo $? 
1
test a != b ; echo $?
0

test $a == $b ; echo $? #这里的 a b 是变量

# 注意 == 两边的空格不能少，[ 右侧空格不能少， ] 左侧空格不能少
[ $USER == root ]; echo $?

# 相等输出 Y，不相等输出 N
[ $USER == root ] && echo Y || echo N

[ $USER != root ] && echo Y || echo N

# 用 -z 可以测试一个字符串是否为空
[ -z "$TEST" ] && echo Y || echo N

# -n 测试一个字符串是否非空
# 注意点： $ 引用一个值时，使用 "" 括起来，否则 -n 判断会出错
[ -n "$TEST" ] && echo Y || echo N
```

## 2. 整数的判断与比较

```bash
# -eq 判断两个数是否相等
test 3 -eq 3 && echo Y || echo N
[ 3 -eq 3 ] && echo Y || echo N

# -gt 大于
[ 6 -gt 4 ] && echo Y || echo N
# -ge 大于等于
[ 6 -ge 4 ] && echo Y || echo N
# -lt 小于
[ 4 -lt 3 ] && echo Y || echo N
# -le 小于等于
[ 3 -le 4 ] && echi Y || echo N
``` 

## 3.文件属性判断

```bash
# 判断文件是否存在
[ -e test.txt ] && echo Y || N
# 判断文件是否不存在
[ ! -e test.txt ] && echo Y || N
# 判断文件存在，且类型是文件
[ -f test.txt ] && echo Y || N
# 判断文件存在，且类型是目录
[ -d hello ] && echo Y || N
# 判断是否为软链接
[ -L soft.txt ] && echo Y || echo N
# 两个文件使用相同设备、相同inode编号，则返回真，否则返回假 即硬链接判断
[ /root/hard -ef /etc/hosts ] && echo Y || echo N
# 判断当前用户对文件是否有读权限
[ -r test.txt ] && echo Y || echo N
# 判断当前用户对文件是否有写权限
[ -w test.txt ] && echo Y || echo N
# 判断当前用户对文件是否有执行权限
[ -x ver1.txt ] && echo Y || echo N

# 需要注意的是 root 用户对所有文件都具有读写权限，但是 root 对文件没有可执行权限的情况下是不可执行该文件的
```

## 4. if 语句

```bash
# if
x=3
if [ x -gt 1 ] then
    echo "hello"
fi

# 双分支 if
if [ x -lt 5 ] then
    echo "hello"
else
    echo "hello world"
fi

# 多分支 if
num=$[RANDOM%10+1]
read -p "请输入 1～10 之间的整数:" guess
if [ $guess -eq $num ];then
 echo "恭喜,猜对了,就是:$num"
elif [ $guess -lt $num ];then
 echo "Oops,猜小了."
else
 echo "Oops,猜大了."
fi
```

## 5. case 语句

```bash
read -p "请输入一个 a～f 之间的字母:" key

case $key in
a)
    echo "I am a.";;
b)
    echo "I am b.";;
c) 
    echo "I am c.";;
d) 
    echo "I am d.";;
e)
    echo "I am e.";;
f)
    echo "I am f.";;
*)
    echo "Out of range.";;
esac
```

## 6. for 循环

```bash
for i in 1 2 3 4 5
do
    ehco $i
done

for ((i=1;i<=5;i++))
do
    echo $i
done

for i in `cat user.txt`
do
    if id $i &>/dev/null ;then
        echo "$i,该账户已经存在!"
    else
        useradd $i
        echo "123456" | passwd --stdin $i
    fi
done

# continue 的使用
for i in {1..5}
do
    [ $i -eq 3 ] && continue
    echo $i
done

# break 的使用
for i in {1..5}
do
    [ $i -eq 3 ] && break
    echo $i
done
echo "game over."


# exit
for i in {1..5}
do
 [ $i -eq 3 ] && exit
 echo $i
done
echo "game over."
```

## 7. 数组

数组是一组数据的集合，数组中的每个数据被称为一个数组元素。目前 Bash 仅支持一维索引数组和关联数组，Bash 对数组大小没有限制。

```bash
# 数组的定义
name[0]="hello"
name[1]="world"

# 数组的引用
echo ${name[0]}
ehco ${name[1]}

# 数组的遍历
for i in "${name[@]}"
do
    echo $i
done
```

关联数组类似 Java 中的 Map，关联数组的下标可以是
任意字符串。关联数组的索引要求具有唯一性。

```bash
declare -A man

man[name]=Jack
man[age]=18

# 将数组作为一个整体输出
echo ${man[*]}

# 数组的遍历
for i in "${man[@]}"
do
    echo $i
done
```

## 8.函数


```bash
# 定义函数
mymkdir() {
    mkdir /tmp/test
    touch /tmp/t.txt
}

#定义函数并不会导致函数内的任何命令被执行，仅当通过函数名称调用时，函数内的命令才会被触发执行
mymkdir
```


## 参考资料

* 《Linux Shell 核心编程指南》 丁明一
* [Bash 脚本教程](https://wangdoc.com/bash/intro)