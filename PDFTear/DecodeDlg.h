#if !defined(AFX_DECODEDLG_H__36D4CD4E_DA67_426A_B9F2_03DB266E6C84__INCLUDED_)
#define AFX_DECODEDLG_H__36D4CD4E_DA67_426A_B9F2_03DB266E6C84__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DecodeDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDecodeDlg dialog

class CDecodeDlg : public CDialog
{
// Construction
public:
	void XorDecByte();
	void XorDecWork();
	void XorAddByte();
	void XorAddWork();
	void XorDWord();
	void XorWord();
	WORD StringToWord(char *Text);
	void XorByte();
	void XorWork();
	int m_terget;
	int m_type;
	CDecodeDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDecodeDlg)
	enum { IDD = IDD_DIALOG_DECODE };
	CString	m_path;
	CString	m_target;
	CString	m_info;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDecodeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
	protected:
		
		// Generated message map functions
		//{{AFX_MSG(CDecodeDlg)
		virtual BOOL OnInitDialog();
		afx_msg void OnButtonBrowser();
		afx_msg void OnButtonBye();
		afx_msg void OnButtonOk();
		afx_msg void OnRadioXor();
		afx_msg void OnRadioAddxor();
		afx_msg void OnRadioDecxor();
		afx_msg void OnRadioByte();
		afx_msg void OnRadioWord();
		afx_msg void OnRadioDword();
		//}}AFX_MSG
		DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DECODEDLG_H__36D4CD4E_DA67_426A_B9F2_03DB266E6C84__INCLUDED_)
