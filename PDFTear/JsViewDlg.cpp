// JsViewDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PDFTear.h"
#include "JsViewDlg.h"
#include "zlib.h"

#pragma comment(lib,"zlib.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CJsViewDlg dialog


CJsViewDlg::CJsViewDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CJsViewDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CJsViewDlg)
	m_StartPos = _T("");
	m_EndPos = _T("");
	m_Info = _T("");
	//}}AFX_DATA_INIT
}


void CJsViewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CJsViewDlg)
	DDX_Text(pDX, IDC_EDIT_START, m_StartPos);
	DDX_Text(pDX, IDC_EDIT_END, m_EndPos);
	DDX_Text(pDX, IDC_EDIT_INFO, m_Info);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CJsViewDlg, CDialog)
	//{{AFX_MSG_MAP(CJsViewDlg)
	ON_BN_CLICKED(IDC_BUTTON_DECODE, OnButtonDecode)
	ON_BN_CLICKED(IDC_BUTTON_CODE, OnButtonCode)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CJsViewDlg message handlers

void CJsViewDlg::OnButtonDecode() 
{
	// TODO: Add your control notification handler code here
	this->UpdateData(TRUE);
	DWORD start,end,length;
	int n,m; 
	n=strlen(m_StartPos);
	if(n==0)
	{
		MessageBox("请核对操作数!","提示",MB_OK);
		return;
	}
	sscanf(m_StartPos, "%x", &n); 
	sscanf(m_EndPos, "%x", &m); 
	start = n;
	end=m;
	length=end-start;

	unsigned char strSrc[1584];
	unsigned char strDst[2*1024]={0};
	unsigned long srcLen=sizeof(strSrc),dstLen=sizeof(strDst);
	CString strc;
	
	DWORD dread;
	HANDLE hFILE;
	CString path;
	this->GetWindowText(path);
	hFILE=CreateFile(path,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hFILE==INVALID_HANDLE_VALUE)
		return;
	SetFilePointer(hFILE,start,0,FILE_BEGIN);
	ReadFile(hFILE,strSrc,length,&dread,NULL);
	
	//解压缩
	uncompress(strDst,&dstLen,strSrc,srcLen);
	strc.Format("%s",strDst);
	
	m_Info+=strc;
	CloseHandle(hFILE);
	this->UpdateData(FALSE);
}

void CJsViewDlg::OnButtonCode() 
{
	// TODO: Add your control notification handler code here
	MessageBox("完善中....","提示",MB_OK);
	/*this->UpdateData(TRUE);
	DWORD start,end,length;
	int n,m; 
	n=strlen(m_StartPos);
	if(n==0)
	{
		MessageBox("请核对操作数!","提示",MB_OK);
		return;
	}
	sscanf(m_StartPos, "%x", &n); 
	sscanf(m_EndPos, "%x", &m); 
	start = n;
	end=m;
	length=end-start;

	unsigned char strSrc[1584];
	unsigned char buff[2*1024]={0},strDst[2*1024]={0};
	unsigned long srcLen=sizeof(strSrc),bufLen=sizeof(buff),dstLen=sizeof(strDst);
	CString strc,strc2,strc3;
	
	DWORD dread;
	HANDLE hFILE;
	hFILE=CreateFile("D:\\Exploit.pdf",GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hFILE==INVALID_HANDLE_VALUE)
		return;
	SetFilePointer(hFILE,0x336,0,FILE_BEGIN);
	ReadFile(hFILE,strSrc,0x3B0,&dread,NULL);

	strc.Format("Src string:%s\nLength:%d\n===================\n",strSrc,srcLen);
	//压缩
	compress(buff,&bufLen,strSrc,srcLen);
	strc2.Format("\nAfter Compressed Length:%d\nCompressed String:%s\n==============\n",bufLen,buff);
	
	//解压缩
	uncompress(strDst,&dstLen,strSrc,srcLen);
	strc3.Format("\nAfter UnCompressed Length:%d\nUnCompressed String:%s\n",dstLen,strDst);
	
	AfxMessageBox(strc+strc3); */
}

BOOL CJsViewDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	SetIcon(AfxGetApp()->LoadIcon(IDI_ICON_PAYLOAD),TRUE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
