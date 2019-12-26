# Atlas 200问题及解决

## Mind Studio的安装
   1. 在多Python版本的机器上安装失败
* 解决办法：
  1. 在安装失败的情况下，查看当前Python版本号，`python -V`，如果输出为`Python 3.7.3`，则使用`sudo update-alternatives --config python`命令[^自行百度ubuntu多版本Python，如何切换]，切换到Python 2的版本，进行安装。
  1. **原因分析**：原始的Mind Studio仅支持Python2，本机测试为Python 2.7.12版本，**与开发板的Pyhton2版本相一致**

***

## 无法连接宿主机器

   1. 复位重启后，查看LED1和LED2是否闪烁或者长亮。闪烁表示固件升级，待闪烁转为长亮，则Atlas 200 DK开发板启动成功；重启，直接长亮表明Atlas 200 DK开发板启动成功。若只有LED3和LED4长亮，表明Hi3559C系统启动成功，Atlas 200 DK并未成功启动。
* 解决办法：
  1. 重新制作SD卡，并修改`make_sd_card.py`中的默认ip：`NETWORK_CARD_DEFAULT_IP`：Atlas 200 DK开发者板NIC网卡的IP地址；`USB_CARD_DEFAULT_IP`：Atlas 200 DK开发者板USB网卡的IP地址。（__注__：usb网卡和NIC网卡的IP要设置合理，避免出现某个ip设置为另一个ip的网关的情形，此情况下，板子会启动失败）

***

## Profiling信息生成失败，导入MSVPfailed

   1. Mind Studio生成Profiling信息时，Processes窗口报错`RuntimeError: Bad magic number in .pyc file`
* 解决办法
  1. stackoverflow上的原因分析和解决办法：   
  ```shell
   The magic number comes from UNIX-type systems where the first few bytes of a file held a marker indicating the file type.
   Python puts a similar marker into its pyc files when it creates them.
   Then the python interpreter makes sure this number is correct when loading it.
   Anything that damages this magic number will cause your problem. 
   
  This includes editing the pyc file or trying to run a pyc from a different version of python (usually later) than your interpreter.
  
   If they are your pyc files, just delete them and let the interpreter re-compile the py files. On UNIX type systems, that could be something as simple as:
   
   rm *.pyc or find . -name '*.pyc' -delete
   
   #If they are not yours, you'll have to either get the py files for re-compilation, or an interpreter that can run the pyc files with that particular magic value.
   #One thing that might be causing the intermittent nature. The pyc that's causing the problem may only be imported under certain conditions. It's highly unlikely it would import sometimes. You should check the actual full stack trace when the import fails?
   #As an aside, the first word of all my 2.5.1(r251:54863) pyc files is 62131, 2.6.1(r261:67517) is 62161. The list of all magic numbers can be found in Python/import.c, reproduced here for completeness (current as at the time the answer was posted, it may have changed since then)
  ```
  
   网址[^https://stackoverflow.com/questions/514371/whats-the-bad-magic-number-error]
  
  2. 主要是安装Mind Studio过程中，宿主机安装用户的python版本与开发板的python版本不一致造成的[^例如开发板python编译版本为3.5，当前用户为3.7，则会产生上述错误]。因此，修改python版本环境，使开发板与宿主机安装用户的python编译环境一致即可。
     1. **原因分析：**在宿主机上安装Mind Studio的过程中，会使用宿主机上的python对`.py`文件进行编译，生成对应的`.pyc`程序；如果在宿主机上调用python3.7生成`.pyc`程序，则其在开发板3.5的环境中将不能运行，版本不匹配，产生Bad magic错误。