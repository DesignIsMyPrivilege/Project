/*在BL0阶段，Irom内固化的代码读取nandflash或sd卡前16K的内容，
* 并比对前16字节中的校验和是否正确，正确则继续，错误则停止。*/
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define BUFSIZE              (16*1024)
#define IMG_SIZE             (16*1024)
#define SPL_HEADER_SIZE       16
#define SPL_HEADER            "S5PC110 HEADER  "

int main(int argc,char argv[]){
	FILE *fp;
	char *Buf, *a;
	int BufLen;
	int nbytes, fileLen;
	unsigned int checksum, count;
	int i;
	
	//1、3个参数
	if (argc != 3){
		printf("Usage:mkbll <source file> <destination file>\n");
		return -1;
	}
	
	//2、分配16K的buffer
	BufLen = BUFSIZE;
	Buf = (char *)malloc(BufLen);
	if (!Buf){
		printf("Malloc buffer failed!\n");
		return -2;
	}
	
	memset(Buf,0x00,Buflen);
	
	
	//3、读源bin到buffer
	//3.1、打开源bin
	fp = fopen(argv[1],"rb");
	if(fp == NULL){
		printf("Source file open error!\n");
		free(Buf);
		return -3;
	} 
	//3.2、获取源bin的长度
	fseek(fp,0L,SEEK_END);
	fileLen = ftell(fp);
	fseek(fp,0L,SEEK_SET);
	//3.3、源bin长度不得超过16K-16byte
	count = (fileLen < (IMG_SIZE - SPL_HEADER_SIZE)) ? fileLen : (IMG_SIZE - SPL_HEADER_SIZE);
	//3.4、buffer[0-15]存放"S5PC110 HEADER"
	memcpy(&Buf[0],SPL_HEADER,SPL_HEADER_SIZE);
	//3.5、读源bin到buffer[16]
	nbytes = fread(Buf + SPL_HEADER_SIZE,1,count,fp);
	if(nbytes != count){
		printf("Source file read error!\n");
		free(Buf);
		fclose(fp);
		return -4;
	}
	fclose(fp);
	
	//4、计算校验和
	//4.1、从第16byte开始统计buffer中共有几个1
	a = Buf + SPL_HEADER_SIZE;
	for(i = 0, checksum = 0; i < IMG_SIZE - SPL_HEADER_SIZE, i++){
		checksum += (0x000000FF) & *a++;
	}
	a = Buf + 8;
	*((unsigned int *)a) = checksum;
	//5、拷贝buffer中的内容到bin
	//5.1、打开目的bin
	fp = fopen(argv[2],"wb");
	if(fp == NULL){
		printf("Destination file open error\n");
		free(Buf);
		return -5;
	}
	//5.2、将16K的buffer拷贝到目的bin中
	a = Buf;
	nbytes = fwrite(a,1,BufLen,fp);
	if(nbytes != BufLen){
		printf("Destination file write error\n");
		free(Buf);
		fclose(fp);
		return -6;
	}
	free(Buf);
	fclose(fp);
	return 0;
}