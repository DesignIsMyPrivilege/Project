# Hi3559使用BusyBox出现的问题

## 添加用户

只能使用`adduser`命令

> 设置新添加用户所使用的shell，新用户的shell在和root的shell一样的情况下，通过新建用户登陆时，会出现`permissions deny、 Using fallback`等情况；该情况不能通过配置`/etc/sudoers`来解决，只能通过设置user的id等于root的id（root id为0）来解决[^SUID的方式] [^参考1]

[^参考1]:http://vanilla.slitaz.org/index.php?p=/discussion/1749/busybox-using-fallback-suid-method/p1

***

