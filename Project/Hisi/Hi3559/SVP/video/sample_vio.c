
#ifdef __cplusplus      //__cplusplus是C++中定义的一个宏，如果这个宏被定义，说明这个程序是C++程序
#if __cplusplus         //那么如果C++程序要调用C实现的函数或库等，则需要使用extern "C"{ 
extern "C" {            //这是实现C++调用C库或函数的一种手段，原因是因为C++和C语言的编译器不同导致
#endif
#endif /* End of #ifdef __cplusplus */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include "sample_comm.h"


/******************************************************************************
* function : show usage
******************************************************************************/
HI_VOID SAMPLE_VIO_VoInterface_Usage(HI_VOID)
{
    printf("intf:\n");
    printf("\t 0) vo HDMI output, default.\n");
    printf("\t 1) vo BT1120 output.\n");
}

void SAMPLE_VIO_Usage(char* sPrgNm)
{
    printf("Usage : %s <index> <intf>\n", sPrgNm);
    printf("index:\n");
    printf("\t 0)parallel SDR8     VI - VPSS - VO - HDMI.      Embeded isp, phychn channel preview.\n");
    printf("\t 1)online   SDR8     VI - VPSS - VO - HDMI.      2 pipe, Embeded isp, phychn channel preview.\n");
    printf("\t 2)offline  SDR8     VI - VPSS - VO - HDMI.      4 pipe, Embeded isp, phychn channel preview.\n");
    printf("\t 3)online  WDR+HDR10 VI - VPSS - VO - HDMI.      Embeded isp, phychn channel preview.\n");
    printf("\t 4)online   SDR8     VI - VO - HDMI.             Embeded isp, LDC+ROTATE.\n");
    printf("\t 5)online   SDR8     VI - VO - HDMI.             Embeded isp, FreeRotation.\n");
    printf("\t 6)online   SDR8     VI - VO - HDMI.             Embeded isp, LDC+SPREAD.\n");

    printf("intf:\n");
    printf("\t 0) vo HDMI output, default.\n");
    printf("\t 1) vo BT1120 output.\n");
    return;
}


/******************************************************************************
* function : to process abnormal case
******************************************************************************/
void SAMPLE_VIO_HandleSig(HI_S32 signo)
{
    if (SIGINT == signo || SIGTERM == signo)
    {
        SAMPLE_COMM_All_ISP_Stop();
        SAMPLE_COMM_SYS_Exit();
        printf("\033[0;31mprogram termination abnormally!\033[0;39m\n");
    }
    exit(-1);
}

HI_S32 SAMPLE_VIO_StartViVo(SAMPLE_VI_CONFIG_S* pstViConfig, SAMPLE_VO_CONFIG_S* pstVoConfig)
{
    HI_S32 s32Ret;

    s32Ret = SAMPLE_COMM_VI_StartVi(pstViConfig);

    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("start vi failed!\n");
        return s32Ret;
    }

    s32Ret = SAMPLE_COMM_VO_StartVO(pstVoConfig);

    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_VIO start VO failed with %#x!\n", s32Ret);
        goto EXIT;
    }

    return s32Ret;

EXIT:
    SAMPLE_COMM_VI_StopVi(pstViConfig);

    return s32Ret;
}

HI_S32 SAMPLE_VIO_StopViVo(SAMPLE_VI_CONFIG_S* pstViConfig, SAMPLE_VO_CONFIG_S* pstVoConfig)
{
    SAMPLE_COMM_VO_StopVO(pstVoConfig);

    SAMPLE_COMM_VI_StopVi(pstViConfig);

    return HI_SUCCESS;
}



HI_S32 SAMPLE_VIO_8K30_PARALLEL(VO_INTF_TYPE_E enVoIntfType)//海思源码中，枚举类型都是以大写字母加下划线构成，并以E结尾，结构体是以S结尾
{
    HI_S32                  s32Ret              = HI_SUCCESS;//HI_S32为int
    VI_DEV                  ViDev0              = 0;//VI_DEV为int        设置输入设备标号
    VI_PIPE                 ViPipe0             = 0;//VI_PIPE为int
    VI_CHN                  ViChn               = 0;//VI_CHN为int
    HI_S32                  s32ViCnt            = 1;
    VPSS_GRP                VpssGrp0            = 0;//VPSS_GRP为int
    VPSS_CHN                VpssChn[4]          = {VPSS_CHN0, VPSS_CHN1, VPSS_CHN2, VPSS_CHN3};//VPSS_CHN为int  定义 VPSS 通道号 取值范围:[0, VPSS_MAX_PHY_CHN_NUM)
    VPSS_GRP_ATTR_S         stVpssGrpAttr       = {0};
    //VPSS GRP属性:(定义 VPSS GROUP 属性)
    // 1.Width of source image(输入图像宽度) 2.Height of source image(输入图像高度) 3.Pixel format of source image 4.DynamicRange of source image 5.Grp frame rate contrl(组帧率)
    // 6.NR enable(布尔值)Hi3559AV100 此参数无效 7.NR attr(包含NR type、Reference frame compress mode、NR motion compensate mode)Hi3559AV100 此参数无效
    VPSS_CHN_ATTR_S         stVpssChnAttr[VPSS_MAX_PHY_CHN_NUM];
    //VPSS channel属性：(定义 VPSS 物理通道的属性)
    // 1.Vpss channel's work mode(User/Auto) 2.Width of target image(目标图像宽度,要求 2 对齐) 3.Height of target image 4.Video format of target image 5.Pixel format of target image
    //6.DynamicRange of target image 7.Compression mode of the output 8.Frame rate control info 9.Mirror enable(水平镜像使能) 10.Flip enable(垂直翻转使能)
    //11.Range: [0, 8]; User get list depth. (用户获取通道图像的队列长度，取值范围:[0, 8])12.Aspect Ratio info(幅形比参数)
    HI_BOOL                 abChnEnable[VPSS_MAX_PHY_CHN_NUM] = {0};//HI_BOOL为布尔值:HI_FALSE = 0  HI_TRUE  = 1
    VO_DEV                  VoDev               = SAMPLE_VO_DEV_DHD0;//VO_DEV为int  SAMPLE_VO_DEV_DHD0为0
    VO_CHN                  VoChn               = 0;//VO_CHN为int
    VO_INTF_SYNC_E          g_enIntfSync        = VO_OUTPUT_3840x2160_30;//输出视频设置
    HI_U32                  g_u32DisBufLen      = 3;//HI_U32为unsigned int
    PIC_SIZE_E              enPicSize           = PIC_3840x2160;//设置picture大小
    WDR_MODE_E              enWDRMode           = WDR_MODE_NONE;//宽动态范围：线性模式
    DYNAMIC_RANGE_E         enDynamicRange      = DYNAMIC_RANGE_SDR8;//动态范围:  DYNAMIC_RANGE_SDR8 = 0
    PIXEL_FORMAT_E          enPixFormat         = PIXEL_FORMAT_YVU_SEMIPLANAR_420;//pixel format
    VIDEO_FORMAT_E          enVideoFormat       = VIDEO_FORMAT_LINEAR;//video format
    COMPRESS_MODE_E         enCompressMode      = COMPRESS_MODE_NONE;//压缩模式
    VI_VPSS_MODE_E          enMastPipeMode      = VI_PARALLEL_VPSS_PARALLEL;//VI和VPSS模式：offline  online parallel
    SIZE_S                  stSize;//定义大小信息结构体
    HI_U32                  u32BlkSize;//unsigned int
    VB_CONFIG_S             stVbConf;
    //VB设置（定义视频缓存池属性结构体）：
    // 1.u32MaxPoolCnt（整个系统中可容纳的缓存池个数）
    // 2.VB_POOL_CONFIG_S  astCommPool[VB_MAX_COMM_POOLS](公共缓存池属性结构体)
    //      1.u64BlkSize     缓存块大小,以 Byte 位单位
    //      2.u32BlkCnt      每个缓存池的缓存块个数,取值范围:(0, 10240]
    //      3.enRemapMode    VB 的内核态虚拟地址映射模式
    //      4.acMmzName[MAX_MMZ_NAME_LEN])     当前缓存池从哪个 MMZ 区域分配内存
    //注意：u32BlkSize 等于 0 或 u32BlkCnt 等于 0,则对应的缓存池不会被创建。建议整个结构体先 memset 为 0 再按需赋值
    SAMPLE_VI_CONFIG_S      stViConfig;
    //1.SAMPLE_VI_INFO_S    astViInfo[VI_MAX_DEV_NUM];
    //    1.SAMPLE_SENSOR_INFO_S stSnsInfo
    //           SAMPLE_SNS_TYPE_E   enSnsType;
    //           int              s32SnsId;
    //           int              s32BusId;
    //           combo_dev_t(unsigned int)  MipiDev;    MIPI 设备号  取值范围:[0, MIPI_RX_MAX_DEV_NUM)
    //    2.SAMPLE_DEV_INFO_S(ViDev enWDRMode)
    //           int      ViDev;
    //           WDR_MODE_E  enWDRMode;
    //    3.SAMPLE_PIPE_INFO_S -->1.VI_Pipe[int](输入Pipe) aPipe[WDR_MAX_PIPE_NUM] 2.VI_VPSS_MODE_E  enMastPipeMode
    //    4.SAMPLE_CHN_INFO_S  -->1.int ViChn 2.enPixFormat 3.enDynamicRange 4.enVideoFormat 5.enCompressMode
    //    5.SAMPLE_SNAP_INFO_S -->
    //           1.HI_BOOL  bDoublePipe;
    //           2.VI_PIPE    VideoPipe;
    //           3.VI_PIPE    SnapPipe;
    //           4.VI_VPSS_MODE_E  enVideoPipeMode;
    //           5.VI_VPSS_MODE_E  enSnapPipeMode;
    //2.int as32WorkingViId[VI_MAX_DEV_NUM]
    //3.int s32WorkingViNum
    SAMPLE_VO_CONFIG_S      stVoConfig;
    //    /* for device */
    //    int                  VoDev;
    //    VO_INTF_TYPE_E(unsigned int)      enVoIntfType;Vo接口类型
    //    VO_INTF_SYNC_E(输出视频设置)        enIntfSync;  Vo接口时序类型
    //    PIC_SIZE_E           enPicSize;
    //    unsigned int         u32BgColor;
    //
    //    /* for layer */
    //    PIXEL_FORMAT_E          enPixFormat;
    //    RECT_S                  stDispRect;//输出矩形：1.x 2.y 3.u32Width; 4.u32Height;
    //    SIZE_S                  stImageSize;//1.u32Width 2.u32Height
    //    VO_PART_MODE_E          enVoPartMode; -->VO_PART_MODE_SINGLE = 0, /* single partition, which use software to make multi-picture in one hardware cell */
    //                                             VO_PART_MODE_MULTI = 1, /* muliti partition, each partition is a hardware cell */
    //                                             VO_PART_MODE_BUTT
    //    HI_U32                  u32DisBufLen;
    //    DYNAMIC_RANGE_E         enDstDynamicRange;
    //
    //    /* for chnnel */
    //    SAMPLE_VO_MODE_E        enVoMode; -->1.VO_MODE_1MUX  ......



    /************************************************
    step 1:  Get all sensors information, need one vi
        ,and need one mipi --
    *************************************************/
    SAMPLE_COMM_VI_GetSensorInfo(&stViConfig);//HI_VOID(void) SAMPLE_COMM_VI_GetSensorInfo(SAMPLE_VI_CONFIG_S* pstViConfig);
    stViConfig.s32WorkingViNum                           = s32ViCnt;

    stViConfig.as32WorkingViId[0]                        = 0;
    stViConfig.astViInfo[0].stSnsInfo.MipiDev            = SAMPLE_COMM_VI_GetComboDevBySensor(stViConfig.astViInfo[0].stSnsInfo.enSnsType, 8);
    //combo_dev_t SAMPLE_COMM_VI_GetComboDevBySensor(SAMPLE_SNS_TYPE_E enMode, HI_S32 s32SnsIdx);
    stViConfig.astViInfo[0].stSnsInfo.s32BusId           = 0;

    stViConfig.astViInfo[0].stDevInfo.ViDev              = ViDev0;
    stViConfig.astViInfo[0].stDevInfo.enWDRMode          = enWDRMode;

    stViConfig.astViInfo[0].stPipeInfo.enMastPipeMode    = enMastPipeMode;
    stViConfig.astViInfo[0].stPipeInfo.aPipe[0]          = ViPipe0;
    stViConfig.astViInfo[0].stPipeInfo.aPipe[1]          = -1;
    stViConfig.astViInfo[0].stPipeInfo.aPipe[2]          = -1;
    stViConfig.astViInfo[0].stPipeInfo.aPipe[3]          = -1;

    stViConfig.astViInfo[0].stChnInfo.ViChn              = ViChn;
    stViConfig.astViInfo[0].stChnInfo.enPixFormat        = enPixFormat;
    stViConfig.astViInfo[0].stChnInfo.enDynamicRange     = enDynamicRange;
    stViConfig.astViInfo[0].stChnInfo.enVideoFormat      = enVideoFormat;
    stViConfig.astViInfo[0].stChnInfo.enCompressMode     = enCompressMode;

    /************************************************
    step 2:  Get  input size
    *************************************************/
    s32Ret = SAMPLE_COMM_VI_GetSizeBySensor(stViConfig.astViInfo[0].stSnsInfo.enSnsType, &enPicSize);
//HI_S32 SAMPLE_COMM_VI_GetSizeBySensor(SAMPLE_SNS_TYPE_E enMode, PIC_SIZE_E* penSize);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VI_GetSizeBySensor failed with %d!\n", s32Ret);
        return s32Ret;
    }

    s32Ret = SAMPLE_COMM_SYS_GetPicSize(enPicSize, &stSize);
//HI_S32 SAMPLE_COMM_SYS_GetPicSize(PIC_SIZE_E enPicSize, SIZE_S* pstSize);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_SYS_GetPicSize failed with %d!\n", s32Ret);
        return s32Ret;
    }

    /************************************************
    step3:  Init SYS and common VB
    *************************************************/
    memset_s(&stVbConf, sizeof(VB_CONFIG_S), 0, sizeof(VB_CONFIG_S));//memset_s函数将c的值（转换为无符号字符）复制到dest所指向的对象的每个第一个计数字符中。
    //errno_t memset_s(void* dest, size_t destMax, int c, size_t count);
    stVbConf.u32MaxPoolCnt              = 2;

    u32BlkSize = COMMON_GetPicBufferSize(stSize.u32Width, stSize.u32Height, SAMPLE_PIXEL_FORMAT, DATA_BITWIDTH_10, COMPRESS_MODE_SEG, DEFAULT_ALIGN);
    //COMMON_GetPicBufferSize  一般 linear 格式的 YUV 缓存池
    stVbConf.astCommPool[0].u64BlkSize  = u32BlkSize;
    stVbConf.astCommPool[0].u32BlkCnt   = 10;

    u32BlkSize = VI_GetRawBufferSize(stSize.u32Width, stSize.u32Height, PIXEL_FORMAT_RGB_BAYER_16BPP, COMPRESS_MODE_NONE, DEFAULT_ALIGN);
    //VI_GetRawBufferSize   VI 写出的 Raw 数据缓存池
    stVbConf.astCommPool[1].u64BlkSize  = u32BlkSize;
    stVbConf.astCommPool[1].u32BlkCnt   = 4;

    s32Ret = SAMPLE_COMM_SYS_Init(&stVbConf);

    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("system init failed with %d!\n", s32Ret);
        goto EXIT;
    }

    s32Ret = SAMPLE_COMM_VI_SetParam(&stViConfig);

    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VI_SetParam failed with %d!\n", s32Ret);
        goto EXIT;
    }


    /************************************************
    step 4: start VI
    *************************************************/
    s32Ret = SAMPLE_COMM_VI_StartVi(&stViConfig);

    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VI_StartVi failed with %d!\n", s32Ret);
        goto EXIT3;
    }

    /************************************************
    step 5: start VPSS, need one grp
    *************************************************/
    stVpssGrpAttr.u32MaxW                        = stSize.u32Width;//设置VPSS grp属性    source
    stVpssGrpAttr.u32MaxH                        = stSize.u32Height;//grp的宽度和高度
    stVpssGrpAttr.enPixelFormat                  = enPixFormat;//grp像素的格式
    stVpssGrpAttr.enDynamicRange                 = enDynamicRange;//grp动态范围
    stVpssGrpAttr.stFrameRate.s32SrcFrameRate    = -1;//帧率:source frame rate原始帧率 即输入帧率 取值范围-1 或[1, 240]
    stVpssGrpAttr.stFrameRate.s32DstFrameRate    = -1;//帧率：dest frame rate目标帧率 即输出帧率  取值范围[-1, s32SrcFrameRate]

    abChnEnable[0]                               = HI_TRUE;//设置VPSS的channel属性       target
    stVpssChnAttr[0].u32Width                    = stSize.u32Width;
    stVpssChnAttr[0].u32Height                   = stSize.u32Height;
    stVpssChnAttr[0].enChnMode                   = VPSS_CHN_MODE_USER;
    stVpssChnAttr[0].enCompressMode              = enCompressMode;
    stVpssChnAttr[0].enDynamicRange              = enDynamicRange;
    stVpssChnAttr[0].enPixelFormat               = enPixFormat;
    stVpssChnAttr[0].enVideoFormat               = enVideoFormat;
    stVpssChnAttr[0].stFrameRate.s32SrcFrameRate = -1;
    stVpssChnAttr[0].stFrameRate.s32DstFrameRate = -1;
    stVpssChnAttr[0].u32Depth                    = 1;
    stVpssChnAttr[0].bMirror                     = HI_FALSE;
    stVpssChnAttr[0].bFlip                       = HI_FALSE;
    stVpssChnAttr[0].stAspectRatio.enMode        = ASPECT_RATIO_NONE;

    s32Ret = SAMPLE_COMM_VPSS_Start(VpssGrp0, abChnEnable, &stVpssGrpAttr, stVpssChnAttr);
//HI_S32 SAMPLE_COMM_VPSS_Start(VPSS_GRP VpssGrp, HI_BOOL* pabChnEnable, VPSS_GRP_ATTR_S* pstVpssGrpAttr, VPSS_CHN_ATTR_S* pastVpssChnAttr);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VPSS_Start Grp0 failed with %d!\n", s32Ret);
        goto EXIT2;
    }

    /************************************************
    step 6:  VI bind VPSS, for total parallel, no need bind
    *************************************************/

    /************************************************
    step 7:  start V0
    *************************************************/
    SAMPLE_COMM_VO_GetDefConfig(&stVoConfig);
    stVoConfig.VoDev                                    = VoDev;
    stVoConfig.enVoIntfType                             = enVoIntfType;
    stVoConfig.enIntfSync                               = g_enIntfSync;
    stVoConfig.enPicSize                                = enPicSize;
    stVoConfig.u32DisBufLen                             = g_u32DisBufLen;
    stVoConfig.enDstDynamicRange                        = enDynamicRange;
    stVoConfig.enVoMode                                 = VO_MODE_1MUX;

    s32Ret = SAMPLE_COMM_VO_StartVO(&stVoConfig);

    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VO_StartVO failed with %d!\n", s32Ret);
        goto EXIT1;
    }
    /************************************************
    step 8:  VI bind VPSS bind VO
    *************************************************/
    s32Ret = SAMPLE_COMM_VI_Bind_VPSS(ViPipe0, ViChn, VpssGrp0);//vi bind vpss
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VI_Bind_VPSS failed with %d!\n", s32Ret);
        goto EXIT1;
    }

    s32Ret = SAMPLE_COMM_VPSS_Bind_VO(VpssGrp0, VpssChn[0], stVoConfig.VoDev, VoChn);//vpss bind Vo

    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VPSS_Bind_VO Grp0 failed with %d!\n", s32Ret);
        goto EXIT0;
    }

    PAUSE();

    SAMPLE_COMM_VPSS_UnBind_VO(VpssGrp0, VpssChn[0], stVoConfig.VoDev, VoChn);//vpss unbind vo
EXIT0:
    SAMPLE_COMM_VI_UnBind_VPSS(ViPipe0, ViChn, VpssGrp0);//vi unbind vpss
EXIT1:
    SAMPLE_COMM_VO_StopVO(&stVoConfig);//vo stop
EXIT2:
    SAMPLE_COMM_VPSS_Stop(VpssGrp0, abChnEnable);//vpss stop
EXIT3:
    SAMPLE_COMM_VI_StopVi(&stViConfig);//vi stop
EXIT:
    SAMPLE_COMM_SYS_Exit();//sys exit

    return s32Ret;
}

HI_S32 SAMPLE_VIO_2X4K60_TotalOnline(VO_INTF_TYPE_E enVoIntfType)
{
    HI_S32                  s32Ret              = HI_SUCCESS;
    VI_DEV                  ViDev0              = 0;
    VI_DEV                  ViDev4              = 4;
    VI_PIPE                 ViPipe0             = 0;
    VI_PIPE                 ViPipe4             = 4;
    VI_CHN                  ViChn               = 0;
    HI_S32                  s32ViCnt            = 2;//设置输入数
    VPSS_GRP                VpssGrp0            = 0;
    VPSS_GRP                VpssGrp1            = 4;
    VPSS_CHN                VpssChn[4]          = {VPSS_CHN0, VPSS_CHN1, VPSS_CHN2, VPSS_CHN3};
    VPSS_GRP_ATTR_S         stVpssGrpAttr       = {0};
    VPSS_CHN_ATTR_S         stVpssChnAttr[VPSS_MAX_PHY_CHN_NUM];
    HI_BOOL                 abChnEnable[VPSS_MAX_PHY_CHN_NUM] = {0};
    VO_DEV                  VoDev               = SAMPLE_VO_DEV_DHD0;
    VO_CHN                  VoChn[2]            = {0, 1};//输出通道
    VO_INTF_SYNC_E          g_enIntfSync        = VO_OUTPUT_1080P30;//接口时序同步
    HI_U32                  g_u32DisBufLen      = 3;
    PIC_SIZE_E              enPicSize           = PIC_3840x2160;
    WDR_MODE_E              enWDRMode           = WDR_MODE_NONE;//宽动态范围：线性模式
    DYNAMIC_RANGE_E         enDynamicRange      = DYNAMIC_RANGE_SDR8;//8bit数据的标准动态范围
    PIXEL_FORMAT_E          enPixFormat         = PIXEL_FORMAT_YVU_SEMIPLANAR_420;
    VIDEO_FORMAT_E          enVideoFormat       = VIDEO_FORMAT_LINEAR;
    COMPRESS_MODE_E         enCompressMode      = COMPRESS_MODE_NONE;//非压缩的视频格式
    VI_VPSS_MODE_E          enMastPipeMode      = VI_ONLINE_VPSS_ONLINE;
    SIZE_S                  stSize;
    HI_U32                  u32BlkSize;
    VB_CONFIG_S             stVbConf;
    SAMPLE_VI_CONFIG_S      stViConfig;
    SAMPLE_VO_CONFIG_S      stVoConfig;


    /************************************************
    step 1:  Get all sensors information, need two vi
        ,and need two mipi --
    *************************************************/
    SAMPLE_COMM_VI_GetSensorInfo(&stViConfig);
    stViConfig.s32WorkingViNum                           = s32ViCnt;

    stViConfig.as32WorkingViId[0]                        = 0;
    stViConfig.astViInfo[0].stSnsInfo.MipiDev            = SAMPLE_COMM_VI_GetComboDevBySensor(stViConfig.astViInfo[0].stSnsInfo.enSnsType, 8);
    stViConfig.astViInfo[0].stSnsInfo.s32BusId           = 0;

    stViConfig.astViInfo[0].stDevInfo.ViDev              = ViDev0;
    stViConfig.astViInfo[0].stDevInfo.enWDRMode          = enWDRMode;

    stViConfig.astViInfo[0].stPipeInfo.enMastPipeMode    = enMastPipeMode;
    stViConfig.astViInfo[0].stPipeInfo.aPipe[0]          = ViPipe0;
    stViConfig.astViInfo[0].stPipeInfo.aPipe[1]          = -1;
    stViConfig.astViInfo[0].stPipeInfo.aPipe[2]          = -1;
    stViConfig.astViInfo[0].stPipeInfo.aPipe[3]          = -1;

    stViConfig.astViInfo[0].stChnInfo.ViChn              = ViChn;
    stViConfig.astViInfo[0].stChnInfo.enPixFormat        = enPixFormat;
    stViConfig.astViInfo[0].stChnInfo.enDynamicRange     = enDynamicRange;
    stViConfig.astViInfo[0].stChnInfo.enVideoFormat      = enVideoFormat;
    stViConfig.astViInfo[0].stChnInfo.enCompressMode     = enCompressMode;
//Hi3559AV100 只有 VI PIPE0 和 PIPE4 支持 VI_ONLINE_VPSS_ONLINE和VI_ONLINE_VPSS_OFFLINE
    stViConfig.as32WorkingViId[1]                        = 1;
    stViConfig.astViInfo[1].stSnsInfo.MipiDev            = SAMPLE_COMM_VI_GetComboDevBySensor(stViConfig.astViInfo[0].stSnsInfo.enSnsType, 2);
    stViConfig.astViInfo[1].stSnsInfo.s32BusId           = 2;

    stViConfig.astViInfo[1].stDevInfo.ViDev              = ViDev4;
    stViConfig.astViInfo[1].stDevInfo.enWDRMode          = enWDRMode;

    stViConfig.astViInfo[1].stPipeInfo.enMastPipeMode    = enMastPipeMode;
    stViConfig.astViInfo[1].stPipeInfo.aPipe[0]          = ViPipe4;
    stViConfig.astViInfo[1].stPipeInfo.aPipe[1]          = -1;
    stViConfig.astViInfo[1].stPipeInfo.aPipe[2]          = -1;
    stViConfig.astViInfo[1].stPipeInfo.aPipe[3]          = -1;

    stViConfig.astViInfo[1].stChnInfo.ViChn              = ViChn;
    stViConfig.astViInfo[1].stChnInfo.enPixFormat        = enPixFormat;
    stViConfig.astViInfo[1].stChnInfo.enDynamicRange     = enDynamicRange;
    stViConfig.astViInfo[1].stChnInfo.enVideoFormat      = enVideoFormat;
    stViConfig.astViInfo[1].stChnInfo.enCompressMode     = enCompressMode;
    /************************************************
    step 2:  Get  input size
    *************************************************/
    s32Ret = SAMPLE_COMM_VI_GetSizeBySensor(stViConfig.astViInfo[0].stSnsInfo.enSnsType, &enPicSize);

    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VI_GetSizeBySensor failed with %d!\n", s32Ret);
        return s32Ret;
    }

    s32Ret = SAMPLE_COMM_SYS_GetPicSize(enPicSize, &stSize);

    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_SYS_GetPicSize failed with %d!\n", s32Ret);
        return s32Ret;
    }

    /************************************************
    step3:  Init SYS and common VB
    *************************************************/
    memset_s(&stVbConf, sizeof(VB_CONFIG_S), 0, sizeof(VB_CONFIG_S));
    stVbConf.u32MaxPoolCnt              = 2;

    u32BlkSize = COMMON_GetPicBufferSize(stSize.u32Width, stSize.u32Height, SAMPLE_PIXEL_FORMAT, DATA_BITWIDTH_10, COMPRESS_MODE_NONE, DEFAULT_ALIGN);
    stVbConf.astCommPool[0].u64BlkSize  = u32BlkSize;
    stVbConf.astCommPool[0].u32BlkCnt   = 10;

    u32BlkSize = VI_GetRawBufferSize(stSize.u32Width, stSize.u32Height, PIXEL_FORMAT_RGB_BAYER_16BPP, COMPRESS_MODE_NONE, DEFAULT_ALIGN);
    stVbConf.astCommPool[1].u64BlkSize  = u32BlkSize;
    stVbConf.astCommPool[1].u32BlkCnt   = 4;

    s32Ret = SAMPLE_COMM_SYS_Init(&stVbConf);

    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("system init failed with %d!\n", s32Ret);
        goto EXIT;
    }

    s32Ret = SAMPLE_COMM_VI_SetParam(&stViConfig);

    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VI_SetParam failed with %d!\n", s32Ret);
        goto EXIT;
    }


    /************************************************
    step 4: start VI
    *************************************************/
    s32Ret = SAMPLE_COMM_VI_StartVi(&stViConfig);

    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VI_StartVi failed with %d!\n", s32Ret);
        goto EXIT5;
    }

    /************************************************
    step 5: start VPSS, need two grp
    *************************************************/
    stVpssGrpAttr.u32MaxW                        = stSize.u32Width;
    stVpssGrpAttr.u32MaxH                        = stSize.u32Height;
    stVpssGrpAttr.enPixelFormat                  = enPixFormat;
    stVpssGrpAttr.enDynamicRange                 = enDynamicRange;
    stVpssGrpAttr.stFrameRate.s32SrcFrameRate    = -1;
    stVpssGrpAttr.stFrameRate.s32DstFrameRate    = -1;

    abChnEnable[0]                               = HI_TRUE;
    stVpssChnAttr[0].u32Width                    = stSize.u32Width;
    stVpssChnAttr[0].u32Height                   = stSize.u32Height;
    stVpssChnAttr[0].enChnMode                   = VPSS_CHN_MODE_USER;
    stVpssChnAttr[0].enCompressMode              = enCompressMode;
    stVpssChnAttr[0].enDynamicRange              = enDynamicRange;
    stVpssChnAttr[0].enPixelFormat               = enPixFormat;
    stVpssChnAttr[0].enVideoFormat               = enVideoFormat;
    stVpssChnAttr[0].stFrameRate.s32SrcFrameRate = -1;
    stVpssChnAttr[0].stFrameRate.s32DstFrameRate = -1;
    stVpssChnAttr[0].u32Depth                    = 0;
    stVpssChnAttr[0].bMirror                     = HI_FALSE;
    stVpssChnAttr[0].bFlip                       = HI_FALSE;
    stVpssChnAttr[0].stAspectRatio.enMode        = ASPECT_RATIO_NONE;//不开启幅形比功能

    s32Ret = SAMPLE_COMM_VPSS_Start(VpssGrp0, abChnEnable, &stVpssGrpAttr, stVpssChnAttr);

    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VPSS_Start Grp0 failed with %d!\n", s32Ret);
        goto EXIT4;
    }

    s32Ret = SAMPLE_COMM_VPSS_Start(VpssGrp1, abChnEnable, &stVpssGrpAttr, stVpssChnAttr);

    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VPSS_Start Grp1 failed with %d!\n", s32Ret);
        goto EXIT3;
    }

    /************************************************
    step 6:  VI bind VPSS, for total online, no need bind
    *************************************************/

    /************************************************
    step 7:  start V0
    *************************************************/
    SAMPLE_COMM_VO_GetDefConfig(&stVoConfig);
    stVoConfig.VoDev                                    = VoDev;
    stVoConfig.enVoIntfType                             = enVoIntfType;
    stVoConfig.enIntfSync                               = g_enIntfSync;
    stVoConfig.enPicSize                                = enPicSize;
    stVoConfig.u32DisBufLen                             = g_u32DisBufLen;
    stVoConfig.enDstDynamicRange                        = enDynamicRange;
    stVoConfig.enVoMode                                 = VO_MODE_1MUX;

    s32Ret = SAMPLE_COMM_VO_StartVO(&stVoConfig);

    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VO_StartVO failed with %d!\n", s32Ret);
        goto EXIT2;
    }
    /************************************************
    step 8:  VO bind VPSS
    *************************************************/
    s32Ret = SAMPLE_COMM_VPSS_Bind_VO(VpssGrp0, VpssChn[0], stVoConfig.VoDev, VoChn[0]);

    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VPSS_Bind_VO Grp0 failed with %d!\n", s32Ret);
        goto EXIT2;
    }

//    s32Ret = SAMPLE_COMM_VPSS_Bind_VO(VpssGrp1, VpssChn[0], stVoConfig.VoDev, VoChn[0]);
//
//    if (HI_SUCCESS != s32Ret)
//    {
//        SAMPLE_PRT("SAMPLE_COMM_VPSS_Bind_VO Grp1 failed with %d!\n", s32Ret);
//        goto EXIT1;
//    }

    PAUSE();

//    SAMPLE_COMM_VPSS_UnBind_VO(VpssGrp1, VpssChn[0], stVoConfig.VoDev, VoChn[0]);
//EXIT1:
    SAMPLE_COMM_VPSS_UnBind_VO(VpssGrp0, VpssChn[0], stVoConfig.VoDev, VoChn[0]);//VPSS unbind VO
EXIT2:
    SAMPLE_COMM_VO_StopVO(&stVoConfig);
EXIT3:
    SAMPLE_COMM_VPSS_Stop(VpssGrp1, abChnEnable);
EXIT4:
    SAMPLE_COMM_VPSS_Stop(VpssGrp0, abChnEnable);
EXIT5:
    SAMPLE_COMM_VI_StopVi(&stViConfig);
EXIT:
    SAMPLE_COMM_SYS_Exit();

    return s32Ret;
}

HI_S32 SAMPLE_VIO_4x4K30_WBC(VO_INTF_TYPE_E enVoIntfType)
{
    HI_S32                  s32Ret              = HI_SUCCESS;
    HI_S32                  i                   = 0;
    VI_DEV                  ViDev[4]            = {0, 2, 4, 6};
    VI_PIPE                 ViPipe[4]           = {0, 2, 4, 6};
    VI_CHN                  ViChn               = 0;
    HI_S32                  s32ViCnt            = 4;
    HI_S32                  s32VpssChnCnt       = 4;
    VPSS_GRP                VpssGrp[4]          = {0, 2, 4, 6};
    VPSS_CHN                VpssChn[4]          = {VPSS_CHN0, VPSS_CHN1, VPSS_CHN2, VPSS_CHN3};
    VPSS_GRP_ATTR_S         stVpssGrpAttr       = {0};
    VPSS_CHN_ATTR_S         stVpssChnAttr[VPSS_MAX_PHY_CHN_NUM];
    HI_BOOL                 abChnEnable[VPSS_MAX_PHY_CHN_NUM] = {0};
    VO_DEV                  VoDev               = SAMPLE_VO_DEV_DHD0;
    VO_CHN                  VoChn[4]            = {0, 1, 2, 3};
    VO_INTF_SYNC_E          g_enIntfSync        = VO_OUTPUT_1080P30;
    HI_U32                  g_u32DisBufLen      = 3;
    PIC_SIZE_E              enPicSize           = PIC_3840x2160;
    WDR_MODE_E              enWDRMode           = WDR_MODE_NONE;
    DYNAMIC_RANGE_E         enDynamicRange      = DYNAMIC_RANGE_SDR8;
    PIXEL_FORMAT_E          enPixFormat         = PIXEL_FORMAT_YVU_SEMIPLANAR_420;
    VIDEO_FORMAT_E          enVideoFormat       = VIDEO_FORMAT_LINEAR;
    COMPRESS_MODE_E         enCompressMode      = COMPRESS_MODE_NONE;
    VI_VPSS_MODE_E          enMastPipeMode      = VI_OFFLINE_VPSS_ONLINE;
    SIZE_S                  stSize;
    HI_U32                  u32BlkSize;
    VB_CONFIG_S             stVbConf;
    SAMPLE_VI_CONFIG_S      stViConfig;
    SAMPLE_VO_CONFIG_S      stVoConfig;
    SAMPLE_VO_CONFIG_S      stVoConfig1;
    SAMPLE_VO_WBC_CONFIG    stWbcConfig;//VO 作为数据源发送回写(WBC)数据
    VI_VPSS_MODE_S          stVIVPSSMode;


    /************************************************
    step 1:  Get all sensors information
    *************************************************/
    SAMPLE_COMM_VI_GetSensorInfo(&stViConfig);
    stViConfig.s32WorkingViNum                           = s32ViCnt;

    stViConfig.as32WorkingViId[0]                        = 0;
    stViConfig.astViInfo[0].stSnsInfo.MipiDev            = SAMPLE_COMM_VI_GetComboDevBySensor(stViConfig.astViInfo[0].stSnsInfo.enSnsType, 8);
    stViConfig.astViInfo[0].stSnsInfo.s32BusId           = 0;

    stViConfig.astViInfo[0].stDevInfo.ViDev              = ViDev[0];
    stViConfig.astViInfo[0].stDevInfo.enWDRMode          = enWDRMode;

    stViConfig.astViInfo[0].stPipeInfo.enMastPipeMode    = enMastPipeMode;
    stViConfig.astViInfo[0].stPipeInfo.aPipe[0]          = ViPipe[0];
    stViConfig.astViInfo[0].stPipeInfo.aPipe[1]          = -1;
    stViConfig.astViInfo[0].stPipeInfo.aPipe[2]          = -1;
    stViConfig.astViInfo[0].stPipeInfo.aPipe[3]          = -1;

    stViConfig.astViInfo[0].stChnInfo.ViChn              = ViChn;
    stViConfig.astViInfo[0].stChnInfo.enPixFormat        = enPixFormat;
    stViConfig.astViInfo[0].stChnInfo.enDynamicRange     = enDynamicRange;
    stViConfig.astViInfo[0].stChnInfo.enVideoFormat      = enVideoFormat;
    stViConfig.astViInfo[0].stChnInfo.enCompressMode     = enCompressMode;

    stViConfig.as32WorkingViId[1]                        = 1;
    stViConfig.astViInfo[1].stSnsInfo.MipiDev            = SAMPLE_COMM_VI_GetComboDevBySensor(stViConfig.astViInfo[1].stSnsInfo.enSnsType, 1);
    stViConfig.astViInfo[1].stSnsInfo.s32BusId           = 1;

    stViConfig.astViInfo[1].stDevInfo.ViDev              = ViDev[1];
    stViConfig.astViInfo[1].stDevInfo.enWDRMode          = enWDRMode;

    stViConfig.astViInfo[1].stPipeInfo.enMastPipeMode    = enMastPipeMode;
    stViConfig.astViInfo[1].stPipeInfo.aPipe[0]          = ViPipe[1];
    stViConfig.astViInfo[1].stPipeInfo.aPipe[1]          = -1;
    stViConfig.astViInfo[1].stPipeInfo.aPipe[2]          = -1;
    stViConfig.astViInfo[1].stPipeInfo.aPipe[3]          = -1;

    stViConfig.astViInfo[1].stChnInfo.ViChn              = ViChn;
    stViConfig.astViInfo[1].stChnInfo.enPixFormat        = enPixFormat;
    stViConfig.astViInfo[1].stChnInfo.enDynamicRange     = enDynamicRange;
    stViConfig.astViInfo[1].stChnInfo.enVideoFormat      = enVideoFormat;
    stViConfig.astViInfo[1].stChnInfo.enCompressMode     = enCompressMode;

    stViConfig.as32WorkingViId[2]                        = 2;
    stViConfig.astViInfo[2].stSnsInfo.MipiDev            = SAMPLE_COMM_VI_GetComboDevBySensor(stViConfig.astViInfo[2].stSnsInfo.enSnsType, 2);
    stViConfig.astViInfo[2].stSnsInfo.s32BusId           = 2;

    stViConfig.astViInfo[2].stDevInfo.ViDev              = ViDev[2];
    stViConfig.astViInfo[2].stDevInfo.enWDRMode          = enWDRMode;

    stViConfig.astViInfo[2].stPipeInfo.enMastPipeMode    = enMastPipeMode;
    stViConfig.astViInfo[2].stPipeInfo.aPipe[0]          = ViPipe[2];
    stViConfig.astViInfo[2].stPipeInfo.aPipe[1]          = -1;
    stViConfig.astViInfo[2].stPipeInfo.aPipe[2]          = -1;
    stViConfig.astViInfo[2].stPipeInfo.aPipe[3]          = -1;

    stViConfig.astViInfo[2].stChnInfo.ViChn              = ViChn;
    stViConfig.astViInfo[2].stChnInfo.enPixFormat        = enPixFormat;
    stViConfig.astViInfo[2].stChnInfo.enDynamicRange     = enDynamicRange;
    stViConfig.astViInfo[2].stChnInfo.enVideoFormat      = enVideoFormat;
    stViConfig.astViInfo[2].stChnInfo.enCompressMode     = enCompressMode;

    stViConfig.as32WorkingViId[3]                        = 3;
    stViConfig.astViInfo[3].stSnsInfo.MipiDev            = SAMPLE_COMM_VI_GetComboDevBySensor(stViConfig.astViInfo[3].stSnsInfo.enSnsType, 3);
    stViConfig.astViInfo[3].stSnsInfo.s32BusId           = 3;

    stViConfig.astViInfo[3].stDevInfo.ViDev              = ViDev[3];
    stViConfig.astViInfo[3].stDevInfo.enWDRMode          = enWDRMode;

    stViConfig.astViInfo[3].stPipeInfo.enMastPipeMode    = enMastPipeMode;
    stViConfig.astViInfo[3].stPipeInfo.aPipe[0]          = ViPipe[3];
    stViConfig.astViInfo[3].stPipeInfo.aPipe[1]          = -1;
    stViConfig.astViInfo[3].stPipeInfo.aPipe[2]          = -1;
    stViConfig.astViInfo[3].stPipeInfo.aPipe[3]          = -1;

    stViConfig.astViInfo[3].stChnInfo.ViChn              = ViChn;
    stViConfig.astViInfo[3].stChnInfo.enPixFormat        = enPixFormat;
    stViConfig.astViInfo[3].stChnInfo.enDynamicRange     = enDynamicRange;
    stViConfig.astViInfo[3].stChnInfo.enVideoFormat      = enVideoFormat;
    stViConfig.astViInfo[3].stChnInfo.enCompressMode     = enCompressMode;


    /************************************************
    step 2:  Get  input size
    *************************************************/
    s32Ret = SAMPLE_COMM_VI_GetSizeBySensor(stViConfig.astViInfo[0].stSnsInfo.enSnsType, &enPicSize);

    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VI_GetSizeBySensor failed with %d!\n", s32Ret);
        return s32Ret;
    }

    s32Ret = SAMPLE_COMM_SYS_GetPicSize(enPicSize, &stSize);

    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_SYS_GetPicSize failed with %d!\n", s32Ret);
        return s32Ret;
    }

    /************************************************
    step3:  Init SYS and common VB
    *************************************************/
    memset_s(&stVbConf, sizeof(VB_CONFIG_S), 0, sizeof(VB_CONFIG_S));
    stVbConf.u32MaxPoolCnt              = 2;

    u32BlkSize = COMMON_GetPicBufferSize(stSize.u32Width, stSize.u32Height, SAMPLE_PIXEL_FORMAT, DATA_BITWIDTH_10, COMPRESS_MODE_NONE, DEFAULT_ALIGN);
    stVbConf.astCommPool[0].u64BlkSize  = u32BlkSize;
    stVbConf.astCommPool[0].u32BlkCnt   = 30;

    u32BlkSize = VI_GetRawBufferSize(stSize.u32Width, stSize.u32Height, PIXEL_FORMAT_RGB_BAYER_16BPP, COMPRESS_MODE_NONE, DEFAULT_ALIGN);
    stVbConf.astCommPool[1].u64BlkSize  = u32BlkSize;
    stVbConf.astCommPool[1].u32BlkCnt   = 15;

    s32Ret = SAMPLE_COMM_SYS_Init(&stVbConf);

    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("system init failed with %d!\n", s32Ret);
        goto EXIT;
    }

    for(i = 0;i < VI_MAX_PIPE_NUM;i++)
    {
        stVIVPSSMode.aenMode[i] = VI_OFFLINE_VPSS_ONLINE;
    }

    s32Ret = HI_MPI_SYS_SetVIVPSSMode(&stVIVPSSMode);

    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("HI_MPI_SYS_SetVIVPSSMode failed with %d!\n", s32Ret);
        goto EXIT;
    }

    /************************************************
    step 4: start VI
    *************************************************/
    s32Ret = SAMPLE_COMM_VI_StartVi(&stViConfig);

    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VI_StartVi failed with %d!\n", s32Ret);
        goto EXIT4;
    }

    /************************************************
    step 5: start VPSS
    *************************************************/
    stVpssGrpAttr.u32MaxW                        = stSize.u32Width;
    stVpssGrpAttr.u32MaxH                        = stSize.u32Height;
    stVpssGrpAttr.enPixelFormat                  = enPixFormat;
    stVpssGrpAttr.enDynamicRange                 = enDynamicRange;
    stVpssGrpAttr.stFrameRate.s32SrcFrameRate    = -1;
    stVpssGrpAttr.stFrameRate.s32DstFrameRate    = -1;

    abChnEnable[0]                               = HI_TRUE;
    stVpssChnAttr[0].u32Width                    = stSize.u32Width;
    stVpssChnAttr[0].u32Height                   = stSize.u32Height;
    stVpssChnAttr[0].enChnMode                   = VPSS_CHN_MODE_USER;
    stVpssChnAttr[0].enCompressMode              = enCompressMode;
    stVpssChnAttr[0].enDynamicRange              = enDynamicRange;
    stVpssChnAttr[0].enPixelFormat               = enPixFormat;
    stVpssChnAttr[0].enVideoFormat               = enVideoFormat;
    stVpssChnAttr[0].stFrameRate.s32SrcFrameRate = -1;
    stVpssChnAttr[0].stFrameRate.s32DstFrameRate = -1;
    stVpssChnAttr[0].u32Depth                    = 1;
    stVpssChnAttr[0].bMirror                     = HI_FALSE;
    stVpssChnAttr[0].bFlip                       = HI_FALSE;
    stVpssChnAttr[0].stAspectRatio.enMode        = ASPECT_RATIO_NONE;

    for (i = 0; i < s32VpssChnCnt; i++)
    {
        s32Ret = SAMPLE_COMM_VPSS_Start(VpssGrp[i], abChnEnable, &stVpssGrpAttr, stVpssChnAttr);

        if (HI_SUCCESS != s32Ret)
        {
            SAMPLE_PRT("SAMPLE_COMM_VPSS_Start failed with %d!\n", s32Ret);
            goto EXIT4;
        }
    }

    /************************************************
    step 6:  VI bind VPSS
    *************************************************/
    for (i = 0; i < s32VpssChnCnt; i++)
    {
        s32Ret = SAMPLE_COMM_VI_Bind_VPSS(ViPipe[i], ViChn, VpssGrp[i]);

        if (HI_SUCCESS != s32Ret)
        {
            SAMPLE_PRT("SAMPLE_COMM_VI_Bind_VPSS failed with %d!\n", s32Ret);
            goto EXIT3;
        }
    }

    /************************************************
    step 7:  start V0
    *************************************************/
    SAMPLE_COMM_VO_GetDefConfig(&stVoConfig);
    stVoConfig.VoDev                                    = VoDev;
    stVoConfig.enVoIntfType                             = enVoIntfType;
    stVoConfig.enIntfSync                               = g_enIntfSync;
    stVoConfig.enPicSize                                = enPicSize;
    stVoConfig.u32DisBufLen                             = g_u32DisBufLen;
    stVoConfig.enDstDynamicRange                        = enDynamicRange;
    stVoConfig.enVoMode                                 = VO_MODE_1MUX;

    s32Ret = SAMPLE_COMM_VO_StartVO(&stVoConfig);

    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VO_StartVO failed with %d!\n", s32Ret);
        goto EXIT2;
    }
    /************************************************
    step 8:  VO bind VPSS
    *************************************************/
    //for (i = 0; i < s32VpssChnCnt; i++)
    for (i = 0; i < 1; i++)
    {
        s32Ret = SAMPLE_COMM_VPSS_Bind_VO(VpssGrp[i], VpssChn[0], stVoConfig.VoDev, VoChn[0]);

        if (HI_SUCCESS != s32Ret)
        {
            SAMPLE_PRT("SAMPLE_COMM_VPSS_Bind_VO Grp%d failed with %d!\n", i, s32Ret);
            goto EXIT2;
        }
    }

    /************************************************
    step 9:  start V0 device 1
    *************************************************/
    SAMPLE_COMM_VO_GetDefConfig(&stVoConfig1);
    stVoConfig1.VoDev                                    = SAMPLE_VO_DEV_DHD1;
    stVoConfig1.enVoIntfType                             = VO_INTF_BT1120;
    stVoConfig1.u32BgColor                               = COLOR_RGB_CYN;

    s32Ret = SAMPLE_COMM_VO_StartVO(&stVoConfig1);

    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VO_StartVO failed with %d!\n", s32Ret);
        goto EXIT2;
    }
    /************************************************
    step 10:  start WBC device 0
    *************************************************/
    SAMPLE_COMM_VO_GetDefWBCConfig(&stWbcConfig);
    s32Ret = SAMPLE_COMM_VO_StartWBC(&stWbcConfig);

    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VO_StartWBC failed with %d!\n", s32Ret);
        goto EXIT1;
    }
    /************************************************
    step 11: WBC bind VO
    *************************************************/
    s32Ret = SAMPLE_COMM_VO_Bind_VO(stWbcConfig.VoWbc,0,stVoConfig1.VoDev,0);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VO_Bind_VO failed with %d!\n", s32Ret);
        goto EXIT0;
    }

    PAUSE();

    //for (i = 0; i < s32VpssChnCnt; i++)
    for (i = 0; i < 1; i++)
    {
        SAMPLE_COMM_VPSS_UnBind_VO(VpssGrp[i], VpssChn[0], stVoConfig.VoDev, VoChn[0]);
    }
EXIT0:
    SAMPLE_COMM_VO_StopWBC(&stWbcConfig);
EXIT1:
    SAMPLE_COMM_VO_StopVO(&stVoConfig1);
EXIT2:
    SAMPLE_COMM_VO_StopVO(&stVoConfig);
EXIT3:
    for (i = 0; i < s32VpssChnCnt; i++)
    {
        SAMPLE_COMM_VPSS_Stop(VpssGrp[i], abChnEnable);
    }
EXIT4:
    SAMPLE_COMM_VI_StopVi(&stViConfig);
EXIT:
    SAMPLE_COMM_SYS_Exit();

    return s32Ret;
}



HI_S32 SAMPLE_VIO_StartViVpssVo(SAMPLE_VI_CONFIG_S stViConfig, VPSS_GRP_ATTR_S stVpssGrpAttr, VPSS_CHN_ATTR_S *pastVpssChnAttr, SAMPLE_VO_CONFIG_S stVoConfig)
{
    HI_S32   s32Ret = HI_SUCCESS;

    VI_PIPE  ViPipe  = 0;
    VI_CHN   ViChn   = 0;

    VPSS_GRP VpssGrp = 0;
    VPSS_CHN VpssChn = VPSS_CHN0;
    HI_BOOL  abChnEnable[VPSS_MAX_PHY_CHN_NUM] = {0};

    VO_CHN   VoChn   = 0;

    /*set vi param*/
    s32Ret = SAMPLE_COMM_VI_SetParam(&stViConfig);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("set vi param failed!\n");
        return s32Ret;
    }

    /*start vi*/
    s32Ret = SAMPLE_COMM_VI_StartVi(&stViConfig);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("start vi failed.s32Ret:0x%x !\n", s32Ret);
        return s32Ret;
    }

    /*start vpss*/
    abChnEnable[0] = HI_TRUE;

    s32Ret = SAMPLE_COMM_VPSS_Start(VpssGrp, abChnEnable, &stVpssGrpAttr, pastVpssChnAttr);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("start vpss group failed. s32Ret: 0x%x !\n", s32Ret);
        goto EXIT1;
    }

    /*vi bind vpss*/
    s32Ret = SAMPLE_COMM_VI_Bind_VPSS(ViPipe, ViChn, VpssGrp);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("vi bind vpss failed. s32Ret: 0x%x !\n", s32Ret);
        goto EXIT2;
    }

    /*start vo*/
    s32Ret = SAMPLE_COMM_VO_StartVO(&stVoConfig);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("start vo failed. s32Ret: 0x%x !\n", s32Ret);
        goto EXIT3;
    }

    /*vpss bind vo*/
    s32Ret = SAMPLE_COMM_VPSS_Bind_VO(VpssGrp, VpssChn, stVoConfig.VoDev, VoChn);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("vo bind vpss failed. s32Ret: 0x%x !\n", s32Ret);
        goto EXIT4;
    }

    goto EXIT;

    //SAMPLE_COMM_VPSS_UnBind_VO(VpssGrp, VpssChn, stVoConfig.VoDev, VoChn); /* unreachable */
EXIT4:
    SAMPLE_COMM_VO_StopVO(&stVoConfig);
EXIT3:
    SAMPLE_COMM_VI_UnBind_VPSS(ViPipe, ViChn, VpssGrp);
EXIT2:
    SAMPLE_COMM_VPSS_Stop(VpssGrp, abChnEnable);
EXIT1:
    SAMPLE_COMM_VI_StopVi(&stViConfig);
EXIT:
    return HI_SUCCESS;
}

HI_S32 SAMPLE_VIO_4K30_WDR_HDR10(VO_INTF_TYPE_E enVoIntfType)
{
    HI_S32             s32Ret;

    HI_S32             s32ViCnt       = 1;
    VI_DEV             ViDev          = 0;
    VI_PIPE            ViPipe0        = 0;
    VI_CHN             ViChn          = 0;
    HI_S32             s32WorkSnsId   = 8;
    SAMPLE_VI_CONFIG_S stViConfig;
    combo_dev_t        ComboDev;
    SAMPLE_PIPE_INFO_S stPipeInfo;

    SIZE_S             stSize;
    VB_CONFIG_S        stVbConf;
    PIC_SIZE_E         enPicSize      = PIC_3840x2160;
    HI_U32             u32BlkSize;

    VO_CHN             VoChn          = 0;
    SAMPLE_VO_CONFIG_S stVoConfig;

    WDR_MODE_E         enWDRMode      = WDR_MODE_NONE;
    DYNAMIC_RANGE_E    enDynamicRange = DYNAMIC_RANGE_HDR10;
    PIXEL_FORMAT_E     enPixFormat    = PIXEL_FORMAT_YVU_SEMIPLANAR_420;
    VIDEO_FORMAT_E     enVideoFormat  = VIDEO_FORMAT_LINEAR;
    COMPRESS_MODE_E    enCompressMode = COMPRESS_MODE_NONE;

    VPSS_GRP           VpssGrp        = 0;
    VPSS_GRP_ATTR_S    stVpssGrpAttr;
    VPSS_CHN           VpssChn        = VPSS_CHN0;
    HI_BOOL            abChnEnable[VPSS_MAX_PHY_CHN_NUM] = {0};
    VPSS_CHN_ATTR_S    astVpssChnAttr[VPSS_MAX_PHY_CHN_NUM];
    VPSS_MOD_PARAM_S   stModParam;

    /*config vi*/
    SAMPLE_COMM_VI_GetSensorInfo(&stViConfig);
    ComboDev = SAMPLE_COMM_VI_GetComboDevBySensor(stViConfig.astViInfo[s32WorkSnsId].stSnsInfo.enSnsType, s32WorkSnsId);

    stViConfig.s32WorkingViNum    = s32ViCnt;
    stViConfig.as32WorkingViId[0] = 0;

    stViConfig.astViInfo[s32WorkSnsId].stSnsInfo.MipiDev         = ComboDev;
    stViConfig.astViInfo[s32WorkSnsId].stSnsInfo.s32BusId        = 0;

    /*get WDR mode*/
    s32Ret = SAMPLE_COMM_VI_GetWDRModeBySensor(stViConfig.astViInfo[s32WorkSnsId].stSnsInfo.enSnsType, &enWDRMode);

    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("get WDR mode by sensor failed!\n");
        return s32Ret;
    }

    stViConfig.astViInfo[s32WorkSnsId].stDevInfo.ViDev           = ViDev;
    stViConfig.astViInfo[s32WorkSnsId].stDevInfo.enWDRMode       = enWDRMode;

    /*get pipe info*/
    s32Ret = SAMPLE_COMM_VI_GetPipeBySensor(stViConfig.astViInfo[s32WorkSnsId].stSnsInfo.enSnsType, &stPipeInfo);

    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("get pipe info by sensor failed!\n");
        return s32Ret;
    }

    stViConfig.astViInfo[s32WorkSnsId].stPipeInfo.enMastPipeMode = stPipeInfo.enMastPipeMode;
    stViConfig.astViInfo[s32WorkSnsId].stPipeInfo.aPipe[0]       = stPipeInfo.aPipe[0];
    stViConfig.astViInfo[s32WorkSnsId].stPipeInfo.aPipe[1]       = stPipeInfo.aPipe[1];
    stViConfig.astViInfo[s32WorkSnsId].stPipeInfo.aPipe[2]       = stPipeInfo.aPipe[2];
    stViConfig.astViInfo[s32WorkSnsId].stPipeInfo.aPipe[3]       = stPipeInfo.aPipe[3];
    ViPipe0 = stPipeInfo.aPipe[0];

    stViConfig.astViInfo[s32WorkSnsId].stChnInfo.ViChn           = ViChn;
    stViConfig.astViInfo[s32WorkSnsId].stChnInfo.enPixFormat     = enPixFormat;
    stViConfig.astViInfo[s32WorkSnsId].stChnInfo.enDynamicRange  = DYNAMIC_RANGE_XDR;
    stViConfig.astViInfo[s32WorkSnsId].stChnInfo.enVideoFormat   = enVideoFormat;
    stViConfig.astViInfo[s32WorkSnsId].stChnInfo.enCompressMode  = enCompressMode;

    /*get picture size*/
    s32Ret = SAMPLE_COMM_VI_GetSizeBySensor(stViConfig.astViInfo[s32WorkSnsId].stSnsInfo.enSnsType, &enPicSize);

    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("get picture size by sensor failed!\n");
        return s32Ret;
    }

    s32Ret = SAMPLE_COMM_SYS_GetPicSize(enPicSize, &stSize);

    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("get picture size failed!\n");
        return s32Ret;
    }

    /*config vb*/
    memset_s(&stVbConf, sizeof(VB_CONFIG_S), 0, sizeof(VB_CONFIG_S));
    stVbConf.u32MaxPoolCnt              = 2;

    u32BlkSize = COMMON_GetPicBufferSize(stSize.u32Width, stSize.u32Height, SAMPLE_PIXEL_FORMAT, DATA_BITWIDTH_10, COMPRESS_MODE_SEG, DEFAULT_ALIGN);
    stVbConf.astCommPool[0].u64BlkSize  = u32BlkSize;
    stVbConf.astCommPool[0].u32BlkCnt   = 10;

    u32BlkSize = VI_GetRawBufferSize(stSize.u32Width, stSize.u32Height, PIXEL_FORMAT_RGB_BAYER_16BPP, COMPRESS_MODE_NONE, DEFAULT_ALIGN);
    stVbConf.astCommPool[1].u64BlkSize  = u32BlkSize;
    stVbConf.astCommPool[1].u32BlkCnt   = 4;

    s32Ret = SAMPLE_COMM_SYS_Init(&stVbConf);

    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("system init failed with %d!\n", s32Ret);
        SAMPLE_COMM_SYS_Exit();
        return s32Ret;
    }

    /*config vpss*/
    stVpssGrpAttr.stFrameRate.s32SrcFrameRate    = -1;
    stVpssGrpAttr.stFrameRate.s32DstFrameRate    = -1;
    stVpssGrpAttr.enDynamicRange                 = DYNAMIC_RANGE_XDR;
    stVpssGrpAttr.enPixelFormat                  = enPixFormat;
    stVpssGrpAttr.u32MaxW                        = stSize.u32Width;
    stVpssGrpAttr.u32MaxH                        = stSize.u32Height;

    astVpssChnAttr[VpssChn].u32Width                    = stSize.u32Width;
    astVpssChnAttr[VpssChn].u32Height                   = stSize.u32Height;
    astVpssChnAttr[VpssChn].enChnMode                   = VPSS_CHN_MODE_USER;
    astVpssChnAttr[VpssChn].enCompressMode              = enCompressMode;
    astVpssChnAttr[VpssChn].enDynamicRange              = enDynamicRange;
    astVpssChnAttr[VpssChn].enVideoFormat               = enVideoFormat;
    astVpssChnAttr[VpssChn].enPixelFormat               = enPixFormat;
    astVpssChnAttr[VpssChn].stFrameRate.s32SrcFrameRate = 30;
    astVpssChnAttr[VpssChn].stFrameRate.s32DstFrameRate = 30;
    astVpssChnAttr[VpssChn].u32Depth                    = 0;
    astVpssChnAttr[VpssChn].bMirror                     = HI_FALSE;
    astVpssChnAttr[VpssChn].bFlip                       = HI_FALSE;
    astVpssChnAttr[VpssChn].stAspectRatio.enMode        = ASPECT_RATIO_NONE;

    /*config vo*/
    SAMPLE_COMM_VO_GetDefConfig(&stVoConfig);

    stVoConfig.enDstDynamicRange = enDynamicRange;
    stVoConfig.enIntfSync = VO_OUTPUT_3840x2160_30;
    stVoConfig.enPicSize = PIC_3840x2160;
    stVoConfig.stImageSize.u32Width = 1920;
    stVoConfig.stImageSize.u32Height = 1080;
    stVoConfig.enVoIntfType = enVoIntfType;

    s32Ret = HI_MPI_VPSS_GetModParam(&stModParam);
    if(s32Ret != HI_SUCCESS)
    {
        SAMPLE_PRT("Get VPSS module param fali %#x!\n", s32Ret);
        SAMPLE_COMM_SYS_Exit();
        return s32Ret;
    }

    stModParam.bHdrSupport = HI_TRUE;

    s32Ret = HI_MPI_VPSS_SetModParam(&stModParam);
    if(s32Ret != HI_SUCCESS)
    {
        SAMPLE_PRT("Set VPSS module param fali %#x!\n", s32Ret);
        SAMPLE_COMM_SYS_Exit();
        return s32Ret;
    }


    s32Ret = SAMPLE_VIO_StartViVpssVo(stViConfig, stVpssGrpAttr, astVpssChnAttr, stVoConfig);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_VIO_StartViVpssVo failed with %d!\n", s32Ret);
        SAMPLE_COMM_SYS_Exit();
        return s32Ret;
    }

#ifdef HDR2SDR
    printf("Press the Enter key to switch WDR mode to linear mode and switch HDR10 to SDR10.\n");
    getchar();

    SAMPLE_COMM_VPSS_UnBind_VO(VpssGrp, VpssChn, stVoConfig.VoDev, VoChn);
    SAMPLE_COMM_VO_StopVO(&stVoConfig);
    SAMPLE_COMM_VI_UnBind_VPSS(ViPipe0, ViChn, VpssGrp);
    SAMPLE_COMM_VPSS_Stop(VpssGrp, abChnEnable);
    SAMPLE_COMM_VI_StopVi(&stViConfig);

    /*change config*/
    stViConfig.astViInfo[s32WorkSnsId].stPipeInfo.aPipe[1]      = -1;
    stViConfig.astViInfo[s32WorkSnsId].stDevInfo.enWDRMode      = WDR_MODE_NONE;
    stViConfig.astViInfo[s32WorkSnsId].stChnInfo.enDynamicRange = DYNAMIC_RANGE_SDR10;

    stVpssGrpAttr.enDynamicRange = DYNAMIC_RANGE_SDR10;

    astVpssChnAttr[VpssChn].enDynamicRange = DYNAMIC_RANGE_SDR10;

    stVoConfig.enDstDynamicRange = DYNAMIC_RANGE_SDR10;

    s32Ret = SAMPLE_VIO_StartViVpssVo(stViConfig, stVpssGrpAttr, astVpssChnAttr, stVoConfig);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_VIO_StartViVpssVo failed with %d!\n", s32Ret);
        SAMPLE_COMM_SYS_Exit();
        return s32Ret;
    }
#endif

    PAUSE();

    SAMPLE_COMM_VPSS_UnBind_VO(VpssGrp, VpssChn, stVoConfig.VoDev, VoChn);
    SAMPLE_COMM_VO_StopVO(&stVoConfig);
    SAMPLE_COMM_VI_UnBind_VPSS(ViPipe0, ViChn, VpssGrp);
    SAMPLE_COMM_VPSS_Stop(VpssGrp, abChnEnable);
    SAMPLE_COMM_VI_StopVi(&stViConfig);
    SAMPLE_COMM_SYS_Exit();

    return s32Ret;
}


HI_S32 SAMPLE_VIO_4K30_LDC_ROTATE(VO_INTF_TYPE_E enVoIntfType)
{
    HI_S32             s32Ret;
    VI_DEV             ViDev               = 0;
    VI_PIPE            ViPipe              = 0;
    VI_CHN             ViChn               = 0;
    HI_S32             s32WorkSnsId        = 0;
    VO_DEV             VoDev               = SAMPLE_VO_DEV_DHD0;
    VO_CHN             VoChn               = 0;
    SIZE_S             stSize;
    VB_CONFIG_S        stVbConf;
    PIC_SIZE_E         enPicSize           = PIC_3840x2160;
    HI_U32             u32BlkSize;
    VI_LDC_ATTR_S      stLDCAttr   = {0};
    SAMPLE_VI_CONFIG_S stViConfig  = {0};
    SAMPLE_VO_CONFIG_S stVoConfig  = {0};
    combo_dev_t          ComboDev;

    DYNAMIC_RANGE_E    enDynamicRange = DYNAMIC_RANGE_SDR8;
    PIXEL_FORMAT_E     enPixFormat    = PIXEL_FORMAT_YVU_SEMIPLANAR_420;
    VIDEO_FORMAT_E     enVideoFormat  = VIDEO_FORMAT_LINEAR;
    COMPRESS_MODE_E    enCompressMode = COMPRESS_MODE_NONE;
    VI_VPSS_MODE_E     enMastPipeMode = VI_ONLINE_VPSS_OFFLINE;

    /************************************************
    step1:  Get all sensors information
    *************************************************/
    SAMPLE_COMM_VI_GetSensorInfo(&stViConfig);
    ComboDev = SAMPLE_COMM_VI_GetComboDevBySensor(stViConfig.astViInfo[s32WorkSnsId].stSnsInfo.enSnsType, s32WorkSnsId);


    stViConfig.s32WorkingViNum                           = 1;

    stViConfig.as32WorkingViId[0]                        = 0;
    stViConfig.astViInfo[0].stSnsInfo.MipiDev            = ComboDev;
    stViConfig.astViInfo[0].stSnsInfo.s32BusId           = 0;

    stViConfig.astViInfo[0].stDevInfo.ViDev              = ViDev;
    stViConfig.astViInfo[0].stDevInfo.enWDRMode          = WDR_MODE_NONE;

    stViConfig.astViInfo[0].stPipeInfo.enMastPipeMode    = enMastPipeMode;
    stViConfig.astViInfo[0].stPipeInfo.aPipe[0]          = ViPipe;
    stViConfig.astViInfo[0].stPipeInfo.aPipe[1]          = -1;
    stViConfig.astViInfo[0].stPipeInfo.aPipe[2]          = -1;
    stViConfig.astViInfo[0].stPipeInfo.aPipe[3]          = -1;

    stViConfig.astViInfo[0].stChnInfo.ViChn              = ViChn;
    stViConfig.astViInfo[0].stChnInfo.enPixFormat        = enPixFormat;
    stViConfig.astViInfo[0].stChnInfo.enDynamicRange     = enDynamicRange;
    stViConfig.astViInfo[0].stChnInfo.enVideoFormat      = enVideoFormat;
    stViConfig.astViInfo[0].stChnInfo.enCompressMode     = enCompressMode;

    /************************************************
    step2:  Get  input size
    *************************************************/
    s32Ret = SAMPLE_COMM_VI_GetSizeBySensor(stViConfig.astViInfo[s32WorkSnsId].stSnsInfo.enSnsType, &enPicSize);

    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VI_GetSizeBySensor failed!\n");
        return s32Ret;
    }

    s32Ret = SAMPLE_COMM_SYS_GetPicSize(enPicSize, &stSize);

    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_SYS_GetPicSize failed!\n");
        return s32Ret;
    }

    /************************************************
    step3:  Init SYS and common VB
    *************************************************/

    memset_s(&stVbConf, sizeof(VB_CONFIG_S), 0, sizeof(VB_CONFIG_S));
    stVbConf.u32MaxPoolCnt              = 2;

    u32BlkSize = COMMON_GetPicBufferSize(stSize.u32Width, stSize.u32Height, SAMPLE_PIXEL_FORMAT, DATA_BITWIDTH_10, COMPRESS_MODE_SEG, DEFAULT_ALIGN);
    stVbConf.astCommPool[0].u64BlkSize  = u32BlkSize;
    stVbConf.astCommPool[0].u32BlkCnt   = 10;

    u32BlkSize = VI_GetRawBufferSize(stSize.u32Width, stSize.u32Height, PIXEL_FORMAT_RGB_BAYER_16BPP, COMPRESS_MODE_NONE, DEFAULT_ALIGN);
    stVbConf.astCommPool[1].u64BlkSize  = u32BlkSize;
    stVbConf.astCommPool[1].u32BlkCnt   = 4;

    s32Ret = SAMPLE_COMM_SYS_Init(&stVbConf);

    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("system init failed with %d!\n", s32Ret);
        SAMPLE_COMM_SYS_Exit();
        return s32Ret;
    }

    s32Ret = SAMPLE_COMM_VI_SetParam(&stViConfig);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_COMM_SYS_Exit();
        return s32Ret;
    }


    /************************************************
    step4:  Init VI and VO
    *************************************************/
    SAMPLE_COMM_VO_GetDefConfig(&stVoConfig);
    s32Ret = SAMPLE_VIO_StartViVo(&stViConfig, &stVoConfig);

    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_VIO_StartViVo failed witfh %d\n", s32Ret);
        goto EXIT;
    }


    /************************************************
    step5:  Bind VI and VO
    *************************************************/
    s32Ret = SAMPLE_COMM_VI_Bind_VO(ViPipe, ViChn, VoDev, VoChn);

    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VI_Bind_VO failed with %#x!\n", s32Ret);
        goto EXIT1;
    }

    stLDCAttr.bEnable = HI_TRUE;
    stLDCAttr.stAttr.bAspect = 0;
    stLDCAttr.stAttr.s32XRatio = 100;
    stLDCAttr.stAttr.s32YRatio = 100;
    stLDCAttr.stAttr.s32XYRatio = 100;
    stLDCAttr.stAttr.s32CenterXOffset = 0;
    stLDCAttr.stAttr.s32CenterYOffset = 0;
    stLDCAttr.stAttr.s32DistortionRatio = 500;

    s32Ret = HI_MPI_VI_SetChnLDCAttr(ViPipe,ViChn,&stLDCAttr);

    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("HI_MPI_VI_SetChnLDCAttr failed witfh %d\n", s32Ret);
        goto EXIT;
    }


    printf("now LDC on, press any key to switch LDC Off!\n");
    getchar();

    stLDCAttr.bEnable = HI_FALSE;
    stLDCAttr.stAttr.bAspect = 0;
    stLDCAttr.stAttr.s32XRatio = 100;
    stLDCAttr.stAttr.s32YRatio = 100;
    stLDCAttr.stAttr.s32XYRatio = 100;
    stLDCAttr.stAttr.s32CenterXOffset = 0;
    stLDCAttr.stAttr.s32CenterYOffset = 0;
    stLDCAttr.stAttr.s32DistortionRatio = 300;

    s32Ret = HI_MPI_VI_SetChnLDCAttr(ViPipe,ViChn,&stLDCAttr);

    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("HI_MPI_VI_SetChnLDCAttr failed witfh %d\n", s32Ret);
        goto EXIT;
    }

    printf("now LDC off, press any key to switch LDC On!\n");
    getchar();

    stLDCAttr.bEnable = HI_TRUE;
    stLDCAttr.stAttr.bAspect = 0;
    stLDCAttr.stAttr.s32XRatio = 100;
    stLDCAttr.stAttr.s32YRatio = 100;
    stLDCAttr.stAttr.s32XYRatio = 100;
    stLDCAttr.stAttr.s32CenterXOffset = 0;
    stLDCAttr.stAttr.s32CenterYOffset = 0;
    stLDCAttr.stAttr.s32DistortionRatio = 300;

    s32Ret = HI_MPI_VI_SetChnLDCAttr(ViPipe,ViChn,&stLDCAttr);

    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("HI_MPI_VI_SetChnLDCAttr failed witfh %d\n", s32Ret);
        goto EXIT;
    }


    printf("now LDC on and Rotation 0, press any key to switch Rotation 90!\n");
    getchar();


    s32Ret = HI_MPI_VI_SetChnRotation(ViPipe,ViChn,ROTATION_90);

    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("HI_MPI_VI_SetChnLDCAttr failed witfh %d\n", s32Ret);
        goto EXIT;
    }

    printf("now LDC on and Rotation 90, press any key to switch Rotation 180!\n");
    getchar();

    s32Ret = HI_MPI_VI_SetChnRotation(ViPipe,ViChn,ROTATION_180);

    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("HI_MPI_VI_SetChnLDCAttr failed witfh %d\n", s32Ret);
        goto EXIT;
    }

    printf("now LDC on and Rotation 180, press any key to switch Rotation 270!\n");
    getchar();

    s32Ret = HI_MPI_VI_SetChnRotation(ViPipe,ViChn,ROTATION_270);

    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("HI_MPI_VI_SetChnLDCAttr failed witfh %d\n", s32Ret);
        goto EXIT;
    }

    printf("now LDC on and Rotation 270, press any key to switch Rotation 0!\n");
    getchar();

    s32Ret = HI_MPI_VI_SetChnRotation(ViPipe,ViChn,ROTATION_0);

    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("HI_MPI_VI_SetChnLDCAttr failed witfh %d\n", s32Ret);
        goto EXIT;
    }


    PAUSE();

    SAMPLE_COMM_VI_UnBind_VO(ViPipe, ViChn, VoDev, VoChn);

    EXIT1:
    SAMPLE_VIO_StopViVo(&stViConfig, &stVoConfig);

    EXIT:
    SAMPLE_COMM_SYS_Exit();

    return s32Ret;
}



HI_S32 SAMPLE_VIO_4K30_FreeRotation(VO_INTF_TYPE_E enVoIntfType)
{
    HI_S32             s32Ret;
    VI_DEV             ViDev               = 0;
    VI_PIPE            ViPipe              = 0;
    VI_CHN             ViChn               = 0;
    HI_S32             s32WorkSnsId        = 0;
    VO_DEV             VoDev               = SAMPLE_VO_DEV_DHD0;
    VO_CHN             VoChn               = 0;
    SIZE_S             stSize;
    VB_CONFIG_S        stVbConf;
    PIC_SIZE_E         enPicSize           = PIC_3840x2160;
    HI_U32             u32BlkSize;
    VI_ROTATION_EX_ATTR_S stViRotationExAttr  = {0};
    SAMPLE_VI_CONFIG_S stViConfig  = {0};
    SAMPLE_VO_CONFIG_S stVoConfig  = {0};
    combo_dev_t          ComboDev;

    DYNAMIC_RANGE_E    enDynamicRange = DYNAMIC_RANGE_SDR8;
    PIXEL_FORMAT_E     enPixFormat    = PIXEL_FORMAT_YVU_SEMIPLANAR_420;
    VIDEO_FORMAT_E     enVideoFormat  = VIDEO_FORMAT_LINEAR;
    COMPRESS_MODE_E    enCompressMode = COMPRESS_MODE_NONE;
    VI_VPSS_MODE_E     enMastPipeMode = VI_ONLINE_VPSS_OFFLINE;

    /************************************************
    step1:  Get all sensors information
    *************************************************/
    SAMPLE_COMM_VI_GetSensorInfo(&stViConfig);
    ComboDev = SAMPLE_COMM_VI_GetComboDevBySensor(stViConfig.astViInfo[s32WorkSnsId].stSnsInfo.enSnsType, s32WorkSnsId);


    stViConfig.s32WorkingViNum                           = 1;

    stViConfig.as32WorkingViId[0]                        = 0;
    stViConfig.astViInfo[0].stSnsInfo.MipiDev            = ComboDev;
    stViConfig.astViInfo[0].stSnsInfo.s32BusId           = 0;

    stViConfig.astViInfo[0].stDevInfo.ViDev              = ViDev;
    stViConfig.astViInfo[0].stDevInfo.enWDRMode          = WDR_MODE_NONE;

    stViConfig.astViInfo[0].stPipeInfo.enMastPipeMode    = enMastPipeMode;
    stViConfig.astViInfo[0].stPipeInfo.aPipe[0]          = ViPipe;
    stViConfig.astViInfo[0].stPipeInfo.aPipe[1]          = -1;
    stViConfig.astViInfo[0].stPipeInfo.aPipe[2]          = -1;
    stViConfig.astViInfo[0].stPipeInfo.aPipe[3]          = -1;

    stViConfig.astViInfo[0].stChnInfo.ViChn              = ViChn;
    stViConfig.astViInfo[0].stChnInfo.enPixFormat        = enPixFormat;
    stViConfig.astViInfo[0].stChnInfo.enDynamicRange     = enDynamicRange;
    stViConfig.astViInfo[0].stChnInfo.enVideoFormat      = enVideoFormat;
    stViConfig.astViInfo[0].stChnInfo.enCompressMode     = enCompressMode;

    /************************************************
    step2:  Get  input size
    *************************************************/
    s32Ret = SAMPLE_COMM_VI_GetSizeBySensor(stViConfig.astViInfo[s32WorkSnsId].stSnsInfo.enSnsType, &enPicSize);

    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VI_GetSizeBySensor failed!\n");
        return s32Ret;
    }

    s32Ret = SAMPLE_COMM_SYS_GetPicSize(enPicSize, &stSize);

    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_SYS_GetPicSize failed!\n");
        return s32Ret;
    }

    /************************************************
    step3:  Init SYS and common VB
    *************************************************/
    memset_s(&stVbConf, sizeof(VB_CONFIG_S), 0, sizeof(VB_CONFIG_S));
    stVbConf.u32MaxPoolCnt              = 2;

    u32BlkSize = COMMON_GetPicBufferSize(stSize.u32Width, stSize.u32Height, SAMPLE_PIXEL_FORMAT, DATA_BITWIDTH_10, COMPRESS_MODE_SEG, DEFAULT_ALIGN);
    stVbConf.astCommPool[0].u64BlkSize  = u32BlkSize;
    stVbConf.astCommPool[0].u32BlkCnt   = 10;

    u32BlkSize = VI_GetRawBufferSize(stSize.u32Width, stSize.u32Height, PIXEL_FORMAT_RGB_BAYER_16BPP, COMPRESS_MODE_NONE, DEFAULT_ALIGN);
    stVbConf.astCommPool[1].u64BlkSize  = u32BlkSize;
    stVbConf.astCommPool[1].u32BlkCnt   = 4;

    s32Ret = SAMPLE_COMM_SYS_Init(&stVbConf);

    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("system init failed with %d!\n", s32Ret);
        SAMPLE_COMM_SYS_Exit();
        return s32Ret;
    }

    s32Ret = SAMPLE_COMM_VI_SetParam(&stViConfig);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_COMM_SYS_Exit();
        return s32Ret;
    }


    /************************************************
    step4:  Init VI and VO
    *************************************************/
    SAMPLE_COMM_VO_GetDefConfig(&stVoConfig);
    s32Ret = SAMPLE_VIO_StartViVo(&stViConfig, &stVoConfig);

    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_VIO_StartViVo failed witfh %d\n", s32Ret);
        goto EXIT;
    }


    /************************************************
    step5:  Bind VI and VO
    *************************************************/
    s32Ret = SAMPLE_COMM_VI_Bind_VO(ViPipe, ViChn, VoDev, VoChn);

    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VI_Bind_VO failed with %#x!\n", s32Ret);
        goto EXIT1;
    }

    stViRotationExAttr.bEnable                           = HI_TRUE;
    stViRotationExAttr.stRotationEx.enViewType           = ROTATION_VIEW_TYPE_ALL;
    stViRotationExAttr.stRotationEx.s32CenterXOffset     = 0;
    stViRotationExAttr.stRotationEx.s32CenterYOffset     = 0;
    stViRotationExAttr.stRotationEx.u32Angle             = 76;
    stViRotationExAttr.stRotationEx.stDestSize.u32Width  = stSize.u32Width;
    stViRotationExAttr.stRotationEx.stDestSize.u32Height = stSize.u32Height;

    s32Ret = HI_MPI_VI_SetChnRotationEx(ViPipe,ViChn,&stViRotationExAttr);

    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("HI_MPI_VI_SetChnRotationEx failed witfh %d\n", s32Ret);
        goto EXIT;
    }


    printf("now FreeAngle Rotation on and enViewType ALL, press any key to switch enViewType TYPICAL!\n");
    getchar();

    stViRotationExAttr.bEnable                           = HI_TRUE;
    stViRotationExAttr.stRotationEx.enViewType           = ROTATION_VIEW_TYPE_TYPICAL;
    stViRotationExAttr.stRotationEx.s32CenterXOffset     = 0;
    stViRotationExAttr.stRotationEx.s32CenterYOffset     = 0;
    stViRotationExAttr.stRotationEx.u32Angle             = 76;
    stViRotationExAttr.stRotationEx.stDestSize.u32Width  = stSize.u32Width;
    stViRotationExAttr.stRotationEx.stDestSize.u32Height = stSize.u32Height;

    s32Ret = HI_MPI_VI_SetChnRotationEx(ViPipe,ViChn,&stViRotationExAttr);

    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("HI_MPI_VI_SetChnRotationEx failed witfh %d\n", s32Ret);
        goto EXIT;
    }


    printf("now FreeAngle Rotation on, press any key to switch enViewType INSIDE!\n");
    getchar();

    stViRotationExAttr.bEnable                           = HI_TRUE;
    stViRotationExAttr.stRotationEx.enViewType           = ROTATION_VIEW_TYPE_INSIDE;
    stViRotationExAttr.stRotationEx.s32CenterXOffset     = 0;
    stViRotationExAttr.stRotationEx.s32CenterYOffset     = 0;
    stViRotationExAttr.stRotationEx.u32Angle             = 76;
    stViRotationExAttr.stRotationEx.stDestSize.u32Width  = stSize.u32Width;
    stViRotationExAttr.stRotationEx.stDestSize.u32Height = stSize.u32Height;

    s32Ret = HI_MPI_VI_SetChnRotationEx(ViPipe,ViChn,&stViRotationExAttr);

    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("HI_MPI_VI_SetChnRotationEx failed witfh %d\n", s32Ret);
        goto EXIT;
    }


    printf("now FreeAngle Rotation on, press any key to switch FreeAngle Rotation off!\n");
    getchar();

    stViRotationExAttr.bEnable                           = HI_FALSE;
    stViRotationExAttr.stRotationEx.enViewType           = ROTATION_VIEW_TYPE_INSIDE;
    stViRotationExAttr.stRotationEx.s32CenterXOffset     = 0;
    stViRotationExAttr.stRotationEx.s32CenterYOffset     = 0;
    stViRotationExAttr.stRotationEx.u32Angle             = 76;
    stViRotationExAttr.stRotationEx.stDestSize.u32Width  = stSize.u32Width;
    stViRotationExAttr.stRotationEx.stDestSize.u32Height = stSize.u32Height;

    s32Ret = HI_MPI_VI_SetChnRotationEx(ViPipe,ViChn,&stViRotationExAttr);

    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("HI_MPI_VI_SetChnRotationEx failed witfh %d\n", s32Ret);
        goto EXIT;
    }


    PAUSE();

    SAMPLE_COMM_VI_UnBind_VO(ViPipe, ViChn, VoDev, VoChn);

    EXIT1:
    SAMPLE_VIO_StopViVo(&stViConfig, &stVoConfig);

    EXIT:
    SAMPLE_COMM_SYS_Exit();

    return s32Ret;
}



HI_S32 SAMPLE_VIO_4K30_LDC_SPREAD(VO_INTF_TYPE_E enVoIntfType)
{
    HI_S32             s32Ret;
    VI_DEV             ViDev               = 0;
    VI_PIPE            ViPipe              = 0;
    VI_CHN             ViChn               = 0;
    HI_S32             s32WorkSnsId        = 0;
    VO_DEV             VoDev               = SAMPLE_VO_DEV_DHD0;
    VO_CHN             VoChn               = 0;
    SIZE_S             stSize;
    VB_CONFIG_S        stVbConf;
    PIC_SIZE_E         enPicSize           = PIC_3840x2160;
    HI_U32             u32BlkSize;
    VI_LDC_ATTR_S      stLDCAttr           = {0} ;
    SPREAD_ATTR_S      stSpreadAttr        = {0};
    SAMPLE_VI_CONFIG_S stViConfig  = {0};
    SAMPLE_VO_CONFIG_S stVoConfig  = {0};
    combo_dev_t          ComboDev;

    DYNAMIC_RANGE_E    enDynamicRange = DYNAMIC_RANGE_SDR8;
    PIXEL_FORMAT_E     enPixFormat    = PIXEL_FORMAT_YVU_SEMIPLANAR_420;
    VIDEO_FORMAT_E     enVideoFormat  = VIDEO_FORMAT_LINEAR;
    COMPRESS_MODE_E    enCompressMode = COMPRESS_MODE_NONE;
    VI_VPSS_MODE_E     enMastPipeMode = VI_ONLINE_VPSS_OFFLINE;

    /************************************************
    step1:  Get all sensors information
    *************************************************/
    SAMPLE_COMM_VI_GetSensorInfo(&stViConfig);
    ComboDev = SAMPLE_COMM_VI_GetComboDevBySensor(stViConfig.astViInfo[s32WorkSnsId].stSnsInfo.enSnsType, s32WorkSnsId);


    stViConfig.s32WorkingViNum                           = 1;

    stViConfig.as32WorkingViId[0]                        = 0;
    stViConfig.astViInfo[0].stSnsInfo.MipiDev            = ComboDev;
    stViConfig.astViInfo[0].stSnsInfo.s32BusId           = 0;

    stViConfig.astViInfo[0].stDevInfo.ViDev              = ViDev;
    stViConfig.astViInfo[0].stDevInfo.enWDRMode          = WDR_MODE_NONE;

    stViConfig.astViInfo[0].stPipeInfo.enMastPipeMode    = enMastPipeMode;
    stViConfig.astViInfo[0].stPipeInfo.aPipe[0]          = ViPipe;
    stViConfig.astViInfo[0].stPipeInfo.aPipe[1]          = -1;
    stViConfig.astViInfo[0].stPipeInfo.aPipe[2]          = -1;
    stViConfig.astViInfo[0].stPipeInfo.aPipe[3]          = -1;

    stViConfig.astViInfo[0].stChnInfo.ViChn              = ViChn;
    stViConfig.astViInfo[0].stChnInfo.enPixFormat        = enPixFormat;
    stViConfig.astViInfo[0].stChnInfo.enDynamicRange     = enDynamicRange;
    stViConfig.astViInfo[0].stChnInfo.enVideoFormat      = enVideoFormat;
    stViConfig.astViInfo[0].stChnInfo.enCompressMode     = enCompressMode;

    /************************************************
    step2:  Get  input size
    *************************************************/
    s32Ret = SAMPLE_COMM_VI_GetSizeBySensor(stViConfig.astViInfo[s32WorkSnsId].stSnsInfo.enSnsType, &enPicSize);

    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VI_GetSizeBySensor failed!\n");
        return s32Ret;
    }

    s32Ret = SAMPLE_COMM_SYS_GetPicSize(enPicSize, &stSize);

    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_SYS_GetPicSize failed!\n");
        return s32Ret;
    }

    /************************************************
    step3:  Init SYS and common VB
    *************************************************/
    memset_s(&stVbConf, sizeof(VB_CONFIG_S), 0, sizeof(VB_CONFIG_S));
    stVbConf.u32MaxPoolCnt              = 2;

    u32BlkSize = COMMON_GetPicBufferSize(stSize.u32Width, stSize.u32Height, SAMPLE_PIXEL_FORMAT, DATA_BITWIDTH_10, COMPRESS_MODE_SEG, DEFAULT_ALIGN);
    stVbConf.astCommPool[0].u64BlkSize  = u32BlkSize;
    stVbConf.astCommPool[0].u32BlkCnt   = 10;

    u32BlkSize = VI_GetRawBufferSize(stSize.u32Width, stSize.u32Height, PIXEL_FORMAT_RGB_BAYER_16BPP, COMPRESS_MODE_NONE, DEFAULT_ALIGN);
    stVbConf.astCommPool[1].u64BlkSize  = u32BlkSize;
    stVbConf.astCommPool[1].u32BlkCnt   = 4;

    s32Ret = SAMPLE_COMM_SYS_Init(&stVbConf);

    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("system init failed with %d!\n", s32Ret);
        SAMPLE_COMM_SYS_Exit();
        return s32Ret;
    }

    s32Ret = SAMPLE_COMM_VI_SetParam(&stViConfig);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_COMM_SYS_Exit();
        return s32Ret;
    }

    /************************************************
    step4:  Init VI and VO
    *************************************************/
    SAMPLE_COMM_VO_GetDefConfig(&stVoConfig);
    s32Ret = SAMPLE_VIO_StartViVo(&stViConfig, &stVoConfig);

    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_VIO_StartViVo failed witfh %d\n", s32Ret);
        goto EXIT;
    }


    /************************************************
    step5:  Bind VI and VO
    *************************************************/
    s32Ret = SAMPLE_COMM_VI_Bind_VO(ViPipe, ViChn, VoDev, VoChn);

    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VI_Bind_VO failed with %#x!\n", s32Ret);
        goto EXIT1;
    }

    stLDCAttr.bEnable = HI_TRUE;
    stLDCAttr.stAttr.bAspect = 0;
    stLDCAttr.stAttr.s32XRatio = 100;
    stLDCAttr.stAttr.s32YRatio = 100;
    stLDCAttr.stAttr.s32XYRatio = 100;
    stLDCAttr.stAttr.s32CenterXOffset = 0;
    stLDCAttr.stAttr.s32CenterYOffset = 0;
    stLDCAttr.stAttr.s32DistortionRatio = 500;

    s32Ret = HI_MPI_VI_SetChnLDCAttr(ViPipe,ViChn,&stLDCAttr);

    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("HI_MPI_VI_SetChnLDCAttr failed witfh %d\n", s32Ret);
        goto EXIT;
    }


    stSpreadAttr.bEnable        = HI_TRUE;
    stSpreadAttr.u32SpreadCoef  = 16;
    stSpreadAttr.stDestSize.u32Width = 3840;
    stSpreadAttr.stDestSize.u32Height = 2160;

    s32Ret = HI_MPI_VI_SetChnSpreadAttr(ViPipe,ViChn,&stSpreadAttr);

    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("HI_MPI_VI_SetChnLDCAttr failed witfh %d\n", s32Ret);
        goto EXIT;
    }


    printf("now LDC on and spread on, press any key to switch spread Off!\n");
    getchar();

    stSpreadAttr.bEnable              = HI_FALSE;
    stSpreadAttr.u32SpreadCoef        = 16;
    stSpreadAttr.stDestSize.u32Width  = 3840;
    stSpreadAttr.stDestSize.u32Height = 2160;

    s32Ret = HI_MPI_VI_SetChnSpreadAttr(ViPipe,ViChn,&stSpreadAttr);

    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("HI_MPI_VI_SetChnSpreadAttr failed witfh %d\n", s32Ret);
        goto EXIT;
    }

    printf("now LDC on and spread off, press any key to switch spread On!\n");
    getchar();

    stSpreadAttr.bEnable             = HI_TRUE;
    stSpreadAttr.u32SpreadCoef       = 16;
    stSpreadAttr.stDestSize.u32Width = 3840;
    stSpreadAttr.stDestSize.u32Height= 2160;


    s32Ret = HI_MPI_VI_SetChnSpreadAttr(ViPipe,ViChn,&stSpreadAttr);

    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("HI_MPI_VI_SetChnSpreadAttr failed witfh %d\n", s32Ret);
        goto EXIT;
    }


    PAUSE();

    SAMPLE_COMM_VI_UnBind_VO(ViPipe, ViChn, VoDev, VoChn);

    EXIT1:
    SAMPLE_VIO_StopViVo(&stViConfig, &stVoConfig);

    EXIT:
    SAMPLE_COMM_SYS_Exit();

    return s32Ret;
}


/******************************************************************************
* function    : main()
* Description : main
******************************************************************************/
#ifdef __HuaweiLite__
int app_main(int argc, char *argv[])
#else
int main(int argc, char* argv[])
#endif
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_S32 s32Index;
    VO_INTF_TYPE_E enVoIntfType = VO_INTF_HDMI;

    if (argc < 2)
    {
        SAMPLE_VIO_Usage(argv[0]);
        return HI_FAILURE;
    }

#ifdef __HuaweiLite__
#else
    signal(SIGINT, SAMPLE_VIO_HandleSig);
    signal(SIGTERM, SAMPLE_VIO_HandleSig);
#endif

    if ((argc > 2) && (1 == atoi(argv[2])))
    {
        enVoIntfType = VO_INTF_BT1120;
    }

    s32Index = atoi(argv[1]);
    switch (s32Index)
    {
        case 0:
            s32Ret = SAMPLE_VIO_8K30_PARALLEL(enVoIntfType);
            break;

        case 1:
            s32Ret = SAMPLE_VIO_2X4K60_TotalOnline(enVoIntfType);
            break;

        case 2:
            s32Ret = SAMPLE_VIO_4x4K30_WBC(enVoIntfType);
            break;

        case 3:
            s32Ret = SAMPLE_VIO_4K30_WDR_HDR10(enVoIntfType);
            break;

        case 4:
            s32Ret = SAMPLE_VIO_4K30_LDC_ROTATE(enVoIntfType);
            break;

        case 5:
            s32Ret = SAMPLE_VIO_4K30_FreeRotation(enVoIntfType);
            break;

        case 6:
            s32Ret = SAMPLE_VIO_4K30_LDC_SPREAD(enVoIntfType);
            break;

        default:
            SAMPLE_PRT("the index %d is invaild!\n",s32Index);
            SAMPLE_VIO_Usage(argv[0]);
            return HI_FAILURE;
    }

    if (HI_SUCCESS == s32Ret)
    {
        SAMPLE_PRT("sample_vio exit success!\n");
    }
    else
    {
        SAMPLE_PRT("sample_vio exit abnormally!\n");
    }
    return s32Ret;
}


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

