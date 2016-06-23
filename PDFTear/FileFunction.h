
#include "stdafx.h"










/************************************************************************************************
 * 函数名称: GetDirectoriesFiles
 * 功能描述: 遍历目录 获取目录与文件
 * 参数列表: 
           Path:        文件目录
           Suffix:      后缀
           FileNames:   保存文件名
           Directories: 保存目录名
* 返回值:  FileNames , Directories
*************************************************************************************************/
void GetDirectoriesFiles(CString Path, CString Suffix,CStringArray &FileNames,CStringArray &Directories);


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
*************************************************************************************************/
void ReadFileBuffer(char *Name, int &FileSize, int &FileSec, BYTE *&FileBuffer);



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
int splitstr(char *SourceStr, char Dot, char *TargetStr, bool front_back);
void substr(char *string,int t ,char *substring,bool top_end);
void cpyfile(char *FileName);




































/*=============================================================================================
参考的函数 FindFirstFile 与 FindNextFile
 
   //根据文件名查找文件 
   HANDLE FindFirstFile(
	  LPCTSTR lpFileName,               //搜索的文件句柄 函数执行的时候搜索的是此句柄的下一文件
	  LPWIN32_FIND_DATA lpFindFileData  //指向一个用于保存文件信息的结构体
	  );

   如果调用成功返回一个非0值
   调用失败 返回为0 可调用GetLastError来获取错误信息
   如果出错，返回一个INVALID_HANDLE_VALUE常数，一旦不再需要，应该用FindClose函数关闭这个句柄 
***********************************************************************************************/


