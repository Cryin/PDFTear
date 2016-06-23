// DecodeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PDFTear.h"
#include "DecodeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDecodeDlg dialog


CDecodeDlg::CDecodeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDecodeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDecodeDlg)
	m_path = _T("");
	m_target = _T("");
	m_info = _T("");
	//}}AFX_DATA_INIT
}


void CDecodeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDecodeDlg)
	DDX_Text(pDX, IDC_EDIT_PATH, m_path);
	DDX_Text(pDX, IDC_EDIT_TARGET, m_target);
	DDX_Text(pDX, IDC_EDIT_INFO, m_info);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDecodeDlg, CDialog)
	//{{AFX_MSG_MAP(CDecodeDlg)
	ON_BN_CLICKED(IDC_BUTTON_BROWSER, OnButtonBrowser)
	ON_BN_CLICKED(IDC_BUTTON_BYE, OnButtonBye)
	ON_BN_CLICKED(IDC_BUTTON_OK, OnButtonOk)
	ON_BN_CLICKED(IDC_RADIO_XOR, OnRadioXor)
	ON_BN_CLICKED(IDC_RADIO_ADDXOR, OnRadioAddxor)
	ON_BN_CLICKED(IDC_RADIO_DECXOR, OnRadioDecxor)
	ON_BN_CLICKED(IDC_RADIO_BYTE, OnRadioByte)
	ON_BN_CLICKED(IDC_RADIO_WORD, OnRadioWord)
	ON_BN_CLICKED(IDC_RADIO_DWORD, OnRadioDword)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDecodeDlg message handlers

BOOL CDecodeDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	SetIcon(AfxGetApp()->LoadIcon(IDI_ICON_PAYLOAD),TRUE);
	((CButton *)GetDlgItem(IDC_RADIO_XOR))->SetCheck(TRUE);//选上
	((CButton *)GetDlgItem(IDC_RADIO_BYTE))->SetCheck(TRUE);//选上
	m_terget=1;
	m_type=1;
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDecodeDlg::OnButtonBrowser() 
{
	// TODO: Add your control notification handler code here
	CFileDialog dlgFileOpen(TRUE,".bin",NULL,OFN_PATHMUSTEXIST | OFN_HIDEREADONLY,"(*.bin)|*.bin||");
	int StructSize = 0;
	DWORD dwVersion = GetVersion();
	if(dwVersion < 0x80000000)			//winnt/xp
		StructSize = 88;				//show new dialog
	else								//win9x
		StructSize = 76;				//show old dialog
	
	dlgFileOpen.m_ofn.lStructSize= StructSize;
	if(dlgFileOpen.DoModal() == IDOK)
	{
		m_path=dlgFileOpen.m_ofn.lpstrFile;
	}
	this->UpdateData(false);
}

void CDecodeDlg::OnButtonBye() 
{
	// TODO: Add your control notification handler code here
	CDialog::OnCancel();
}

void CDecodeDlg::OnButtonOk() 
{
	// TODO: Add your control notification handler code here
	this->UpdateData(true);
	if(m_path.IsEmpty())
	{
		MessageBox("请选择文件!","提示",MB_OK);
		return;
	}
	switch (m_type)
	{
	case 1:
		XorWork();
		break;
	case 2:
		XorAddWork();
		break;
	case 3:
		XorDecWork();
		break;

	}
}

void CDecodeDlg::OnRadioXor() 
{
	// TODO: Add your control notification handler code here
	m_type=1;
	GetDlgItem(IDC_RADIO_WORD)->EnableWindow(TRUE);
	GetDlgItem(IDC_RADIO_DWORD)->EnableWindow(TRUE);
}

void CDecodeDlg::OnRadioAddxor() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_RADIO_WORD)->EnableWindow(FALSE);
	GetDlgItem(IDC_RADIO_DWORD)->EnableWindow(FALSE);
	m_type=2;
}

void CDecodeDlg::OnRadioDecxor() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_RADIO_WORD)->EnableWindow(FALSE);
	GetDlgItem(IDC_RADIO_DWORD)->EnableWindow(FALSE);
	m_type=3;
}

void CDecodeDlg::OnRadioByte() 
{
	// TODO: Add your control notification handler code here
	m_terget=1;
}

void CDecodeDlg::OnRadioWord() 
{
	// TODO: Add your control notification handler code here
	m_terget=2;
}

void CDecodeDlg::OnRadioDword() 
{
	// TODO: Add your control notification handler code here
	m_terget=3;
}

void CDecodeDlg::XorWork()
{
	if(m_target.IsEmpty())
	{
		MessageBox("操作数不能为空!","提示",MB_OK);
		return;
	}
	switch(m_terget)
	{
	case 1:
		XorByte();
		break;
	case 2:
		XorWord();
		break;
	case 3:
		XorDWord();
		break;

	}
}
void CDecodeDlg::XorByte()
{
	HANDLE hFile;
	HANDLE DecFile;
	char* filebuf;
	char* decodebuf;
	DWORD filesize;
	DWORD Byteread;
	BYTE target;
	char opcode[128];
	int n; 
	n=strlen(m_target);
	if(n!=2)
	{
		MessageBox("请核对操作数!","提示",MB_OK);
		return;
	}
	sscanf(m_target, "%x", &n); 
	target = n;
	m_info="        ";
	hFile=CreateFile(m_path,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hFile==INVALID_HANDLE_VALUE)
	{
		MessageBox("打开文件出错!","提示",MB_OK);
		return;
	}
	filesize=GetFileSize(hFile,NULL);
	filebuf=(char*)VirtualAlloc(NULL,filesize,MEM_COMMIT,PAGE_READWRITE);
	decodebuf=(char*)VirtualAlloc(NULL,filesize,MEM_COMMIT,PAGE_READWRITE);
	ReadFile(hFile,filebuf,filesize,&Byteread,NULL);
	DWORD Index=0;
	
	for(Index=0;Index<filesize;Index++)
	{
		if(Index%16==0&&Index!=0)
		{
			m_info+="\r\n        ";
		}
		decodebuf[Index]=filebuf[Index]^target;
		wsprintf(opcode,"%02X ",(BYTE)decodebuf[Index]);
		m_info+=opcode;
	}
	this->UpdateData(false);
	CString strpath;
	strpath=m_path;
	strpath+="_decode";
	DecFile=CreateFile(strpath,GENERIC_WRITE,FILE_SHARE_READ,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	if(DecFile==INVALID_HANDLE_VALUE)
	{
		MessageBox("打开文件出错!","提示",MB_OK);
		return;
	}
	WriteFile(DecFile,decodebuf,filesize,&Byteread,NULL);
	//MessageBox("操作成功!","提示",MB_OK);
	CloseHandle(DecFile);
	CloseHandle(hFile);
	VirtualFree(filebuf,filesize,MEM_RELEASE); 
	VirtualFree(decodebuf,filesize,MEM_RELEASE); 
}

WORD CDecodeDlg::StringToWord(char *Text)
{
	 DWORD AsmDwordNum=(DWORD)Text;
    WORD DwordNum=0;
	
	_asm{
		PUSHAD
			PUSHF
			XOR ECX,ECX
			XOR EAX,EAX
			XOR EDI,EDI
			MOV EDI,4H
			MOV ESI,AsmDwordNum
_start:
		MOV CL,[ESI]
			CMP CL,30H
			JL _lower
			CMP CL,39H
			JG _upper
			SUB CL,30H
			JMP _jmp1
_upper:
		SUB CL,37H
			JMP _jmp1
_lower:
_jmp1:
		ADD EAX,ECX
			CMP EDI,1
			JZ _out
			SHL EAX,4H
_out:
		INC ESI
			DEC EDI
			JNZ _start
			MOV DwordNum,AX
			POPF
			POPAD
	}
	
	return DwordNum;
}

void CDecodeDlg::XorWord()
{
	HANDLE hFile;
	HANDLE DecFile;
	char* filebuf;
	char* decodebuf;
	DWORD filesize;
	DWORD Byteread;
	WORD target;
	BYTE high,low;					//高低字节
	char opcode[128];
	int n; 
	n=strlen(m_target);
	if(n!=4)
	{
		MessageBox("请核对操作数!","提示",MB_OK);
		return;
	}
	sscanf(m_target, "%x", &n); 
	target = n;
	m_info="        ";
	hFile=CreateFile(m_path,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hFile==INVALID_HANDLE_VALUE)
	{
		MessageBox("打开文件出错!","提示",MB_OK);
		return;
	}
	filesize=GetFileSize(hFile,NULL);
	filebuf=(char*)VirtualAlloc(NULL,filesize,MEM_COMMIT,PAGE_READWRITE);
	decodebuf=(char*)VirtualAlloc(NULL,filesize,MEM_COMMIT,PAGE_READWRITE);
	ReadFile(hFile,filebuf,filesize,&Byteread,NULL);
	DWORD Index=0;
	low=LOBYTE(target);
	high=HIBYTE(target);
	for(Index=0;Index<filesize;Index=Index+2)
	{
		if(Index%16==0&&Index!=0)
		{
			m_info+="\r\n        ";
		}
		decodebuf[Index]=filebuf[Index]^high;
		decodebuf[Index+1]=filebuf[Index+1]^low;
		wsprintf(opcode,"%02X ",(BYTE)decodebuf[Index]);
		m_info+=opcode;
		wsprintf(opcode,"%02X ",(BYTE)decodebuf[Index+1]);
		m_info+=opcode;
	}
	this->UpdateData(false);
	CString strpath;
	strpath=m_path;
	strpath+="_decode";
	DecFile=CreateFile(strpath,GENERIC_WRITE,FILE_SHARE_READ,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	if(DecFile==INVALID_HANDLE_VALUE)
	{
		MessageBox("打开文件出错!","提示",MB_OK);
		return;
	}
	WriteFile(DecFile,decodebuf,filesize,&Byteread,NULL);
//	MessageBox("操作成功!","提示",MB_OK);
	CloseHandle(DecFile);
	CloseHandle(hFile);
	VirtualFree(filebuf,filesize,MEM_RELEASE); 
	VirtualFree(decodebuf,filesize,MEM_RELEASE); 
}

void CDecodeDlg::XorDWord()
{
	HANDLE hFile;
	HANDLE DecFile;
	char* filebuf;
	char* decodebuf;
	DWORD filesize;
	DWORD Byteread;
	DWORD target;
	BYTE lhigh,llow,hhigh,hlow;					//高低字节
	char opcode[128];
	int n; 
	n=strlen(m_target);
	if(n!=8)
	{
		MessageBox("请核对操作数!","提示",MB_OK);
		return;
	}
	sscanf(m_target, "%x", &n); 
	target = n;
	m_info="        ";
	hFile=CreateFile(m_path,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hFile==INVALID_HANDLE_VALUE)
	{
		MessageBox("打开文件出错!","提示",MB_OK);
		return;
	}
	filesize=GetFileSize(hFile,NULL);
	filebuf=(char*)VirtualAlloc(NULL,filesize,MEM_COMMIT,PAGE_READWRITE);
	decodebuf=(char*)VirtualAlloc(NULL,filesize,MEM_COMMIT,PAGE_READWRITE);
	ReadFile(hFile,filebuf,filesize,&Byteread,NULL);
	DWORD Index=0;
	llow=LOBYTE(LOWORD(target));
	lhigh=HIBYTE(LOWORD(target));
	hlow=LOBYTE(HIWORD(target));
	hhigh=HIBYTE(HIWORD(target));
	for(Index=0;Index<filesize;Index=Index+4)
	{
		if(Index%16==0&&Index!=0)
		{
			m_info+="\r\n        ";
		}
		decodebuf[Index]=filebuf[Index]^hhigh;
		decodebuf[Index+1]=filebuf[Index+1]^hlow;
		decodebuf[Index+2]=filebuf[Index+2]^lhigh;
		decodebuf[Index+3]=filebuf[Index+3]^llow;

		wsprintf(opcode,"%02X ",(BYTE)decodebuf[Index]);
		m_info+=opcode;
		wsprintf(opcode,"%02X ",(BYTE)decodebuf[Index+1]);
		m_info+=opcode;
		wsprintf(opcode,"%02X ",(BYTE)decodebuf[Index+2]);
		m_info+=opcode;
		wsprintf(opcode,"%02X ",(BYTE)decodebuf[Index+3]);
		m_info+=opcode;
	}
	this->UpdateData(false);
	CString strpath;
	strpath=m_path;
	strpath+="_decode";
	DecFile=CreateFile(strpath,GENERIC_WRITE,FILE_SHARE_READ,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	if(DecFile==INVALID_HANDLE_VALUE)
	{
		MessageBox("打开文件出错!","提示",MB_OK);
		return;
	}
	WriteFile(DecFile,decodebuf,filesize,&Byteread,NULL);
//	MessageBox("操作成功!","提示",MB_OK);
	CloseHandle(DecFile);
	CloseHandle(hFile);
	VirtualFree(filebuf,filesize,MEM_RELEASE); 
	VirtualFree(decodebuf,filesize,MEM_RELEASE);
}

void CDecodeDlg::XorAddWork()
{
	if(m_target.IsEmpty())
	{
		MessageBox("操作数不能为空!","提示",MB_OK);
		return;
	}
	switch(m_terget)
	{
	case 1:
		XorAddByte();
		break;

	}
}

void CDecodeDlg::XorAddByte()
{
	HANDLE hFile;
	HANDLE DecFile;
	char* filebuf;
	char* decodebuf;
	DWORD filesize;
	DWORD Byteread;
	BYTE target;
	char opcode[128];
	int n; 
	n=strlen(m_target);
	if(n!=2)
	{
		MessageBox("请核对操作数!","提示",MB_OK);
		return;
	}
	sscanf(m_target, "%x", &n); 
	target = n;
	m_info="        ";
	hFile=CreateFile(m_path,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hFile==INVALID_HANDLE_VALUE)
	{
		MessageBox("打开文件出错!","提示",MB_OK);
		return;
	}
	filesize=GetFileSize(hFile,NULL);
	filebuf=(char*)VirtualAlloc(NULL,filesize,MEM_COMMIT,PAGE_READWRITE);
	decodebuf=(char*)VirtualAlloc(NULL,filesize,MEM_COMMIT,PAGE_READWRITE);
	ReadFile(hFile,filebuf,filesize,&Byteread,NULL);
	DWORD Index=0;
	
	for(Index=0;Index<filesize;Index++)
	{
		if(Index%16==0&&Index!=0)
		{
			m_info+="\r\n        ";
		}
		decodebuf[Index]=filebuf[Index]^target;
		wsprintf(opcode,"%02X ",(BYTE)decodebuf[Index]);
		m_info+=opcode;
		target++;
	}
	this->UpdateData(false);
	CString strpath;
	strpath=m_path;
	strpath+="_decode";
	DecFile=CreateFile(strpath,GENERIC_WRITE,FILE_SHARE_READ,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	if(DecFile==INVALID_HANDLE_VALUE)
	{
		MessageBox("打开文件出错!","提示",MB_OK);
		return;
	}
	WriteFile(DecFile,decodebuf,filesize,&Byteread,NULL);
	//MessageBox("操作成功!","提示",MB_OK);
	CloseHandle(DecFile);
	CloseHandle(hFile);
	VirtualFree(filebuf,filesize,MEM_RELEASE); 
	VirtualFree(decodebuf,filesize,MEM_RELEASE); 
}

void CDecodeDlg::XorDecWork()
{
	if(m_target.IsEmpty())
	{
		MessageBox("操作数不能为空!","提示",MB_OK);
		return;
	}
	switch(m_terget)
	{
	case 1:
		XorDecByte();
		break;

	}
}

void CDecodeDlg::XorDecByte()
{
	HANDLE hFile;
	HANDLE DecFile;
	char* filebuf;
	char* decodebuf;
	DWORD filesize;
	DWORD Byteread;
	BYTE target;
	char opcode[128];
	int n; 
	n=strlen(m_target);
	if(n!=2)
	{
		MessageBox("请核对操作数!","提示",MB_OK);
		return;
	}
	sscanf(m_target, "%x", &n); 
	target = n;
	m_info="        ";
	hFile=CreateFile(m_path,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hFile==INVALID_HANDLE_VALUE)
	{
		MessageBox("打开文件出错!","提示",MB_OK);
		return;
	}
	filesize=GetFileSize(hFile,NULL);
	filebuf=(char*)VirtualAlloc(NULL,filesize,MEM_COMMIT,PAGE_READWRITE);
	decodebuf=(char*)VirtualAlloc(NULL,filesize,MEM_COMMIT,PAGE_READWRITE);
	ReadFile(hFile,filebuf,filesize,&Byteread,NULL);
	DWORD Index=0;
	
	for(Index=0;Index<filesize;Index++)
	{
		if(Index%16==0&&Index!=0)
		{
			m_info+="\r\n        ";
		}
		decodebuf[Index]=filebuf[Index]^target;
		wsprintf(opcode,"%02X ",(BYTE)decodebuf[Index]);
		m_info+=opcode;
		target--;
	}
	this->UpdateData(false);
	CString strpath;
	strpath=m_path;
	strpath+="_decode";
	DecFile=CreateFile(strpath,GENERIC_WRITE,FILE_SHARE_READ,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	if(DecFile==INVALID_HANDLE_VALUE)
	{
		MessageBox("打开文件出错!","提示",MB_OK);
		return;
	}
	WriteFile(DecFile,decodebuf,filesize,&Byteread,NULL);
	//MessageBox("操作成功!","提示",MB_OK);
	CloseHandle(DecFile);
	CloseHandle(hFile);
	VirtualFree(filebuf,filesize,MEM_RELEASE); 
	VirtualFree(decodebuf,filesize,MEM_RELEASE); 
}
