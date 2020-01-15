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


