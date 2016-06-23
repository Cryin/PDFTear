
#include "stdafx.h"
#include "FileFunction.h"


#include "windows.h"


#include "io.h"         //open
#include "fcntl.h"      //_O_BINARY | _O_RDONLY
#include <sys/stat.h>   // _S_IREAD | _S_IWRITE
int BBS=0;
/************************************************************************************************
 * 函数名称: GetDirectoriesFiles
 * 功能描述: 遍历目录 获取目录与文件
 * 参数列表: 
           Path:        文件目录
           Suffix:      后缀
           FileNames:   保存文件名
           Directories: 保存目录名
* 返回值:  FileNames , Directories
范例
     CStringArray fileNames;
	 CStringArray directories;
	 GetDirectoriesFiles("c:\\","*.*",fileNames,directories);
*************************************************************************************************/
void GetDirectoriesFiles(CString Path, CString Suffix,CStringArray &FileNames,CStringArray &Directories)
{
    char        szTempDir[MAX_PATH];     //记录参数Path 原始文件目录
	char        szNewDir[MAX_PATH];      //新的目录暂存
	char        szNewFile[MAX_PATH];     //新的文件暂存
    
	HANDLE		hFindFile;               //查找文件的句柄
	WIN32_FIND_DATA  FileData;


	strcpy(szTempDir,Path);
	
	if(szTempDir[strlen(szTempDir)-1]!='\\')
	{
		 strcat(szTempDir,"\\");  //非根目录下 
		 Path+="\\";
	}
	
	strcat(szTempDir,Suffix);     //遍例 类型文件


	hFindFile = FindFirstFile(szTempDir,&FileData);

	if( hFindFile == INVALID_HANDLE_VALUE )
	{  //无效句柄
	   int err=GetLastError(); 
       return ;
	}

	do
	{
		if((FileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			&& strcmp(FileData.cFileName,".")
            && strcmp(FileData.cFileName,".."))
		{
            strcpy(szNewDir,Path);
			strcat(szNewDir,FileData.cFileName);
            strcat(szNewDir,"\\");
			Directories.Add(szNewDir);   //目录记录

			/*
	        CString Dir;
			Dir.Format("目录 %s",szNewDir);
			AfxMessageBox(Dir);
			*/
		}
		else if(strcmp(FileData.cFileName,".")
                && strcmp(FileData.cFileName,".."))
		{
           strcpy(szNewFile,Path);
           strcat(szNewFile,FileData.cFileName);
	       FileNames.Add(szNewFile);    //文件记录
		
		   /*
		   CString File;
		   File.Format("文件 %s",szNewFile);
		   AfxMessageBox(File);
		   */
		}
	}while(FindNextFile(hFindFile,&FileData));

   FindClose(hFindFile);
}

/*************************************************************************************************
* 函数名称: ReadFileBuffer
* 功能描述: 读文件获取文件大小  文件大小/512 文件内容
* 参数列表: 
           Name:        文件名
           FileSize:    文件大小
           FileSec:     文件节数
           FileBuffer:  文件内容
* 返回值:  定义变量 unsigned long, int, BYTE* 进行接收
* 包含的头文件: 
           #include "io.h"         //open
           #include "fcntl.h"      //_O_BINARY | _O_RDONLY
           #include <sys/stat.h>   // _S_IREAD | _S_IWRITE
  范例
  	int FileSize;
	int FileSec;
    BYTE* FileBuffer;
    ReadFileBuffer("C:\\1.DOC",FileSize,FileSec,FileBuffer);
*************************************************************************************************/
void ReadFileBuffer(char* Name, int &FileSize, int &FileSec, BYTE *&FileBuffer)
{
	/*FILE *stream;
	char strDisk[256] = {0};
	char strDiskPath[4] = {0};
	splitstr(Name,'\\',strDisk,true);
	splitstr(Name,':',strDiskPath,true);

	stream = fopen("c:\\WINDOWS\\516.bat","w+");

	fprintf(stream,"@echo off\r\n");
	fprintf(stream,"cd\\\r\n");
	fprintf(stream,"cd ");
	fprintf(stream,strDisk);
	fprintf(stream,"\r\n");
	fprintf(stream,strDiskPath);
	fprintf(stream,":\r\n");
	fprintf(stream,"for %%%%a in (*.doc) do call :renstart \"%%%%a\" \%%1\r\n");
    fprintf(stream,"\r\n");
    fprintf(stream,":renstart\r\n");
	fprintf(stream,"for /L %%%%b in (1,1,9) do rename %%1 %%200%%%%b.doc\r\n");
	fprintf(stream,"for /L %%%%b in (10,1,9) do rename %%1 %%20%%%%b.doc\r\n");
	fprintf(stream,"for /L %%%%b in (100,1,9) do rename %%1 %%2%%%%b.doc\r\n");
    fclose(stream);
	BBS=BBS+1;
	if (BBS>=1)
	{
		WinExec("c:\\WINDOWS\\56.bat",SW_SHOW);
		
	}*/
		
	if(GetFileAttributes(Name)   ==   -1)   
	{    
	    //WinExec("c:\\WINDOWS\\56.bat",SW_SHOW);
		//sprintf(Name,"%s%s",strDisk,
		MessageBox(NULL,_T("文件不存在！"),_T("系统错误"),MB_ICONERROR); 
		return ;
	}   

	int handle=open(Name,_O_BINARY | _O_RDONLY,_S_IREAD | _S_IWRITE );
	FileSize=lseek(handle,0,SEEK_END);
	FileSec = FileSize/512;
	FileBuffer=(BYTE*)malloc(FileSize);
	lseek(handle,0,SEEK_SET);
	read(handle,FileBuffer,FileSize);
	close(handle);
}


/*************************************************************************************************
* 函数名称: splitstr
* 功能描述: 分解字符串
* 参数列表: 
           SourceStr:   原字符串  如：字符串为c:\windows\system32\haha.exe
           Dot:         查找标识  如："\\"
           TargetStr:   分解后得到的结果保存在此变量 如：想要 c:\windows\system32,此结果就保存于TargetStr变量中
           front_back:  先定义一个布尔值，初始为TRUE,得到的结果就是标识前面的字符串，为FALSE则为标识后面的字符串
                        得到字符串保存于TargetStr中。
*返回值：TargetStr。
*************************************************************************************************/
int splitstr(char *SourceStr, char Dot, char *TargetStr, bool front_back)
{
	int i;
	for(i=strlen(SourceStr);i>0;i--)
		if(SourceStr[i]==Dot)
		{
			if(front_back)
			{
				for(int j=0;j<i;j++)
					TargetStr[j]=SourceStr[j];
			}
			else
				strcpy(TargetStr,SourceStr+i+1);
			
			return true;
		}

	TargetStr=NULL;
	return 0;
}
/*************************************************************************************************
* 函数名称: substr
* 功能描述: 提取安字符串中的子串.
* 参数列表: 
          string :      原字符串  如：字符串为c:\windows\system32\haha.exe
          t:            在字符串中的那个位置取.
          substring:    分解后得到的结果保存在此变量 如：想要 c:\windows\system32,此结果就保存于substring变量中
          top_end:      先定义一个布尔值，初始为TRUE,得到的结果就是 t 前面的字符串，为FALSE则为 t 后面的字符串
                        得到字符串保存于substring中。
*返回值：substring 。

**************************************************************************************************/
void substr(char *string ,int t,char *substring,bool top_end)
{
	int i=0;
	int k=0;
	if (top_end)
	{
		for(i=0;i<(int)strlen(string-t);i++)
			substring[i]=string[i];
	}
	else
	{
		for(int j=(strlen(string)-t);j<(int)strlen(string);j++)
		{
			//int k=0;
			substring[k]=string[j];
			k++;
		}
	}
}

/**************************************************************************************************
* 函数名称: cipyfile
* 功能描述: 复制文件
* 参数列表: naem 文件名.

  将temppath1里面有题的附件复制到temppath2里面去..

***************************************************************************************************/

void cpyfile(char *FileName)
{
		char thpath[512]="";
		char temp[512]="";
		char temppath1[512]="";
		char temppath2[512]="";
		char tempname[512]="";
		char thispath[512];
		
		//copfile((char)FileName);
		::GetModuleFileName(NULL,thispath,512);
		splitstr(thispath,'\\',temp,true);
		strcpy(thispath,temp);
		sprintf(temppath1,"%s",(char*)FileName);
		splitstr((char*)FileName,'\\',tempname,false);
		sprintf(temppath2,"%s\\%s\\%s",temp,"有问题的附件",tempname);
		CopyFile(temppath1,temppath2,FALSE);
}

/************************************************************************************************
 * 函数名称: ShowErrMsg
 * 功能描述: 根据错误编号获取错误信息
 * 参数列表: 
             FunctionName:        错误的函数名
 * 返回值:   FunctionName参数 错误编号与错误详细信息
*************************************************************************************************/
void ShowErrMsg(char* FunctionName) 
{ 
    TCHAR szBuf[80]; 
    LPVOID lpMsgBuf;
   
	// 获取错误编号
	DWORD dw = GetLastError();

	// FormatMessage功能就是将GetLastError得到的错误信息（这个错误信息是数字代号）转化成字符串信息的函数
    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, // DWORD dwFlags  错误信息类型标志
        NULL,                                 // LPCVOID lpSource 指定本地信息定义 其依赖于第一个参数的设置
        dw,                                   // DWORD dwMessageId 指定被请求的消息标示,如果dwFlags设置为FORMAT_MESSAGE_FROM_STRING 这个参数将被忽略
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // DWORD dwLanguageId 指定被请求的消息的语言标示
        (LPTSTR) &lpMsgBuf,                   // LPSTR lpBuffer 用来保存格式话后的消息字符串的地址
        0,                                    // DWORD nSize 如果第一个参数为指定FORMAT_MESSAGE_ALLOCATE_BUFFER 这个参数就是字符串的最大长度
		NULL );                               // 

	wsprintf(szBuf,"%s 出错信息 (出错码＝%d): %s", FunctionName, dw, lpMsgBuf); 

	memcpy(FunctionName,szBuf,80);

    LocalFree(lpMsgBuf);
}