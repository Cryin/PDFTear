// FilePdf.cpp: implementation of the CFilePdf class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FilePdf.h"
#include <sstream>   


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


#include "FileFunction.h"


PdfVersion *pVersion;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFilePdf::CFilePdf(CString Name)
{
	strcpy(FileName,Name);
}

CFilePdf::~CFilePdf()
{

}
//解析PDF文件格式
BOOL CFilePdf::PdfDetection()
{
	int FileSec;
	if (GetFileAttributes(FileName)==-1)
	{
		return false;
	}
    ReadFileBuffer(FileName,FileSize,FileSec,FileBuffer);
	
	//解析PDF文件版本----------------------------------------------
	pVersion =(PdfVersion *)FileBuffer;
	if(strncmp((char *)pVersion->user,"%PDF",4)!=0)
	{
		sprintf(PdfFileStatus.PdfVersion,"%s","Unknow");
		return FALSE;
	}
	char vers[4];
	strncpy(vers,pVersion->version,4);
	vers[4]='\0';
	double d_vers=atof(&vers[1]);
	if(d_vers>1.8)
	{
		sprintf(PdfFileStatus.PdfVersion,"%s","Unknow");
	}

	else
	{
		sprintf(PdfFileStatus.PdfVersion,"%s","PDF");
		strcat(PdfFileStatus.PdfVersion,vers);
	}
	//---------------------------------------------------------------
	
	//定义PDF文件各关键字段计数器------------------------------------
	int stream=0,endstream=0,streamlength=0;
	int openaction=0,javascript=0,jspos=0;
	int obj=0,endobj=0,objlength=0;
	int xref=0,startxref=0,objinxref=0;
	int richmedia=0;
	int Launch=0;
	int JBIG2Decode=0;
	int FlateDecode=0;
	int Page=0;
	int AcroForm=0;
	int trailer=0;
	int Encrypt=0;
	int ObjStm=0;
	
	//---------------------------------------------------------------
	
	//查找PDF文件各关键字段------------------------------------------
	for(int i=0;i<FileSize;i++)
	{
		// 查找stream.
		if(  
			(FileBuffer[i+2]==0x73) && ( FileBuffer[i+3]==0x74) && 
			(FileBuffer[i+4]==0x72) && ( FileBuffer[i+5]==0x65) &&
			(FileBuffer[i+6]==0x61) && ( FileBuffer[i+7]==0x6D)
		  )
		{
			stream=stream+1;
		}
		
		// 查找endstream.
		if(  
			(FileBuffer[i]  ==0x6E) && ( FileBuffer[i+1]==0x64) && 
			(FileBuffer[i+2]==0x73) && ( FileBuffer[i+3]==0x74) && 
			(FileBuffer[i+4]==0x72) && ( FileBuffer[i+5]==0x65) &&
			(FileBuffer[i+6]==0x61) && ( FileBuffer[i+7]==0x6D)
			)
		{
			endstream=endstream+1;
		}
		
		// 查找OpenAction
		if( 
			(FileBuffer[i-2]==0x4F) && ( FileBuffer[i-1]==0x70) &&
			(FileBuffer[i]  ==0x65) && ( FileBuffer[i+1]==0x6E) &&  
			(FileBuffer[i+2]==0x41) && ( FileBuffer[i+3]==0x63) &&  
			(FileBuffer[i+4]==0x74) && ( FileBuffer[i+5]==0x69) &&
			(FileBuffer[i+6]==0x6F) && ( FileBuffer[i+7]==0x6E)
			)
		{
			openaction=openaction+1;
			
		}

		if(  // 查找JavaScript
			(FileBuffer[i-2]==0x4A) && ( FileBuffer[i-1]==0x61) &&
			(FileBuffer[i]  ==0x76) && ( FileBuffer[i+1]==0x61) &&  
			(FileBuffer[i+2]==0x53) && ( FileBuffer[i+3]==0x63) &&  
			(FileBuffer[i+4]==0x72) && ( FileBuffer[i+5]==0x69) &&
			(FileBuffer[i+6]==0x70) && ( FileBuffer[i+7]==0x74)
			)
		{
			javascript=javascript+1;
			
		}
		//  查找obj.
		if( (FileBuffer[i]  ==0x20) && ( FileBuffer[i+1]==0x6F) && 
			(FileBuffer[i+2]==0x62) && ( FileBuffer[i+3]==0x6A)
			)
		{
			obj=obj+1;
		}
		// 查找.endobj.
		if(
			( FileBuffer[i]  ==0x65) &&	(FileBuffer[i+1]==0x6E) && 
			( FileBuffer[i+2]==0x64) && (FileBuffer[i+3]==0x6F) && 
			( FileBuffer[i+4]==0x62) && (FileBuffer[i+5]==0x6A)
			)
		{
			endobj=endobj+1;
		}
		//查找startxref交叉引用表
		if (
			(FileBuffer[i]  ==0x73) && (FileBuffer[i+1]==0x74) &&
			(FileBuffer[i+2]==0x61) && (FileBuffer[i+3]==0x72) &&
			(FileBuffer[i+4]==0x74) && (FileBuffer[i+5]==0x78) &&
			(FileBuffer[i+6]==0x72) && (FileBuffer[i+7]==0x65) && 
			(FileBuffer[i+8]==0x66)
			)
		{
			startxref=startxref+1;
			
		}
		//查找交叉引用表的开始标识xref.
		if (
			(FileBuffer[i]  ==0x0A)&& (FileBuffer[i+1]==0x78) &&
			(FileBuffer[i+2]==0x72) && (FileBuffer[i+3]==0x65) && 
			(FileBuffer[i+4]==0x66) && (FileBuffer[i+5]==0x0A)
			)
		{
			xref=xref+1;
		}
		//查找  n.的位置objinxref
		//计算在交叉表中的每个对像
		if (
			(FileBuffer[i]  ==0x20) && (FileBuffer[i+1]==0x6E) && 
			((FileBuffer[i+2]==0x0D) || (FileBuffer[i+2]==0x20))
			)
		{
			objinxref=objinxref+1;
		}
		//查找RichMedia
		if (
			(FileBuffer[i]  ==0x52) && (FileBuffer[i+1]==0x69) &&
			(FileBuffer[i+2]==0x63) && (FileBuffer[i+3]==0x68) && 
			(FileBuffer[i+4]==0x4D) && (FileBuffer[i+5]==0x65) &&
			(FileBuffer[i+6]==0x64) && (FileBuffer[i+7]==0x69) &&
			(FileBuffer[i+8]==0x61)
			)
		{
			richmedia=richmedia+1;
		}
		//查找Launch
		if (
			(FileBuffer[i]  ==0x4C) && (FileBuffer[i+1]==0x61) &&
			(FileBuffer[i+2]==0x75) && (FileBuffer[i+3]==0x6E) && 
			(FileBuffer[i+4]==0x63) && (FileBuffer[i+5]==0x68)
			)
		{
			Launch=Launch+1;
		}
		//查找JBIG2Decode
		if (
			(FileBuffer[i]  ==0x4A) && (FileBuffer[i+1]==0x42) &&
			(FileBuffer[i+2]==0x49) && (FileBuffer[i+3]==0x47) && 
			(FileBuffer[i+4]==0x02) && (FileBuffer[i+5]==0x44) &&
			(FileBuffer[i+6]==0x65) && (FileBuffer[i+7]==0x63) &&
			(FileBuffer[i+8]==0x6F)
			)
		{
			JBIG2Decode=JBIG2Decode+1;
		}
		//查找/JS
		if (
			(FileBuffer[i]  ==0x2F) && (FileBuffer[i+1]==0x4A) &&
			(FileBuffer[i+2]==0x53) && (FileBuffer[i+3]==0x20) 
			)
		{
			jspos=jspos+1;
		}
		//查找AcroForm
		if (
			(FileBuffer[i]  ==0x41) && (FileBuffer[i+1]==0x63) &&
			(FileBuffer[i+2]==0x72) && (FileBuffer[i+3]==0x6F) && 
			(FileBuffer[i+4]==0x46) && (FileBuffer[i+5]==0x6F) &&
			(FileBuffer[i+6]==0x72) && (FileBuffer[i+7]==0x6D) 
			)
		{
			AcroForm=AcroForm+1;
		}
		//查找ObjStm
		if (
			(FileBuffer[i]  ==0x4F) && (FileBuffer[i+1]==0x62) &&
			(FileBuffer[i+2]==0x6A) && (FileBuffer[i+3]==0x53) && 
			(FileBuffer[i+4]==0x74) && (FileBuffer[i+5]==0x6D) 
			)
		{
			ObjStm=ObjStm+1;
		}

		//查找Encrypt
		if (
			(FileBuffer[i]  ==0x45) && (FileBuffer[i+1]==0x6E) &&
			(FileBuffer[i+2]==0x63) && (FileBuffer[i+3]==0x72) && 
			(FileBuffer[i+4]==0x79) && (FileBuffer[i+5]==0x70) && 
			(FileBuffer[i+6]==0x74)   
			)
		{
			Encrypt=Encrypt+1;
		}

		//查找Page
		if (
			(FileBuffer[i]  ==0x50) && (FileBuffer[i+1]==0x61) &&
			(FileBuffer[i+2]==0x67) && (FileBuffer[i+3]==0x65) 
			)
		{
			Page=Page+1;
		}
		//查找[/FlateDecode]
		if (
			(FileBuffer[i]  ==0x5B) && (FileBuffer[i+1]==0x2F) &&
			(FileBuffer[i+2]==0x46) && (FileBuffer[i+3]==0x6C) && 
			(FileBuffer[i+4]==0x61) && (FileBuffer[i+5]==0x74) &&
			(FileBuffer[i+6]==0x65) && (FileBuffer[i+7]==0x64) &&
			(FileBuffer[i+8]==0x6F) 
			)
		{
			FlateDecode=FlateDecode+1;
		}
	}

   
		if(endobj!=obj)
		{
			sprintf(PdfFileStatus.Pdfsecurity,"%s","Malicious");
			
		}
		else if(openaction!=0||javascript!=0)
		{
			if(javascript!=0)
			{
				sprintf(PdfFileStatus.Pdfsecurity,"%s","Malicious");
			}
			else
			{
				sprintf(PdfFileStatus.Pdfsecurity,"%s","Good");
			}
			
		}
		else if (endobj>objinxref||objinxref==0)
		{
			sprintf(PdfFileStatus.Pdfsecurity,"%s","Malicious");
		}
		else
		{
			sprintf(PdfFileStatus.Pdfsecurity,"%s","Good");
			
		}
		
		
		sprintf(PdfFileStatus.PdfSize,"%d",FileSize);
		sprintf(PdfFileStatus.PdfObject,"%d",endobj);
		sprintf(PdfFileStatus.PdfOpenAction,"%d",openaction);
		sprintf(PdfFileStatus.PdfJavaScipt,"%d",javascript);
		
		sprintf(PdfFileStatus.PdfLaunch,"%d",Launch);
		sprintf(PdfFileStatus.PdfRichMedia,"%d",richmedia);
		
		sprintf(PdfFileStatus.PdfStream,"%d",endstream);
		sprintf(PdfFileStatus.PdfStartxref,"%d",startxref);
		sprintf(PdfFileStatus.PdfXref,"%d",xref);

		sprintf(PdfFileStatus.PdfAcroForm,"%d",AcroForm);

		sprintf(PdfFileStatus.PdfEncrypt,"%d",Encrypt);

		sprintf(PdfFileStatus.PdfJBIG2Decode,"%d",JBIG2Decode);

		sprintf(PdfFileStatus.PdfObjStm,"%d",ObjStm);

		sprintf(PdfFileStatus.PdfJS,"%d",jspos);

		sprintf(PdfFileStatus.PdfPage,"%d",Page);
		sprintf(PdfFileStatus.PdfFlateDecode,"%d",FlateDecode);
		
		return TRUE;
}
