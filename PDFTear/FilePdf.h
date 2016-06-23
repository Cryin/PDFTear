// FilePdf.h: interface for the CFilePdf class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILEPDF_H__1FB8958B_5DAB_4567_AE34_65B63A072646__INCLUDED_)
#define AFX_FILEPDF_H__1FB8958B_5DAB_4567_AE34_65B63A072646__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


struct PdfVersion
{
	unsigned char user[4];   //pdf标识
	char version[4];		 //PDF版本号
};  //8节 

class CFilePdf  
{
public:
	BOOL PdfDetection();
	CFilePdf(CString Name);
	virtual ~CFilePdf();

	 char  FileName[255];     //文件名
     int   FileSize;          //文件大小
     BYTE* FileBuffer;        //文件内容



	struct  PdfStatus
	{
		char PdfName[255];          //文件名
		char PdfVersion[20];        //版本号
		char PdfObject[20];         //文件OBJ对象
		char PdfStream[20];         //文件PdfStream
		char PdfEncrypt[20];		//Encrypt
		char PdfPage[20];			//Page
		char Pdftrailer[20];		//trailer
		char PdfXref[20];			//Xref大小
		char PdfStartxref[20];      //文件Startxref数量
		char PdfOpenAction[20];     //文件OpenAction
		char PdfAcroForm[20];       //文件AcroForm
		char PdfJavaScipt[20];      //文件JavaScipt
		char PdfFlateDecode[20];	//文件/FlateDecode
		char PdfJS[20];				//文件/JS
		char PdfRichMedia[20];       //文件Pdf嵌入的flash文件
		char PdfLaunch[20];			 //文件中Launch
		char PdfJBIG2Decode[20];	//文件中JBIG2Decode
		char PdfObjStm[20];			//文件中ObjStm
		char PdfSize[20];           //文件大小
		char Pdfsecurity[20];		//pdf是否为可疑文件
	}PdfFileStatus;



};

#endif // !defined(AFX_FILEPDF_H__1FB8958B_5DAB_4567_AE34_65B63A072646__INCLUDED_)
