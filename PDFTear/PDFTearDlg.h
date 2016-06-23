// PDFTearDlg.h : header file
//

#if !defined(AFX_PDFTEARDLG_H__CD68C1B2_9D7F_4283_87D8_95C4EE5BB21B__INCLUDED_)
#define AFX_PDFTEARDLG_H__CD68C1B2_9D7F_4283_87D8_95C4EE5BB21B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CPDFTearDlg dialog

class CPDFTearDlg : public CDialog
{
// Construction
public:
	BOOL m_IsFile;
	void FindAllKeyWord();
	CString m_FilePath;
	CPDFTearDlg(CWnd* pParent = NULL);	// standard constructor
	CStatusBar m_StatusBar;
// Dialog Data
	//{{AFX_DATA(CPDFTearDlg)
	enum { IDD = IDD_PDFTEAR_DIALOG };
	CListCtrl	m_ListInfo;
	CString	m_fileName;

	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPDFTearDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	
	// Generated message map functions
	//{{AFX_MSG(CPDFTearDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnFileOpenf();
	afx_msg void OnHelpAbout();
	afx_msg void OnButtonBrowser();
	afx_msg void OnPayloadTool();
	afx_msg void OnDecodeXor();
	afx_msg void OnButtonDowork();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnMenuFile();
	afx_msg void OnJsviewTool();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PDFTEARDLG_H__CD68C1B2_9D7F_4283_87D8_95C4EE5BB21B__INCLUDED_)
