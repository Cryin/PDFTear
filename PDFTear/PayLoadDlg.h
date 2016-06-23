#if !defined(AFX_PAYLOADDLG_H__98F51AC7_A178_453F_B360_6EFD88CC1C9B__INCLUDED_)
#define AFX_PAYLOADDLG_H__98F51AC7_A178_453F_B360_6EFD88CC1C9B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PayLoadDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPayLoadDlg dialog
typedef struct DISASSEMBLY{
	
	// Define Decoded instruction struct

    char Assembly[256]; // Menemonics
    char Remarks[256];  // Menemonic addons
    char Opcode[30];    // Opcode Byte forms
    DWORD Address;      // Current address of decoded instruction
    BYTE  OpcodeSize;   // Opcode Size
	BYTE  PrefixSize;   // Size of all prefixes used

} DISASSEMBLY;
// Register(s) Size
#define REG8  0
#define REG16 1
#define REG32 2
#define FPU   3 // Not in use.

// 8Bit Registers
#define  REG_AL 0
#define  REG_CL 1
#define  REG_DL 2
#define  REG_BL 3
#define  REG_AH 4
#define  REG_CH 5
#define  REG_DH 6
#define  REG_BH 7

// 16Bit Registers
#define  REG_AX 0
#define  REG_CX 1
#define  REG_DX 2
#define  REG_BX 3
#define  REG_SP 4
#define  REG_BP 5
#define  REG_SI 6
#define  REG_DI 7

// 32bit Registers
#define  REG_EAX 0 
#define  REG_ECX 1
#define  REG_EDX 2
#define  REG_EBX 3
#define  REG_ESP 4
#define  REG_EBP 5
#define  REG_ESI 6
#define  REG_EDI 7

// Segments
#define SEG_ES 0
#define SEG_CS 1
#define SEG_SS 2
#define SEG_DS 3
#define SEG_FS 4
#define SEG_GS 5

// Sib extention
#define SIB_EX 4 
// Disasm Fucntions
class CPayLoadDlg : public CDialog
{
	// Construction
public:
	CString Getstr();
	void MakeDecoded(DISASSEMBLY Disasm);
	void DisAssembly(char *file,int start,int lenght);
	void ShowDecoded(DISASSEMBLY Disasm);
	void Mod_11_RM_EX(BYTE d, BYTE w,char **Opcode,DISASSEMBLY **Disasm,bool PrefixReg,BYTE Op,DWORD **index,bool RepPrefix);
	void Mod_RM_SIB_EX(DISASSEMBLY **Disasm,char **Opcode, int pos, bool AddrPrefix,int SEG,DWORD **index,BYTE Op,bool PrefixReg,bool PrefixSeg,bool PrefixAddr,BYTE Bit_d,BYTE Bit_w,bool RepPrefix);
	void Mod_RM_SIB(DISASSEMBLY **Disasm,char **Opcode, int pos, bool AddrPrefix,int SEG,DWORD **index,BYTE Bit_d, BYTE Bit_w, char *instruction,BYTE Op,bool PrefixReg,bool PrefixSeg,bool PrefixAddr);
	void Mod_11_RM(BYTE d, BYTE w,char **Opcode,DISASSEMBLY **Disasm,char instruction[],bool PrefixReg,BYTE Op,DWORD **index);
	void GetJumpInstruction(BYTE Opcode,char *menemonic);
	void SwapWord(BYTE *MemPtr,WORD *Original,WORD* Mirrored);
	void SwapDword(BYTE *MemPtr,DWORD *Original,DWORD* Mirrored);
	int GetNewInstruction(BYTE Op,char *ASM,bool RegPrefix);
	void GetInstruction(BYTE Opcode,char *menemonic);
	void Decode(DISASSEMBLY *Disasm,char *Opcode,DWORD *Index);
	void FlushDecoded(DISASSEMBLY *Disasm);
	WORD StringToWord(char *Text);
	DWORD StringToDword(char *Text);
	CPayLoadDlg(CWnd* pParent = NULL);   // standard constructor
	
	// Dialog Data
	//{{AFX_DATA(CPayLoadDlg)
	enum { IDD = IDD_DIALOG_PAYLOAD };
	CString	m_path;
	CString	m_info;
	
	//}}AFX_DATA
	
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPayLoadDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	
	// Implementation
protected:
	
	// Generated message map functions
	//{{AFX_MSG(CPayLoadDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonBrowser();
	afx_msg void OnButtonDasm();
	afx_msg void OnButtonCode();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAYLOADDLG_H__98F51AC7_A178_453F_B360_6EFD88CC1C9B__INCLUDED_)
