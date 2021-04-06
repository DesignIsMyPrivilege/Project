# OPENCV install

## OPECV install in PC

`opencv3.4.2`为例

PC机环境：Ubuntu 16.04

1. 安装包下载

   请到`OpenCV`的官方网站:http://opencv.org/releases.html 下载`Opencv3.4.2`。

2. 依赖库安装

   ```shell
   # 1. 安装cmake
   sudo apt install cmake
   # 2. 安装GTK+ 2.0或更高版本(可选),输入（GTK主要是界面有关的库）
   sudo apt install libgtk2.0-dev
   # 3. 安装libav开发包:libavcodec-dev, libavformat-dev, libswscale-dev
   sudo apt install libavcodec-dev libavformat-dev libswscale-dev
   ```

3. 安装前配置

   ```shell
   # 1. 解压opencv-3.4.2.zip，得到opencv-3.4.2文件夹
   unzip opencv-3.4.2.zip
   # 2. 在opencv-3.4.2内,建一个build文件夹,用来存放编译文件
   cd opencv-3.4.2
   mkdir build
   # 3. 进入build目录，进行cmake配置
   cd build
   cmake -D CMAKE_BUILD_TYPE=RELEASE \
   -D CMAKE_INSTALL_PREFIX=/最终编译后结果的存储路径（建议绝对路径，在opencv-3.4.2新建一个output文件夹来存储，当前教程以opencv-3.4.2为结果路径，没有建立output文件夹） \
   -D WITH_GPHOTO2=OFF \
   -D WITH_GTK=OFF \
   -D WITH_CUDA=OFF \
   -D WITH_FFMPEG=OFF \
   -D WITH_LAPACK=OFF ../
   # WITH_GPHOTO2=OFF和WITH_GTK=OFF WITH_CUDA=OFF -D WITH_FFMPEG=OFF
   # -D WITH_LAPACK=OFF用于关闭编译依赖项,建议关闭编译依赖项,降低编译复杂度。
   ```

4. 编译与安装

   在`build`目录下输入

   ```shell
   make -j18     # 编译
   make install  # 安装
   ```

5. 环境变量配置

   ```shell
   vim ~/.bashrc
   # 在.bashrc末尾加上如下字段
   #opencv3.4.2
   export PATH=/....../opencv-3.4.2/bin:$PATH
   export LD_LIBRARY_PATH=/....../opencv-3.4.2/lib:$LD_LIBRARY_PATH
   export PKG_CONFIG_PATH=/....../opencv-3.4.2/lib/pkgconfig
   # 退出.bashrc
   source ~/.bashrc
   ```

6. 测试

   ```shell
   # 进入Opencv下的sample/cpp/下，编译人脸识别的示例程序
   gcc `pkg-config --cflags opencv` -o facedetect facedetect.cpp `pkg-config --libs opencv` -lstdc++
   # 如果顺利编译，说明Opencv安装成功，并且会在该目录下生成facedetect的可执行程序
   # 输入以下指令，运行该程序
   ./facedetect ../data/lena.jpg
   ```

## OPENCV cross-compling

`opencv3.4.2`为例

交叉编译工具链：aarch64-himix100-linux

移植平台：Hi3559AV100

1. 安装包下载

   请到`OpenCV`的官方网站:http://opencv.org/releases.html 下载`Opencv3.4.2`。

2. 配置和编译

   进入`Opencv3.4.2`目录下，添加如下的Makefile文件

   ```makefile
   CUR_DIR      := $(shell pwd)
   
   .PHONY:all prepare clean install
   .PHONY:opencv_cmake opencv_make
   
   all: clean prepare opencv_cmake opencv_make install
   
   prepare:
   	@mkdir -p $(CUR_DIR)/build
   	@mkdir -p $(CUR_DIR)/output
   
   opencv_cmake:
   	@pushd build; \
   	 cmake  -D CMAKE_BUILD_TYPE=RELEASE  \                    # build 类型(Debug,Release,…)
   			-D CMAKE_INSTALL_PREFIX=../output  \
   			-D CMAKE_C_COMPILER=aarch64-himix100-linux-gcc \      # 指定C编译器
   			-D CMAKE_CXX_COMPILER=aarch64-himix100-linux-g++ \    # 指定C++编译器
   			-D CMAKE_EXE_LINKER_FLAGS=-lrt -lpthread \            # 解决问题1
   			-D WITH_MATLAB=OFF \
   			-D WITH_GTK=OFF \                                     
   			# GTK是类似界面的东西，运行X-WINDOWS的GNOME界面的，但ARM里运行GTK需要X-WINDOWS支持
   			# 两者都是很耗资源的，对ARM板的动态存储能力要求很高，一般关闭
   			# 由于Hi3559是终端操作，不是图形化界面，因此建议关闭，不然后续也会报错
   			-D BUILD_SHARED_LIBS=ON \                             # ON 静态库，OFF 动态库
   			-D OPENCV_EXTRA_C_FLAGS=-fPIC  \
   			-D BUILD_opencv_apps=OFF \
   			-D BUILD_ZLIB=ON \                                    # 编译ZLib库文件使能
   			-D BUILD_PNG=ON \                                     # 编译PNG库文件使能
   			-D BUILD_opencv_js=OFF \
   			-D BUILD_TESTS=OFF \
   			-D BUILD_ANDROID_EXAMPLES=OFF \
   			-D BUILD_PACKAGE=OFF \
   			-D BUILD_TESTS=OFF \
   			-D BUILD_PERF_TESTS=OFF \
   			-D ENABLE_PIC=TRUE \
   			-D OpenCV_ARCH=AARCH64 \
   			-D ZLIB_INCLUDE_DIR=$(CUR_DIR)/3rdparty/zlib \        
   			-D ZLIB_INCLUDE_DIRS=$(CUR_DIR)/3rdparty/zlib \
   			# 设置查找zlib的路径
   			# OpenCV3.4.2已经包含了第三方的库
   			..  \
   		  ;popd
   
   opencv_make:
   	@pushd build;make -j16;popd
   
   install:
   	 @pushd build;make install;popd
   
   clean:
   	@rm -rf $(CUR_DIR)/build
   	@rm -rf $(CUR_DIR)/output
   ```

   参照参考2，将`opencv-3.4.1/3rdparty/libpng/arm/`下的代码全选，放到`opencv-3.4.1/3rdparty/libpng`目录中，进行编译；**注意提前修改问题6中的说明**。

   执行步骤：
   
   ```shell
   make prepare
   make opencv_cmake
   # 执行opencv_cmake后，在build目录下会存在CMakeCache.txt
   # 该文件是上次cmake时候留下的缓存文件，如果在编译过程中报错，可以将该文件删除，然后再执行cmake
   # 或者make clean，从头开始
   make opencv_make
   make install
   ```
   **注意**：
   
   ```shell
   CMAKE_C_FLAGS：
      # 编译C文件时的选项，如-g；也可以通过add_definitions添加编译选项
      EXECUTABLE_OUTPUT_PATH：              # 可执行文件的存放路径
      LIBRARY_OUTPUT_PATH：                 # 库文件路径
      CMAKE_BUILD_TYPE:：                   # build 类型(Debug, Release, …)
      BUILD_SHARED_LIBS：                   # ON 静态库， OFF 动态库
      # 最后两个配置不能省略
   ```
   
3. 测试

   编辑人脸检测cpp文件
   
   ```c++
   #include <stdio.h>
   #include <sys/mman.h>
   #include <sys/types.h>
   #include <sys/stat.h>
   #include <fcntl.h>
   #include <stdlib.h>
   #include <string.h>
   #include <unistd.h>
   #include <signal.h>
   #include <sys/reboot.h>
   #include <sys/time.h>
   
   #include "opencv/cv.h"
   #include "opencv/highgui.h"
   #include "opencv2/highgui.hpp"
   
   using namespace cv;
   using namespace std;
   
   #ifdef __cplusplus
   #if __cplusplus
   extern "C"{
   #endif
   #endif /* __cplusplus */
   /******************for time mesurement*************************/
   struct timeval tpstart,tpend;
   unsigned long timeuses;
   void timeRec()
   {
       gettimeofday(&tpstart,0);
   }
   
   int timeRep()
   {
       gettimeofday(&tpend,0);
       timeuses=(tpend.tv_sec-tpstart.tv_sec)*1000000+tpend.tv_usec-tpstart.tv_usec;
       printf("use time: %luus\n",timeuses);
       return timeuses;
   }
   /********************end**************************************/
   int g_bRunning = 1;
   
   void exit_process()
   {
   	g_bRunning = 0;
   }
   
   static void exit_sig_process(int signo)
   {
   	
       if (SIGINT == signo || SIGTERM == signo)
       {
           exit_process();
       }
   	return;
   }
   
   
   int main(int argc, char* argv[])
   {
   	signal(SIGINT, exit_sig_process);
   	signal(SIGTERM, exit_sig_process);
   
   	printf("%s \n", __FUNCTION__);
   	IplImage* img = NULL;
   //	IplImage* cutImg = NULL;
   	CvMemStorage* storage = cvCreateMemStorage(0);
   	Mat dest;
   	CvRect *pface_res;
   
   	printf("%s	cvLoad \n", __FUNCTION__);
   
   	//加载检测器
   	CvHaarClassifierCascade* cascade = (CvHaarClassifierCascade*)cvLoad("./haarcascade_frontalface_alt2.xml", 0, 0, 0);
   	CvSeq* faces; 
   	
   	printf("%s  cvLoadImage\n", __FUNCTION__);
   
   	img = cvLoadImage(argv[1], 0);
    // 缩放到1/4大小
    // IplImage *smallImg = cvCreateImage(cvSize(img->width/4, img->height/4), 8, img->nChannels);
    // printf("smallImg w=%d h=%d\n", smallImg->width, smallImg->height);
       // 检测并计时
   	timeRec();
   	
   	printf("%s  cvLoadImage %s \n", __FUNCTION__,argv[1]);
    // faces = cvHaarDetectObjects(smallImg, cascade,  storage, 1.5, 4, 0, cvSize(25,25) );
       faces = cvHaarDetectObjects(img, cascade,  storage, 1.2, 2, 0, cvSize(25,25) );
   	timeRep();
   	if (faces->total == 0){
   		printf("no face!\n");
   	}
   
   	printf("faces total is :%d \n",faces->total);
   	// 切取出头像
    // cvSetImageROI(smallImg, *((CvRect*)cvGetSeqElem( faces, 0)));
       
   	pface_res = (CvRect*)cvGetSeqElem( faces, 0);
   	printf("(%d,%d),(%d,%d), \n",pface_res->x,pface_res->y,pface_res->width,pface_res->height);
   	
   	printf("cvSetImageROI!  start \n");
   	cvSetImageROI(img, *(pface_res)); 
   	printf("cvSaveImage!  start \n");
   	
       // cvSaveImage("face.bmp", img);  
    // cvSaveImage("face.bmp", smallImg);
   	dest = cvarrToMat(img);
   	imwrite("face.bmp", dest);	
   	printf("cvResetImageROI!  start \n");
   	cvResetImageROI(img);
    // cvResetImageROI(smallImg);
   	printf("face detected! in face.bmp!\n");
      	// 释放内存
       cvReleaseImage(&img);
   	return 0;
   }
   
   #ifdef __cplusplus
   #if __cplusplus
   }
   #endif
   #endif /* __cplusplus */
   ```
   
   在PC机上进行如下命令的交叉编译，生成`face`可执行文件：
   
   ```shell
   aarch64-himix100-linux-g++ -I/your opencv path/opencv-3.4.2/output/include -I/your opencv path/opencv-3.4.2/output/include/opencv -Wl,-rpath-link -Wl,/your opencv path/opencv-3.4.2/output/lib -L/your opencv path/opencv-3.4.2/output/lib -lopencv_highgui -lopencv_core -lopencv_imgproc -lpthread -lrt -lopencv_objdetect -lopencv_imgcodecs -o face face_detection.cpp
   ```
   
   将`.../opencv-3.4.2/haarcascades_cuda`文件夹下的`haarcascade_frontalface_alt2.xml`拷贝到当前`face`文件夹下，并将`/your opencv path/opencv-3.4.2/output/lib`下，需要用到的`lib`拷贝到板端的`/usr/lib`目录下（如果是root用户telnat到板端的话，拷贝用到的`lib`到板端的`/lib`下，如果在自建用户登录的情况下，拷贝`lib`到板端的`/lib`下，会出现问题7，改为拷贝到`/usr/lib`目录下即可）。寻找一张人脸图片到当前`face`文件夹下，以`lena.jpg`为例。
   
   执行如下命令：
   
   ```shell
   ./face lena.jpg
   main 
   main    cvLoad 
   main  cvLoadImage
   main  cvLoadImage lena.jpg 
   use time: 161223us
   faces total is :1 
   (217,202),(173,173), 
   cvSetImageROI!  start 
   cvSaveImage!  start 
   cvResetImageROI!  start 
   face detected! in face.bmp!
   ```

### cmake过程注意

`make opencv_cmake`在build文件夹下生成内容的过程中，注意查看终端输出的编译信息：

```shell
.......
--   GUI: 
--     GTK+:                        YES (ver 2.24.30)           # GTK选项未关闭，编译会出现问题3
--       GThread :                  YES (ver 2.48.2)              
--       GtkGlExt:                  NO                            
--     VTK support:                 NO
--    
--   Media I/O: 
--     ZLib:                        /....../anaconda2/lib/libz.so (ver 1.2.11)
# cmake在生成CMakeCache.txt的过程中，会搜索PC环境里面的所有文件，如果找到名字匹配的库，就会使用该库
# 但是该库是x86_64环境下的库，并不是ARM环境下的库，也就是不是由指定的aarch64-himix100-linux-gcc
# 编译生成的动态库文件，最终，在make opencv_make过程中会产生问题5
# 在cmake中添加-D BUILD_ZLIB=ON可解决该问题，或者选用solution方法解决
--     JPEG:                        /....../anaconda2/lib/libjpeg.so (ver 90)
# 和Zlib的描述类似
# 可采用solution方法解决
--     WEBP:                        build (ver encoder: 0x020e)
--     PNG:                         build (ver 1.6.34)
--     TIFF:                        /home/q7/anaconda2/lib/libtiff.so (ver 42 / 4.0.10)
# 和Zlib的描述类似
# 在cmake中添加-D BUILD_TIFF=ON可解决该问题，或者选用solution方法解决
--     JPEG 2000:                   build (ver 1.900.1)
--     OpenEXR:                     build (ver 1.7.1)
--     HDR:                         YES
--     SUNRASTER:                   YES
--     PXM:                         YES
.........
```
[solution](#jump)


```shell
........
--   GUI: 
--     VTK support:                 NO                          # GTK选项正确关闭
-- 
--   Media I/O: 
--     ZLib:                        build (ver 1.2.11)
--     JPEG:                        libjpeg-turbo (ver 1.5.3-62)
--     WEBP:                        build (ver encoder: 0x020e)
--     PNG:                         build (ver 1.6.34)
--     TIFF:                        build (ver 42 - 4.0.10)
--     JPEG 2000:                   build (ver 1.900.1)
--     OpenEXR:                     build (ver 1.7.1)
--     HDR:                         YES
--     SUNRASTER:                   YES
--     PXM:                         YES
........
```

### Problem

#### 问题1

该问题一般为`cmake-gui`方式进行`cmake`配置带来的问题

```shell
../../lib/libopencv_core.so: undefined reference to `pthread_mutexattr_destroy'
../../lib/libopencv_core.so: undefined reference to `pthread_once'
../../lib/libopencv_core.so: undefined reference to `pthread_spin_lock'
../../lib/libopencv_core.so: undefined reference to `pthread_spin_unlock'
../../lib/libopencv_core.so: undefined reference to `pthread_spin_init'
../../lib/libopencv_core.so: undefined reference to `pthread_spin_trylock'
../../lib/libopencv_core.so: undefined reference to `pthread_spin_destroy'
```

没有在cmake的配置过程中配置`CMAKE_EXE_LINKER_FLAGS=-lrt -lpthread`；方法1，在cmake添加该配置，重新cmake；方法2，在build中生成的`CMakeCache.txt`中，修改`CMAKE_EXE_LINKER_FLAGS:STRING=`为`CMAKE_EXE_LINKER_FLAGS:STRING=-lpthread -ldl -lrt `；任选方法1、2其1即可，之后进行`make opencv_make`

#### 问题2

该问题一般为`cmake-gui`方式进行`cmake`配置带来的问题

```
common.cc:52:2:error: #error "No suitable threading library available."
```

进入`.../opencv-3.4.2/3rdparty/protobuf/src/google/protobuf/stubs`，在`common.cc`文件中，添加`#define HAVE_PTHREAD`

   #### 问题3

   ```shell
   /opt/hisi-linux/x86-arm/aarch64-himix100-linux/host_bin/../lib/gcc/aarch64-linux-gnu/6.3.0/../../../../aarch64-linux-gnu/bin/ld: 找不到 -lgtk-x11-2.0
   /opt/hisi-linux/x86-arm/aarch64-himix100-linux/host_bin/../lib/gcc/aarch64-linux-gnu/6.3.0/../../../../aarch64-linux-gnu/bin/ld: 找不到 -lgdk-x11-2.0
   /opt/hisi-linux/x86-arm/aarch64-himix100-linux/host_bin/../lib/gcc/aarch64-linux-gnu/6.3.0/../../../../aarch64-linux-gnu/bin/ld: 找不到 -lpangocairo-1.0
   /opt/hisi-linux/x86-arm/aarch64-himix100-linux/host_bin/../lib/gcc/aarch64-linux-gnu/6.3.0/../../../../aarch64-linux-gnu/bin/ld: 找不到 -latk-1.0
   /opt/hisi-linux/x86-arm/aarch64-himix100-linux/host_bin/../lib/gcc/aarch64-linux-gnu/6.3.0/../../../../aarch64-linux-gnu/bin/ld: 找不到 -lcairo
   /opt/hisi-linux/x86-arm/aarch64-himix100-linux/host_bin/../lib/gcc/aarch64-linux-gnu/6.3.0/../../../../aarch64-linux-gnu/bin/ld: 找不到 -lgdk_pixbuf-2.0
   /opt/hisi-linux/x86-arm/aarch64-himix100-linux/host_bin/../lib/gcc/aarch64-linux-gnu/6.3.0/../../../../aarch64-linux-gnu/bin/ld: 找不到 -lpangoft2-1.0
/opt/hisi-linux/x86-arm/aarch64-himix100-linux/host_bin/../lib/gcc/aarch64-linux-gnu/6.3.0/../../../../aarch64-linux-gnu/bin/ld: 找不到 -lpango-1.0
   /opt/hisi-linux/x86-arm/aarch64-himix100-linux/host_bin/../lib/gcc/aarch64-linux-gnu/6.3.0/../../../../aarch64-linux-gnu/bin/ld: 找不到 -lfontconfig
   /opt/hisi-linux/x86-arm/aarch64-himix100-linux/host_bin/../lib/gcc/aarch64-linux-gnu/6.3.0/../../../../aarch64-linux-gnu/bin/ld: 找不到 -lfreetype
   ```

没有在cmake的配置过程中关闭`GTK`，在cmake命令中添加`-D WITH_GTK=OFF`，重新配置即可

#### 问题4

```shell
../../lib/libopencv_core.so.3.4.2：对‘__TBB_machine_fetchadd4’未定义的引用
```

在`opencv-3.4.2/3rdparty/ittnotify/src/ittnotify/ittnotify_config.h`文件中添加+号的部分

```CQL
#ifdef __INTEL_COMPILER
#define __TBB_machine_fetchadd4(addr, val) __fetchadd4_acq((void *)addr, val)
#else  /* __INTEL_COMPILER */
/* TODO: Add Support for not Intel compilers for IA-64 architecture */
+#define __TBB_machine_fetchadd4(addr, val) __sync_fetch_and_add(addr, val)

#endif /* __INTEL_COMPILER */
```

#### 问题5

```shell
libz.so: 无法添加符号: 文件格式错误
```

<span id='jump'>solution</span>：可暂时将anaconda2/lib/目录下查找到的库文件更换名字，即可解决，其他两个Zlib和TIFF同样可以使用该方法

#### 问题6

```shell
arm_init.c:17:24: fatal error: ../pngpriv.h: No such file or directory
 #include "../pngpriv.h"
```

在拷贝后的`opencv-3.4.1/3rdparty/libpng`目录中，编辑`arm_init.c`和`filter_neon_intrinsics.c`，经`#include "../pngpriv.h"`中的`../`去掉即可。

#### 问题7

```shell
./face: error while loading shared libraries: libopencv_highgui.so.3.4: cannot open shared object file: No such file or directory
```

#### 问题8

```shell
./face: error while loading shared libraries: libopencv_videoio.so.3.4: cannot open shared object file: No such file or directory
```

缺少`libopencv_videoio.so`库文件，`cp /opencv-3.4.2 output path/lib/libopencv_videoio.so* /usr/lib`

#### 问题9

```shell
main 
main    cvLoad 
terminate called after throwing an instance of 'cv::Exception'
  what():  OpenCV(3.4.2) /home/q7/Software/Driver/opencv_install/Hisiopencv/opencv-3.4.2/modules/core/src/persistence_c.cpp:1426: error: (-2:Unspecified error) The node does not represent a user object (unknown type?) in function 'cvRead'

Aborted
```

更换`haarcascade_frontalface_alt2.xml`为`.../opencv-3.4.2/haarcascades_cuda`文件夹下的`haarcascade_frontalface_alt2.xml`

#### 参考4问题

##### 问题1

交叉编译glib执行原有的`./confighure`命令出现下列报错：

```shell
configure: error: Package requirements (libffi >= 3.0.0) were not met:
 
No package 'libffi' found
```
或者

```shell
./.libs/libgobject-2.0.so: undefined reference to `ffi_type_void'
./.libs/libgobject-2.0.so: undefined reference to `ffi_type_double'
./.libs/libgobject-2.0.so: undefined reference to `ffi_prep_cif'
./.libs/libgobject-2.0.so: undefined reference to `ffi_type_sint32'
./.libs/libgobject-2.0.so: undefined reference to `ffi_type_float'
./.libs/libgobject-2.0.so: undefined reference to `ffi_type_uint64'
./.libs/libgobject-2.0.so: undefined reference to `ffi_type_sint64'
./.libs/libgobject-2.0.so: undefined reference to `ffi_type_pointer'
./.libs/libgobject-2.0.so: undefined reference to `ffi_type_uint32'
./.libs/libgobject-2.0.so: undefined reference to `ffi_call'
```

可以在将原有的`./confighure`改为如下的形式（详解见参考5）

```shell
CC=aarch64-himix100-linux-gcc LIBFFI_CFLAGS+=-I/.../libffi-3.0.11/include LIBFFI_LIBS+="-L/.../libffi-3.2.1/output/lib -lffi" ./configure --prefix=/.../glib-2.47.3/output --host=aarch64-himix100-linux --cache-file=glib.cache 
```

同时在`glib-2.47.3`下的三个Makefile中进行改动

```shell
#vim gobject/Makefile
LDFLAGS = -Wl,-rpath-link -Wl,/.../libffi-3.2.1/output/lib64
LIBFFI_CFLAGS = -I/.../libffi-3.2.1/include
LIBFFI_LIBS = -L/.../libffi-3.2.1/output/lib -lffi

#vim tests/gobject/Makefile
LDFLAGS = -Wl,-rpath-link -Wl,/.../libffi-3.2.1/output/lib64
LIBFFI_CFLAGS = -I/.../libffi-3.2.1/include
LIBFFI_LIBS = -L/.../libffi-3.2.1/output/lib -lffi

#vim gio/Makefile
ZLIB_CFLAGS = -I/.../zlib-1.2.11/output/include
ZLIB_LIBS = -L/.../zlib-1.2.11/output/lib -lz      # 解决问题2
LDFLAGS = -Wl,-rpath-link -Wl,/.../zlib-1.2.11/output/lib -Wl,-rpath-link -Wl,/.../libffi-3.2.1/output/lib64
```

##### 问题2

```shell
./.libs/libgio-2.0.so: undefined reference to `inflate'
./.libs/libgio-2.0.so: undefined reference to `deflateReset'
./.libs/libgio-2.0.so: undefined reference to `deflate'
./.libs/libgio-2.0.so: undefined reference to `deflateInit2_'
./.libs/libgio-2.0.so: undefined reference to `deflateSetHeader'
./.libs/libgio-2.0.so: undefined reference to `deflateInit_'
./.libs/libgio-2.0.so: undefined reference to `inflateEnd'
./.libs/libgio-2.0.so: undefined reference to `inflateGetHeader'
./.libs/libgio-2.0.so: undefined reference to `deflateEnd'
./.libs/libgio-2.0.so: undefined reference to `inflateInit_'
./.libs/libgio-2.0.so: undefined reference to `inflateInit2_'
./.libs/libgio-2.0.so: undefined reference to `inflateReset'
```

参考问题1中`glib-2.47.3`下三个Makefile的改动

##### 问题3

```shell
../../gobject/gclosure.c:28:17: fatal error: ffi.h: No such file or directory
#include <ffi.h>
```

这是libffi安装的时候将它的头文件安装在lib目录下，如下解决（参照参考6）：

```shell
cd /.../glib-2.47.3/gobject
ln -s ../libffi-3.2.1/output/lib/libffi-3.2.1/include/ffi.h ffi.h
ln -s ../libffi-3.2.1/output/lib/libffi-3.2.1/include/ffitarget.h ffitarget.h
```

## 参考

[参考1]:https://blog.csdn.net/zhenglie110/article/details/87933483	"海思3559上移植OpenCV3.4.1 (四)在3559A上的适配"
[参考2]:https://blog.csdn.net/zhenglie110/article/details/83009675	"海思3559上移植OpenCV3.4.1 (一) 代码移植"
[参考3]:https://blog.csdn.net/jacke121/article/details/56038046	"cmake-gui方式出现的问题"
[参考4]:https://blog.csdn.net/Will_young111/article/details/98851351#t11	"opencv3.2.0移植Hi3559完整总结版"
[参考5]:https://stackoverflow.com/questions/12494072/compiling-glib-with-non-standard-paths/12494589	"compiling glib with non-standard paths"
[参考6]:https://www.cnblogs.com/danxi/p/6382631.html	"那些年，交叉编译那些事"
[参考7]:https://blog.csdn.net/zhenglie110/article/details/83039701	"海思3559上移植OpenCV3.4.1 (二) 人脸检测"
[参考8]:http://bbs.ebaina.com/forum.php?mod=viewthread&tid=4858&highlight=opencv	"[分享] 海思移植opencv+人脸检测+速度优化"
[参考9]:https://blog.csdn.net/qq_40421682/article/details/100138214	"海思Hi3519A开发（4.移植OpenCV4.0.1到Hi3519A开发板）"