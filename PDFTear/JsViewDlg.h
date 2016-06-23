#if !defined(AFX_JSVIEWDLG_H__0E08EF72_7154_46B5_BB26_ECF65D06248C__INCLUDED_)
#define AFX_JSVIEWDLG_H__0E08EF72_7154_46B5_BB26_ECF65D06248C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// JsViewDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CJsViewDlg dialog

class CJsViewDlg : public CDialog
{
// Construction
public:
	CJsViewDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CJsViewDlg)
	enum { IDD = IDD_DIALOG_JS };
	CString	m_StartPos;
	CString	m_EndPos;
	CString	m_Info;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CJsViewDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CJsViewDlg)
	afx_msg void OnButtonDecode();
	afx_msg void OnButtonCode();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_JSVIEWDLG_H__0E08EF72_7154_46B5_BB26_ECF65D06248C__INCLUDED_)
