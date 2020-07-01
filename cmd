uninstall software:
终端输入：dpkg --list查看程序列表
在终端中找到你需要卸载的软件的名称，列表是按照首字母排序的
在终端上输入命令sudo apt-get --purge remove 包名（--purge是可选项，写上这个属性是将软件及其配置文件一并删除，如不需要删除配置文件，可执行sudo apt-get remove 包名）。
例如：删除的是polipo ，那么在终端输入sudo apt-get --purge remove polipo，按下回车，输入密码，再次回车。 

删除PPA源(PPA是Personal Package Archives首字母简写。翻译为中文意思是：个人软件包文档)
到源的目录：cd /etc/apt/sources.list.d

释放显存sudo kill -9 PID(替换进程号)

双系统挂在问题：
步骤1：
安装ntfs
sudo apt-get install ntfs-3g
首先我们要知道无法挂载的磁盘号，从报错信息中查找磁盘号；
当然我们也可以通过fdisk -l 来查询没有挂载的磁盘号，mount成功了之后，再输入这个指令不会有任何提示信息。
sudo ntfsfix /dev/sda5（这个是你要修复的磁盘号）
步骤2：
步骤1失败的情况下，进入windows关闭快速启动



wget --no-check-certificate -O shadowsocks-all.sh https://raw.githubusercontent.com/teddysun/shadowsocks_install/master/shadowsocks-all.sh

./shadowsocks-all.sh 2>&1 | tee shadowsocks-all.log

Shadowsocks-Python 版配置文件：
/etc/shadowsocks-python/config.json

修改配置信息：
bash shadowsocks-libev.sh
选择第 8 项：修改 Shadowsocks 配置即可重新设置 Shadowsocks 的密码、端口以及加密方式。

卸载：
bash shadowsocks-libev.sh
选择第 2 项：卸载 Shadowsocks 服务即可从服务器中卸载掉 Shadowsocks 服务

nvidia-smi 实时刷新：
watch -n 0.1 -d nvidia-smi     #每隔0.1秒刷新一次
////////////////////////////////////////////////////////////////
切换cuda版本：
sudo rm -rf cuda
sudo ln -s /usr/local/cuda-9.0 /usr/local/cuda
查看cuda版本：
//stat cuda
//nvcc --version(前两个必须在/usr/local目录下)
cat /usr/local/cuda/version.txt
cat /usr/local/cuda/include/cudnn.h | grep CUDNN_MAJOR -A 2
cudnn版本：
cat /usr/local/cuda/include/cudnn.h | grep CUDNN_MAJOR -A 2
动态链接重新生成（cuda 8.0 需要）：
cd /usr/local/cuda/lib64/
sudo rm -rf libcudnn.so libcudnn.so.7    #删除原有动态文件
sudo ln -s libcudnn.so.7.6.3 libcudnn.so.7  #生成软衔接
sudo ln -s libcudnn.so.7 libcudnn.so      #生成软链接
sudo ldconfig /usr/local/cuda/lib64/      #动态库文件配置
///////////////////////////////////////////////////////////////////  
拷贝对应的cudnn：
tar xvzf cudnn-8.0-linux-x64-v6.0.tgz
sudo cp cuda/include/cudnn.h /usr/local/cuda-8.0/include
sudo cp cuda/lib64/libcudnn* /usr/local/cuda-8.0/lib64
sudo chmod a+r /usr/local/cuda-8.0/include/cudnn.h /usr/local/cuda-8.0/lib64/libcudnn*


/sbin/ldconfig -p | grep boost_thread


apt-file search libcblas.so


####切换Python版本
update-alternatives --config python

/usr/lib/x86_64-linux-gnu错误：
libboost_thread.so -> libboost_thread.so.1.58.0


ssh -p 22122 root@81.90.191.17
f3b8A9HkMbf38zGCF4
https://www.onevps.com/portal/aff.php?aff=13511

sudo apt-key adv --keyserver keyserver.ubuntu.com --recv-keys BA300B7755AFCFAE

@sxzyOk582

