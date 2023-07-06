# 指向你自己的 Kernel 路径
KERN_DIR = /home/android/Project/Kernel/goldfish

all:
	make -C $(KERN_DIR) M=`pwd` modules 

clean:
	make -C $(KERN_DIR) M=`pwd` modules clean
	rm -rf modules.order
	rm -f hello_drv_test

obj-m	+= hello_drv.o