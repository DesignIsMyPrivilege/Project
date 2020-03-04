# Problems about the Ubuntu

## 手动升级内核

选择适合Ubuntu distribution的稳定主版本（**注：**内核的选择，应当选择适应于当前使用distribution的稳定master版本内核）。

```shell
sudo dpkg --get-selections  |  grep  'linux' #查看内核详细信息
```

Ubuntu内核列表网址`https://kernel.ubuntu.com/~kernel-ppa/mainline/?C=N;O=D`，选择需要的版本，进入文件夹，针对个人电脑，通过以下命令下载内核（以5.1.8版本内核为例）：

```shell
wget https://kernel.ubuntu.com/~kernel-ppa/mainline/v5.18/linux-headers-5.1.8-050108_5.1.8-050108.201906090832_all.deb
wget https://kernel.ubuntu.com/~kernel-ppa/mainline/v5.18/linux-headers-5.1.8-050108-generic_5.1.8-050108.201906090832_amd64.deb
wget https://kernel.ubuntu.com/~kernel-ppa/mainline/v5.18/linux-image-unsigned-5.1.8-050108-generic_5.1.8-050108.201906090832_amd64.deb
wget https://kernel.ubuntu.com/~kernel-ppa/mainline/v5.18/linux-modules-5.1.8-050108-generic_5.1.8-050108.201906090832_amd64.deb
```

即下载`linux-headers、linux-headers-generic、linux-image-generic、linux-modules-generic`，之后，通过`dpkg`命令安装`deb`文件：

```shell
sudo dpkg -i *.deb
```
如果出现缺少依赖的错误信息，使用`sudo apt-get install -f `命令进行依赖自动安装；程序结束没有报错，输出done，则表示更新内核成功；如果出现缺少`libssl1.1.0(>=1.1.0)`之类的错误，通过以下命令手动安装依赖项`libssl1.1_1.1.0g-2ubuntu4_amd64.deb`

```shell
wget http://archive.ubuntu.com/ubuntu/pool/main/o/openssl/libssl1.1_1.1.0g-2ubuntu4_amd64.deb
sudo dpkg -i libssl1.1_1.1.0g-2ubuntu4_amd64.deb
```
依赖项安装完成后，重新通过`dpkg`命令安装内核`deb`文件，安装内核。
内核设置[^https://www.cnblogs.com/carle-09/p/11363020.html]

## Ubuntu源`sudo apt-get update`出现key过期的问题
使用`apt-key list`查看密钥列表，以及对应密钥的有效期
1. 使用`sudo apt-key del number `删除过期的密钥，number[^对应密钥的八位数字]
2. 使用`sudo apt-key adv --keyserver keyserver.ubuntu.com --recv-keys number`来重新下载对应number的密钥
3. 使用`sudo apt-get update`重新更新源

## Ubuntu安装多版本[GCC](https://www.cnblogs.com/zzb-Dream-90Time/p/9656545.html)

### 源安装

1. 查看gcc原版本

   ```shell
   gcc -v
   g++ -v
   ```

2. 安装需要的GCC和G++版本

   ```shell
   sudo apt-get install gcc-xxx g++-xxx
   ```

   安装完成后，查看当前系统中已安装的gcc和g++版本

   ```shell
   ls /usr/bin/gcc*
   ls /usr/bin/g++*
   ```

3. 将某个版本的gcc加入候选，最后的数字是优先级

   ```shell
   sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-3.4 50#自己安装的gcc版本
   sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-5 60#原来的gcc版本
   ###g++配置
   sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/g++-3.4 50#自己安装的gcc版本
   sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/g++-5 60#原来的gcc版本
   ```

4. 通过如下指令设置版本

   ```shell
   sudo update-alternatives --config gcc
   ###g++配置
   sudo update-alternatives --config g++
   ```

**注意：**当切换使用了其他版本的gcc时，自行选择是否需要切换g++的版本，在项目遇到c++代码会使用到gcc。

### 手动安装

低版本的[安装](https://blog.csdn.net/u013547134/article/details/83586124)