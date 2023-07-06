#include <linux/module.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/slab.h>

//定义链表节点
struct list_node_student {
    char *name;
    int age;
    int score;
    struct list_head list;
};

//定义红黑树节点类型
struct my_tree_node {
    int data;
    struct rb_node node;
};

//内核中没有提供现成的插入，查找函数，需要使用者自己实现
int rb_insert(struct rb_root *root, struct my_tree_node *insert_node) {
    struct rb_node **n = &(root->rb_node);
    struct rb_node *parent = NULL;
    while (*n) {
        struct my_tree_node *thiz = container_of(*n, struct my_tree_node, node);
        parent = *n;
        if (thiz->data > insert_node->data) {
            n = &((*n)->rb_left);
        } else if (thiz->data < insert_node->data) {
            n = &((*n)->rb_right);
        } else {
            return -1;
        }
    }

    rb_link_node(&insert_node->node, parent, n);
    rb_insert_color(&insert_node->node, root);
}

struct my_tree_node *rb_search(struct rb_root *root, int new) {
    struct rb_node *node = root->rb_node;
    while (node) {
        struct my_tree_node *my_node = container_of(node, struct my_tree_node, node);

        if (my_node->data > new) {
            node = node->rb_left;
        } else if (my_node->data < new) {
            node = node->rb_right;
        } else {
            return my_node;
        }
    }

    return NULL;
}

static int __init ds_init(void) {
    //printk 是内核中的日志打印函数
    printk("data structure test init!\n");

    //定义并初始化链表头
    LIST_HEAD(header);

    //定义并初始化一个节点
    struct list_node_student jack = {
            .name = "jack",
            .age  = 18,
            .score = 99,
            .list = LIST_HEAD_INIT(jack.list)
    };

    //链表插入节点,加入链表头
    list_add(&jack.list, &header);

    //定义并初始化第二个节点
    struct list_node_student bob = {
            .name = "bob",
            .age = 19,
            .score = 99,
            .list = LIST_HEAD_INIT(bob.list)
    };

    //链表插入节点,加入链表尾
    list_add_tail(&bob.list, &header);

    //删除节点
    list_del(&bob.list);

    //链表遍历
    //list_head 遍历
    struct list_head *pos;
    list_for_each(pos, &header) {

    }

    list_for_each_prev(pos, &header) {

    }

    //宿主结构的遍历
    struct list_node_student *student;
    list_for_each_entry(student, &header, list) {

    }

    //hlist 的基本使用

    //定义宿主结构体
    struct hdata_node {
        int data;
        struct hlist_node list;
    };

    //hash 数组
    struct hlist_head htable[256];

    struct hdata_node *hnode;

    //初始化
    for (int i = 0; i < 256; ++i) {
        INIT_HLIST_HEAD(&htable[i]);
        hnode = kmalloc(sizeof(struct hdata_node), GFP_KERNEL);
        INIT_HLIST_NODE(&(hnode->list));
        hnode->data = i * 9;
        //链表中插入数据
        //自定义 hash 算法，这里简单取余
        int key = hnode->data % 256;
        //添加到链表首部
        hlist_add_head(&hnode->list, &htable[key]);
    }

    //查询
    int search = 67 * 9;
    int key = search % 256;

    if (hlist_empty(&htable[key])) {
        //没有需要查询的项
    } else {
        //遍历查询
        hlist_for_each_entry(hnode, &htable[key], list) {
            if (hnode->data == search) {
                //找到了
                break;
            }
        }
    }

    //删除
    int delete = 88 * 9;
    int key2 = search % 256;
    struct hlist_node *n;

    if (hlist_empty(&htable[key])) {
        //没有需要查询的项
    } else {
        //遍历查询
        hlist_for_each_entry_safe(hnode, n, &htable[key], list) {
            if (hnode->data == search) {
                //找到了
                hlist_del(hnode);
                break;
            }
        }
    }

    //退出程序前释放资源
    for (i = 0; i < 256; i++) {
        //遍历每一个槽，有结点就删除
        hlist_for_each_entry_safe(hnode, n, &htable[i], list) {
            hlist_del(&hnode->list);
            kfree(hnode);
            hnode = NULL;
        }
    }

    struct my_tree_node *data;
    struct rb_node *node;
    
    struct rb_root mytree = RB_ROOT;
    
    //插入元素
    for (int j = 0; j < 10; ++j) {
        data = kmalloc(sizeof(struct my_tree_node), GFP_KERNEL);
        data->data = i * 36;
        rb_insert(&mytree, data);
    }
    
    //遍历红黑树
    for(node = rb_first(&mytree); node; node = rb_next(node)) {
        printk("key=%d\n", rb_entry(node, struct my_tree_node, node)->data);
    }
    
    //红黑树内存清理
    for(node = rb_first(&mytree); node; node = rb_next(node)) {
        data = rb_entry(node, struct my_tree_node, node);
        if (data) {
            rb_erase(&data->node, &mytree);
            kfree(data);
        }
    }
    
    return 0;
}



static void __exit ds_exit(void) {
    printk("data structure test exit\n");
}


module_init(ds_init);
module_exit(ds_exit);
