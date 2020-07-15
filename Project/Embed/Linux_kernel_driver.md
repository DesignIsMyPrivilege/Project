# Linux驱动

步骤：

1. 内核源码的编译
2. 编写驱动
3. 驱动编译
4. 运行开发板
5. 装载运行驱动

```mermaid
graph LR
A(.c 驱动源码) -->|"编译：使用Makefile"|B(.ko 内核模块 二进制文件)
B -->|"装载"|C(内核)  
```

## 内核源码编译

1. 获取源码：www.kernel.org，并解压`tar -xvf xxxxxx`

2. 内核编译步骤

   1. 设置交叉工具链 

      ```makefile
      ARCH  ?=    arm
      
      CROSS_COMPILE  ?=  arm-none-linux-gnueabi-
      ```
      
   2. 选择SOC
      例：`make s5pv210_defconfig` 

   3. `make menuconfig`

   4. make uImage 或者 make zImage
   
3. 移植内核

## 编写驱动

1. 头文件
   ```c
   #include <linux/init.h>
   #include <linux/module.h>
   ```

2. 声明驱动模块的装载和卸载函数

   ```c
   module_init(hello_init);
   module_exit(hello_exit);
   ```
   
3. 实现驱动模块的装载和卸载函数

   ```c
   static int __init hello_init(void){
       printk("-----^_^-----\n");
       return 0;
   }
   static void __exit hello_exit(void){
       printk("-----^_^-----\n");
   }
   ```

4. 添加GPL认证

   ```c
   MODULE_LICENSE("GPL");
   ```

```c
#include <linux/init.h>
#include <linux/module.h>

static int __init hello_init(void){
    printk("-----^_^-----\n");
    return 0;
}
static void __exit hello_exit(void){
    printk("-----^_^-----\n");
}


module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
```

## 驱动编译

`Makefile`结论：如果要编译驱动模块就必须先编译内核源码

```makefile
#内核源码路径
KERNEL_DIR := XXXXXXXX\kernel\linux-4.19
CUR_DIR := $(shell pwd)

all:
	#表示进入内核目录，并且告诉内核要将当前的源码编译成模块
	make -C $(KERNEL_DIR) M=$(CUR_DIR) modules
install:
	cp -raf *.ko /opt/rootfs/drv_module/
clean:
	make -C $(KERNEL_DIR) M=$(CUR_DIR) clean
	
#指定编译哪个源文件
obj-m := hello_drv.o
```

## 运行开发板

启动Linux：

1. 烧写`uboot`

2. 下载启动内核

   使用`tftp`传输文件

   1. PC端：必须是编译并且移植后的内核

      将其拷贝到传输的文件夹下：`cp -raf arch/arm/boot/uImage /tftpboot`

   2. `uboot`中：

      ```
      set serverip xxx.xxx.xx.xx
      set ipaddr xxx.xxx.xx.xx
      set bootcmd tftp 0xXXXXXXXX uImage \; bootm 0xXXXXXXXX
      save
      ```

3. 挂载根文件系统

   使用`nfs`挂载

   1. PC端：

      解压根文件系统`tar -xvf rootfs_XXXX.tgz`，解压后，会生成roofts文件夹；配置`nfs`服务器：假设挂载目录是/opt/rootfs，修改配置`vim /etc/exports`；重启`nfs`服务，`service nfs-kernel-server restart`

   2. uboot中

      ```
      set bootarg console=ttySAC0,115200 root=/dev/nfs nfsroot=xxx.xxx.xx.xx(PC端ip):/opt/rootfs ip=XXX.XXX.XX.XX(板子ip)
      save
      ```

      `bootarg`是一个变量，传递给内核

## 装载运行驱动

驱动模块必须在Linux运行的状态下进行装载

`.ko`程序：1. 在Linux的用户态进行装载	2. 运行在Linux的内核态

例：`cp -raf hello_drv.ko /opt/rootfs`

开发板上：`insmod hello_drv.ko`进行装载驱动、`rmmod hello_drv`进行卸载驱动、`lsmod`查看装载的驱动

# Linux驱动编写

## 驱动模块特性

1. 模块传参

   `insmod hello_drv.ko number=250 name="ruhua"`

   代码：

   `module_param(参数名，参数类型，权限)`

   ​	例子：`module_param(number,int,0644);`

   ​	生成的参数可以在`/sys/module/hello_drv/parameters/`中查看

2. 模块调用

   1.ko和2.ko，1.ko的驱动代码调用2.ko的驱动代码中的函数；在2.ko的驱动代码中使用`EXPORT_SYMBOL(被调用函数名)`使得该函数能够被调用；其次，使用2.h文件声明被调用的函数，并在1.ko的驱动代码中包含2.h为文件。`Makefile`中添加`obj-m += 2.o`，使得make时同时生成2.ko驱动文件。最后，装载驱动，先装载2.ko，在装载1.ko

## 真正的设备驱动需要的元素

1. 需要设备号
   		因为内核中有很多的设备驱动,所以需要一个设备号id来进行区分
      		设备号分成两个部分:
      			主设备号:某一类设备
      			次设备号: 某类设备中某个设备设备
      				比如:前置和后置摄像头都是camera这类设备
						   前置 :  0
      					后置 :  1
      		在内核中:　dev_t来表示设备号, 32bit的整数
      					高12bit: 主设备号
      					低20bit:次设备号
   需要有一个设备号----申请/注册
   参数1---指定一个号码也可以由系统分配,填0表示由系统分配
   参数2--字符串--描述设备驱动信息--自定义--/proc/devices
   参数3---文件操作对象
   返回值---如果是系统分配,返回分配之后的号码,否则返回负数为错误

   `int register_chrdev(unsigned int major, const char *name,
   				  const struct file_operations *fops)`

   模块卸载函数中主要完成系统资源的释放
   参数1---已经申请到的设备号
   参数2--字符串--描述设备驱动信息--自定义

   `unregister_chrdev(dev_major, "hello_drv");`

   使用`cat /proc/device`查看设备号

2. 需要设备文件

   Linux中将所有的设备都看成是文件,操作设备其实就是操作文件

   设备文件称为设备节点

   创建:　手动和自动创建

   1. 手动：每次都要创建, /dev中所有的文件都是在内存中

      `mknod　文件名   类型 主设备号  次设备号`

      `mknod /dev/hello c 254 0`

      ```shell
      ls /dev/hello -l
      			crw-r--r--    1 0        0         254,   0 Jan  1 00:52 /dev/hello
      ```

3. 需要设备的操作

   ```c
   int hello_drv_open(struct inode *inode, struct file *filp){
   
   			printk("-------^_^ %s-------\n", __FUNCTION__);
   			return 0;
   }
   const struct file_operations hello_fops = {
   			.open = hello_drv_open,
   };
   ```

   