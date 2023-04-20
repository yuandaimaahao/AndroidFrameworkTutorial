Linux Shell 脚本编程入门3——数组与函数

## 1. 数组

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
```


  