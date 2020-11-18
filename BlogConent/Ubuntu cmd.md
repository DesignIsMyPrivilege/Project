## 查看python版本

```shell
python2 --version#查看python2版本
python3 --version#查看python3版本
```

## 查看用户[信息][1]

```shell
useradd -m username -s /bin/bash#添加用户，并创建家目录、指定shell，默认配置给用户shell较老，不适用
password username#修改用户的密码，对于已有密码用户相当于修改密码，对于新创建用户相当于添加密码
cat /etc/passwd#查看用户
#按照分号划分，一共有7个字段
#用户名:加密的口令或口令占位符:UID（用户ID）:GID（组ID）:comment:主目录：登陆的shell
cat /etc/shells#查看系统有哪些shell
cat /etc/group#查看组信息
#一共分为四段：组名、加密口令、GID号、成员列表
sudo tail -n 10 /var/log/auth.log#查看登陆日志
```
[1]:https://www.cnblogs.com/woshimrf/p/linux-user-group-command.html

* 在添加用户后，忘记指定shell，可通过以下两种方式指定shell：
  1. 当前shell输入chsh，按照提示输入shell
  2. 切换到管理员，`vi /etc/passwd`，在需要指定的用户后添加shell

##  nvidia-smi 实时刷新

`watch -n 0.1 -d nvidia-smi`     #每隔0.1秒刷新一次

## apt-get update和upgrade区别

`apt-get update` 是同步 `/etc/apt/sources.list` 和 `/etc/apt/sources.list.d` 中列出的源的索引，这样才能获取到最新的软件包

`upgrade` is used to ___install___ the newest versions of all packages currently installed on the system from the sources enumerated in` /etc/apt/sources.list`. Packages currently installed with new versions available are retrieved and upgraded.

## 锁定Ubuntu内核， 禁止自动升级

1. 首先，查看自己系统中已经安装的内核：

```shell
dpkg --get-selections |grep linux-image
```

2. 查看自己目前正在使用的是什么内核：

```shell
uname -r
```

在以上两个查询中我们通过对比第二步和第一步的结果来决定我们下一步是否要删除旧内核。

3. 卸载旧内核

```shell
apt remove linux-image-xx.xx.x-xx-generic
```

这里我们要选择我们要删除的内核来进行卸载，如果有多个内核可以直接填写多个内核的版本号。例如这样：

```shell
apt remove linux-image-xx.xx.x-xx-generic linux-image-xx.xx.x-xx-generic
```

4. 然而光以上apt remove命令卸载后其实还是不够的。我们用第一步的命令再来看一遍之后，发现内核只是被卸载了但还没删除。我们需要用以下命令来进行删除：

```shell
dpkg --purge linux-image-xx.xx.x-xx-generic
```

如果有多个内核的话要删除的话，得起多行进行删除：

```shell
dpkg --purge linux-image-xx.xx.x-xx-generic && dpkg --purge linux-image-xx.xx.x-xx-generic
```

5. 锁定我们想留下来的内核版本：

```shell
apt-mark hold linux-image-xx.xx.x-xx-generic
```

[参考]:https://www.cnblogs.com/carle-09/p/11363020.html

## vi命令

从A文件拷贝内容到B文件：

* A文件中："任意单个变量名+需要复制的行数+yy，例如`"a5yy`；注意，从当前闪光处开始往下复制，如果在文件最后则无法复制
* B文件中："A中输入的变量名+p，例如`"ap`

## 查找

find命令：`.`代表的是当前目录，`/`代表根目录

`find / -name a.txt`：在根目录下查找`a.txt`文件