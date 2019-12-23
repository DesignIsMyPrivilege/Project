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

