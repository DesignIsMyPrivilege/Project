# 海思模型转换步骤

模型转换的过程中，最好先安装按Anconda，可以创建一个单独的使用环境。

[注]：修改Anconda镜像

```bash
conda config --add channels https://mirrors.tuna.tsinghua.edu.cn/anaconda/pkgs/free/
conda config --add channels https://mirrors.tuna.tsinghua.edu.cn/anaconda/pkgs/main/
conda config --set show_channel_urls yes
```

修改后，可在home目录下可以发现`.condarc`文件

## Dark2caffe[以yolov3转caffe为例]

### 配置环境（ubuntu16.04）

1. 下载[caffe][1]源码

2. 安装显卡驱动、cuda和cudnn（以及opencv）

3. 编译caffe源码

   **注**：安装了Anconda的话，在base环境下编译，因为编译caffe需要的protoc版本是protobuf-2.6.1，而Anconda默认安装的是3.4.0版本

   1. [安装依赖][2]

      `sudo apt-get install libprotobuf-dev libleveldb-dev libsnappy-dev libopencv-dev libhdf5-serial-dev protobuf-compiler`

      `sudo apt-get install --no-install-recommends libboost-all-dev`

   2. 修改Makefile.config

      首先使用`cp Makefile.config.example Makefile.config`复制caffe给出的Makefile例子。

      1. 若使用cudnn，取消注释`USE_CUDNN := 1`

      2. 若使用opencv，取消注释`OPENCV_VERSION := 3`（前提是已经安装好opencv 3系列版本）

      3. 取消注释`WITH_PYTHON_LAYER := 1`，使得caffe支持python编写的layer

      4. 根据自行安装的cuda版本，删除对应的cuda架构设置
   
         ```makefile
         # For CUDA < 6.0, comment the *_50 through *_61 lines for compatibility.
         # For CUDA < 8.0, comment the *_60 and *_61 lines for compatibility.
         # For CUDA >= 9.0, comment the *_20 and *_21 lines for compatibility.
         ```
      
      5. 修改INCLUDE_DIRS和LIBRARY_DIRS内的内容
      
         ```makefile
         # Whatever else you find you need goes here.
         INCLUDE_DIRS := $(PYTHON_INCLUDE) /usr/local/include /usr/include/hdf5/serial /home/q7/Software/Driver/opencv_install/opencv-3.4.2/include
         #在前面取消注释opencv后，在后面增加对应opencv的安装目录
         LIBRARY_DIRS := $(PYTHON_LIB) /usr/local/lib /usr/lib /usr/lib/x86_64-linux-gnu /usr/lib/x86_64-linux-gnu/hdf5/serial /home/q7/Software/Driver/opencv_install/opencv-3.4.2/lib
   #在前面取消注释opencv后，在后面增加对应opencv的安装目录
         ```
   
   3. 在caffe目录下编译caffe：`make -j 32`，并生成caffe.pd.h文件，并拷贝到`caffe/include/caffe/proto`目录下
   
      ```bash
   protoc src/caffe/proto/caffe.proto --cpp_out=.
      mkdir include/caffe/proto
   mv src/caffe/proto/caffe.pb.h include/caffe/proto
      ```

   4. 生成支持caffe的python版本：`make pycaffe -j 32`
   
   5. 在`/etc/profile`文件的最后，增加如下内容：
   
      ```
      # 2020年 06月 24日 星期三
      # caffe path
export PYTHONPATH="/home/q7/Project/Hisi/model_convert/caffe/python:$PYTHONPATH"
      ```
   
      防止在import caffe过程中，出现`no module named caffe`的错误。
   
   6. 在caffe目录下测试caffe：`make runtest`

### （在Anconda环境下进行）转换darknet模型到caffe

   1. 下载转换工具[darknet2caffe][3]

   2. 环境依赖：Python2.7、Caffe、Pytorch >= 0.40

      1. Pytorch[安装][4]：在cuda10版本下`conda install pytorch==1.2.0 torchvision==0.4.0 cudatoolkit=10.0 -c pytorch`（该命令从Pytorch官网下载安装包，速度较慢；可去掉`-c pytorch`，使得其从自己设置的镜像环境中安装。）

   3. 在原始caffe中增加caffe层

      **注**：Yolov3中的shortcut层可以用eltwise替代，route层可以用concat替代，而upsample层和Yolo层则需要自己实现，并添加到Caffe中即可。

      1. 拷贝 `caffe_layers/mish_layer/mish_layer.hpp,caffe_layers/upsample_layer/upsample_layer.hpp` 到 `include/caffe/layers/`.

      2. 拷贝 `caffe_layers/mish_layer/mish_layer.cpp mish_layer.cu,caffe_layers/upsample_layer/upsample_layer.cpp upsample_layer.cu`到 `src/caffe/layers/`.

      3. 拷贝 `caffe_layers/pooling_layer/pooling_layer.cpp` 到 `src/caffe/layers/`.Note:only work for yolov3-tiny,use with caution.

         **注**：执行步骤3的话，在yolov3-tiny转换模型的过程中，就不再需要注释掉yolov3-tiny模型cfg文件中`[maxpool] size=2 stride=1`部分的内容，因为在原有的pooling_layer下，不删除该部分，会出现特征大小不匹配的情况，在覆盖了原有的pooling_layer后，应为处理方式比不同，所以不会出现特征不匹配的情况。

      4. 在 `src/caffe/proto/caffe.proto`中增加前面出现++号的部分.

      ```
      // LayerParameter next available layer-specific ID: 147 (last added: recurrent_param)
      message LayerParameter {
        optional TileParameter tile_param = 138;
        optional VideoDataParameter video_data_param = 207;//官网的caffe文件中，暂时没有该配置。可忽略
        optional WindowDataParameter window_data_param = 129;
      ++optional UpsampleParameter upsample_param = 149; //added by chen for Yolov3, make sure this id 149 not the same as before.
      ++optional MishParameter mish_param = 150; //added by chen for yolov4,make sure this id 150 not the same as before.
      }
      
      // added by chen for YoloV3
      ++message UpsampleParameter{
      ++  optional int32 scale = 1 [default = 1];
      ++}
      
      // Message that stores parameters used by MishLayer
      ++message MishParameter {
      ++  enum Engine {
      ++    DEFAULT = 0;
      ++    CAFFE = 1;
      ++    CUDNN = 2;
      ++  }
      ++  optional Engine engine = 2 [default = DEFAULT];
      ++}
      ```

   4. 配置完成后重新编译caffe

   5. 在工具目录下（需要先建立一个caffemodel的文件夹，来存储转换后的模型），通过`python cfg/yolov4.cfg weights/yolov4.weights prototxt/yolov4.prototxt caffemodel/yolov4.caffemodel`转换模型；可以修改yolo的版本号，对应不同的转换。

      `python darknet2caffe.py cfg/yolov3.cfg weights/yolov3.weights prototxt/yolov3.prototxt caffemodel/yolov3.caffemodel`，yolov3需要增加`darknet2caffe.py`

### 转换后模型的测试（base环境下）

1. 配置环境：下载测试工具[caffe-yolov3][5]，新建build目录，在目录下编译，整个过程命令如下：

   ```bash
   git clone https://github.com/ChenYingpeng/caffe-yolov3
   cd caffe-yolov3
   mkdir build
   cd build
   cmake ..
   make -j6
   ```

   将转换darknet模型到caffe步骤中，转换得到的caffemodel和对应的.prototxt文件，分别放在caffemodel（需创建）和prototxt文件夹下。

   **注**：可通过修改`src/detector.h`中的`int m_classes = 80; //coco classes`类别信息，来匹配自己训练得到的不同类别数目的模型。

2. 在build目录下测试：`./x86_64/bin/demo ../prototxt/yolov4.prototxt ../caffemodel/yolov4.caffemodel ../images/dog.jpg`

### 常见错误

1. No module named skimage.io，解决办法：`pip install scikit-image`

2. ImportError: No module named google.protobuf.internal，解决办法：

   ```bash
   #没有anaconda: 
   pip install protobuf 
   #（编译caffe就需要这个protobuf）
   
   #有anaconda: 
   conda install protobuf 
   #(最后装了libprotobuf和protobuf 3.5.2)
   ```

3. 在caffe-yolov3的build中make时，出现：`caffe.pb.h:#error This file was generated by a newer version of protoc which is`，主要是因为build在make过程中使用的protoc版本与编译caffe过程中版本不匹配。

   解决办法：先通过`protoc --version`查看对应的版本，是否与`caffe/include/caffe/proto/caffe.pb.h`中11行和16行的版本要求匹配，不匹配的情况下，先切换到caffe目录下，通过`make clean`清除编译的文件，在对应protoc版本下，通过前文caffe编译过程重新编译。在生成caffe.pd.h文件和拷贝文件的过程中，会覆盖源文件，caffe.pb.h中的protoc版本，会更新为当前环境的版本。之后，在当前环境下重新配置转换后模型的测试环境。

4. 编译 pycaffe时报错：fatal error: numpy/arrayobject.h没有那个文件或目录，解决办法：`sudo apt install python-numpy`

5. caffe/blob.hpp:9:34: fatal error: caffe/proto/caffe.pb.h: 没有那个文件或目录，解决办法：

   ```bash
   protoc src/caffe/proto/caffe.proto --cpp_out=.
   mkdir include/caffe/proto
   mv src/caffe/proto/caffe.pb.h include/caffe/proto
   ```

[1]:https://github.com/BVLC/caffe	"git clone https://github.com/BVLC/caffe"
[2]:http://caffe.berkeleyvision.org/install_apt.html	"官方指导"
[3]:https://github.com/ChenYingpeng/darknet2caffe	"git clone https://github.com/ChenYingpeng/darknet2caffe"
[4]: https://pytorch.org/get-started/previous-versions/	"Pytorch官网安装指南"

[5]:https://github.com/ChenYingpeng/caffe-yolov3.git

## 参考

Appreciate the great work from the following repositories:

- [[caffe-yolov3](https://github.com/ChenYingpeng/caffe-yolov3)]
- [[海思hi3516dv300开发--yolov3转caffe](https://zhuanlan.zhihu.com/p/95882397)]

