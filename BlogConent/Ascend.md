# Atlas 200教程

## usb链接网卡手工配置

USB网卡的默认IP地址: `192.168.1.2`

### 宿主机上的操作

vim 编辑`/etc/network/interfaces`

输入：

```shell
auto usb虚拟网卡名称(usb_netCard_name)
iface usb_netCard_mane inet static
address ×××.×××.×××.×
netmask 255.255.255.0
```

vim 编辑`/etc/NetworkManager/NetworkManager.conf`

输入:

```shell
managed=true
```

终端输入：

```shell
ifdown usb_netCard_name
ifup usb_netCard_name
service NetworkManager restart
```

登陆板端：`ssh HwHiAiUser@192.168.1.2`

`HwHiAiUser`缺省登录密码为`Mind@123`

***

## 网线链接配置

NIC网卡的默认IP地址: `192.168.0.2`

### 宿主机上的操作

vim 编辑`/etc/network/interfaces`

```shell
auto eth0:1
iface eth0:1 inet static
address 192.168.0.×
netmask 255.255.255.×
```

vim 编辑`/etc/NetworkManager/NetworkManager.conf`

输入:

```shell
managed=true
```

终端输入：

```shell
service networking restart
service NetworkManager restart
```

登陆板端：`ssh HwHiAiUser@192.168.0.2`

`HwHiAiUser`缺省登录密码为`Mind@123`

___

## 配置交叉编译环境

交叉编译环境就是在一个硬件架构下,编译产生可以在另一个硬件架构下

运行的程序;比如,在PC的`x86`环境下编译能在`Arm`板子上运行的软件.

图片`U8` `yuv`数据`rgb`数据

