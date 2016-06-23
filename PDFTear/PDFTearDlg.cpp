// PDFTearDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PDFTear.h"
#include "PDFTearDlg.h"
#include "shlwapi.h"
#include "FilePdf.h"
#include "FileFunction.h"
#include "PayLoadDlg.h"
#include "DecodeDlg.h"
#include "JsViewDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma comment (lib,"shlwapi.lib")
/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About
void CheckPdfFormat(CString path, CPDFTearDlg *CTeardlg);

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPDFTearDlg dialog

CPDFTearDlg::CPDFTearDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPDFTearDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPDFTearDlg)
	m_fileName = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPDFTearDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPDFTearDlg)
	DDX_Control(pDX, IDC_LIST_VIEW, m_ListInfo);
	DDX_Text(pDX, IDC_EDIT_FILENAME, m_fileName);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPDFTearDlg, CDialog)
	//{{AFX_MSG_MAP(CPDFTearDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(IDR_FILE_OPENF, OnFileOpenf)
	ON_COMMAND(IDR_HELP_ABOUT, OnHelpAbout)
	ON_BN_CLICKED(IDC_BUTTON_BROWSER, OnButtonBrowser)
	ON_COMMAND(IDR_PAYLOAD_TOOL, OnPayloadTool)
	ON_COMMAND(IDR_DECODE_XOR, OnDecodeXor)
	ON_BN_CLICKED(IDC_BUTTON_DOWORK, OnButtonDowork)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_MENU_FILE, OnMenuFile)
	ON_COMMAND(IDR_JSVIEW_TOOL, OnJsviewTool)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPDFTearDlg message handlers

BOOL CPDFTearDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
/*	CString Str="2010-10-12";
	//为以后添加分栏留足够位置
	UINT Array[7];
	for(int i=0;i<4;i++)
	{
		Array[i]=10000+1;
	}
	m_StatusBar.Create(this);
	m_StatusBar.SetIndicators(Array,4);
	
	//先用两个栏
	for(int n=0;n<4;n++)
	{
		m_StatusBar.SetPaneInfo(n,Array[n],0,100);
	}
	m_StatusBar.SetPaneInfo(0,Array[0],0,0);
	m_StatusBar.SetPaneInfo(1,Array[1],0,676);
	m_StatusBar.SetPaneInfo(2,Array[2],0,0);
	m_StatusBar.SetPaneInfo(3,Array[3],0,0);

//	m_StatusBar.SetPaneText(1,"Open File...");
//	m_StatusBar.SetPaneText(2,"Size :");
	
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST,AFX_IDW_CONTROLBAR_LAST,0);*/
	
	m_ListInfo.SetExtendedStyle(LVS_EX_FLATSB|LVS_EX_FULLROWSELECT|LVS_EX_HEADERDRAGDROP|LVS_EX_ONECLICKACTIVATE|LVS_EX_GRIDLINES);

/*	LONG IStyle=m_ListInfo.SendMessage(LVM_GETEXTENDEDLISTVIEWSTYLE);
	IStyle|=LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP;
	m_ListInfo.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE,0,(LPARAM)IStyle);

	m_ListInfo.InsertColumn(0,"FileName",LVCFMT_LEFT,90,0);
	m_ListInfo.InsertColumn(1,"Version",LVCFMT_LEFT,80,0);
	m_ListInfo.InsertColumn(2,"Object",LVCFMT_LEFT,80,0);
	m_ListInfo.InsertColumn(3,"Stream",LVCFMT_LEFT,80,0);
	m_ListInfo.InsertColumn(4,"Xref",LVCFMT_LEFT,80,0);
	m_ListInfo.InsertColumn(5,"Startxref",LVCFMT_LEFT,80,0);
	m_ListInfo.InsertColumn(6,"OpenAction",LVCFMT_LEFT,90,0);

	m_ListInfo.InsertColumn(7,"JavaScript",LVCFMT_LEFT,90,0);

	m_ListInfo.InsertColumn(8,"RichMedia",LVCFMT_LEFT,90,0);
	m_ListInfo.InsertColumn(9,"Launch",LVCFMT_LEFT,80,0);
	m_ListInfo.InsertColumn(10,"Size",LVCFMT_LEFT,80,0);
	m_ListInfo.InsertColumn(11,"检测结果",LVCFMT_LEFT,80,0);*/

	m_ListInfo.InsertColumn(0,"Name",LVCFMT_CENTER,150,0);
	m_ListInfo.InsertColumn(1,"Value ",LVCFMT_CENTER,150,0);
	m_ListInfo.InsertColumn(2,"Offset",LVCFMT_CENTER,150,0);
	m_ListInfo.InsertColumn(3,"Size",LVCFMT_CENTER,100,0);
	m_ListInfo.InsertColumn(4,"Comment",LVCFMT_CENTER,120,0);

	//-让这个状态栏最终显示在对话框中-
	this->UpdateData(false);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CPDFTearDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CPDFTearDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CPDFTearDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CPDFTearDlg::OnFileOpenf() 
{
	// TODO: Add your command handler code here
	CFileDialog dlgFileOpen(TRUE,".pdf","Exploit.pdf",OFN_PATHMUSTEXIST | OFN_HIDEREADONLY,"(*.pdf)|*.pdf||");
	
	int StructSize = 0;
	DWORD dwVersion = GetVersion();
	if(dwVersion < 0x80000000)			//winnt/xp
		StructSize = 88;				//show new dialog
	else								//win9x
		StructSize = 76;				//show old dialog
	
	dlgFileOpen.m_ofn.lStructSize= StructSize;
	if(dlgFileOpen.DoModal() == IDOK)
	{
		m_fileName=dlgFileOpen.m_ofn.lpstrFile;
		m_IsFile=TRUE;
	}
	else
	{
		return;
	}
	this->UpdateData(false);
	
}

DWORD OpenFileThread(LPVOID lpParameter)
{
	CPDFTearDlg *CTeardlg=(CPDFTearDlg *)lpParameter;
	CTeardlg->m_ListInfo.DeleteAllItems();
	CString path=CTeardlg->m_fileName;
	CheckPdfFormat(path,CTeardlg);
	
	return 0;
}
void CPDFTearDlg::FindAllKeyWord()
{
	if(m_fileName.IsEmpty())
	{
		MessageBox("请选择文件","提示",MB_OK);
		return;
	}
//	m_StatusBar.SetPaneText(1,"File: "+m_fileName);
	HANDLE tHandle=CreateThread(NULL,NULL,(LPTHREAD_START_ROUTINE)OpenFileThread,(LPVOID)this,0,NULL); 
	
	DWORD dRet=WaitForSingleObject(tHandle,0);
	
	if(dRet!=WAIT_FAILED)
	{
		return;
	}
	else
	{
		MessageBox("解析文件错误!","提示",MB_OK);
	}

	

	
}

void CheckPdfFormat(CString path, CPDFTearDlg *CTeardlg)
{
//	int iPos;
	char FileName[255];

	splitstr(path.GetBuffer(strlen(path)),'\\',FileName,FALSE);
	CFilePdf* CPdf=new CFilePdf(path);
	CPdf->PdfDetection();
	
	CTeardlg->m_ListInfo.InsertItem(0,"");
	CTeardlg->m_ListInfo.SetItemText(0,0,"Version");
	CTeardlg->m_ListInfo.SetItemText(0,1,CPdf->PdfFileStatus.PdfVersion);

	CTeardlg->m_ListInfo.InsertItem(1,"");
	CTeardlg->m_ListInfo.SetItemText(1,0,"Object");
	CTeardlg->m_ListInfo.SetItemText(1,1,CPdf->PdfFileStatus.PdfObject);
	

	CTeardlg->m_ListInfo.InsertItem(2,"");
	CTeardlg->m_ListInfo.SetItemText(2,0,"Stream");
	CTeardlg->m_ListInfo.SetItemText(2,1,CPdf->PdfFileStatus.PdfStream);
	

	CTeardlg->m_ListInfo.InsertItem(3,"");
	CTeardlg->m_ListInfo.SetItemText(3,0,"Xref");
	CTeardlg->m_ListInfo.SetItemText(3,1,CPdf->PdfFileStatus.PdfXref);
	
	CTeardlg->m_ListInfo.InsertItem(4,"");
	CTeardlg->m_ListInfo.SetItemText(4,0,"Startxref");
	CTeardlg->m_ListInfo.SetItemText(4,1,CPdf->PdfFileStatus.PdfStartxref);

	CTeardlg->m_ListInfo.InsertItem(5,"");
	CTeardlg->m_ListInfo.SetItemText(5,0,"OpenAction");
	CTeardlg->m_ListInfo.SetItemText(5,1,CPdf->PdfFileStatus.PdfOpenAction);

	CTeardlg->m_ListInfo.InsertItem(6,"");
	CTeardlg->m_ListInfo.SetItemText(6,0,"JavaScipt");
	CTeardlg->m_ListInfo.SetItemText(6,1,CPdf->PdfFileStatus.PdfJavaScipt);

	CTeardlg->m_ListInfo.InsertItem(7,"");
	CTeardlg->m_ListInfo.SetItemText(7,0,"RichMedia");
	CTeardlg->m_ListInfo.SetItemText(7,1,CPdf->PdfFileStatus.PdfRichMedia);

	CTeardlg->m_ListInfo.InsertItem(8,"");
	CTeardlg->m_ListInfo.SetItemText(8,0,"Launch");
	CTeardlg->m_ListInfo.SetItemText(8,1,CPdf->PdfFileStatus.PdfLaunch);

	CTeardlg->m_ListInfo.InsertItem(9,"");
	CTeardlg->m_ListInfo.SetItemText(9,0,"ObjStm");
	CTeardlg->m_ListInfo.SetItemText(9,1,CPdf->PdfFileStatus.PdfObjStm);

	CTeardlg->m_ListInfo.InsertItem(10,"");
	CTeardlg->m_ListInfo.SetItemText(10,0,"AcroForm");
	CTeardlg->m_ListInfo.SetItemText(10,1,CPdf->PdfFileStatus.PdfAcroForm);

	CTeardlg->m_ListInfo.InsertItem(11,"");
	CTeardlg->m_ListInfo.SetItemText(11,0,"Encrypt");
	CTeardlg->m_ListInfo.SetItemText(11,1,CPdf->PdfFileStatus.PdfEncrypt);

	CTeardlg->m_ListInfo.InsertItem(12,"");
	CTeardlg->m_ListInfo.SetItemText(12,0,"Page");
	CTeardlg->m_ListInfo.SetItemText(12,1,CPdf->PdfFileStatus.PdfPage);

	CTeardlg->m_ListInfo.InsertItem(13,"");
	CTeardlg->m_ListInfo.SetItemText(13,0,"JS");
	CTeardlg->m_ListInfo.SetItemText(13,1,CPdf->PdfFileStatus.PdfJS);

	CTeardlg->m_ListInfo.InsertItem(14,"");
	CTeardlg->m_ListInfo.SetItemText(14,0,"JBIG2Decode");
	CTeardlg->m_ListInfo.SetItemText(14,1,CPdf->PdfFileStatus.PdfJBIG2Decode);
	
	CTeardlg->m_ListInfo.InsertItem(15,"");
	CTeardlg->m_ListInfo.SetItemText(15,0,"FlateDecode");
	CTeardlg->m_ListInfo.SetItemText(15,1,CPdf->PdfFileStatus.PdfFlateDecode);

	CTeardlg->m_ListInfo.InsertItem(16,"");
	CTeardlg->m_ListInfo.SetItemText(16,0,"FileSize");
	CTeardlg->m_ListInfo.SetItemText(16,1,CPdf->PdfFileStatus.PdfSize);
	
	CTeardlg->m_ListInfo.InsertItem(17,"");
	CTeardlg->m_ListInfo.SetItemText(17,0,"Security");
	CTeardlg->m_ListInfo.SetItemText(17,1,CPdf->PdfFileStatus.Pdfsecurity);
}

void CPDFTearDlg::OnHelpAbout() 
{
	// TODO: Add your command handler code here
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}

void CPDFTearDlg::OnButtonBrowser() 
{
	// TODO: Add your control notification handler code here
	OnFileOpenf();
}

void CPDFTearDlg::OnPayloadTool() 
{
	// TODO: Add your command handler code here
	CPayLoadDlg   *p   =   new   CPayLoadDlg();                            //创建一个非模态窗体 CXXX是非模式对话框对应的类名   
	BOOL bCreateResult;
	if(!p->GetSafeHwnd())     
	{   
		
		bCreateResult   =   p->Create(IDD_DIALOG_PAYLOAD,this);            //IDD_DIALOG_PAYLOAD   为你写的dialog的资源 
		p->ShowWindow(SW_SHOW);   
	}   
}

void CPDFTearDlg::OnDecodeXor() 
{
	// TODO: Add your command handler code here
	CDecodeDlg   *p   =   new   CDecodeDlg();                            //创建一个非模态窗体 CXXX是非模式对话框对应的类名   
	BOOL bCreateResult;
	if(!p->GetSafeHwnd())     
	{   
		
		bCreateResult   =   p->Create(IDD_DIALOG_DECODE,this);            //IDD_DIALOG_PAYLOAD   为你写的dialog的资源 
		p->ShowWindow(SW_SHOW);   
	}   
}

void CPDFTearDlg::OnButtonDowork() 
{
	// TODO: Add your control notification handler code here
	FindAllKeyWord();
}

void CPDFTearDlg::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// TODO: Add your message handler code here
	CMenu menu;
	CPoint pt=point;
	ScreenToClient(&pt);
	menu.LoadMenu(IDR_MENU_FILE);
	CMenu *file = menu.GetSubMenu(0);
	POINT ptMouse;
	GetCursorPos(&ptMouse);
	file->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,ptMouse.x,ptMouse.y,this);
	return;
}

void CPDFTearDlg::OnMenuFile() 
{
	// TODO: Add your command handler code here
	POSITION pos =m_ListInfo.GetFirstSelectedItemPosition();
	int i=m_ListInfo.GetNextSelectedItem(pos);
	if(i>=0)
	{
		CJsViewDlg   *p   =   new   CJsViewDlg();                            //创建一个非模态窗体 CXXX是非模式对话框对应的类名   
		BOOL bCreateResult;
		if(!p->GetSafeHwnd())     
		{   
			CString strip ="";
			strip += m_fileName;
			bCreateResult   =   p->Create(IDD_DIALOG_JS,this);            //IDD_DIALOG_PAYLOAD   为你写的dialog的资源 
			p->SetWindowText(strip); 
			p->ShowWindow(SW_SHOW);   
	}   
	}
	else
	{
		MessageBox("请先选定列表框内容!","提示",0);
		return;
	}
}

void CPDFTearDlg::OnJsviewTool() 
{
	// TODO: Add your command handler code here
	OnMenuFile();
}
