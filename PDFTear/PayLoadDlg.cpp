// PayLoadDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PDFTear.h"
#include "PayLoadDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPayLoadDlg dialog
const char *Regs[3][9] = {
    {"al", "cl", "dl", "bl", "ah", "ch", "dh", "bh"},
    {"ax", "cx", "dx", "bx", "sp", "bp", "si", "di"},
    {"eax","ecx","edx","ebx","esp","ebp","esi","edi"} 
};

const char *RegSize[4] = {"Qword","Dword","Word","Byte"};
const char *Segs[8]    = {"ES","CS","SS","DS","FS","GS","SEG?","SEG?"};


char *regs[3][9] = {
    { "al", "cl", "dl", "bl", "ah", "ch", "dh", "bh"  }, // 8Bit
    { "ax", "cx", "dx", "bx", "sp", "bp", "si", "di"  }, // 16Bit
    { "eax","ecx","edx","ebx","esp","ebp","esi","edi" }  // 32bit
  //{ "eeax","eecx","eedx","eebx","eesp","eebp","eesi",eedi"
};

// x86 Data Size
const char *regSize[10]              = { "Qword","Dword","Word","Byte","Fword","TByte","(28)Byte","(108)Byte","DQword", "(512)Byte" }; // Registers Size of addressing

// x86 Segments
const char *segs[8]                  = { "ES","CS","SS","DS","FS","GS","SEG?","SEG?"}; // Segments

// x86 SIB
const char *Scale[5]                 = { "-","+","*2+","*4+","*8+" };     // Scale in SIB

// 16Bit Addressing
const char *addr16[8]                = { "BX+SI","BX+DI","BP+SI","BP+DI","SI","DI","BX","BP" }; // 16bit addressing

// x86 Instructions
const char *Instructions[8]          = { "add" , "or"  , "adc" ,    "sbb" , "and" , "sub"    , "xor" , "cmp"  }; // Basic      Repetive Assembly
const char *ArtimaticInstructions[8] = { "rol" , "ror" , "rcl" ,    "rcr" , "shl" , "shr"    , "sal" , "sar"  }; // Bitwise    Repetive Assembly
const char *InstructionsSet2[8]      = { "test", "test", "not" ,    "neg" , "mul" , "imul"   , "div" , "idiv" }; // Arithmatic Repetive Assembly (test is Twice -> long repetive set)
const char *InstructionsSet3[8]      = { "inc" , "dec" , "???" ,    "???" , "???" , "???"    , "???" , "???"  }; // Arithmatic Repetive Assebly (Opcode 0xFE)
const char *InstructionsSet4[8]      = { "inc" , "dec" , "call","call far", "jmp" , "jmp far", "push", "???"  }; // Arithmatic Repetive Assebly (Opcode 0xFE)

// FPU instructions
const char *FpuRegs[8]                      = { "st(0)", "st(1)", "st(2)", "st(3)" , "st(4)" , "st(5)" , "st(6)" , "st(7)"  }; // FPU Registers
const char *FpuInstructions[8]              = { "fadd" , "fmul" , "fcom" , "fcomp" , "fsub"  , "fsubr" , "fdiv"  , "fdivr"  }; // Unsigned fpu instructions
const char *FpuInstructionsSigned[8]        = { "fiadd", "fimul", "ficom", "ficomp", "fisub" , "fisubr", "fidiv" , "fidivr" }; // Signed fpu instructions
const char *FpuInstructionsSet2[8]          = { "fld"  , "???"  , "fst"  , "fstp"  , "fldenv", "fldcw" , "fstenv", "fstcw"  }; // set2 of Unsigned fpu instructions
const char *FpuInstructionsSet2Signed[8]    = { "fild" , "???"  , "fist" , "fistp" , "???"   , "fld"   , "???"   , "fstp"   }; // set2 of Signed fpu instructions
const char *FpuInstructionsSet3[8]          = { "fld"  , "???"  , "fst"  , "fstp"  , "frstor", "???"   , "fsave" , "fstsw"  }; // set3 of Unsigned fpu instructions
const char *FpuInstructionsSet2Signed_EX[8] = { "fild" , "???"  , "fist" , "fistp" , "fbld"  , "fild"  , "fbstp" , "fistp"  }; // set2 of Signed fpu instructions With Extended 2 instructions

// MMX, 3DNow! Registers
const char *Regs3DNow  [8]  = { "mm0"       , "mm1"       , "mm2"       , "mm3"       , "mm4"       , "mm5"     , "mm6"     , "mm7"    }; // 3DNow! Registers
const char *MMXRegs    [8]  = { "xmm0"      , "xmm1"      , "xmm2"      , "xmm3"      , "xmm4"      , "xmm5"    , "xmm6"    , "xmm7"   }; // MMX Registers

// MMX, 3DNow! (+extended), SSE , SSE2 Instructions
const char *NewSet     [8]  = { "sldt"      , "str"       , "lldt"      , "ltr"       , "verr"      , "verw"    , "???"     , "???"    }; // New Set1
const char *NewSet2    [8]  = { "sgdt"      , "sidt"      , "lgdt"      , "lidt"      , "smsw"      , "???"     , "lmsw"    , "invlpg" }; // New Set2
const char *NewSet3    [8]  = { "prefetchnta", "prefetcht0", "prefetcht1", "prefetcht2", "???"       , "???"     , "???"     , "???"    }; // New Set3
const char *NewSet4    [8]  = { "movaps"    , "movaps"    , "cvtpi2ps"  , "???"       , "cvttps2pi" , "cvtps2pi", "ucomiss" , "comiss" }; // New Set4
const char *NewSet5    [16] = { "cmovo"     , "cmovno"    , "cmovb"     , "cmovnb"    , "cmove"     , "cmovne"  , "cmovbe"  , "cmova"   , "cmovs"    , "cmovns"   , "cmovpe"     , "cmovpo"  , "cmovl"   , "cmovge" , "cmovle", "cmovg" }; // New Set5
const char *NewSet6    [16] = { "???"       , "sqrtps"    , "rsqrtps"   , "rcpps"     , "andps"     , "andnps"  , "orps"    , "xorps"   , "addps"    , "mulps"    , "???"        , "???"     , "subps"   , "minps"  , "divps" , "maxps" }; // New Set6
const char *NewSet6Ex  [16] = { "???"       , "sqrtss"    , "rsqrtss"   , "rcpss"     , "andps"     , "andnps"  , "orps"    , "xorps"   , "addss"    , "mulss"    , "???"        , "???"     , "subss"   , "minss"  , "divss" , "maxss" }; // New Set6 Extended (Prefix 0xF3)
const char *NewSet7    [16] = { "punpcklbw" , "punpcklwd" , "punpckldq" , "packsswb"  , "pcmpgtb"   , "pcmpgtw" , "pcmpgtd" , "packuswb", "punpckhbw", "punpckhwd", "punpckhdq"  , "packssdw", "???"     , "???"    , "movd"  , "movq"  }; // New Set7
const char *NewSet8    [8]  = { "pshufw"    , "???"       , "???"       , "???"       , "pcmpeqb"   , "pcmpeqw" , "pcmpeqd" , "emms" };                                                                                                    // New Set8
const char *NewSet9    [16] = { "seto"      , "setno"     , "setb"      , "setnb"     , "sete"      , "setne"   , "setbe"   , "seta"    , "sets"     , "setns"    , "setpe"      , "setpo"   , "setl"    , "setge"  , "setle" , "setg"  }; // New Set9
const char *NewSet10   [16] = { "push fs"   , "pop fs"    , "cpuid"     , "bt"        , "shld"      , "shld"    , "???"     , "???"     , "push gs"  , "pop gs"   , "rsm"        , "bts"     , "shrd"    , "shrd"   , "fxsave", "imul"  }; // New Set10
const char *NewSet10Ex [8]  = { "fxsave"    , "fxrstor"   , "ldmxcsr"   , "stmxcsr"   , "???"       , "???"     , "???"     , "???" };                                                                                                     // New Set10 Extended (Opcode 0xAE)
const char *NewSet11   [16] = { "cmpxchg"   , "cmpxchg"   , "lss"       , "btr"       , "lfs"       , "lgs"     , "movzx"   , "movzx"   , "???"      , "???"      , "???"        , "btc"     , "bsf"     , "bsr"    , "movsx" , "movsx" }; // New Set11
const char *NewSet12   [8]  = { "cmpeqps"   , "cmpltps"   , "cmpleps"   , "cmpunordps", "cmpneqps"  , "cmpnltps", "cmpnleps", "cmpordps" };                                                                                                // New Set12
const char *NewSet12Ex [8]  = { "cmpeqss"   , "cmpltss"   , "cmpless"   , "cmpunordss", "cmpneqss"  , "cmpnltss", "cmpnless", "cmpordss" };                                                                                                // New Set12 Extended (Prefix 0xF3)
const char *NewSet13   [16] = { "???"       , "psrlw"     , "psrld"     , "psrlq"     , "???"       , "pmullw"  , "???"     , "pmovmskb", "psubusb"  , "psubusw"  , "pminub"     , "pand"    , "paddusb" , "paddusw", "pmaxub", "pandn" }; // New Set13
const char *NewSet14   [16] = { "pavgb"     , "psraw"     , "psrad"     , "pavgw"     , "pmulhuw"   , "pmulhw"  , "???"     , "movntq"  , "psubsb"   , "psubsw"   , "pminsw"     , "por"     , "paddsb"  , "paddsw" , "pmaxsw", "pxor"  }; // New Set14
const char *NewSet15   [16] = { "???"       , "psllw"     , "pslld"     , "psllq"     , "???"       , "pmaddwd" , "psadbw"  , "maskmovq", "psubb"    , "psubw"    , "psubd"      , "???"     , "paddb"   , "paddw"  , "paddd" , "???"   }; // New Set15

// Debug/Control/Test Registers
const char *DebugRegs  [8]  = { "dr0"       , "dr1"       , "dr2"       , "dr3"       , "dr4"       , "dr5"     , "dr6"     , "dr7"    }; // Debug Registers
const char *ControlRegs[8]  = { "cr0"       , "cr1"       , "cr2"       , "cr3"       , "cr4"       , "cr5"     , "cr6"     , "cr7"    }; // Control Registers
//const char *TestRegs [8]  = { "tr0"       , "tr1"       , "tr2"       , "tr3"       , "tr4"       , "tr5"     , "tr6"     , "tr7"    }; // Test Registers

CString strinfo;
CPayLoadDlg::CPayLoadDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPayLoadDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPayLoadDlg)
	m_path = _T("");
	m_info = _T("");
	//}}AFX_DATA_INIT
}


void CPayLoadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPayLoadDlg)
	DDX_Text(pDX, IDC_EDIT_PATH, m_path);
	DDX_Text(pDX, IDC_EDIT_INFO, m_info);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPayLoadDlg, CDialog)
	//{{AFX_MSG_MAP(CPayLoadDlg)
	ON_BN_CLICKED(IDC_BUTTON_BROWSER, OnButtonBrowser)
	ON_BN_CLICKED(IDC_BUTTON_DASM, OnButtonDasm)
	ON_BN_CLICKED(IDC_BUTTON_CODE, OnButtonCode)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPayLoadDlg message handlers

BOOL CPayLoadDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	SetIcon(AfxGetApp()->LoadIcon(IDI_ICON_PAYLOAD),TRUE);
	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPayLoadDlg::OnButtonBrowser() 
{
	// TODO: Add your control notification handler code here
	CFileDialog dlgFileOpen(TRUE,".bin","shellcode.bin",OFN_PATHMUSTEXIST | OFN_HIDEREADONLY,"(*.bin)|*.bin||");
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

void CPayLoadDlg::OnButtonDasm() 
{
	// TODO: Add your control notification handler code here
	if(m_path.IsEmpty())
	{
		MessageBox("请选择文件!","提示",MB_OK);
		return;
	}
	DISASSEMBLY Disasm;
	m_info="";
	char *Opcodes;
	DWORD lenght;
	FILE *hFile;
	hFile=fopen(m_path,"r");
	if(hFile==NULL)
	{
		MessageBox("打开文件出错!","提示",MB_OK);
		return;
	}
	fseek(hFile,0,SEEK_END);
	//获取文件长度;
    lenght=ftell(hFile);
	//printf("%d",lenght);
	fseek(hFile,0,SEEK_SET);
	Opcodes=(char*)malloc(lenght);
	fread(Opcodes,1,lenght,hFile);

	char *Linear="";
	// Index of opcoded to decode
    DWORD Index=0;
              
    Linear=(char*)Opcodes;     // Points to the address of array to decode.
    Disasm.Address=0x00401000; // Common Entry Point (usually default..)
    FlushDecoded(&Disasm);     // reset all content

	for(Index=0;Index<lenght;Index++)
    {
		// Decode instruction
        Decode(&Disasm,Linear,&Index);

		// Upper case Assembly (Optional)
        CharUpper(Disasm.Assembly);
		// Show Decoded instruction, size, remarks...
        ShowDecoded(Disasm);
		// Calculate total Size of an instruction + Prefixes, and
		// Fix the address of IP 
        Disasm.Address+=Disasm.OpcodeSize+Disasm.PrefixSize;
		// Clear all information
        FlushDecoded(&Disasm);
    }
	this->UpdateData(FALSE);
	free(Opcodes);
	fclose(hFile);
}

DWORD CPayLoadDlg::StringToDword(char *Text)
{
/*
this function will convert and return
an Hexadecimel String into a real
DWORD hex number using assembly directive.
    */
	
    DWORD AsmDwordNum=(DWORD)Text;
    DWORD DwordNum=0;
	
	_asm{
		PUSHAD
			PUSHF
			XOR ECX,ECX
			XOR EAX,EAX
			XOR EDI,EDI
			MOV EDI,8H
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
			MOV DwordNum,EAX
			POPF
			POPAD
	}
	
	return DwordNum;
}

WORD CPayLoadDlg::StringToWord(char *Text)
{
	/*
	this function will convert and return
	an Hexadecimel String into a real
	WORD hex number using assembly directive.
    */
	
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

void CPayLoadDlg::FlushDecoded(DISASSEMBLY *Disasm)
{
	strcpy(Disasm->Assembly,""); // Clear menemonic
    strcpy(Disasm->Remarks,"");  // Clear commets
    strcpy(Disasm->Opcode,"");   // Clear opcodes linear
    Disasm->OpcodeSize=1;        // Smallest opcode size
	Disasm->PrefixSize=0;        // No Prefixes
}

void CPayLoadDlg::Decode(DISASSEMBLY *Disasm, char *Opcode, DWORD *Index)
{
	// intializations
    DWORD dwMem=0,dwOp=0;
    int  i=*Index,RegRepeat=0,LockRepeat=0,SegRepeat=0,RepRepeat=0,AddrRepeat=0; // Repeated Prefixes    
	int  RM=REG32,SEG=SEG_DS,ADDRM=REG32;       // default modes	
	int PrefixesSize=0,PrefixesRSize=0;         // PrefixesSize = all Prefixes(no rep), PrefixesRsize (with Rep Prefix)
    WORD wMem=0,wOp=0;    
    bool RegPrefix=0,LockPrefix=0,SegPrefix=0,RepPrefix=0,AddrPrefix=0;  // default size of Prefixes
    BYTE Bit_D=0, Bit_W=0; // bit d/w for R/M
	char menemonic[256]="";
    char RSize[10]="Dword";                     // default size of menemonic
    BYTE Op=(BYTE)Opcode[i];                    // current opcode
    


    //=======================================================
    //               Decoding Prefixes                     //
    //=======================================================

    // we first assume there is prefix !
	// if we skip this, our decoding might be currupted

	while( // check only RegPreifix/LockProfix/SegPrefixes/RepPrefix/AddrPerfix
		   (Op==0x66) || (Op==0x0F0)|| (Op==0x2E) || (Op==0x36) ||
		   (Op==0x3E) || (Op==0x26) || (Op==0x64) || (Op==0x65) || 
		   (Op==0xF2) || (Op==0xF3) || (Op==0x67)
		 ) 
	{
		switch(Op)
		{
			case 0x66: // reg prefix, change default size, dword->word
			{                   
				RM=REG16; // 66 prefix, change default size
				RegPrefix=1; 
				BYTE temp;
				wsprintf(RSize,"%s",RegSize[2]); // change default size of menemonic to 'Word'
				//LockPrefix=0;
				lstrcat(Disasm->Opcode,"66:");
				i++;
				++(*Index);
				Op=(BYTE)Opcode[i];
				temp=(BYTE)Opcode[i+1];
				RegRepeat++;
				if(RegRepeat>1) 
				{
					strcpy(Disasm->Opcode,"66:");
					strcpy(Disasm->Remarks,"Prefix DataSize:");
					Disasm->OpcodeSize=1;
					Disasm->PrefixSize=0;
					(*Index)-=RegRepeat;
					return;
				}
			}
			break;

			case 0x67: // Addr prefix, change default Reg size, (EDI->DI) and more!
			{
				ADDRM=REG16; // 67 prefix, change default size, in this case: Memory Reg Size
				AddrPrefix=1; 
				BYTE temp;
				lstrcat(Disasm->Opcode,"67:");
				i++;
				++(*Index);
				Op=(BYTE)Opcode[i];
				temp=(BYTE)Opcode[i+1];
				AddrRepeat++;
				if(AddrRepeat>1) 
				{
					strcpy(Disasm->Opcode,"67:");
					strcpy(Disasm->Remarks,"Prefix AddrSize:");
					Disasm->OpcodeSize=1;
					Disasm->PrefixSize=0;
					(*Index)-=AddrRepeat;
					return;
				}
			}
			break;

			case 0x0F0: // LockPrefix, Add bus lock menemonic opcode in front of every menemonic
			{
				//BYTE temp;
				LockPrefix=1;
				//RegPrefix=0; 
				lstrcat(Disasm->Opcode,"F0:");
				strcpy(Disasm->Assembly,"lock ");
				i++;
				++(*Index);
				Op=(BYTE)Opcode[i];
				//temp=(BYTE)Opcode[i+1];
				LockRepeat++;
				if(LockRepeat>1) 
				{
					strcpy(Disasm->Assembly,"");
					strcpy(Disasm->Opcode,"F0:");
					strcpy(Disasm->Remarks,"Prefix LOCK:");
					Disasm->OpcodeSize=1;
					Disasm->PrefixSize=0;
					(*Index)-=LockRepeat;
					return;
				}
			}
			break;

			case 0xF2: case 0xF3: // RepPrefix (only string instruction!!)
			{
                BYTE NextOp=(BYTE)Opcode[i+1];              // Next followed opcode
                BYTE NextOp2=(BYTE)Opcode[i+2];
				RepPrefix=1;
				wsprintf(menemonic,"%02X:",Op);
				lstrcat(Disasm->Opcode,menemonic);
				switch(Op)
				{
					case 0xF2:wsprintf(menemonic,"repne ");break;
					case 0xF3:wsprintf(menemonic,"repe ");break;
				}
				lstrcat(Disasm->Assembly,menemonic);
				i++;
				++(*Index);
				Op=(BYTE)Opcode[i];
				RepRepeat++;
				// REPE/REPNE Prefixes affect only string operations:
				// MOVS/LODS/SCAS/CMPS/STOS/CMPSS.CMPPS..etc (NewSet Instructions)
				if(!( 
                      (Op>=0xA4 && Op<=0xA7) || 
                      (Op>=0xAA && Op<=0xAF) || 
                      (NextOp==0x0F && NextOp2==0x2A) ||
                      (NextOp==0x0F && NextOp2==0x10) ||
                      (NextOp==0x0F && NextOp2==0x11) ||
                      (NextOp==0x0F && NextOp2==0x2C) ||
                      (NextOp==0x0F && NextOp2==0x2D) ||
                      (NextOp==0x0F && NextOp2==0x51) ||
                      (NextOp==0x0F && NextOp2==0x52) ||
                      (NextOp==0x0F && NextOp2==0x53) ||
                      (NextOp==0x0F && NextOp2==0x58) ||
                      (NextOp==0x0F && NextOp2==0x59) ||
                      (NextOp==0x0F && NextOp2==0x5C) ||
                      (NextOp==0x0F && NextOp2==0x5D) ||
                      (NextOp==0x0F && NextOp2==0x5E) ||
                      (NextOp==0x0F && NextOp2==0x5F) ||
                      (NextOp==0x0F && NextOp2==0xC2) 
                    )
                  )
				{
					strcpy(Disasm->Assembly,"");
					strcpy(Disasm->Remarks,"Prefix REP:");
					Disasm->OpcodeSize=1;
					Disasm->PrefixSize=0;
					(*Index)-=RepRepeat;
					return;
				}
					
			}
			break;

			case 0x2E: case 0x36: // Segment Prefixes
			case 0x3E: case 0x26: // Segment Prefixes
			case 0x64: case 0x65: // Segment Prefixes
			{
				BYTE temp;
				switch(Op)
				{
					// Change Default Segment
					case 0x2E: SEG = SEG_CS; break; // Segment CS
					case 0x36: SEG = SEG_SS; break; // Segment SS
					case 0x3E: SEG = SEG_DS; break; // Segment DS
					case 0x26: SEG = SEG_ES; break; // Segment ES
					case 0x64: SEG = SEG_FS; break; // Segment FS
					case 0x65: SEG = SEG_GS; break; // Segment GS
				}

				SegPrefix=1;
				wsprintf(menemonic,"%02X:",Op);
				lstrcat(Disasm->Opcode,menemonic);
				i++;
				++(*Index);
				Op=(BYTE)Opcode[i];
				temp=(BYTE)Opcode[i-2];
				SegRepeat++;

				// Check if SegPrefix is repeating
				if(SegRepeat>1) 
				{                    
					BYTE opc=(BYTE)Opcode[i-1];

					if(  temp==0x2E || temp==0x36 ||
					     temp==0x3E || temp==0x26 ||
                         temp==0x64 || temp==0x65 ||
                         temp==0x66 || temp==0xF0 ||
                         temp==0x67                        
					  )
					{
						// Check if last byte was an seg prefix and show it
						if(temp==0x66 || temp==0xF0 || temp==0x67)
                        {
                           opc=(BYTE)Opcode[i-3];
                           SegRepeat++;
                        }
                        else
                           opc=(BYTE)Opcode[i-2];

						switch(opc)
						{
							// Change Segment, accurding to last segPrefix (if repeated)
							case 0x2E: SEG = SEG_CS; break; // Segment CS
							case 0x36: SEG = SEG_SS; break; // Segment SS
							case 0x3E: SEG = SEG_DS; break; // Segment DS
							case 0x26: SEG = SEG_ES; break; // Segment ES
							case 0x64: SEG = SEG_FS; break; // Segment FS
							case 0x65: SEG = SEG_GS; break; // Segment GS
						}
					

                        strcpy(Disasm->Assembly,"");
                        wsprintf(menemonic,"%02X:",opc);
                        strcpy(Disasm->Opcode,menemonic);
                        wsprintf(menemonic,"Prefix %s:",Segs[SEG]);
                        strcpy(Disasm->Remarks,menemonic);
                        Disasm->OpcodeSize=0;
                        Disasm->PrefixSize=1;
                        (*Index)-=SegRepeat;
                    }
					return;
				}
			}
			break;
			
		    default:
			{
				// reset prefixes/repeats to default
				LockRepeat=0;
				RegRepeat=0;
				SegRepeat=0;
				RegPrefix=0;
				LockPrefix=0;
				SegPrefix=0;
				strcpy(RSize,RegSize[1]); // Default size
			}
			break;
		}
	}

    
	//=============================================
	//        Main Decoding starts here!         //
	//=============================================

	// Calculate Prefixes Sizes
	PrefixesSize  = RegPrefix+LockPrefix+SegPrefix+AddrPrefix; // No RepPrefix
	PrefixesRSize = PrefixesSize+RepPrefix; // Special Case (Rep Prefix is being used -> String menemonics only)

    switch(Op) // Find & Decode Big Set Opcodes
    {
		case 0x00: case 0x01: case 0x02: case 0x03: // ADD  XX/XXX, XX/XXX
		case 0x08: case 0x09: case 0x0A: case 0x0B: // OR   XX/XXX, XX/XXX
		case 0x10: case 0x11: case 0x12: case 0x13: // ADC  XX/XXX, XX/XXX 
		case 0x18: case 0x19: case 0x1A: case 0x1B: // SBB  XX/XXX, XX/XXX 
		case 0x20: case 0x21: case 0x22: case 0x23: // AND  XX/XXX, XX/XXX 
		case 0x28: case 0x29: case 0x2A: case 0x2B: // SUB  XX/XXX, XX/XXX 
		case 0x30: case 0x31: case 0x32: case 0x33: // XOR  XX/XXX, XX/XXX 
		case 0x38: case 0x39: case 0x3A: case 0x3B: // CMP  XX/XXX, XX/XXX 
        case 0x88: case 0x89: case 0x8A: case 0x8B: // MOV  XX/XXX, XX/XXX 
        case 0x8C: case 0x8E:                       // MOV  XX/XXX, XX/XXX
		case 0x62: case 0x63:                       // BOUND / ARPL XX/XXX, XX/XXX
        case 0x69:                                  // IMUL RM,IIM32 (DWORD)
        case 0x6B:                                  // IMUL <reg>,<RM>
		case 0x80: case 0x81: case 0x82: case 0x83: // MIXED Instructions
		case 0x84: case 0x85:                       // TEST
		case 0x86: case 0x87:                       // XCHG
        case 0x8D:                                  // LEA 
        case 0x8F:                                  // POP
        case 0xC0: case 0xC1:                       // MIXED Instructions
        case 0xC4: case 0xC5:                       // LES / LDS REG,MEM
        case 0xC6: case 0xC7:                       // MOV [MEM],IIM8/16/32
        case 0xD0: case 0xD1: case 0xD2: case 0xD3: // MIXED Bitwise Instructions
        case 0xD8: case 0xD9: case 0xDA: case 0xDB: // FPU Instructions
        case 0xDC: case 0xDD: case 0xDE: case 0xDF: // FPU Instructions
        case 0xF6: case 0xF7: case 0xFE: case 0xFF: // MIX Instructions
        {
			if(((BYTE)Opcode[i+1] & 0xC0)==0xC0)   // Check Opcode Range
			{
				char mene[10]="";
				
				GetInstruction(Op,mene); // Get instruction from Opcode Byte
				Bit_D=(Op&0x02)>>1;      // Get bit d (direction)
				Bit_W=(Op&0x01);         // Get bit w (full/partial reg size)
				
				// Check Special Cases for alone Opcodes
				switch(Op)
				{
                    case 0x63:{Bit_D=0;Bit_W=1;}               break;
                    case 0x62:{Bit_D=1;Bit_W=1;}               break;
                    case 0x86:{Bit_D=0;Bit_W=0;}               break;
                    case 0x87:{Bit_D=0;Bit_W=1;}               break;
                    case 0x80: case 0x82: { Bit_D=0;Bit_W=0; } break;
                    case 0x81: case 0x83: { Bit_D=0;Bit_W=1; } break;
                    case 0x8C:{ Bit_D=0;Bit_W=0;}              break;
                    case 0x8E:{ Bit_D=1;Bit_W=0;}              break;
                    case 0xC4: case 0xC5: { Bit_D=1;Bit_W=1; } break;
				}

				Mod_11_RM(Bit_D,Bit_W,&Opcode,&Disasm,mene,RegPrefix,Op,&Index); // Decode with bits
				Disasm->PrefixSize=PrefixesSize; // PrefixSize (if prefix present)				
				break;
			}
			
			// operand doesn't have byte(s) extension in addressing mode
			if((BYTE)Opcode[i+1]>=0x00 && (BYTE)Opcode[i+1]<=0xBF)
			{
				char mene[10]="";
				GetInstruction(Op,mene); // Get instruction from Opcode Byte
				Bit_D=(Op&0x02)>>1;      // Get bit d (direction)
				Bit_W=(Op&0x01);         // Get bit w (full/partial reg size)
				Mod_RM_SIB(&Disasm,&Opcode,i,AddrPrefix,SEG,&Index,Bit_D,Bit_W,mene,Op,RegPrefix,SegPrefix,AddrPrefix);
				Disasm->PrefixSize=PrefixesSize;
				break;
			}	  				
		}
		break;

		case 0x04:case 0x0C:case 0x14: // INSTRUCTION AL,XX
		case 0x1C:case 0x24:case 0x2C: // INSTRUCTION AL,XX
        case 0x34:case 0x3C:case 0xA8: // INSTRUCTION AL,XX
		case 0xE4:
        {
			char mene[10]="";
			GetInstruction(Op,mene); // Get instruction for a specified Byte
            wsprintf(menemonic,"%s al, %02X",mene,(BYTE)Opcode[i+1]);
            lstrcat(Disasm->Assembly,menemonic);
            strcpy(Disasm->Remarks,"");
            wsprintf(menemonic,"%02X%02X",Op,(BYTE)*(Opcode+i+1));
            lstrcat(Disasm->Opcode,menemonic);
            Disasm->OpcodeSize=2;
			Disasm->PrefixSize=PrefixesSize;
            ++(*Index);
        }
        break;

        case 0x05:case 0x0D:case 0x15: // INSTRUCTION EAX/AX,XXXXXXXX
        case 0x1D:case 0x25:case 0x2D: // INSTRUCTION EAX/AX,XXXXXXXX
		case 0x35:case 0x3D:case 0xA9: // INSTRUCTION EAX/AX,XXXXXXXX
        {
			char mene[10]="";
			GetInstruction(Op,mene); // Get instruction for a specified Byte

			if(RegPrefix==0) // no prefix
			{   
				// read 4 bytes into EAX
				SwapDword((BYTE*)(Opcode+i+1),&dwOp,&dwMem);
                wsprintf(menemonic,"%s %s, %08X",mene,Regs[REG32][0],dwMem);
				lstrcat(Disasm->Assembly,menemonic);
				wsprintf(menemonic,"%02X %08X",Op,dwOp);
				lstrcat(Disasm->Opcode,menemonic);
				Disasm->OpcodeSize=5;
				Disasm->PrefixSize=PrefixesSize;
				(*Index)+=4;
			}
			else if(RegPrefix==1) // RegPrefix is being used
			{   
				// read 2 bytes into AX (REG16)
				SwapWord((BYTE*)(Opcode+i+1),&wOp,&wMem);
                wsprintf(menemonic,"%s %s, %04X",mene,Regs[REG16][0],wMem);
				lstrcat(Disasm->Assembly,menemonic);
				wsprintf(menemonic,"%02X %04X",Op,wOp);
				lstrcat(Disasm->Opcode,menemonic);
				Disasm->OpcodeSize=3;
				Disasm->PrefixSize=PrefixesSize;
				(*Index)+=2;
			}
        }
        break;

        case 0x06: // PUSH ES
        {
            lstrcat(Disasm->Assembly,"push es");
            strcpy(Disasm->Remarks,"Push ES register to the stack");            
			lstrcat(Disasm->Opcode,"06");            
			Disasm->PrefixSize=PrefixesSize;
        }
        break;

		case 0x07: // POP ES
        {
            lstrcat(Disasm->Assembly,"pop es");
            strcpy(Disasm->Remarks,"Pop top stack to ES");            
			lstrcat(Disasm->Opcode,"07");            
			Disasm->PrefixSize=PrefixesSize;
        }
        break;

		case 0x0E: // PUSH CS
        {
            lstrcat(Disasm->Assembly,"push cs");
            strcpy(Disasm->Remarks,"Push CS register to the stack");
			lstrcat(Disasm->Opcode,"0E");
			Disasm->PrefixSize=PrefixesSize;
        }
        break;

        // INTEL - NEW SET (MMX/3DNow!/SSE/SSE2)
        case 0x0F:
        {
          char Instruction[128],m_bytes[128];
          int RetVal;
          BYTE Code=(BYTE)Opcode[i+1];

          lstrcat(Disasm->Opcode,"0F");
          RetVal=GetNewInstruction(Code,Instruction,RegPrefix);

          switch(RetVal) // check if we need to decode instruction
          {
            case 0:
            {
                // Decode SIB + ModRM
                if((BYTE)Opcode[i+2]>=0x00 && (BYTE)Opcode[i+2]<=0xBF)
                {
                    (*Index)++;
                    i=*Index;
                    Bit_D=(Op&0x02)>>1;      // Get bit d (direction)
				    Bit_W=(Op&0x01);         // Get bit w (full/partial reg size)
                    Mod_RM_SIB_EX(&Disasm,&Opcode,i,AddrPrefix,SEG,&Index,Code,RegPrefix,SegPrefix,AddrPrefix,Bit_D,Bit_W,RepPrefix);
                    Disasm->PrefixSize=PrefixesSize;
                    Disasm->OpcodeSize++; // 0F extra Byte
                    break;
			    }
                else
                {
                    if(((BYTE)Opcode[i+2] & 0xC0)==0xC0)
                    {                    
                        Bit_D=(Op&0x02)>>1;      // Get bit d (direction)
                        Bit_W=(Op&0x01);         // Get bit w (full/partial reg size)
                        (*Index)++;
                        i=*Index;
                        Mod_11_RM_EX(Bit_D,Bit_W,&Opcode,&Disasm,RegPrefix,Code,&Index,RepPrefix); // Decode with bits
                        Disasm->PrefixSize=PrefixesSize;
                        Disasm->OpcodeSize++; // 0F extra Byte
                    }
                    break;
                }
            }
            break; // big set instructions
            
            case 1: // 1 byte instructions set
            {
                lstrcat(Disasm->Assembly,Instruction);
                wsprintf(Instruction,"%02X",Code);
                lstrcat(Disasm->Opcode,Instruction);
                Disasm->OpcodeSize=2;
                Disasm->PrefixSize=PrefixesSize;
                (*Index)++;
            }
            break;
            
            case 2: // NEAR JUMP (JXX)
            {
                SwapDword((BYTE*)(Opcode+i+2),&dwOp,&dwMem);
                dwMem+=Disasm->Address+PrefixesSize+6; // calculate dest addr
                wsprintf(m_bytes,"%08X",dwMem);
                strcat(Instruction,m_bytes);
                lstrcat(Disasm->Assembly,Instruction);

                wsprintf(m_bytes,"%08X",dwOp);
                wsprintf(Instruction,"%02X ",Code);
                lstrcat(Disasm->Opcode,Instruction);
                lstrcat(Disasm->Opcode,m_bytes);       
                Disasm->OpcodeSize=6;
                Disasm->PrefixSize=PrefixesSize;
                (*Index)+=5;
                
            }
            break; // jump instructions set

            case 3:
            {
                if(((BYTE)Opcode[i+2]&0xC0)==0xC0)
                {
                    Bit_D=(Op&0x02)>>1;      // Get bit d (direction)
                    Bit_W=(Op&0x01);         // Get bit w (full/partial reg size)
                    (*Index)++;
                    i=*Index;
                    Mod_11_RM_EX(Bit_D,Bit_W,&Opcode,&Disasm,RegPrefix,Code,&Index,RepPrefix); // Decode with bits
                    Disasm->PrefixSize=PrefixesSize;
                    Disasm->OpcodeSize++; // 0F extra Byte
                }
                else
                {
                    lstrcat(Disasm->Assembly,Instruction);
                    wsprintf(Instruction,"%02X",Code);
                    lstrcat(Disasm->Opcode,Instruction);
                    Disasm->OpcodeSize=2;
                    Disasm->PrefixSize=PrefixesSize;
                    (*Index)++;
                }
            }
            break;

			case 4:
			{
				if( ((BYTE)Opcode[i+2]>=0x08) && ((BYTE)Opcode[i+2]<=0x0F) )
				{
                    (*Index)++;
                    i=*Index;
                    Bit_D=(Op&0x02)>>1;      // Get bit d (direction)
				    Bit_W=(Op&0x01);         // Get bit w (full/partial reg size)
                    Mod_RM_SIB_EX(&Disasm,&Opcode,i,AddrPrefix,SEG,&Index,Code,RegPrefix,SegPrefix,AddrPrefix,Bit_D,Bit_W,RepPrefix);
                    Disasm->PrefixSize=PrefixesSize;
                    Disasm->OpcodeSize++; // 0F extra Byte
				}
				else{
					lstrcat(Disasm->Assembly,"???");
					wsprintf(Instruction,"%02X",Code);
					lstrcat(Disasm->Opcode,Instruction);
					Disasm->OpcodeSize=2;
					Disasm->PrefixSize=PrefixesSize;
					(*Index)++;
				}
			}
			break;
          }
          
          
        }
        break;

		case 0x16: // PUSH SS
        {
            lstrcat(Disasm->Assembly,"push ss");
            strcpy(Disasm->Remarks,"Push SS register to the stack");            
			lstrcat(Disasm->Opcode,"16");            
			Disasm->PrefixSize=PrefixesSize;
        }
        break;

		case 0x17: // POP SS
        {
            lstrcat(Disasm->Assembly,"pop ss");
            strcpy(Disasm->Remarks,"Pop top stack to SS");            
			lstrcat(Disasm->Opcode,"17");
			Disasm->PrefixSize=PrefixesSize;
        }
        break;

		case 0x1E: // PUSH DS
        {
            lstrcat(Disasm->Assembly,"push ds");
            strcpy(Disasm->Remarks,"Push DS register to the stack");            
			lstrcat(Disasm->Opcode,"1E");            
			Disasm->PrefixSize=PrefixesSize;
        }
        break;

		case 0x1F: // POP DS
        {
            lstrcat(Disasm->Assembly,"pop ds");
            strcpy(Disasm->Remarks,"Pop top stack to DS"); 
			lstrcat(Disasm->Opcode,"1F");
			Disasm->PrefixSize=PrefixesSize;
        }
        break;

		case 0x27: // DAA
        {
            lstrcat(Disasm->Assembly,"daa");
            lstrcat(Disasm->Opcode,"27");
			Disasm->PrefixSize=PrefixesSize;
        }
        break;

		case 0x2F: // DAS
        {
            lstrcat(Disasm->Assembly,"das");
            lstrcat(Disasm->Opcode,"2F");
			Disasm->PrefixSize=PrefixesSize;
        }
        break;

		case 0x37: // AAA
        {
            lstrcat(Disasm->Assembly,"aaa");
            lstrcat(Disasm->Opcode,"37");
			Disasm->PrefixSize=PrefixesSize;
        }
        break;

		case 0x3F: // AAS
        {
            lstrcat(Disasm->Assembly,"aas");
            lstrcat(Disasm->Opcode,"3F");
			Disasm->PrefixSize=PrefixesSize;
        }
        break;

		case 0x40:case 0x41: // INC XXX/XX
		case 0x42:case 0x43: // INC XXX/XX
		case 0x44:case 0x45: // INC XXX/XX
		case 0x46:case 0x47: // INC XXX/XX
        {
			wsprintf(menemonic,"inc %s",Regs[RM][Op&0x0F]); // Find reg by Masking (Op&0x0F)
            lstrcat(Disasm->Assembly,menemonic);
			wsprintf(menemonic,"%02X",Op);
            lstrcat(Disasm->Opcode,menemonic);            
			Disasm->PrefixSize=PrefixesSize;
        }
        break;

		case 0x48:case 0x49: // DEC XXX/XX
		case 0x4A:case 0x4B: // DEC XXX/XX
		case 0x4C:case 0x4D: // DEC XXX/XX
		case 0x4E:case 0x4F: // DEC XXX/XX
        {
			wsprintf(menemonic,"dec %s",Regs[RM][Op&0x0F-0x08]);// Find reg by Masking (Op&0x0F-0x08)
            lstrcat(Disasm->Assembly,menemonic);
			wsprintf(menemonic,"%02X",Op);
            lstrcat(Disasm->Opcode,menemonic);
			Disasm->PrefixSize=PrefixesSize;
        }
        break;

		case 0x50:case 0x51: // PUSH XXX/XX
		case 0x52:case 0x53: // PUSH XXX/XX
		case 0x54:case 0x55: // PUSH XXX/XX
		case 0x56:case 0x57: // PUSH XXX/XX
        {
			wsprintf(menemonic,"push %s",Regs[RM][Op&0x0F]);// Find reg by Masking (Op&0x0F)
            lstrcat(Disasm->Assembly,menemonic);
			wsprintf(menemonic,"%02X",Op);
            lstrcat(Disasm->Opcode,menemonic);
			Disasm->PrefixSize=PrefixesSize;
        }
        break;

		case 0x58:case 0x59: // POP XXX/XX
		case 0x5A:case 0x5B: // POP XXX/XX
		case 0x5C:case 0x5D: // POP XXX/XX
		case 0x5E:case 0x5F: // POP XXX/XX
        {
			wsprintf(menemonic,"pop %s",Regs[RM][(Op&0x0F)-0x08]);// Find reg by Masking (Op&0x0F-0x08)
            lstrcat(Disasm->Assembly,menemonic);
			wsprintf(menemonic,"%02X",Op);
            lstrcat(Disasm->Opcode,menemonic);
			Disasm->PrefixSize=PrefixesSize;
        }
        break;

		case 0x60: // PUSHAD/W (Prefix)
        {
			if(!RegPrefix) // if RegPrefix == 0
				lstrcat(Disasm->Assembly,"pushad");
			else if(RegPrefix==1)// Change Reg Size
				lstrcat(Disasm->Assembly,"pushaw");
            
            lstrcat(Disasm->Opcode,"60");            
			Disasm->PrefixSize=PrefixesSize;
        }
        break;

		case 0x61: // POPAD/W (Prefix) 
        {
			if(!RegPrefix) // if RegPrefix == 0
				lstrcat(Disasm->Assembly,"popad");
			else if(RegPrefix==1)// Change Reg Size
				lstrcat(Disasm->Assembly,"popaw");
            
            lstrcat(Disasm->Opcode,"61");            
			Disasm->PrefixSize=PrefixesSize;
        }
        break;

		case 0x68: // PUSH XXXXXXXX
        {
			if(RegPrefix==0)
			{   // PUSH 4 bytes
                SwapDword((BYTE*)(Opcode+i+1),&dwOp,&dwMem);
				wsprintf(menemonic,"push %08X",dwMem);
				lstrcat(Disasm->Assembly,menemonic);
				wsprintf(menemonic,"68 %08X",dwOp);
				lstrcat(Disasm->Opcode,menemonic);
				Disasm->OpcodeSize=5;
				Disasm->PrefixSize=PrefixesSize;
				(*Index)+=4;
			}
			else 
			{
				// PUSH 2 bytes
				SwapWord((BYTE*)(Opcode+i+1),&wOp,&wMem);
                wsprintf(menemonic,"push %04X",wMem);
				lstrcat(Disasm->Assembly,menemonic);
				wsprintf(menemonic,"68 %04X",wOp);
				lstrcat(Disasm->Opcode,menemonic);
				Disasm->OpcodeSize=3;
				Disasm->PrefixSize=PrefixesSize;
				(*Index)+=2;
			}
        }
        break;

		case 0x6A: // PUSH XX
        {
			if((BYTE)Opcode[i+1]>=0x80) // Signed Numebers (Negative)
				wsprintf(menemonic,"push -%02X",(0x100-(BYTE)Opcode[i+1]));
			else
				wsprintf(menemonic,"push %02X",(BYTE)Opcode[i+1]); // Unsigned Numbers (Positive)
            lstrcat(Disasm->Assembly,menemonic);
            wsprintf(menemonic,"6A%02X",(BYTE)*(Opcode+i+1));
            lstrcat(Disasm->Opcode,menemonic);
            Disasm->OpcodeSize=2;
			Disasm->PrefixSize=PrefixesSize;
            ++(*Index);
        }
        break;

		case 0x6C: case 0x6D: // INSB/INSW/INSD
        {
			if((Op&0x0F)==0x0C)
			{
				lstrcat(Disasm->Assembly,"insb");
				wsprintf(menemonic,"Byte ptr ES:[%s], DX",Regs[ADDRM][7]);
				strcpy(Disasm->Remarks,menemonic);
			}
			else  if((Op&0x0F)==0x0D)
					if(!RegPrefix) // If RegPrefix == 0
					{
						lstrcat(Disasm->Assembly,"insd");
						wsprintf(menemonic,"Dword ptr ES:[%s], DX",Regs[ADDRM][7]);
						strcpy(Disasm->Remarks,menemonic);
					}
					else if(RegPrefix==1) // Found RegPrefix == 1
						{
							lstrcat(Disasm->Assembly,"insw");
							wsprintf(menemonic,"Word ptr ES:[%s], DX",Regs[ADDRM][7]);
							strcpy(Disasm->Remarks,menemonic);
						}

			wsprintf(menemonic,"%02X",Op);
            lstrcat(Disasm->Opcode,menemonic);            
			Disasm->PrefixSize=PrefixesSize;
        }
        break;

		case 0x6E: case 0x6F: // OUTSB/OUTSW/OUTSD
        {
			if((Op&0x0F)==0x0E)
			{
				lstrcat(Disasm->Assembly,"outsb");
				wsprintf(menemonic,"DX, Byte ptr ES:[%s]",Regs[ADDRM][7]);
				strcpy(Disasm->Remarks,menemonic);
			}
			else  if((Op&0x0F)==0x0F)
					if(!RegPrefix) // If RegPrefix == 0
					{
						lstrcat(Disasm->Assembly,"outsd");
						wsprintf(menemonic,"DX, Dword ptr ES:[%s]",Regs[ADDRM][7]);
						strcpy(Disasm->Remarks,menemonic);
					}
					else if(RegPrefix==1) // Found RegPrefix == 1
						{
							lstrcat(Disasm->Assembly,"outsw");
							wsprintf(menemonic,"DX, Word ptr ES:[%s]",Regs[ADDRM][7]);
							strcpy(Disasm->Remarks,menemonic);
						}

			wsprintf(menemonic,"%02X",Op);
            lstrcat(Disasm->Opcode,menemonic);            
			Disasm->PrefixSize=PrefixesSize;
        }
        break;

		case 0x70: case 0x71: case 0x72: case 0x73: //JUMP XXXXXXXX
		case 0x74: case 0x75: case 0x76: case 0x77: //JUMP XXXXXXXX 
		case 0x78: case 0x79: case 0x7A: case 0x7B: //JUMP XXXXXXXX
		case 0x7C: case 0x7D: case 0x7E: case 0x7F: //JUMP XXXXXXXX
		case 0xE0: case 0xE1: case 0xE2: case 0xEB: //JUMP XXXXXXXX
        case 0xE3:
        {
			DWORD JumpAddress=0;
			BYTE JumpSize;
			char temp[10];
			JumpSize=(BYTE)Opcode[i+1];

            // Short Jump $+2
			if((BYTE)Opcode[i+1]>0x7F)
				JumpAddress=Disasm->Address + ((2 + PrefixesSize + JumpSize)-0x100);
			else
				JumpAddress=Disasm->Address + 2 + JumpSize  +PrefixesSize;
			
			GetJumpInstruction(Op,temp);
            if(Op==0xE3 && AddrPrefix==1)
			     strcpy(temp,"jcxz");

            wsprintf(menemonic,"%s %08X",temp,JumpAddress);
			lstrcat(Disasm->Assembly,menemonic);
			wsprintf(menemonic,"%02X%02X",Op,(BYTE)Opcode[i+1]);
			lstrcat(Disasm->Opcode,menemonic);
			Disasm->OpcodeSize=2;
			Disasm->PrefixSize=PrefixesSize;
			++(*Index);		
        }
        break;

		case 0x90: // NOP (XCHG EAX, EAX) 
        {
            lstrcat(Disasm->Assembly,"nop");
            lstrcat(Disasm->Opcode,"90");            
			Disasm->PrefixSize=PrefixesSize;
        }
        break;

		case 0x91:case 0x92: // XCHG XXX, XXX
		case 0x93:case 0x94: // XCHG XXX, XXX
		case 0x95:case 0x96: // XCHG XXX, XXX
		case 0x97:           // XCHG XXX, XXX
		{
			Mod_11_RM(1,1,&Opcode,&Disasm,"xchg",RegPrefix,Op,&Index);//+ 0x30			
			Disasm->PrefixSize=PrefixesSize;
		}
		break;

		case 0x98: // CWDE/CDW (Prefix) 
        {
			if(!RegPrefix)
				lstrcat(Disasm->Assembly,"cwde");
			else if(RegPrefix==1)
				lstrcat(Disasm->Assembly,"cbw");

            lstrcat(Disasm->Opcode,"98");            
			Disasm->PrefixSize=PrefixesSize;
        }
        break;

		case 0x99: // CWDE/CDW (Prefix) 
        {
			if(!RegPrefix)
				lstrcat(Disasm->Assembly,"cdq");
			else if(RegPrefix==1)
				lstrcat(Disasm->Assembly,"cwd");

            lstrcat(Disasm->Opcode,"98");            
			Disasm->PrefixSize=PrefixesSize;
        }
        break;

		case 0x9A: case 0xEA: // CALL/JMP XXXX:XXXXXXXX (FAR CALL)
		{
			char temp[10];

			switch(Op)
			{
				case 0x9A:strcpy(temp,"call");break;
				case 0xEA:strcpy(temp,"jmp");break;
			}

			if(AddrPrefix==0)
			{
                SwapDword((BYTE*)(Opcode+i+1),&dwOp,&dwMem);
                SwapWord((BYTE*)(Opcode+i+5),&wOp,&wMem);

				wsprintf(menemonic,"%s %04X:%08X",temp,wMem,dwMem);
				lstrcat(Disasm->Assembly,menemonic);
				wsprintf(menemonic,"%02X %08X %04X",Op,dwOp,wOp);
				lstrcat(Disasm->Opcode,menemonic);
				Disasm->OpcodeSize=7;
				Disasm->PrefixSize=PrefixesSize;
				(*Index)+=6;
			}
			else
			{
                WORD w_op,w_mem;
                SwapWord((BYTE*)(Opcode+i+3),&wOp,&wMem);
                SwapWord((BYTE*)(Opcode+i+1),&w_op,&w_mem);  
                
				wsprintf(menemonic,"%s %04X:%08X",temp,wMem,w_mem);
				lstrcat(Disasm->Assembly,menemonic);
				wsprintf(menemonic,"%02X %04X %04X",Op,w_op,wOp);
				lstrcat(Disasm->Opcode,menemonic);
				Disasm->OpcodeSize=5;
				Disasm->PrefixSize=PrefixesSize;
				(*Index)+=4;
			}
			wsprintf(menemonic,"Far %s",temp);
			strcpy(Disasm->Remarks,menemonic);
			
		}
		break;

		case 0x9B: // WAIT
        {
            lstrcat(Disasm->Assembly,"wait");
            lstrcat(Disasm->Opcode,"9B");            
			Disasm->PrefixSize=PrefixesSize;
        }
        break;

		case 0x9C: // PUSHFD/PUSHFW 
        {
			if(!RegPrefix)
				lstrcat(Disasm->Assembly,"pushfd");
			else if(RegPrefix==1)
				lstrcat(Disasm->Assembly,"pushfw");

            lstrcat(Disasm->Opcode,"9C");            
			Disasm->PrefixSize=PrefixesSize;
        }
        break;

		case 0x9D: // POPFD/POPFW 
        {
			if(!RegPrefix)
				lstrcat(Disasm->Assembly,"popfd");
			else if(RegPrefix==1)
				lstrcat(Disasm->Assembly,"popfw");

            lstrcat(Disasm->Opcode,"9D");            
			Disasm->PrefixSize=PrefixesSize;
        }
        break;

		case 0x9E: // SAHF
        {
            lstrcat(Disasm->Assembly,"sahf");
            lstrcat(Disasm->Opcode,"9E");            
			Disasm->PrefixSize=PrefixesSize;
        }
        break;

		case 0x9F: // LAHF
        {
            lstrcat(Disasm->Assembly,"lahf");
            lstrcat(Disasm->Opcode,"9F");            
			Disasm->PrefixSize=PrefixesSize;
        }
        break;

		case 0xA0:case 0xA2: // MOV AL, BYTE PTR XX:[XXXXXXXX], AL
		{
			if(!AddrPrefix)
			{
                SwapDword((BYTE*)(Opcode+i+1),&dwOp,&dwMem);
				switch(Op)
				{
					case 0xA0:wsprintf(menemonic,"mov al, Byte ptr %s:[%08X]",Segs[SEG],dwMem);break;
					case 0xA2:wsprintf(menemonic,"mov Byte ptr %s:[%08X], al",Segs[SEG],dwMem);break;
				}

				lstrcat(Disasm->Assembly,menemonic);
				wsprintf(menemonic,"%02X %08X",Op,dwOp);
				lstrcat(Disasm->Opcode,menemonic);
				Disasm->OpcodeSize=5;
				Disasm->PrefixSize=PrefixesSize;
				(*Index)+=4;
			}
			else 
			{
                SwapWord((BYTE*)(Opcode+i+1),&wOp,&wMem);
				switch(Op)
				{
					case 0xA0:wsprintf(menemonic,"mov al, Byte ptr %s:[%04X]",Segs[SEG],wMem);break;
					case 0xA2:wsprintf(menemonic,"mov Byte ptr %s:[%04X], al",Segs[SEG],wMem);break;
				}
				lstrcat(Disasm->Assembly,menemonic);
				wsprintf(menemonic,"%02X %04X",Op,wOp);
				lstrcat(Disasm->Opcode,menemonic);
				Disasm->OpcodeSize=3;
				Disasm->PrefixSize=PrefixesSize;
				(*Index)+=2;
			}
		}
		break;

		case 0xA1:case 0xA3: // MOV EAX/AX, BYTE PTR XX:[XXXXXXXX], EAX/AX
		{
			if(!AddrPrefix)// no addr size change
			{
                SwapDword((BYTE*)(Opcode+i+1),&dwOp,&dwMem);
				switch(Op)
				{
					case 0xA1:wsprintf(menemonic,"mov %s, %s ptr %s:[%08X]",Regs[RM][0],RSize,Segs[SEG],dwMem);break;
					case 0xA3:wsprintf(menemonic,"mov %s ptr %s:[%08X], %s",RSize,Segs[SEG],dwMem,Regs[RM][0]);break;
				}
				lstrcat(Disasm->Assembly,menemonic);
				wsprintf(menemonic,"%02X %08X",Op,dwOp);
				lstrcat(Disasm->Opcode,menemonic);
				Disasm->OpcodeSize=5;
				Disasm->PrefixSize=PrefixesSize;
				(*Index)+=4;
			}
			else if(AddrPrefix==1)
			{
                SwapWord((BYTE*)(Opcode+i+1),&wOp,&wMem);
				switch(Op) // change addr size DWORD->WORD
				{
					case 0xA1:wsprintf(menemonic,"mov %s, %s ptr %s:[%04X]",Regs[RM][0],RSize,Segs[SEG],wMem);break;
					case 0xA3:wsprintf(menemonic,"mov %s ptr %s:[%04X], %s",RSize,Segs[SEG],wMem,Regs[RM][0]);break;
				}
				lstrcat(Disasm->Assembly,menemonic);
				wsprintf(menemonic,"%02X %04X",Op,wOp);
				lstrcat(Disasm->Opcode,menemonic);
				Disasm->OpcodeSize=3;
				Disasm->PrefixSize=PrefixesSize;
				(*Index)+=2;
			}						
		}
		break;

        case 0xA4:case 0xA5: // MOVSB/MOVSW/MOVSD
        {
			if(RepPrefix==1 && (BYTE)Opcode[i-1]==0xF3)
				strcpy(Disasm->Assembly,"rep ");

			if((Op&0x0F)==0x04)
				wsprintf(menemonic,"Byte ptr %s:[%s], Byte ptr %s:[%s]",Segs[SEG_ES],Regs[ADDRM][7],Segs[SEG],Regs[ADDRM][6]);
			else if((Op&0x0F)==0x05)
				  wsprintf(menemonic,"%s ptr %s:[%s], %s ptr %s:[%s]",RSize,Segs[SEG_ES],Regs[ADDRM][7],RSize,Segs[SEG],Regs[ADDRM][6]);
			
			lstrcat(Disasm->Assembly,"movs");
            strcpy(Disasm->Remarks,menemonic);
			wsprintf(menemonic,"%02X",Op);
            lstrcat(Disasm->Opcode,menemonic);            
			Disasm->PrefixSize=PrefixesRSize;
        }
        break;

		case 0xA6:case 0xA7: // CMPSB/CMPSW/CMPSD
        {
			if((Op&0x0F)==0x06)
			{
				if(RepPrefix==1)
					wsprintf(menemonic,"Byte ptr %s:[%s], Byte ptr %s:[%s]",Segs[SEG_ES],Regs[ADDRM][7],Segs[SEG],Regs[ADDRM][6]);
				else 
					wsprintf(menemonic,"Byte ptr %s:[%s], Byte ptr %s:[%s]",Segs[SEG],Regs[ADDRM][6],Segs[SEG_ES],Regs[ADDRM][7]);
			}
			else if((Op&0x0F)==0x07)
			{
				if(RepPrefix==1)
				  wsprintf(menemonic,"%s ptr %s:[%s], %s ptr %s:[%s]",RSize,Segs[SEG_ES],Regs[ADDRM][7],RSize,Segs[SEG],Regs[ADDRM][6]);
				else
				  wsprintf(menemonic,"%s ptr %s:[%s], %s ptr %s:[%s]",RSize,Segs[SEG],Regs[ADDRM][6],RSize,Segs[SEG_ES],Regs[ADDRM][7]);
			}
			
			lstrcat(Disasm->Assembly,"cmps");
            strcpy(Disasm->Remarks,menemonic);
			wsprintf(menemonic,"%02X",Op);
            lstrcat(Disasm->Opcode,menemonic);            
			Disasm->PrefixSize=PrefixesRSize;
        }
        break;

		case 0xAA:case 0xAB: // STOSB/STOSW/STOSD
        {
			if(RepPrefix==1 && (BYTE)Opcode[i-1]==0xF3)
				strcpy(Disasm->Assembly,"rep ");

			if((Op&0x0F)==0x0A)
				wsprintf(menemonic,"Byte ptr %s:[%s]",Segs[SEG_ES],Regs[ADDRM][7]);
			else if((Op&0x0F)==0x0B)
				  wsprintf(menemonic,"%s ptr %s:[%s]",RSize,Segs[SEG_ES],Regs[ADDRM][7]);
			
			lstrcat(Disasm->Assembly,"stos");
            strcpy(Disasm->Remarks,menemonic);
			wsprintf(menemonic,"%02X",Op);
            lstrcat(Disasm->Opcode,menemonic);            
			Disasm->PrefixSize=PrefixesRSize;
        }
        break;

		case 0xAC:case 0xAD: // LODSB/LODSW/LODSD
        {
			if(RepPrefix==1 && (BYTE)Opcode[i-1]==0xF3)
				strcpy(Disasm->Assembly,"rep ");

			if((Op&0x0F)==0x0C)
				wsprintf(menemonic,"Byte ptr %s:[%s]",Segs[SEG_DS],Regs[ADDRM][6]);
			else if((Op&0x0F)==0x0D)
				  wsprintf(menemonic,"%s ptr %s:[%s]",RSize,Segs[SEG_DS],Regs[ADDRM][6]);
			
			lstrcat(Disasm->Assembly,"lods");
            strcpy(Disasm->Remarks,menemonic);
			wsprintf(menemonic,"%02X",Op);
            lstrcat(Disasm->Opcode,menemonic);            
			Disasm->PrefixSize=PrefixesRSize;
        }
        break;

		case 0xAE:case 0xAF: // SCASB/SCASW/SCASD
        {
			if((Op&0x0F)==0x0E)
				wsprintf(menemonic,"Byte ptr %s:[%s]",Segs[SEG_ES],Regs[ADDRM][7]);
			else if((Op&0x0F)==0x0F)
				  wsprintf(menemonic,"%s ptr %s:[%s]",RSize,Segs[SEG_ES],Regs[ADDRM][7]);
			
			lstrcat(Disasm->Assembly,"scas");
            strcpy(Disasm->Remarks,menemonic);
			wsprintf(menemonic,"%02X",Op);
            lstrcat(Disasm->Opcode,menemonic);            
			Disasm->PrefixSize=PrefixesRSize;
        }
        break;

		case 0xB0:case 0xB1: // MOV XX, XX
	    case 0xB2:case 0xB3: // MOV XX, XX
		case 0xB4:case 0xB5: // MOV XX, XX
		case 0xB6:case 0xB7: // MOV XX, XX
        {
            wsprintf(menemonic,"mov %s, %02X",Regs[REG8][Op&0xF],(BYTE)Opcode[i+1]);
            lstrcat(Disasm->Assembly,menemonic);
            wsprintf(menemonic,"%02X%02X",Op,(BYTE)*(Opcode+i+1));
            lstrcat(Disasm->Opcode,menemonic);
            Disasm->OpcodeSize=2;
			Disasm->PrefixSize=PrefixesSize;
            ++(*Index);
        }
        break;

		case 0xB8:case 0xB9: // MOV XX/XXX, XXXXXXXX
		case 0xBA:case 0xBB: // MOV XX/XXX, XXXXXXXX
		case 0xBC:case 0xBD: // MOV XX/XXX, XXXXXXXX
		case 0xBE:case 0xBF: // MOV XX/XXX, XXXXXXXX
		{
			if(!RegPrefix) // check if default prefix has changed
			{
                SwapDword((BYTE*)(Opcode+i+1),&dwOp,&dwMem);
				wsprintf(menemonic,"mov %s, %08X",Regs[RM][(Op&0xF)-0x08],dwMem);
				lstrcat(Disasm->Assembly,menemonic);
				wsprintf(menemonic,"%02X %08X",Op,dwOp);
				lstrcat(Disasm->Opcode,menemonic);
				Disasm->OpcodeSize=5;
				Disasm->PrefixSize=PrefixesSize;
				(*Index)+=4;
			}
			else
			{
                SwapWord((BYTE*)(Opcode+i+1),&wOp,&wMem);
				wsprintf(menemonic,"mov %s, %04X",Regs[RM][(Op&0xF)-0x08],wMem);
				lstrcat(Disasm->Assembly,menemonic);    
                wsprintf(menemonic,"%02X %04X",Op,wOp);
				lstrcat(Disasm->Opcode,menemonic);
				Disasm->OpcodeSize=3;
				Disasm->PrefixSize=PrefixesSize;
				(*Index)+=2;
			}
		}
		break;

		case 0xC2:case 0xCA: // RET/F XXXX
		{
			char code[6];
			switch(Op)
			{
		 	    case 0xC2:wsprintf(code,"ret");break;
				case 0xCA:wsprintf(code,"retf");break;
			}
            
            SwapWord((BYTE*)(Opcode+i+1),&wOp,&wMem);
            if(wMem>=0xA000)
              wsprintf(menemonic,"%s %05X",code,wMem);
            else
                wsprintf(menemonic,"%s %04X",code,wMem);
			
			lstrcat(Disasm->Assembly,menemonic);
			wsprintf(menemonic,"%02X %04X",Op,wOp);
			lstrcat(Disasm->Opcode,menemonic);
			Disasm->OpcodeSize=3;
			Disasm->PrefixSize=PrefixesSize;
			if(LockPrefix==1)
				lstrcat(Disasm->Remarks,"<Illegal Lock Prefix>");
			
			(*Index)+=2;
		}
		break;

		case 0xC3: // RET
        {
            lstrcat(Disasm->Assembly,"ret");
            lstrcat(Disasm->Opcode,"C3");
			Disasm->PrefixSize=PrefixesSize;
			lstrcat(Disasm->Remarks,"Pop IP");
        }
        break;

		case 0xC8: // ENTER XXXX, XX
		{
            SwapWord((BYTE*)(Opcode+i+1),&wOp,&wMem);
			wsprintf(menemonic,"enter %04X, %02X",wMem,(BYTE)Opcode[i+3]);
			lstrcat(Disasm->Assembly,menemonic);
			wsprintf(menemonic,"C8 %04X %02X",wOp,(BYTE)Opcode[i+3]);
			lstrcat(Disasm->Opcode,menemonic);
			Disasm->OpcodeSize=4;
			Disasm->PrefixSize=PrefixesSize;
			(*Index)+=3;
        }
		break;

		case 0xC9: // LEAVE
        {
            lstrcat(Disasm->Assembly,"leave");
            lstrcat(Disasm->Opcode,"C9");
			Disasm->PrefixSize=PrefixesSize;
        }
        break;

		case 0xCB: // RETF
        {
            lstrcat(Disasm->Assembly,"retf");
            lstrcat(Disasm->Opcode,"CB");
			Disasm->PrefixSize=PrefixesSize;
        }
        break;

		case 0xCC: // INT 3
        {
            lstrcat(Disasm->Assembly,"int3");
            lstrcat(Disasm->Opcode,"CC");
			Disasm->PrefixSize=PrefixesSize;
        }
        break;

		case 0xCD: // INT XX
        {
			wsprintf(menemonic,"int %02X",(BYTE)Opcode[i+1]);
            lstrcat(Disasm->Assembly,menemonic);
            wsprintf(menemonic,"CD %02X",(BYTE)*(Opcode+i+1));
            lstrcat(Disasm->Opcode,menemonic);
            Disasm->OpcodeSize=2;
			Disasm->PrefixSize=PrefixesSize;
            ++(*Index);
        }
        break;

		case 0xCE: // INTO
        {
            lstrcat(Disasm->Assembly,"into");
            lstrcat(Disasm->Opcode,"CE");
			Disasm->PrefixSize=PrefixesSize;
        }
        break;

		case 0xCF: // IRETD/W
        {
			if(!RegPrefix){
				lstrcat(Disasm->Assembly,"iretd");
			}
			else if(RegPrefix==1){
				lstrcat(Disasm->Assembly,"iretw");
			}
            lstrcat(Disasm->Opcode,"CF");
			Disasm->PrefixSize=PrefixesSize;
        }
        break;

		case 0xD4:case 0xD5: // AAM/AAD
        {
			char opcode[5];
			switch(Op)
			{
				case 0xD4:wsprintf(opcode,"aam");break;
				case 0xD5:wsprintf(opcode,"aad");break;
			}
			wsprintf(menemonic,"%s %02X",opcode,(BYTE)Opcode[i+1]);
            lstrcat(Disasm->Assembly,menemonic);
            wsprintf(menemonic,"%02X%02X",Op,(BYTE)*(Opcode+i+1));
            lstrcat(Disasm->Opcode,menemonic);
            Disasm->OpcodeSize=2;
			Disasm->PrefixSize=PrefixesSize;
            ++(*Index);
        }
        break;

		case 0xD6: // SALC
        {
            lstrcat(Disasm->Assembly,"salc");
            lstrcat(Disasm->Opcode,"D6");
			Disasm->PrefixSize=PrefixesSize;
        }
        break;

		case 0xD7: // XLAT
		{
			lstrcat(Disasm->Assembly,"xlat");
			lstrcat(Disasm->Opcode, "D7");
			wsprintf(menemonic,"Byte ptr %s:[%s+al]",Segs[SEG],Regs[ADDRM][3]);
			lstrcat(Disasm->Remarks,menemonic);
			Disasm->PrefixSize=PrefixesSize;
		}
		break;

		// 0xE4 is at 'in al, xxx' cases
		case 0xE5: // IN EAX/AX, XX
        {
			// special case Opcode, insted of reading DWORD (4 bytes), we read 1 BYTE.
			char mene[10]="";
			GetInstruction(Op,mene); // get instruction from opcode

			if(RegPrefix==0) // no prefix
			{   
				// read 4 bytes into EAX
				wsprintf(menemonic,"%s %s, %02X",mene,Regs[REG32][0],(BYTE)Opcode[i+1]);
				lstrcat(Disasm->Assembly,menemonic);
			}
			else if(RegPrefix==1) // prefix is being used
			{   
				// read 2 bytes into AX
				wsprintf(menemonic,"%s %s, %02X",mene,Regs[REG16][0],(BYTE)Opcode[i+1]);
				lstrcat(Disasm->Assembly,menemonic);
			}
			wsprintf(menemonic,"%02X%02X",Op,(BYTE)Opcode[i+1]);
			lstrcat(Disasm->Opcode,menemonic);
			Disasm->OpcodeSize=2;
			Disasm->PrefixSize=PrefixesSize;
			lstrcat(Disasm->Remarks,"I/O Instruction");
			++(*Index);
        }
        break;

		case 0xE6: // OUT XX, AL
        {
			char mene[10]="";
			GetInstruction(Op,mene);
            wsprintf(menemonic,"%s %02X, al",mene,(BYTE)Opcode[i+1]);
            lstrcat(Disasm->Assembly,menemonic);
            strcpy(Disasm->Remarks,"");
            wsprintf(menemonic,"%02X%02X",Op,(BYTE)*(Opcode+i+1));
            lstrcat(Disasm->Opcode,menemonic);
            Disasm->OpcodeSize=2;
			Disasm->PrefixSize=PrefixesSize;
			lstrcat(Disasm->Remarks,"I/O Instruction");
            ++(*Index);
        }
        break;

		case 0xE7: // OUT XX, AX/EAX
        {
			// special case Opcode, insted of reading DWORD (4 bytes), we read 1 BYTE.
			char mene[10]="";
			GetInstruction(Op,mene); // get instruction from opcode

			if(RegPrefix==0) // no prefix
			{   
				// read 1 byte into EAX
				wsprintf(menemonic,"%s %02X, %s",mene,(BYTE)Opcode[i+1],Regs[REG32][0]);
				lstrcat(Disasm->Assembly,menemonic);
			}
			else if(RegPrefix==1) // prefix is being used
			{   
				// read 1 byte into AX
				wsprintf(menemonic,"%s %02X, %s",mene,(BYTE)Opcode[i+1],Regs[REG16][0]);
				lstrcat(Disasm->Assembly,menemonic);
			}
			wsprintf(menemonic,"%02X%02X",Op,(BYTE)Opcode[i+1]);
			lstrcat(Disasm->Opcode,menemonic);
			Disasm->OpcodeSize=2;
			Disasm->PrefixSize=PrefixesSize;
			lstrcat(Disasm->Remarks,"I/O Instruction");
			++(*Index);
        }
        break;

		case 0xE8:case 0xE9: // CALL/JMP XXXX/XXXXXXXX
		{
			DWORD CallAddress=0;
			DWORD CallSize=0;
			char temp[10];

			switch(Op)
			{
				case 0xE8:strcpy(temp,"call");break;
				case 0xE9:strcpy(temp,"jmp");break;
			}

			if(!RegPrefix)
			{
                SwapDword((BYTE*)(Opcode+i+1),&dwOp,&dwMem);				
				dwMem+= Disasm->Address + CallSize + 5 + (PrefixesSize-RegPrefix);
				wsprintf(menemonic,"%s %08X",temp,dwMem);
				lstrcat(Disasm->Assembly,menemonic);
				wsprintf(menemonic,"%02X %08X",Op,dwOp);
				lstrcat(Disasm->Opcode,menemonic);
				Disasm->OpcodeSize=5;
				Disasm->PrefixSize = PrefixesSize;                
				(*Index)+=4;
			}
			else 
			{
                SwapWord((BYTE*)(Opcode+i+1),&wOp,&wMem);                
				if(wMem>=0x0000F000)
					CallAddress = (wMem + 4 + (PrefixesSize-RegPrefix))-0x0000F000;
				else
					CallAddress = (Disasm->Address-0x00400000) + wMem + 4 + (PrefixesSize-RegPrefix);
				wsprintf(menemonic,"%s %08X",temp, CallAddress);
				lstrcat(Disasm->Assembly,menemonic);
				wsprintf(menemonic,"%02X %04X",Op,wOp);
				lstrcat(Disasm->Opcode,menemonic);
				Disasm->OpcodeSize=3;
				Disasm->PrefixSize = PrefixesSize;
				(*Index)+=2;
			}
		}
		break;

		case 0xEC: // IN AL, DX
        {
            lstrcat(Disasm->Assembly,"in al, dx");
            lstrcat(Disasm->Opcode,"EC");
			Disasm->PrefixSize=PrefixesSize;
			lstrcat(Disasm->Remarks,"I/O Instruction");
        }
        break;

		case 0xED: // IN AX/EAX, DX
        {
			wsprintf(menemonic,"in %s, dx",Regs[RM][0]);
			lstrcat(Disasm->Assembly,menemonic);
            lstrcat(Disasm->Opcode,"ED");
			Disasm->PrefixSize=PrefixesSize;
			lstrcat(Disasm->Remarks,"I/O Instruction");
        }
        break;

		case 0xEE: // OUT DX, AL
        {
            lstrcat(Disasm->Assembly,"out dx, al");
            lstrcat(Disasm->Opcode,"EE");
			Disasm->PrefixSize=PrefixesSize;
			lstrcat(Disasm->Remarks,"I/O Instruction");
        }
        break;

		case 0xEF: // OUT DX, AX/EAX
        {
			wsprintf(menemonic,"out dx, %s",Regs[RM][0]);
			lstrcat(Disasm->Assembly,menemonic);
            lstrcat(Disasm->Opcode,"EF");
			Disasm->PrefixSize=PrefixesSize;
			lstrcat(Disasm->Remarks,"I/O Instruction");
        }
        break;

		case 0xF1: // ICEBP (INT1)
        {
			lstrcat(Disasm->Assembly,"int1");
            lstrcat(Disasm->Opcode,"F1");
			Disasm->PrefixSize=PrefixesSize;
			lstrcat(Disasm->Remarks,"(icebp)");
        }
        break;

		case 0xF4: // HLT (HALT)
        {
			lstrcat(Disasm->Assembly,"hlt");
            lstrcat(Disasm->Opcode,"F4");
			Disasm->PrefixSize=PrefixesSize;
			lstrcat(Disasm->Remarks,"Halts CPU until RESET");
        }
        break;

		case 0xF5: // CMC
        {
			lstrcat(Disasm->Assembly,"cmc");
            lstrcat(Disasm->Opcode,"F4");
			Disasm->PrefixSize=PrefixesSize;
			lstrcat(Disasm->Remarks,"inverts the Carry Flag");
        }
        break;

		case 0xF8: // CLC
        {
			lstrcat(Disasm->Assembly,"clc");
            lstrcat(Disasm->Opcode,"F8");
			Disasm->PrefixSize=PrefixesSize;
			lstrcat(Disasm->Remarks,"Clears the Carry Flag");
        }
        break;

		case 0xF9: // STC
        {
			lstrcat(Disasm->Assembly,"stc");
            lstrcat(Disasm->Opcode,"F9");
			Disasm->PrefixSize=PrefixesSize;
			lstrcat(Disasm->Remarks,"Sets the Carry Flag to 1");
        }
        break;

		case 0xFA: // CLI
        {
			lstrcat(Disasm->Assembly,"cli");
            lstrcat(Disasm->Opcode,"FA");
			Disasm->PrefixSize=PrefixesSize;
			lstrcat(Disasm->Remarks,"Set Interrupt flag to 0");
        }
        break;

		case 0xFB: // STI
        {
			lstrcat(Disasm->Assembly,"sti");
            lstrcat(Disasm->Opcode,"FB");
			Disasm->PrefixSize=PrefixesSize;
			lstrcat(Disasm->Remarks,"Set Interrupt flag to 1");
        }
        break;

		case 0xFC: // CLD
        {
			lstrcat(Disasm->Assembly,"cld");
            lstrcat(Disasm->Opcode,"FC");
			Disasm->PrefixSize=PrefixesSize;
			lstrcat(Disasm->Remarks,"Set Direction Flag to 0");
        }
        break;

		case 0xFD: // STD
        {
			lstrcat(Disasm->Assembly,"std");
            lstrcat(Disasm->Opcode,"FD");
			Disasm->PrefixSize=PrefixesSize;
			lstrcat(Disasm->Remarks,"Set Direction Flag to 1");
        }
        break;
    }
}

void CPayLoadDlg::GetInstruction(BYTE Opcode, char *menemonic)
{
	// Function GetInstance gets 2 parameters:
	// Opcode - byte to get the instruction for
	// Menemonic - pointer, we put menemonic in here
	
	// This function check which instruction belongs
	// To what opcode(s).
	// There are standard 9 groups of instruction with 4 diff
	// Codes (check bit d/w)
	
	
	switch (Opcode)
	{
        // Opcodes for Menemonics
		case 0x04: case 0x05: case 0x00: case 0x01: case 0x02: case 0x03: strcpy(menemonic,"add");break; // ADD
		case 0x0C: case 0x0D: case 0x08: case 0x09: case 0x0A: case 0x0B: strcpy(menemonic,"or"); break; // OR
		case 0x14: case 0x15: case 0x10: case 0x11: case 0x12: case 0x13: strcpy(menemonic,"adc");break; // ADC
		case 0x1C: case 0x1D: case 0x18: case 0x19: case 0x1A: case 0x1B: strcpy(menemonic,"sbb");break; // SBB
		case 0x24: case 0x25: case 0x20: case 0x21: case 0x22: case 0x23: strcpy(menemonic,"and");break; // AND
		case 0x2C: case 0x2D: case 0x28: case 0x29: case 0x2A: case 0x2B: strcpy(menemonic,"sub");break; // SUB
		case 0x34: case 0x35: case 0x30: case 0x31: case 0x32: case 0x33: strcpy(menemonic,"xor");break; // XOR
		case 0x3C: case 0x3D: case 0x38: case 0x39: case 0x3A: case 0x3B: strcpy(menemonic,"cmp");break; // CMP
        case 0x88: case 0x89: case 0x8A: case 0x8B: case 0x8C: case 0x8E: case 0xC6: strcpy(menemonic,"mov");break; // MOV
		case 0x62: strcpy(menemonic,"bound");           break; // BOUND
		case 0x63: strcpy(menemonic,"arpl");            break; // ARPL
		case 0xA8: case 0xA9: strcpy(menemonic,"test"); break; // TEST
		case 0xE4: case 0xE5: strcpy(menemonic,"in");   break; // IN
		case 0xE6: case 0xE7: strcpy(menemonic,"out");  break; // OUT
		case 0x84: case 0x85: strcpy(menemonic,"test"); break; // TEST
		case 0x86: case 0x87: strcpy(menemonic,"xchg"); break; // XCHG
        case 0x8D: strcpy(menemonic,"lea");             break; // LEA
        case 0x8F: strcpy(menemonic,"pop");             break; // POP
        case 0xC4: strcpy(menemonic,"les");             break; // LES
        case 0xC5: strcpy(menemonic,"lds");             break; // LDS
	}
}

int CPayLoadDlg::GetNewInstruction(BYTE Op, char *ASM, bool RegPrefix)
{
	// return values:
    // Found = 0 -> big set instruction
    // Found = 1 -> 1 byte Instruction
    // Found = 2 -> Jump Instruction
    
    int Found=1,RM=REG32;
    char Inst[50]="";
    
    char *JumpTable[16]={
        "jo","jno","jb","jnb","jz","jnz",
        "jbe","ja","js","jns","jpe",
        "jpo","jl","jge","jle","jg"
    };

    switch(Op)
    {
        // 1 BYTE INSTRUCTIONS
        case 0x05: strcpy(Inst,"SysCall"); break;
        case 0x06: strcpy(Inst,"clts");    break;
        case 0x07: strcpy(Inst,"sysret");  break;
        case 0x08: strcpy(Inst,"invd");    break;
        case 0x09: strcpy(Inst,"wbinvd");  break;
        case 0x0B: strcpy(Inst,"ud2");     break;
        case 0x0E: strcpy(Inst,"femms");   break;       
        case 0x30: strcpy(Inst,"wrmsr");   break;
        case 0x31: strcpy(Inst,"rdtsc");   break;
        case 0x32: strcpy(Inst,"rdmsr");   break;
        case 0x33: strcpy(Inst,"rdpmc");   break;
        case 0x34: strcpy(Inst,"sysenter");break;
        case 0x35: strcpy(Inst,"sysexit"); break;
        case 0x77: strcpy(Inst,"emms");    break;        
        case 0xA0: strcpy(Inst,"push fs"); break;
        case 0xA1: strcpy(Inst,"pop fs");  break;
        case 0xA2: strcpy(Inst,"cpuid");   break;
        case 0xA8: strcpy(Inst,"push gs"); break;
        case 0xA9: strcpy(Inst,"pop gs");  break;
        case 0xAA: strcpy(Inst,"rsm");     break;
        
        // BSWAP <REG>
        case 0xC8: case 0xC9: case 0xCA: case 0xCB: 
        case 0xCC: case 0xCD: case 0xCE: case 0xCF:
        {
            if(RegPrefix) // check prefix
                RM=REG16;

            wsprintf(Inst,"bswap %s",regs[RM][Op&7]);
        }
        break;        

        // Invalid instructions, but have s valid 0xC0
        case 0x20: case 0x21:case 0x22:
        case 0x23: case 0x50: case 0xBA:case 0x71: 
        case 0x72:
        {
            strcpy(Inst,"???"); 
            Found=3;
        }
        break;

        // INVALID INSTRUCTIONS!!
        case 0x0F: case 0x19: case 0x1A: case 0x1B: 
        case 0x1C: case 0x1D: case 0x1E: case 0x04: 
        case 0x1F: case 0x0A: case 0x0C: case 0x24:
        case 0x2B: case 0x36: case 0x37: case 0x25:
        case 0x38: case 0x39: case 0x3A: case 0x3B:
        case 0x3C: case 0x3D: case 0x3E: case 0x3F:
        case 0x5A: case 0x5B: case 0x6C: case 0x6D:
        case 0x73: case 0x78: case 0x26: case 0x27:
        case 0x79: case 0x7A: case 0x7B: case 0x7C:
        case 0x7D: case 0xA6: case 0xA7: case 0xB8:
        case 0xB9: case 0xC3: case 0xD0:
        case 0xD4: case 0xD6: case 0xE6: case 0xF0:
        case 0xF4: case 0xFB: case 0xFF:          
        {
            strcpy(Inst,"???");
        }
        break;

        // JUMPS [JXX]
        case 0x80:case 0x81:case 0x82:case 0x83:
        case 0x84:case 0x85:case 0x86:case 0x87:
        case 0x88:case 0x89:case 0x8A:case 0x8B:
        case 0x8C:case 0x8D:case 0x8E:case 0x8F:         
        {
            wsprintf(Inst,"%s ",JumpTable[Op&0x0F]);
            Found=2;
        }
        break;

		// 0F C7 XX [XX- has valid 0x08-0x0F]
		case 0xC7:
		{
			strcpy(Inst,"cmpxchg8b");
			Found=4;
		}
		break;
        
        default:Found=0;break;
    }

    strcpy(ASM,Inst);
    return Found;
}

void CPayLoadDlg::SwapDword(BYTE *MemPtr, DWORD *Original, DWORD *Mirrored)
{
	 DWORD OriginalDword;
    DWORD MirroredDword;
    
    _asm {
            pushad
            mov edi,MemPtr
            mov eax,dword ptr[edi]
            mov MirroredDword,eax  // 15141312
            bswap eax
            mov OriginalDword,eax  // 12131415 
            popad
    }
    
    *Original = OriginalDword;
    *Mirrored = MirroredDword;
}

void CPayLoadDlg::SwapWord(BYTE *MemPtr, WORD *Original, WORD *Mirrored)
{
	WORD OriginalWord;
    WORD MirroredWord;
    
    _asm {
            pushad
            xor eax,eax
            mov edi,MemPtr
            mov ax,word ptr[edi]
            mov MirroredWord,ax  // 1312
            bswap eax
            shr eax,16
            mov OriginalWord,ax  // 1213 
            popad
    }
    
    *Original = OriginalWord;
    *Mirrored = MirroredWord;
}

void CPayLoadDlg::GetJumpInstruction(BYTE Opcode, char *menemonic)
{
	// Function returns the name of the menemonic,
	// Asociated with an opcode
	
	switch (Opcode)
	{
        case 0x70: strcpy(menemonic,"jo");      break;
        case 0x71: strcpy(menemonic,"jno");     break;
        case 0x72: strcpy(menemonic,"jb");      break;
        case 0x73: strcpy(menemonic,"jnb");     break;
        case 0x74: strcpy(menemonic,"jz");      break;
        case 0x75: strcpy(menemonic,"jnz");     break;
        case 0x76: strcpy(menemonic,"jbe");     break;
        case 0x77: strcpy(menemonic,"ja");      break;
        case 0x78: strcpy(menemonic,"js");      break;
        case 0x79: strcpy(menemonic,"jns");     break;
        case 0x7A: strcpy(menemonic,"jp");      break;
        case 0x7B: strcpy(menemonic,"jnp");     break;
        case 0x7C: strcpy(menemonic,"jl");      break;
        case 0x7D: strcpy(menemonic,"jge");     break;
        case 0x7E: strcpy(menemonic,"jle");     break;
        case 0x7F: strcpy(menemonic,"jg");	    break;
        case 0xE0: strcpy(menemonic,"loopne");  break;
        case 0xE1: strcpy(menemonic,"loope");   break;
        case 0xE2: strcpy(menemonic,"loop");    break;
        case 0xE3: strcpy(menemonic,"jecxz");   break;
        case 0xEB: strcpy(menemonic,"jmp");     break;
	}
}

void CPayLoadDlg::Mod_11_RM(BYTE d, BYTE w, char **Opcode, DISASSEMBLY **Disasm, char instruction[], bool PrefixReg, BYTE Op, DWORD **index)
{
	/* 
       Function Mod_11_RM Checks whatever we have
	   Both bit d (direction) and bit w (full/partial size).
	 
       There are 4 states:
	   00 - d=0 / w=0 ; direction -> (ie: DH->DL),   partial size (AL,DH,BL..)
	   01 - d=0 / w=1 ; direction -> (ie: EDX->EAX), partial size (EAX,EBP,EDI..)
	   10 - d=1 / w=0 ; direction <- (ie: DH<-DL),   partial size (AL,DH,BL..)
	   11 - d=1 / w=1 ; direction <- (ie: EDX<-EAX), partial size (EAX,EBP,EDI..)
	
       Also deals with harder opcodes which have diffrent
       Addresing type.
    */

    DWORD dwMem=0,dwOp=0;
	int RM,IndexAdd=1,m_OpcodeSize=2,Pos; // Register(s) Pointer
    WORD wMem=0,wOp=0;
	BYTE reg1=0,reg2=0,m_Opcode=0,REG;
    BYTE FOpcode;
	char assembly[50]="",temp[128]="",m_Bytes[128]="";
    
    Pos=(*(*index)); // Current Position
    
    m_Opcode = (BYTE)(*(*Opcode+Pos+1));// Decode registers from second byte
    
    // Strip Used Instructions / Used Segment
    REG=(BYTE)(*(*Opcode+Pos+1)); 
    REG>>=3;
	REG&=0x07;

    // Check Opcode range
    if((Op>=0x80 && Op<=0x83) || Op==0xC7 || Op==0x69)
    {    
        switch(Op) // Find Current Opcode
        {
            // Diffrent Opcodes ahs different Modes

            case 0x80: case 0x82: case 0x83:// 1 byte
            {
                RM=REG8;
                if(Op==0x83 && PrefixReg==0) // full size reg
                    RM=REG32;
				
				if(PrefixReg==1)
					RM=REG16;
				
                reg1=(m_Opcode&7); // Get Destination Register                
                SwapWord((BYTE*)(*Opcode+Pos+1),&wOp,&wMem);
                FOpcode=wOp&0x00FF;
                
                if(FOpcode>0x7F) // check for signed numbers!!
                {
                    FOpcode = 0x100-FOpcode; // -XX
                    wsprintf(temp,"%s%02X",Scale[0],FOpcode); // '-' aritmathic                    
				}
                else
                    wsprintf(temp,"%02X",FOpcode);

                // Read Opcodes: Opcode - imm8
                wsprintf(m_Bytes,"%02X%04X",Op,wOp);

                m_OpcodeSize=3;
                (*(*index))+=2; // Prepare to read next Instruction
            }
            break;
            
            case 0x81: case 0xC7: case 0x69: // 2 (WORD)/4 (DWORD) bytes
            {
                // 0x66 is being Used
                if(PrefixReg==1) // READ WORD
                {
                    RM=REG16;
                    reg1=(m_Opcode&0x07); // Get Destination Register
                    SwapWord((BYTE*)(*Opcode+Pos+2),&wOp,&wMem);
                    SwapDword((BYTE*)(*Opcode+Pos),&dwOp,&dwMem);
                    // Read imm16
                    wsprintf(temp,"%04X",wMem);                    
                    // Read Opcodes: Opcode - imm16
                    wsprintf(m_Bytes,"%08X",dwOp);                    
                    m_OpcodeSize=4; // Instruction Size
                    (*(*index))+=3;
                }
                else // READ DWORD
                {
                    RM=REG32;
                    reg1=(m_Opcode&0x07); // Get Destination Register                    
                    SwapDword((BYTE*)(*Opcode+Pos+2),&dwOp,&dwMem);
                    SwapWord((BYTE*)(*Opcode+Pos),&wOp,&wMem);
                    // Read Dword Data number (imm32)
                    wsprintf(temp,"%08X",dwMem);                    
                    // Read Opcodes: Opcode - imm32
                    wsprintf(m_Bytes,"%04X %08X",wOp,dwOp);
                    m_OpcodeSize=6; // Instruction Size
                    (*(*index))+=5;                    
                }                
            }
            break;
        }
        
        if(Op==0xC7)
        {
			/* 
				Instruction rule: Mem,Imm ->  1100011woo000mmm,imm
				Code Block: 1100011
				w = Reg Size
				oo - Mod
				000 - Must be!
				mmm - Reg/Mem
				imm - Immidiant (麽弪)
			*/
                          
          if(((m_Opcode&0x38)>>3)!=0) // check 000
              lstrcat((*Disasm)->Remarks,"Invalid Instruction");

          wsprintf(assembly,"%s %s, %s","mov",regs[RM][reg1],temp);
        }
        else{        
            // Build assembly
            if(Op==0x69)
            {
                reg2=((m_Opcode&0x038)>>3);
                wsprintf(assembly,"imul %s, %s, %s",regs[RM][reg2],regs[RM][reg1],temp);
            }
            else
                wsprintf(assembly,"%s %s, %s",Instructions[REG],regs[RM][reg1],temp);
        }
        
        lstrcat((*Disasm)->Assembly,assembly);
        (*Disasm)->OpcodeSize=m_OpcodeSize;
        lstrcat((*Disasm)->Opcode,m_Bytes);
        return; // RET
        
    }
    else{ // Check Other Set of Opcodes        
        
        // Special Types usnig Segments
        if(Op==0x8C || Op==0x8E)
        {
            RM=REG16;
            reg1=(m_Opcode&0x07);
            SwapWord((BYTE*)(*Opcode+Pos),&wOp,&wMem);
            wsprintf(m_Bytes,"%04X",wOp);
            
            if(REG<=5) // SEG IS KNOWN
            {
                if(d==0) // (->) Direction
                {
                    wsprintf(assembly,"%s %s, %s",instruction,regs[RM][reg1],segs[REG]);
                }
                else // (<-) Direction
                {
                    wsprintf(assembly,"%s %s, %s",instruction,segs[REG],regs[RM][reg1]);
                }
            }
            else // UNKNOWN SEG (NOT IN RANGE 0-5)
            {
                if(d==0) // (->) Direction
                {                
                    wsprintf(assembly,"%s %s, SEG ??",instruction,regs[RM][reg1]);
                }
                else //(<-) Direction
                {
                    wsprintf(assembly,"%s SEG ??,%s",instruction,regs[RM][reg1]);
                }

                // Put warning
                lstrcat((*Disasm)->Remarks,"Unknown Segment Used,");
            }
            
            // Add data to the Struct
            (*Disasm)->OpcodeSize=2; // Instruction Size
            lstrcat((*Disasm)->Assembly,assembly);
            lstrcat((*Disasm)->Opcode,m_Bytes); 
            
            // Segment Modification Opcode ( MOV <SEG>, <REG>)
            if(Op==0x8E) 
                lstrcat((*Disasm)->Remarks,"Segment Is Being Modified!");
            
            (*(*index))++;

            return;
        }

		if(Op==0xC6)
		{
			RM=REG8;
			if(m_Opcode>=0xC0 && m_Opcode<=0xC7)
			{
				reg1=(m_Opcode&0x07); // Get Destination Register
				SwapWord((BYTE*)(*Opcode+Pos+1),&wOp,&wMem);
				// Read imm16
				wsprintf(temp,"%02X",*((BYTE*)(*Opcode+Pos+2)));
				wsprintf(m_Bytes,"C6 %04X",wOp);
				// Read Opcodes: Opcode - imm16                   
				m_OpcodeSize=3; // Instruction Size
				(*(*index))+=2;
				wsprintf(assembly,"%s %s, %s","mov",regs[RM][reg1],temp);
			}
			else
			{
				SwapWord((BYTE*)(*Opcode+Pos+1),&wOp,&wMem);
				wsprintf(m_Bytes,"C6 %04X",wOp);
				m_OpcodeSize=3;
				(*(*index))+=2;
				lstrcpy(assembly,"???");
			}
			lstrcat((*Disasm)->Assembly,assembly);
			(*Disasm)->OpcodeSize=m_OpcodeSize;
			lstrcat((*Disasm)->Opcode,m_Bytes);
			return;
		}

        // Mixed Instructions
        if(Op==0xC0 || Op==0xC1)
        {
            // Check register Size
            if(w==0) 
                RM=REG8;
            else
            {
                if(PrefixReg==1)
                    RM=REG16;
                else
                    RM=REG32;
            }       

            reg1=(m_Opcode&7); // Get Destination Register
            SwapWord((BYTE*)(*Opcode+Pos+1),&wOp,&wMem);
            wsprintf(temp,"%02X",wOp&0x00FF);            
            // Read Opcodes: Opcode - imm8
            wsprintf(m_Bytes,"%02X%04X",Op,wOp);
            m_OpcodeSize=3;
            (*(*index))+=2; // Prepare to read next Instruction
            // Build assembly
            wsprintf(assembly,"%s %s, %s",ArtimaticInstructions[REG],regs[RM][reg1],temp);
            lstrcat((*Disasm)->Assembly,assembly);
            (*Disasm)->OpcodeSize=m_OpcodeSize;
            lstrcat((*Disasm)->Opcode,m_Bytes);
            return; // exit the function
        }

        // XCHG Register
        if(Op>=0x91 && Op<=0x97) 
        {
            m_Opcode=(*(*Opcode+Pos)); // 1 byte Opcode
            m_Opcode+=0x30;            // Add 0x30 in order to get values of EAX-EDI (trick)
            IndexAdd=0;                // Dont Add to the index counter.
            m_OpcodeSize=1;            // 1 byte opcode          
        }

        // (->) / reg8
        if(d==0 && w==0)
        {    
            RM=REG8;
            reg1=(m_Opcode&0x07);
            reg2=(m_Opcode&0x38)>>3;
        }
        
        // (->) / reg32
        if(d==0 && w==1)
        {    
            RM=REG32;
            if(PrefixReg==1)
                RM=REG16; // (->) / reg16 (RegPerfix is being used)

            reg1=(m_Opcode&0x07);
            reg2=(m_Opcode&0x38)>>3;
        }
        
        // (<-) / reg8
        if(d==1 && w==0)
        {    
            RM=REG8;
            reg2=(m_Opcode&0x07);
            reg1=(m_Opcode&0x38)>>3;
        }
        
        // (<-) / reg32
        if(d==1 && w==1)
        {    
            RM=REG32;
            if(PrefixReg==1)
                RM=REG16; // (<-) / reg16

           reg2=(m_Opcode&0x07);
           reg1=(m_Opcode&0x38)>>3;
        }
        
        // Check Opcode Size (XCHG changes it)
       if(m_OpcodeSize==1)
       {
           wsprintf(temp,"%02X",Op);
       }
       else // Default
       {   
           SwapWord((BYTE*)(*Opcode+Pos),&wOp,&wMem);
           wsprintf(temp,"%04X",wOp);
       }       

       switch(Op) 
       {
         case 0x6B: // IMUL REG,REG,IIM
         {
             SwapWord((BYTE*)(*Opcode+Pos+1),&wOp,&wMem);
             FOpcode=wOp&0x00FF;

             if(FOpcode>0x7F) // check for signed numbers!!
             {
                 FOpcode = 0x100-FOpcode; // -XX (Signed)
                 wsprintf(temp,"%s",Scale[0]); // '-' aritmathic (Signed)                            
             }
             else                    
                strcpy(temp,"");

             m_OpcodeSize=3;
             (*(*index))++;
             wsprintf(assembly,"imul %s,%s,%s%02X",regs[RM][reg1],regs[RM][reg2],temp,FOpcode);
             wsprintf(temp,"%02X%04X",Op,wOp);
         }
         break;
           
         case 0x8F: // POP REG
         {
           if((BYTE)(*(*Opcode+Pos+1))>=0xC8) // above bytes has !=000 there for invalid
              lstrcat((*Disasm)->Remarks,"Invalid Instruction");
                 
           wsprintf(assembly,"%s %s",instruction,regs[RM][reg2]);
         }
         break;

         case 0xD0: case 0xD1: 
         {
             wsprintf(assembly,"%s %s, 1",ArtimaticInstructions[REG],regs[RM][reg1]);
         }
         break;

         case 0xD2: case 0xD3: 
         {
             wsprintf(assembly,"%s %s, cl",ArtimaticInstructions[REG],regs[RM][reg2]);
         }
         break;

         case 0xD8:// FPU Instruction
         {
             if(REG==3) // fcomp uses 1 operand
             {
                 wsprintf(assembly,"%s %s",FpuInstructions[REG],FpuRegs[reg1]);
             }
             else // st(0) is the dest
             {
                 wsprintf(assembly,"%s st,%s",FpuInstructions[REG],FpuRegs[reg1]);
             }
         }
         break;
         
         case 0xD9: // FPU Instructions
         {
             // 2 byte FPU Instructions
             switch((BYTE)(*(*Opcode+Pos+1)))
             {
                case 0xC8:case 0xC9:case 0xCA:case 0xCB:
                case 0xCC:case 0xCD:case 0xCE:case 0xCF:
                {
                    wsprintf(assembly,"fxch %s",FpuRegs[reg1]);
                }
                break;
                
                case 0xD1:case 0xD2:case 0xD3:case 0xD4:
                case 0xD5:case 0xD6:case 0xD7:
                {
                   wsprintf(assembly,"fst %s",FpuRegs[reg1]);
                }
                break;

                case 0xD8:case 0xD9:case 0xDA:case 0xDB:
                case 0xDC:case 0xDD:case 0xDE:case 0xDF:
                {
                   wsprintf(assembly,"fstp %s",FpuRegs[reg1]);
                }
                break;

                case 0xE2:case 0xE3:case 0xE6:case 0xE7:
                {
                   wsprintf(assembly,"fldenv %s",FpuRegs[reg1]);
                }
                break;

                case 0xEF:
                {
                   wsprintf(assembly,"fldcw %s",FpuRegs[reg1]);
                }
                break;

                case 0xC0:case 0xC1:case 0xC2:case 0xC3:case 0xC4:
				case 0xC5:case 0xC6:case 0xC7:
				{
					wsprintf(assembly,"fld %s",FpuRegs[reg1]);
				}
                break;
					
                case 0xD0: strcpy(assembly,"fnop");    break;
                case 0xE0: strcpy(assembly,"fchs");    break;
                case 0xE1: strcpy(assembly,"fabs");    break;
                case 0xE4: strcpy(assembly,"ftst");    break;
                case 0xE5: strcpy(assembly,"fxam");    break;
                case 0xE8: strcpy(assembly,"fld1");    break;
                case 0xE9: strcpy(assembly,"fldl2t");  break;
                case 0xEA: strcpy(assembly,"fldl2e");  break;
                case 0xEB: strcpy(assembly,"fldpi");   break;
                case 0xEC: strcpy(assembly,"fldlg2");  break;
                case 0xED: strcpy(assembly,"fldln2");  break;
                case 0xEE: strcpy(assembly,"fldz");    break;
                case 0xF0: strcpy(assembly,"f2xm1");   break;
                case 0xF1: strcpy(assembly,"fyl2x");   break;
                case 0xF2: strcpy(assembly,"fptan");   break;
                case 0xF3: strcpy(assembly,"fpatan");  break;
                case 0xF4: strcpy(assembly,"fxtract"); break;
                case 0xF5: strcpy(assembly,"fprem1");  break;
                case 0xF6: strcpy(assembly,"fdecstp"); break;
                case 0xF7: strcpy(assembly,"fincstp"); break;
                case 0xF8: strcpy(assembly,"fprem");   break;
                case 0xF9: strcpy(assembly,"fyl2xp1"); break;
                case 0xFA: strcpy(assembly,"fsqrt");   break;
                case 0xFB: strcpy(assembly,"fsincos"); break;
                case 0xFC: strcpy(assembly,"frndint"); break;
                case 0xFD: strcpy(assembly,"fscale");  break;
                case 0xFE: strcpy(assembly,"fsin");    break;
                case 0xFF: strcpy(assembly,"fcos");    break;                                
             }                 
         }
         break;
         
         case 0xDA: // FPU Instructions
         {
             switch((BYTE)(*(*Opcode+Pos+1)))
             {
                case 0xC0:case 0xC1:case 0xC2:case 0xC3: // FCMOVB
                case 0xC4:case 0xC5:case 0xC6:case 0xC7:
                {
                    wsprintf(assembly,"fcmovb st,%s",FpuRegs[reg2]);
                }
                break;

                case 0xC8:case 0xC9:case 0xCA:case 0xCB: // FCMOVE
                case 0xCC:case 0xCD:case 0xCE:case 0xCF:
                {
                   wsprintf(assembly,"fcmove st,%s",FpuRegs[reg2]);
                }
                break;

                case 0xD0:case 0xD1:case 0xD2:case 0xD3: // FCMOVBE
                case 0xD4:case 0xD5:case 0xD6:case 0xD7:
                {
                   wsprintf(assembly,"fcmovbe st,%s",FpuRegs[reg2]);
                }
                break;
                    
                case 0xD8:case 0xD9:case 0xDA:case 0xDB: // FCMOVU
                case 0xDC:case 0xDD:case 0xDE:case 0xDF:
                {
                   wsprintf(assembly,"fcmovu st,%s",FpuRegs[reg2]);
                }
                break;

                // Default Signed FPU Instructions
                default: wsprintf(assembly,"%s %s",FpuInstructionsSigned[REG],FpuRegs[reg2]); break;
             }
         }
         break;

         case 0xDB: // FPU Instruction
         {
             switch((BYTE)(*(*Opcode+Pos+1)))
             {
                 case 0xC0:case 0xC1:case 0xC2:case 0xC3: // FCMOVNB
                 case 0xC4:case 0xC5:case 0xC6:case 0xC7: // FCMOVNB
                 {
                     wsprintf(assembly,"fcmovnb st,%s",FpuRegs[reg2]);
                 }
                 break;
                 
                 case 0xC8:case 0xC9:case 0xCA:case 0xCB: // FCMOVNE
                 case 0xCC:case 0xCD:case 0xCE:case 0xCF: // FCMOVNE
                 {
                     wsprintf(assembly,"fcmovne st,%s",FpuRegs[reg2]);
                 }
                 break;
                 
                 case 0xD0:case 0xD1:case 0xD2:case 0xD3: // FCMOVNBE
                 case 0xD4:case 0xD5:case 0xD6:case 0xD7: // FCMOVNBE
                 {
                     wsprintf(assembly,"fcmovnbe st,%s",FpuRegs[reg2]);
                 }
                 break;
                 
                 case 0xD8:case 0xD9:case 0xDA:case 0xDB: // FCMOVNU
                 case 0xDC:case 0xDD:case 0xDE:case 0xDF: // FCMOVNU
                 {
                     wsprintf(assembly,"fcmovnu st,%s",FpuRegs[reg2]);
                 }
                 break;
                 
                 case 0xE0: strcpy(assembly,"feni");  break;
                 case 0xE1: strcpy(assembly,"fdisi"); break;
                 case 0xE2: strcpy(assembly,"fclex"); break;
                 case 0xE3: strcpy(assembly,"finit"); break;

                 case 0xE4: case 0xE5: case 0xE6: case 0xE7: // (Invalid) Reserved instructions..???
                 {
                     lstrcat((*Disasm)->Remarks,"Invalid Instruction");
                     strcpy(assembly,"???");
                 }
                 break;

                 case 0xE8:case 0xE9:case 0xEA:case 0xEB: // 
                 case 0xEC:case 0xED:case 0xEE:case 0xEF: // 
                 {
                     wsprintf(assembly,"fucomi st,%s",FpuRegs[reg2]);
                 }
                 break;

                 case 0xF0:case 0xF1:case 0xF2:case 0xF3: // 
                 case 0xF4:case 0xF5:case 0xF6:case 0xF7: // 
                 {
                     wsprintf(assembly,"fcomi st,%s",FpuRegs[reg2]);
                 }
                 break;

                 default: wsprintf(assembly,"fstp %s",FpuRegs[reg2]); break;
             }
         }
         break;
         
         case 0xDC:// FPU Instruction
         {                          
            if(REG==3) // fcomp uses 1 operand
            {
              wsprintf(assembly,"%s %s",FpuInstructions[REG],FpuRegs[reg1]);
            }
            else // st(0) is the src
            {                
                switch(REG) // fdiv<->fdivr / fsub <-> fsubr (changed positions)
                {
                   case 4:REG++;break;
                   case 5:REG--;break;
                   case 6:REG++;break;
                   case 7:REG--;break;
                }                                              
                            
              wsprintf(assembly,"%s %s,st",FpuInstructions[REG],FpuRegs[reg1]);
            }
         }
         break;

         case 0xDD: // FPU Instruction
         {
             switch((BYTE)(*(*Opcode+Pos+1)))
             {             
                case 0xC0:case 0xC1:case 0xC2:case 0xC3: 
                case 0xC4:case 0xC5:case 0xC6:case 0xC7: 
                {
                   wsprintf(assembly,"ffree %s",FpuRegs[reg1]);
                }
                break;
                 
                case 0xC8:case 0xC9:case 0xCA:case 0xCB: 
                case 0xCC:case 0xCD:case 0xCE:case 0xCF: 
                {
                   lstrcat((*Disasm)->Remarks,"Invalid Instruction");
                   strcpy(assembly,"???");
                }
                break;
                 
                case 0xD0:case 0xD1:case 0xD2:case 0xD3: 
                case 0xD4:case 0xD5:case 0xD6:case 0xD7: 
                case 0xD8:case 0xD9:case 0xDA:case 0xDB: 
                case 0xDC:case 0xDD:case 0xDE:case 0xDF:
                {
                  wsprintf(assembly,"%s %s",FpuInstructionsSet2[REG],FpuRegs[reg1]);
                }
                break;

                case 0xE0:case 0xE1:case 0xE2:case 0xE3: 
                case 0xE4:case 0xE5:case 0xE6:case 0xE7: 
                {
                  wsprintf(assembly,"fucom %s",FpuRegs[reg1]);
                }
                break;
                
                case 0xE8:case 0xE9:case 0xEA:case 0xEB: 
                case 0xEC:case 0xED:case 0xEE:case 0xEF: 
                {
                  wsprintf(assembly,"fucomp %s",FpuRegs[reg1]);
                }
                break;

                case 0xF0:case 0xF1:case 0xF2:case 0xF3:  
                case 0xF4:case 0xF5:case 0xF6:case 0xF7: 
                case 0xF8:case 0xF9:case 0xFA:case 0xFB:  
                case 0xFC:case 0xFD:case 0xFE:case 0xFF: 
                {
                   wsprintf(assembly,"%s %s",FpuInstructionsSet3[REG],FpuRegs[reg1]);
                }
                break;                
             }             
         }
         break;

         case 0xDE: // FPU Instruction
         {
             switch((BYTE)(*(*Opcode+Pos+1)))
             {
                 case 0xC0:case 0xC1:case 0xC2:case 0xC3: 
                 case 0xC4:case 0xC5:case 0xC6:case 0xC7: 
                 {
                     wsprintf(assembly,"faddp %s,st",FpuRegs[reg2]);
                 }
                 break;
                 
                 case 0xC8:case 0xC9:case 0xCA:case 0xCB: 
                 case 0xCC:case 0xCD:case 0xCE:case 0xCF: 
                 {                     
                     wsprintf(assembly,"fmulp %s,st",FpuRegs[reg2]);
                 }
                 break;

                 case 0xD0:case 0xD1:case 0xD2:case 0xD3: 
                 case 0xD4:case 0xD5:case 0xD6:case 0xD7: 
                 {
                   wsprintf(assembly,"ficom %s",FpuRegs[reg2]);
                 }
                 break;

                 case 0xD8:case 0xD9:case 0xDA:case 0xDB: 
                 case 0xDC:case 0xDD:case 0xDE:case 0xDF:
                 {
                   wsprintf(assembly,"ficomp %s",FpuRegs[reg2]);
                 }
                 break;

                 case 0xE0:case 0xE1:case 0xE2:case 0xE3: 
                 case 0xE4:case 0xE5:case 0xE6:case 0xE7: 
                 {
                   wsprintf(assembly,"fsubrp %s,st",FpuRegs[reg2]);
                 }
                 break;

                 case 0xE9:
                 {
                     strcpy(assembly,"fcompp");
                 }
                 break;

                 case 0xE8:case 0xEA:case 0xEB: 
                 case 0xEC:case 0xED:case 0xEE:case 0xEF: 
                 {
                   wsprintf(assembly,"fsubp %s,st",FpuRegs[reg2]);
                 }
                 break;

                 case 0xF0:case 0xF1:case 0xF2:case 0xF3:  
                 case 0xF4:case 0xF5:case 0xF6:case 0xF7: 
                 {
                   wsprintf(assembly,"fdivrp %s,st",FpuRegs[reg2]);
                 }
                 break;

                 case 0xF8:case 0xF9:case 0xFA:case 0xFB:  
                 case 0xFC:case 0xFD:case 0xFE:case 0xFF: 
                 {
                   wsprintf(assembly,"fdivp %s,st",FpuRegs[reg2]);
                 }
                 break;
             }
         }
         break;

         case 0xDF: // FPU Instruction
         {
             switch((BYTE)(*(*Opcode+Pos+1)))
             {
                case 0xC0:case 0xC1:case 0xC2:case 0xC3: 
                case 0xC4:case 0xC5:case 0xC6:case 0xC7: 
                {
                   wsprintf(assembly,"ffreep %s",FpuRegs[reg2]);
                }
                break;
                     
                case 0xC8:case 0xC9:case 0xCA:case 0xCB: 
                case 0xCC:case 0xCD:case 0xCE:case 0xCF: 
                {                     
                   lstrcat((*Disasm)->Remarks,"Invalid Instruction");
                   strcpy(assembly,"???");
                }
                break;
                     
                case 0xD0:case 0xD1:case 0xD2:case 0xD3: 
                case 0xD4:case 0xD5:case 0xD6:case 0xD7: 
                {
                   wsprintf(assembly,"fist %s",FpuRegs[reg2]);
                }
                break;
                     
                case 0xD8:case 0xD9:case 0xDA:case 0xDB: 
                case 0xDC:case 0xDD:case 0xDE:case 0xDF:
                {
                  wsprintf(assembly,"fistp %s",FpuRegs[reg2]);
                }
                break;
                     
                case 0xE0:
                {
                    strcpy(assembly,"fnstsw ax");
                }
                break;

                case 0xE1:case 0xE2:case 0xE3: 
                case 0xE4:case 0xE5:case 0xE6:case 0xE7: 
                {
                  wsprintf(assembly,"fbld %s",FpuRegs[reg2]);
                }
                break;
                     
                case 0xE9:case 0xE8:case 0xEA:case 0xEB: 
                case 0xEC:case 0xED:case 0xEE:case 0xEF: 
                {
                   wsprintf(assembly,"fucomip st,%s",FpuRegs[reg2]);
                }
                break;
                     
                case 0xF0:case 0xF1:case 0xF2:case 0xF3:  
                case 0xF4:case 0xF5:case 0xF6:case 0xF7: 
                {
                  wsprintf(assembly,"fcomip st,%s",FpuRegs[reg2]);
                }
                break;
                     
                case 0xF8:case 0xF9:case 0xFA:case 0xFB:  
                case 0xFC:case 0xFD:case 0xFE:case 0xFF: 
                {
                  wsprintf(assembly,"fistp %s",FpuRegs[reg2]);
                }
                break;
            }
         }
         break;

         case 0xF6:
         {             
             if(reg1==0 || reg1==1)
             {
                 SwapWord((BYTE*)(*Opcode+Pos+1),&wOp,&wMem);
                 wsprintf(assembly,"%s %s,%02X",InstructionsSet2[REG],regs[RM][reg2],wOp&0x00FF);
                 (*(*index))++;
                 m_OpcodeSize++;
                 wsprintf(m_Bytes,"%02X",wOp&0x00FF);
                 lstrcat(temp,m_Bytes);
             }
             else
                 wsprintf(assembly,"%s %s",InstructionsSet2[REG],regs[RM][reg2]);
         }
         break;

         case 0xF7:
         {             
           if(reg1==0 || reg1==1)
           {
               if(!PrefixReg) // no 0x66 prefix used (read DWORD)
               {
                 SwapDword((BYTE*)(*Opcode+Pos+2),&dwOp,&dwMem);
                 wsprintf(assembly,"%s %s,%08X",InstructionsSet2[REG],regs[RM][reg2],dwMem);                 
                 wsprintf(m_Bytes," %08X",dwOp);
                  (*(*index))+=4; 
                  m_OpcodeSize+=4;
               }
               else // prefix 0x66 is being used (read WORD)
               {
                   SwapWord((BYTE*)(*Opcode+Pos+2),&wOp,&wMem);
                   wsprintf(assembly,"%s %s,%04X",InstructionsSet2[REG],regs[RM][reg2],wMem);                   
                   wsprintf(m_Bytes," %04X",wOp);                   
                   (*(*index))+=2;
                   m_OpcodeSize+=2;
               }
                          
             lstrcat(temp,m_Bytes);
           }
           else
               wsprintf(assembly,"%s %s",InstructionsSet2[REG],regs[RM][reg2]);
         }
         break;

         case 0xFE: // MIX Instructions (INC,DEC,INVALID,INVALID...)
         {
             wsprintf(assembly,"%s %s",InstructionsSet3[REG],regs[RM][reg2]);
             if(REG>1)
                 lstrcat((*Disasm)->Remarks,"Illegal Instruction");
         }
         break;
         
         case 0xFF:
         {
             wsprintf(assembly,"%s %s",InstructionsSet4[REG],regs[RM][reg2]);

             if(REG==7)
                 lstrcat((*Disasm)->Remarks,"Illegal Instruction");
         }
         break;

		 case 0x8D:{
			wsprintf(assembly,"%s %s, %s",instruction,regs[RM][reg2],regs[RM][reg1]);
			lstrcat((*Disasm)->Remarks,"Illegal Instruction");
		 }
		 break;
         
         // Default General Instructions
         default: wsprintf(assembly,"%s %s, %s",instruction,regs[RM][reg1],regs[RM][reg2]); break;
       }

       lstrcat((*Disasm)->Assembly,assembly);
       (*Disasm)->OpcodeSize=m_OpcodeSize;
       lstrcat((*Disasm)->Opcode,temp);

       (*(*index))+=IndexAdd;
       // strcpy(menemonic,assembly);
    }

    return; // RET
}

void CPayLoadDlg::Mod_RM_SIB(DISASSEMBLY **Disasm, char **Opcode, int pos, bool AddrPrefix, int SEG, DWORD **index, BYTE Bit_d, BYTE Bit_w, char *instruction, BYTE Op, bool PrefixReg, bool PrefixSeg, bool PrefixAddr)
{
	 /*
        This Function will resolve BigSet menemonics: 
        ADC, ADD, AND, CMP, MOV, OR, SBB, SUB, XOR,ARPL, BOUND..
        We analyze the opcode using ;
        BitD, BitW,SIB ( SS III BBB : Sacle-Index-Base)
        MOD/RM
    */

	// Set Defaults    
    DWORD dwOp,dwMem;
	int RM=REG8,SCALE=0,SIB,ADDRM=REG32;
    WORD wOp,wMem;
    bool bound=0,UsesFPU=0;
    char RSize[10]="byte",Aritmathic[5]="+",tempAritmathic[5]="+";
	BYTE reg1=0,reg2=0,REG=0,Extension=0,FOpcode=0;
    char menemonic[128]="",tempMeme[128]="",Addr[15]="",temp[128]="";
	char instr[50]="";
	

    // Get used Register
	// Get target register, example:
	// 1. add byte ptr [ecx], -> al <-
	// 2. add -> al <- ,byte ptr [ecx]
    REG=(BYTE)(*(*Opcode+pos+1)); 
	REG>>=3;
	REG&=0x07;

    //Displacement MOD (none|BYTE/WORD|DWORD)
	Extension=(BYTE)(*(*Opcode+pos+1))>>6;
	/*
	    There are 3 types of Displacement to RegMem
	    00 -> [00] 000 000 ; no byte extention ([RegMem])
	    40->  [01] 000 000 ; 1 byte extenton ([RegMem+XX])
	    80 -> [10] 000 000 ; 4 bytes extention ([RegMem+XXXXXXXX])
	*/

    //===================//
    // Bitwise OverRides //
    //===================//
	// Arpl, Bound, Test, Xchg menemonics are special cases! when alone.
	// so we need to set specific static bits for d/w
    // We specify Size of Data corresponding to each mnemonic.

	switch((BYTE)(*(*Opcode+pos)))
	{
      case 0x20:            { PrefixReg=0;                                        }     break; // Force Byte Size Regardless Operands.
      case 0x39: case 0x3B:                     strcpy(RSize,regSize[1]);               break; // DWORD
      case 0x63:            { Bit_d=0; Bit_w=1; strcpy(RSize,regSize[1]); }             break; // DWORD
	  case 0x62:            { RM=REG32; bound=1; Bit_d=1; Bit_w=0; strcpy(RSize,regSize[0]); }    break; // QWORD
      case 0x69:            { Bit_d=0; Bit_w=1; strcpy(RSize,regSize[1]);         }     break; // DWORD
      case 0x6B:            { Bit_d=0; Bit_w=1; strcpy(RSize,regSize[1]);         }     break; // DWORD
      case 0x84: case 0x86: { Bit_d=0; Bit_w=0; }                                       break; // BYTE
      case 0x85: case 0x87: { Bit_d=0; Bit_w=1; strcpy(RSize,regSize[1]); }             break; // DWORD
      case 0x80: case 0x82: case 0xC6: case 0xF6:{ Bit_d=0;Bit_w=0; strcpy(RSize,regSize[3]); } break; // BYTE
      case 0x81: case 0x83: case 0xC7: case 0xF7: case 0x89:{ Bit_d=0;Bit_w=1; strcpy(RSize,regSize[1]); } break;	
      case 0x8B: strcpy(RSize,regSize[1]);                                              break; // DWORD
      case 0x8C: case 0x8E: { strcpy(RSize,regSize[2]); }                               break; // WORD
      case 0x8D: case 0x8F: { Bit_d=1; Bit_w=1; strcpy(RSize,regSize[1]); }             break; // POP/LEA
      case 0xC0:            { Bit_d=1; Bit_w=0;}                                        break; // BYTE
      case 0xC1:            { Bit_d=1; Bit_w=1; strcpy(RSize,regSize[1]); }             break; // MIX
      case 0xC4: case 0xC5: { RM=REG32; Bit_d=1; Bit_w=0; strcpy(RSize,regSize[4]); }             break; // LES/LDS
      case 0xD0: case 0xD2: { Bit_d=0; Bit_w=0; strcpy(RSize,regSize[3]); }             break; // MIX
      case 0xD1: case 0xD3: { Bit_d=0; Bit_w=1; strcpy(RSize,regSize[1]); }             break; // MIXED
      case 0xD8:            { UsesFPU=1; Bit_d=0; Bit_w=1; strcpy(RSize,regSize[1]); }  break; // FPU
      case 0xD9:{ 
                  UsesFPU=1; Bit_d=0; Bit_w=0; 
                  switch(REG)
                  {
                    case 0: case 2: case 3:strcpy(RSize,regSize[1]); break; // DWORD (REAL4)
                    case 4: case 6: strcpy(RSize,regSize[6]);        break; // 28Bytes                                       
                    case 5: case 7: strcpy(RSize,regSize[2]);        break; // WORD (REAL2)
                  }
                  
                }
                break; // FPU

      case 0xDA: { UsesFPU=1; Bit_d=0; Bit_w=1; strcpy(RSize,regSize[1]); }                                         break; // FPU
      case 0xDB: { UsesFPU=1; Bit_d=0; Bit_w=0; if(REG<4) strcpy(RSize,regSize[1]); else strcpy(RSize,regSize[5]);} break; // FPU
      case 0xDC: { UsesFPU=1; Bit_d=0; Bit_w=0; strcpy(RSize,regSize[0]); }                                         break; // FPU
      case 0xDD: { 
                   UsesFPU=1; Bit_d=0; Bit_w=0;
                   switch(REG)
                   {
                      case 0: case 1: case 2: case 3: strcpy(RSize,regSize[0]); break; // QWORD
                      case 4: case 5: case 6: strcpy(RSize,regSize[7]);         break; // (108)Byte
                      case 7: strcpy(RSize,regSize[2]);                         break; // WORD
                   }
                 }
                 break; // FPU

      case 0xDE: { UsesFPU=1; Bit_d=0; Bit_w=0; strcpy(RSize,regSize[2]); }     break; // WORD
      case 0xDF: {
                   UsesFPU=1; Bit_d=0; Bit_w=0;
                   switch(REG)
                   {
                       case 0: case 1: case 2: case 3: strcpy(RSize,regSize[2]); break; // WORD
                       case 4: case 6: strcpy(RSize,regSize[5]);                 break; // TByte
                       case 5: case 7: strcpy(RSize,regSize[0]);                 break; // QWord
                   }
                 }
                 break;
      case 0xFE: { Bit_d=0; Bit_w=0; strcpy(RSize,regSize[3]); }                 break; // BYTE
      case 0xFF: { 
                   Bit_d=0; Bit_w=0; 
                   if(REG==3 || REG==5) // FAR JMP/CALL
                       strcpy(RSize,regSize[4]); // FWORD
                   else
                       strcpy(RSize,regSize[1]);
                 }
                 break; // DWORD

    }

	// check for bit register size : 16bit/32bit
	if(Bit_w==1)
	{
	   RM=REG32; // 32bit registers set       

	   //if(/*!bound/&& Op==0x62*/)// Special Case
		strcpy(RSize,regSize[1]); // dword ptr   
           
	}

	// check for prefix 0x66 OverRide (change default size)
	if(PrefixReg==1)
	{
        if(!UsesFPU) // FPU DataSize doesn't Change, others are, on prefix 0x66.
        { 
		   if(lstrcmp(RSize,"Byte")!=0) // doesn't affect byte mode
		   {
			   RM=REG16; // 16bit registers
			   strcpy(RSize,regSize[2]); // word ptr
			   if(Op==0x62 || Op==0xC4 || Op==0xC5) // Special Case, 66 Prefix doesn't affect Memory Size.
				   strcpy(RSize,regSize[1]);
		   }
        }
	}
	
    // SCALE INDEX BASE :
	SIB=(BYTE)(*(*Opcode+pos+1))&0x07; // Get SIB extension
	/*
	   Exmaple:
	   --------
       
       format of sib is:
       ss iii bbb.
       where ss is 2 upper bits for scale
       and they represent power (exponent) of 2 for
       scale index multipyer.
       iii is 3 middle bits for index.
       bbb is 3 low bits for base.

       *SIB == 4
	   *NO SIB != 4

       0x04 -> 00 000 [100] <- SIB
	   0x0C -> 00 001 [100] <- SIB
	   0x64 -> 01 100 [100] <- SIB
	   0x60 -> 01 100 [000] <- NO SIB
	   0xB5 -> 10 110 [101] <- NO SIB
	   0x76 -> 01 110 [110] <- NO SIB

       Extract SS II BB information (3rd byte)
       =======================================
       0x81,0xAC,0x20

       0x20 =  00 100 000
       
       Scale: 00 = *1 (not shown)
       100 - ESP = not Shown, Cannot be an Index register
       000 - EAX = shown

       if MOD 10/01 is being used, get displacement data after 
       the SIB.
	*/


    // ===================================================//
    //             AddrPrefix is being used!               //
    // ===================================================//

	if(PrefixAddr==1) // Prefix 0x67 is set, Change Segments/Addressing Modes to 16 bits
	{		
        FOpcode=((BYTE)(*(*Opcode+pos+1))&0x0F); // Get addressing Mode (8 types of mode)
		reg1=((BYTE)(*(*Opcode+pos+1))&0x38)>>3;

        // Check if we decode POP instruction, which had few valid instruction.
        if(Op==0x8F && reg1!=0)
            lstrcat((*Disasm)->Remarks,"Invalid Instruction");
        
        // Choose Mode + Segment
		switch(FOpcode)
		{
		  case 0x00: case 0x08: wsprintf(Addr,"%s",addr16[0]); /*SEG=SEG_DS;*/ break; // Mode 0:[BX+SI]
		  case 0x01: case 0x09: wsprintf(Addr,"%s",addr16[1]); /*SEG=SEG_DS;*/ break; // Mode 1:[BX+DI]
		  case 0x02: case 0x0A: wsprintf(Addr,"%s",addr16[2]); SEG=SEG_SS; break; // Mode 2:[BP+SI]
		  case 0x03: case 0x0B: wsprintf(Addr,"%s",addr16[3]); SEG=SEG_SS; break; // Mode 3:[BP+DI]
		  case 0x04: case 0x0C: wsprintf(Addr,"%s",addr16[4]); /*SEG=SEG_DS;*/ break; // Mode 4:[SI]
		  case 0x05: case 0x0D: wsprintf(Addr,"%s",addr16[5]); /*SEG=SEG_DS;*/ break; // Mode 5:[DI]
		  case 0x06: case 0x0E: // Mode 6: [BP+XX/XXXX] | [XX]
		  {
				  if(Extension==0) // 0x00-0x3F only! has special [XXXX]
				  {
					  /*SEG=SEG_DS;*/
                      SwapWord((BYTE*)(*Opcode+pos+2),&wOp,&wMem);
					  wsprintf(Addr,"%04X",wMem);
					  (*(*index))+=2; // read 2 bytes
				  }
				  else{ // 0x50-0xBF has [BP+]

					  SEG=SEG_SS; // SS Segment
					  wsprintf(Addr,"%s",addr16[7]);
				  }
		  }
		  break;
          
		  case 0x07: case 0x0F: wsprintf(Addr,"%s",addr16[6]); /*SEG=SEG_DS;*/ break; // Mode 7: [BX]
		}

		// Choose used extension 
		// And Decode properly the menemonic
		switch(Extension)
		{
			case 0: // No extension of bytes to RegMem (except mode 6)
			{
				wsprintf(tempMeme,"%s ptr %s:[%s]",RSize,segs[SEG],Addr);
				SwapDword((BYTE*)(*Opcode+pos),&dwOp,&dwMem);
				SwapWord((BYTE*)(*Opcode+pos),&wOp,&wMem);

                if(((wOp&0x00FF)&0x0F)==0x06) // 0x00-0x3F with mode 6 only!
				{	                    
					wsprintf(menemonic,"%08X",dwOp);
					(*Disasm)->OpcodeSize=4;
					lstrcat((*Disasm)->Opcode,menemonic);
				}
				else{ // other modes                    
					wsprintf(menemonic,"%04X",wOp);
					(*Disasm)->OpcodeSize=2;
					lstrcat((*Disasm)->Opcode,menemonic);
				}
			}
			break;

			case 1: // 1 Byte Extension to regMem
			{
                SwapWord((BYTE*)(*Opcode+pos+1),&wOp,&wMem);
				FOpcode=wOp&0x00FF;
				
				if(FOpcode>0x7F) // check for signed numbers
				{
					wsprintf(Aritmathic,"%s",Scale[0]); // '-' Signed Numbers
					FOpcode = 0x100-FOpcode; // -XX
				}
				wsprintf(menemonic,"%02X%04X",Op,wOp);
				lstrcat((*Disasm)->Opcode,menemonic);
				wsprintf(tempMeme,"%s ptr %s:[%s%s%02X]",RSize,segs[SEG],Addr,Aritmathic,FOpcode);
				++(*(*index)); // 1 byte read
				(*Disasm)->OpcodeSize=3;
			}
			break;
			
			case 2: // 2 Bytes Extension to RegMem
			{
                SwapDword((BYTE*)(*Opcode+pos),&dwOp,&dwMem);
                SwapWord((BYTE*)(*Opcode+pos+2),&wOp,&wMem);
				wsprintf(menemonic,"%08X",dwOp);
				(*Disasm)->OpcodeSize=4;
				lstrcat((*Disasm)->Opcode,menemonic);
				wsprintf(tempMeme,"%s ptr %s:[%s%s%04X]",RSize,segs[SEG],Addr,Aritmathic,wMem);
				(*(*index))+=2; // we read 2 bytes
			}
			break;
		}

		// Switch Direction Mode.
		// And Build Menemonic from that direction
		switch(Bit_d)
		{
			case 0: // (->)
			{
				// Check for More Menemonics Addons
				switch(Op)// Check for all Cases
				{
                    case 0x6B:
                    {
                        // We check Extension because there is a diff
						// Reading position of bytes depend on the extension
						// 1 = read byte, 3rd position
						// 2 = read dword, 6th position
						
						if(Extension==1) // read 1 byte at 3rd position
						{
                            SwapWord((BYTE*)(*Opcode+pos+2),&wOp,&wMem);
							FOpcode=wOp&0x00FF;
							wsprintf(temp,"%02X",FOpcode);
							lstrcat((*Disasm)->Opcode,temp);
						}
						else{ 
                            if(Extension==2) //read byte at 7th position (dword read before)
                            {   
                                SwapWord((BYTE*)(*Opcode+pos+3),&wOp,&wMem);
                                FOpcode=wOp&0x00FF;
                                wsprintf(temp,"%02X",FOpcode);
                                lstrcat((*Disasm)->Opcode,temp);
                            }
                            else
                            { // Extension==0
                                SwapWord((BYTE*)(*Opcode+pos+1),&wOp,&wMem);
                                FOpcode=wOp&0x00FF;
                                wsprintf(temp,"%02X",FOpcode);
                                lstrcat((*Disasm)->Opcode,temp);
                            }
						}
                        
						if(FOpcode>0x7F) // check for signed numbers!!
                        {
                            FOpcode = 0x100-FOpcode; // -XX (Signed)
                            wsprintf(Aritmathic,"%s",Scale[0]); // '-' aritmathic (Signed)                            
                        }
                        else                    
                            strcpy(Aritmathic,"");
						
					    strcpy(instruction,"imul");
						wsprintf(temp,"%s %s,%s,%s%02X",instruction,regs[RM][reg2],tempMeme,Aritmathic,FOpcode);

						(*(*index))++;
						(*Disasm)->OpcodeSize++;
                    }
                    break;

                    case 0x81: case 0xC7: case 0x69:
					{
                        // Get Extensions!
                        //================    
                        
                        if(Extension==0)
                        {
                            if(PrefixReg==0)
                            {   
                                SwapDword((BYTE*)(*Opcode+pos+2),&dwOp,&dwMem);
                                wsprintf(temp," %08X",dwOp);                                
                                lstrcat((*Disasm)->Opcode,temp);                                                                
                                wsprintf(temp,"%08X",dwMem);
                            }
                            else
                            {
                                SwapWord((BYTE*)(*Opcode+pos+2),&wOp,&wMem);
                                wsprintf(temp," %04X",wOp);                                
                                lstrcat((*Disasm)->Opcode,temp);
                                wsprintf(temp,"%04X",wMem);
                            }
                        }
                        else if(Extension==1)
                        {
                            if(PrefixReg==0)
                            {   
                                SwapDword((BYTE*)(*Opcode+pos+3),&dwOp,&dwMem);
                                wsprintf(temp," %08X",dwOp);                                
                                lstrcat((*Disasm)->Opcode,temp);                                                                
                                wsprintf(temp,"%08X",dwMem);                                
                            }
                            else
                            {
                                SwapWord((BYTE*)(*Opcode+pos+3),&wOp,&wMem);
                                wsprintf(temp," %04X",wOp);                                
                                lstrcat((*Disasm)->Opcode,temp);
                                wsprintf(temp,"%04X",wMem);                                
                            }
                        }
                        else if(Extension==2)
                        {     
                            if(PrefixReg==0)
                            {                            
                                SwapDword((BYTE*)(*Opcode+pos+4),&dwOp,&dwMem);
                                wsprintf(temp," %08X",dwOp);                                
                                lstrcat((*Disasm)->Opcode,temp);                                                                
                                wsprintf(temp,"%08X",dwMem); 
                            }
                            else
                            {
                                SwapWord((BYTE*)(*Opcode+pos+4),&wOp,&wMem);
                                wsprintf(temp," %04X",wOp);                                
                                lstrcat((*Disasm)->Opcode,temp);
                                wsprintf(temp,"%04X",wMem);                                                                
                            }
                        }
                        
                        if(Op==0xC7)
                        {
                            /* 
                                Instruction rule: Mem,Imm ->  1100011woo000mmm,imm
                                Code Block: 1100011
                                w = Reg Size
                                oo - Mod
                                000 - Must be!
                                mmm - Reg/Mem
                                imm - Immidiant (麽弪)
                            */
                            
                            if(reg1!=0)
                                lstrcat((*Disasm)->Remarks,"Invalid Instruction");

                            wsprintf(instruction,"%s","mov");
                        }
                        else
                        {
                            if (Op==0x69) // IMUL REG,MEM,IIM32
                            {
                                wsprintf(instruction,"imul %s,",regs[RM][reg1]);
                            }
                            else
                                wsprintf(instruction,"%s",Instructions[REG]);
                        }
                        wsprintf(menemonic,"%s %s,%s",instruction,tempMeme,temp);
                        strcpy(temp,menemonic);
                        (*(*index))+=4;
                        (*Disasm)->OpcodeSize+=4;
					}
					break;
					
                    case 0x80:case 0x82: case 0x83: case 0xC6:
					{
						// We check Extension because there is a diff
						// Reading position of bytes depend on the extension
						// 1 = read byte, 3rd position
						// 2 = read dword, 6th position
						
						if(Extension==1) // read 1 byte at 3rd position
						{
                            SwapWord((BYTE*)(*Opcode+pos+2),&wOp,&wMem);
                            FOpcode=wOp&0x00FF;
							wsprintf(temp,"%02X",FOpcode);
							lstrcat((*Disasm)->Opcode,temp);
						}
						else{ 
                            if(Extension==2) //read byte at 7th position (dword read before)
                            {                            
                                SwapWord((BYTE*)(*Opcode+pos+4),&wOp,&wMem);
                                FOpcode=wOp&0x00FF;
                                wsprintf(temp,"%02X",FOpcode);
                                lstrcat((*Disasm)->Opcode,temp);
                            }
                            else
                            { // Extension==0
                                SwapWord((BYTE*)(*Opcode+pos+1),&wOp,&wMem);
                                FOpcode=wOp&0x00FF;
                                wsprintf(temp,"%02X",FOpcode);
                                lstrcat((*Disasm)->Opcode,temp);
                            }
						}
						strcpy(Aritmathic,"");
						
                        if(Op==0x82 || Op==0x83)
						  if(FOpcode>0x7F) // check for signed numbers
                          {
							 wsprintf(Aritmathic,"%s",Scale[0]); // '-' Signed Numbers
							 FOpcode = 0x100-FOpcode; // -XX (Negative the Number)
                          }
						
                        // Check Opcode
                        if(Op==0xC6)
                        {
                             /* 
                                Instruction rule: Mem,Imm ->  1100011woo000mmm,imm
                                Code Block: 1100011
                                w = Reg Size
                                oo - Mod
                                000 - Must be!
                                mmm - Reg/Mem
                                imm - Immidiant (麽弪)
                            */
                            // Check valid Opcode, must have 000 bit
                            if(reg1!=0)
                                lstrcat( (*Disasm)->Remarks,"Invalid Instruction!");

                            // Instruction
                            wsprintf(instruction,"%s","mov");
                        }
                        else
						    wsprintf(instruction,"%s",Instructions[REG]);
                        
						wsprintf(temp,"%s %s,%s%02X",instruction,tempMeme,Aritmathic,FOpcode);
						
						(*(*index))++;
						(*Disasm)->OpcodeSize++;
					}
					break;
					
                    case 0x8C: // Segments in Source Register
                    {
                        wsprintf(temp,"%s %s,%s",instruction,tempMeme,segs[REG]);
                    }
                    break;

                    case 0xD0: case 0xD1:
                    {
                        wsprintf(temp,"%s %s,1",ArtimaticInstructions[REG],tempMeme);
                    }
                    break;

                    case 0xD2: case 0xD3:
                    {
                        wsprintf(temp,"%s %s,cl",ArtimaticInstructions[REG],tempMeme);
                    }
                    break;

                    case 0xD8: case 0xDC:// Unsigned FPU Instructions (unsigned)
                    {
                        wsprintf(temp,"%s %s",FpuInstructions[REG],tempMeme);
                    }
                    break;

                    case 0xD9: // FPU Instructions Set2 (UnSigned)
                    {
                        if(REG==0 && reg1!=0) // (11011001oo[000]mmm) must have 00 else invalid! fld instruction only
                            lstrcat((*Disasm)->Remarks,"Invalid Instruction");
                        else {
                            if(REG==1) // no such fpu instruction!
                                lstrcat((*Disasm)->Remarks,"Invalid Instruction");
                        }
                        
                        wsprintf(temp,"%s %s",FpuInstructionsSet2[REG],tempMeme);
                    }
                    break;

                    case 0xDA: case 0xDE:// FPU Instructions (Signed)
                    {
                        wsprintf(temp,"%s %s",FpuInstructionsSigned[REG],tempMeme);
                    }
                    break;

                    case 0xDB: // FPU Instructions Set2 (Signed)
                    {
                       if(REG==1 || REG==4 || REG==6) // No such fpu instructions!
                           lstrcat((*Disasm)->Remarks,"Invalid Instruction");
                            
                       wsprintf(temp,"%s %s",FpuInstructionsSet2Signed[REG],tempMeme);
                    }
                    break;

                    case 0xDD:// FPU Instructions Set2 (Signed)
                    {
                       if(REG==1 ||  REG==5) // no such fpu instruction!
                           lstrcat((*Disasm)->Remarks,"Invalid Instruction");
                            
                       wsprintf(temp,"%s %s",FpuInstructionsSet3[REG],tempMeme);
                    }
                    break;

                    case 0xDF: // Extended FPU Instructions Set2 (Signed)
                    {
                       if(REG==1) // no such fpu instruction!
                          lstrcat((*Disasm)->Remarks,"Invalid Instruction");
                            
                       wsprintf(temp,"%s %s",FpuInstructionsSet2Signed_EX[REG],tempMeme);
                    }
                    break;
                    
                    case 0xF6:
                    {
                        // We check Extension because there is a diff
						// Reading position of bytes depend on the extension
						// 1 = read byte, 3rd position
						// 2 = read dword, 6th position
						
                        if(Extension==1) // read 1 byte at 3rd position
                        {
                            SwapWord((BYTE*)(*Opcode+pos+2),&wOp,&wMem);
                            FOpcode=wOp&0x00FF;
                            wsprintf(temp,"%02X",FOpcode);
                            lstrcat((*Disasm)->Opcode,temp);
                        }
                        else{ 
                            if(Extension==2) //read byte at 7th position (dword read before)
                            {                            
                                SwapWord((BYTE*)(*Opcode+pos+4),&wOp,&wMem);
                                FOpcode=wOp&0x00FF;
                                wsprintf(temp,"%02X",FOpcode);
                                lstrcat((*Disasm)->Opcode,temp);
                            }
                            else
                            { // Extension==0
                                SwapWord((BYTE*)(*Opcode+pos+1),&wOp,&wMem);
                                FOpcode=wOp&0x00FF;
                                wsprintf(temp,"%02X",FOpcode);
                                lstrcat((*Disasm)->Opcode,temp);
                            }
                        }

						strcpy(Aritmathic,"");
  				        wsprintf(instruction,"%s",InstructionsSet2[REG]);
                        
                        if(reg1==0 || reg1==1)
                        {                        
						   wsprintf(temp,"%s %s,%s%02X",instruction,tempMeme,Aritmathic,FOpcode);
						   (*(*index))++;
						   (*Disasm)->OpcodeSize++;
                        }
                        else
                            wsprintf(temp,"%s %s",instruction,tempMeme);
                    }
                    break;

                    case 0xF7:
                    {
                        // get instruction
                        wsprintf(instruction,"%s",InstructionsSet2[REG]);
                        
                        // Get Extensions!
                        //================    
                        if(reg1==0 || reg1==1)
                        {                        
                            if(Extension==0)
                            {
                                if(PrefixReg==0)
                                {   
                                    SwapDword((BYTE*)(*Opcode+pos+2),&dwOp,&dwMem);
                                    wsprintf(temp," %08X",dwOp);                                    
                                    lstrcat((*Disasm)->Opcode,temp);                                    
                                    wsprintf(temp,"%08X",dwMem);
                                }
                                else
                                {
                                    SwapWord((BYTE*)(*Opcode+pos+2),&wOp,&wMem);
                                    wsprintf(temp," %04X",wOp);                                    
                                    lstrcat((*Disasm)->Opcode,temp);
                                    wsprintf(temp,"%04X",wMem);
                                }
                            }
                            else if(Extension==1)
                            {
                                if(PrefixReg==0)
                                {                                
                                    SwapDword((BYTE*)(*Opcode+pos+3),&dwOp,&dwMem);
                                    wsprintf(temp," %08X",dwOp);                                    
                                    lstrcat((*Disasm)->Opcode,temp);                                    
                                    wsprintf(temp,"%08X",dwMem);
                                    
                                }
                                else
                                {
                                    SwapWord((BYTE*)(*Opcode+pos+3),&wOp,&wMem);
                                    wsprintf(temp," %04X",wOp);                                    
                                    lstrcat((*Disasm)->Opcode,temp);
                                    wsprintf(temp,"%04X",wMem);
                                }
                            }
                            else if(Extension==2)
                            {         
                                if(PrefixReg==0)
                                {   
                                    SwapDword((BYTE*)(*Opcode+pos+4),&dwOp,&dwMem);
                                    wsprintf(temp," %08X",dwOp);                                    
                                    lstrcat((*Disasm)->Opcode,temp);                                    
                                    wsprintf(temp,"%08X",dwMem);
                                }
                                else
                                {
                                    SwapWord((BYTE*)(*Opcode+pos+4),&wOp,&wMem);
                                    wsprintf(temp," %04X",wOp);                                    
                                    lstrcat((*Disasm)->Opcode,temp);
                                    wsprintf(temp,"%04X",wMem);
                                }
                            }
                            wsprintf(menemonic,"%s %s,%s",instruction,tempMeme,temp);                            
                            (*(*index))+=4;
                            (*Disasm)->OpcodeSize+=4;
                        }
                        else
                            wsprintf(menemonic,"%s %s",instruction,tempMeme);

                        strcpy(temp,menemonic);
                    }
                    break;

                    case 0xFE: // MIX Instructions (INC,DEC,INVALID,INVALID,INVALID...)
                    {                                                
                      wsprintf(temp,"%s %s",InstructionsSet3[REG],tempMeme);
                            
                      if(REG>1) // Invalid instructions
                        lstrcat((*Disasm)->Remarks,"Invalid Instruction");
                    }
                    break;

                    case 0xFF: // MIX Instructions (INC,DEC,CALL,PUSH,JMP,FAR JMP,FAR CALL,INVALID)
                    {
                       wsprintf(temp,"%s %s",InstructionsSet4[REG],tempMeme);
                            
                       if(REG==3)// FAR CALL
                       {
                           lstrcat((*Disasm)->Remarks,"Far Call");
                           break;
                       }
                            
                       if(REG==5) // FAR JUMP
                       {
                          lstrcat((*Disasm)->Remarks,"Far Jump");
                          break;
                       }
                            
                       if(REG==7) // Invalid instructions
                          lstrcat((*Disasm)->Remarks,"Invalid Instruction");
                    }
                    break;

  			        default:
					{       
					   wsprintf(temp,"%s %s,%s",instruction,tempMeme,regs[RM][REG]);                            
					}
					break;
				}
				
				lstrcat((*Disasm)->Assembly,temp);
				/*
				wsprintf(menemonic,"%s %s,%s",instruction,tempMeme,regs[RM][REG]);
				lstrcat((*Disasm)->Assembly,menemonic);
				*/
			}
			break;

			case 1: // (<-) Direction (Bit_D)
			{
                // Check Used Opcode Set
                switch(Op)
                {
                    case 0x8E:// Segments in Destination Register
                    {
                        wsprintf(menemonic,"%s %s,%s",instruction,segs[REG],tempMeme);
                    }
                    break;

                    // Mixed Bit Rotation Instructions (rol/ror/shl..)
                    case 0xC0: case 0xC1:
                    {
                        // Check Extension
                        switch(Extension)
                        {
                            case 0: // No Extension
                            {
                                SwapWord((BYTE*)(*Opcode+pos+1),&wOp,&wMem);
                                FOpcode=wOp&0x00FF;
                                wsprintf(menemonic,"%s %s,%02X",ArtimaticInstructions[REG],tempMeme,FOpcode);                                
                                wsprintf(tempMeme," %02X",FOpcode);
                                lstrcat((*Disasm)->Opcode,tempMeme);
                                (*(*index))++;
                                (*Disasm)->OpcodeSize++;
                            }
                            break;

                            case 1: // 1 byte Extension (Displacement)
                            {
                                SwapWord((BYTE*)(*Opcode+pos+2),&wOp,&wMem);
                                FOpcode=wOp&0x00FF;
                                wsprintf(menemonic,"%s %s,%02X",ArtimaticInstructions[REG],tempMeme,FOpcode);                                
                                wsprintf(tempMeme," %02X",FOpcode);
                                lstrcat((*Disasm)->Opcode,tempMeme);
                                (*(*index))++;
                                (*Disasm)->OpcodeSize++;
                            }
                            break;

                            case 2: // 2 Bytes Extension (Displacement)
                            {
                                SwapWord((BYTE*)(*Opcode+pos+3),&wOp,&wMem);
                                FOpcode=wOp&0x00FF;
                                wsprintf(menemonic,"%s %s,%02X",ArtimaticInstructions[REG],tempMeme,FOpcode);                                
                                wsprintf(tempMeme," %02X",FOpcode);
                                lstrcat((*Disasm)->Opcode,tempMeme);
                                (*(*index))++;
                                (*Disasm)->OpcodeSize++;
                            }
                            break;
                        }                        
                    }
                    break;

                    // POP DWORD PTR[REG/MEM/DISP]
                    case 0x8F:
                    {
                      wsprintf(menemonic,"%s %s",instruction,tempMeme);
                    }
                    break;
                    
                    case 0xC4: // LES
                    {
                      strcpy(instruction,"les");
                      wsprintf(menemonic,"%s %s,%s",instruction,regs[RM][REG],tempMeme);
                    }
                    break;

                    case 0xC5: // LDS
                    {
                      strcpy(instruction,"lds");
                      wsprintf(menemonic,"%s %s,%s",instruction,regs[RM][REG],tempMeme);
                    }
                    break;

                    // Default Decode, using regular registers
                    default:
                    {
                        wsprintf(menemonic,"%s %s,%s",instruction,regs[RM][REG],tempMeme);
                    }
                    break;
                }

                strcpy(tempMeme,menemonic);
				lstrcat((*Disasm)->Assembly,tempMeme);
			}
			break;
		}

		++(*(*index)); // add 1 byte to index
		// no need to continue!! exit the function and proeed with decoding next bytes.
		return;
	}

    // ===================================================//
    //                NO SIB Being used!                  //
    // ===================================================//
	if(SIB!=SIB_EX) // NO SIB extension (i.e: 0x0001 = add byte ptr [ecx], al)
	{
		reg1=((BYTE)(*(*Opcode+pos+1))&0x07); // get register (we have only one)
		reg2=(((BYTE)(*(*Opcode+pos+1))&0x38)>>3);
        
        // Check for valid/invalid pop instruction,
        // pop insteruction must have reg bit 000
        if(Op==0x8F && reg2!=0)
            lstrcat((*Disasm)->Remarks,"Invalid Instruction");

		switch(Extension) // Check what extension we have (None/Byte/Dword)
		{
			case 00: // no extention to regMem
			{
				if(reg1==REG_EBP) // cannot display EBP as memoryReg, use DWORD mem location
				{
                    SwapDword((BYTE*)(*Opcode+pos+2),&dwOp,&dwMem);
                    SwapWord((BYTE*)(*Opcode+pos),&wOp,&wMem);
					wsprintf(menemonic,"%04X%08X",wOp,dwOp);
					lstrcat((*Disasm)->Opcode,menemonic);
					wsprintf(instr,"%08X",dwMem);
					wsprintf(menemonic,"%s ptr %s:[%s]",RSize,segs[SEG],instr);					
					(*Disasm)->OpcodeSize=6;
					(*(*index))+=5;
				}
				else{
                    SwapWord((BYTE*)(*Opcode+pos),&wOp,&wMem);
					wsprintf(menemonic,"%04X",wOp);
					lstrcat((*Disasm)->Opcode,menemonic);
					wsprintf(menemonic,"%s ptr %s:[%s]",RSize,segs[SEG],regs[ADDRM][reg1]);
					++(*(*index)); // only 1 byte read
					(*Disasm)->OpcodeSize=2; // total used opcodes
				}
			}
			break;
			
			case 01: // 1 btye extention to regMem
			{
                SwapWord((BYTE*)(*Opcode+pos+1),&wOp,&wMem);				
				wsprintf(menemonic,"%02X%04X",Op,wOp);
				lstrcat((*Disasm)->Opcode,menemonic);
				                
                FOpcode=wOp&0xFF; // get lower part of word.

				if(FOpcode>0x7F) // check for signed numbers
				{
					wsprintf(Aritmathic,"%s",Scale[0]); // '-' aritmathic
					FOpcode = 0x100-FOpcode; // -XX
				}
				
				if(reg1==REG_EBP && PrefixSeg==0)
					SEG=SEG_SS;
				
				wsprintf(menemonic,"%s ptr %s:[%s%s%02Xh]",RSize,segs[SEG],regs[ADDRM][reg1],Aritmathic,FOpcode);
				(*(*index))+=2; // x + 1 byte(s) read
				(*Disasm)->OpcodeSize=3; // total used opcodes
			}
			break;
			
			case 02: // 4 btye extention to regMem
			{
				// if ebp and there is no prefix 0x67, use SS segment
				if(reg1==REG_EBP && PrefixSeg==0)
					SEG=SEG_SS;

                SwapDword((BYTE*)(*Opcode+pos+2),&dwOp,&dwMem);
				SwapWord((BYTE*)(*Opcode+pos),&wOp,&wMem);                
				wsprintf(menemonic,"%04X %08X",wOp,dwOp);
				lstrcat((*Disasm)->Opcode,menemonic);

				wsprintf(instr,"%08X",dwMem);
				wsprintf(menemonic,"%s ptr %s:[%s+%s]",RSize,segs[SEG],regs[ADDRM][reg1],instr);
				(*(*index))+=5; // x + 1 + 4 byte(s) read
				(*Disasm)->OpcodeSize=6; // total used opcodes
			}
			break;
			//case 02:break;
		}
		// check direction of menemonic
		switch(Bit_d)
		{
			case 0: // (->) Direction
			{
				// Check for More Menemonics Addons
				switch(Op)// Check for all Cases Availble
				{

                    case 0x6B:
                    {
                        // We check Extension because there is a diff
						// Reading position of bytes depend on the extension
						// 1 = read byte, 3rd position
						// 2 = read dword, 6th position

						if(Extension==1) // read 1 byte at 3rd position
						{
							FOpcode=(BYTE)(*(*Opcode+pos+3));
							wsprintf(temp,"%02X",FOpcode);
							lstrcat((*Disasm)->Opcode,temp);
						}
						else
                        { 
							if(Extension==2) // read byte at 7th position (dword read before)
                            {
								FOpcode=(BYTE)(*(*Opcode+pos+6));
								wsprintf(temp,"%02X",FOpcode);
								lstrcat((*Disasm)->Opcode,temp);
							}
                            else if(Extension==0)
                            {
                                FOpcode=(BYTE)(*(*Opcode+pos+2));
                                wsprintf(temp,"%02X",FOpcode);
                                lstrcat((*Disasm)->Opcode,temp);
                            }

                        }
                        
                        if(FOpcode>0x7F) // check for signed numbers!!
                        {
                            FOpcode = 0x100-FOpcode; // -XX
                            wsprintf(Aritmathic,"%s",Scale[0]); // '-' aritmathic (Signed)                            
                        }
                        else                    
						   strcpy(Aritmathic,"");	
                        
                        strcpy(instruction,"imul");                        						
                        wsprintf(tempMeme,"%s %s,%s,%s%02Xh",instruction,regs[RM][reg2],menemonic,Aritmathic,FOpcode);
						
						(*(*index))++;
						(*Disasm)->OpcodeSize++;
                    }
                    break;


                    case 0x81: case 0xC7: case 0x69:// Opcode 0x81/0xC7/0x69
					{
                        // Get Extensions!
                        //================
						if(Extension==1) // 1 byte extersion
						{	
                            if(PrefixReg==0)
                            {   
                                SwapDword((BYTE*)(*Opcode+pos+3),&dwOp,&dwMem);
                                wsprintf(temp," %08X",dwOp);                                
                                lstrcat((*Disasm)->Opcode,temp);                                
                                wsprintf(temp,"%08X",dwMem);
                            }
                            else
                            {
                                SwapWord((BYTE*)(*Opcode+pos+3),&wOp,&wMem);
                                wsprintf(temp," %04X",wOp);                                
                                lstrcat((*Disasm)->Opcode,temp);
                                wsprintf(temp,"%04X",wMem);
                            }
						}
						else 
                            if(Extension==2) // 4 bytes Extensions
                            {  
                                if(PrefixReg==0)
                                {   
                                    SwapDword((BYTE*)(*Opcode+pos+6),&dwOp,&dwMem);
                                    wsprintf(temp," %08X",dwOp);                                    
                                    lstrcat((*Disasm)->Opcode,temp);                                                                        
                                    wsprintf(temp,"%08X",dwMem);
                                }
                                else //0x66 prefix
                                {
                                    SwapWord((BYTE*)(*Opcode+pos+6),&wOp,&wMem);
                                    wsprintf(temp," %04X",wOp);                                    
                                    lstrcat((*Disasm)->Opcode,temp);                                                                        
                                    wsprintf(temp,"%04X",wMem);
                                }
                            }
                            else{ // No Extension!

                                if(PrefixReg==0)
                                {   
                                    if(reg1==REG_EBP)
                                    {
                                        SwapDword((BYTE*)(*Opcode+pos+6),&dwOp,&dwMem);
                                        wsprintf(temp," %08X",dwOp);                                        
                                        lstrcat((*Disasm)->Opcode,temp);
                                        wsprintf(temp,"%08X",dwMem);
                                    }
                                    else{
                                        SwapDword((BYTE*)(*Opcode+pos+2),&dwOp,&dwMem);
                                        wsprintf(temp," %08X",dwOp);                                        
                                        lstrcat((*Disasm)->Opcode,temp);                                                                                
                                        wsprintf(temp," %08X",dwMem);
                                    }
                                }
                                else // 0x66 prefix
                                {
                                    if(reg1==REG_EBP)
                                    {
                                        SwapWord((BYTE*)(*Opcode+pos+6),&wOp,&wMem);
                                        wsprintf(temp," %04X",wOp);                                        
                                        lstrcat((*Disasm)->Opcode,temp);                                                                                
                                        wsprintf(temp,"%04X",wMem);
                                    }
                                    else
                                    {   
                                        SwapWord((BYTE*)(*Opcode+pos+2),&wOp,&wMem);
                                        wsprintf(temp," %04X",wOp);                                        
                                        lstrcat((*Disasm)->Opcode,temp);
                                        wsprintf(temp,"%04X",wMem);
                                    }
                                }
                        }

                        if(Op==0xC7)
                        {
                             /* 
                                Instruction rule: Mem,Imm ->  1100011woo000mmm,imm
                                Code Block: 1100011
                                w = Reg Size
                                oo - Mod
                                000 - Must be!
                                mmm - Reg/Mem
                                imm - Immidiant (麽弪)
                            */
                            if(reg2!=0)
                                lstrcat((*Disasm)->Remarks,"Invalid Instruction");
                            
                            wsprintf(instruction,"%s","mov");
                        }
                        else{
                            if(Op==0x69) // IMUL REG,MEM,IIM
                            {
                                wsprintf(instruction,"imul %s,",regs[RM][reg2]);
                            }
                            else
                                wsprintf(instruction,"%s",Instructions[REG]);
                        }
                        
                        wsprintf(tempMeme,"%s %s,%s",instruction,menemonic,temp);
                        if(PrefixReg==0)
                        {                        
                            (*(*index))+=4;
                            (*Disasm)->OpcodeSize+=4;
                        }
                        else
                        {
                            (*(*index))+=2;
                            (*Disasm)->OpcodeSize+=2;
                        }

					}
					break;

                    case 0x80:case 0x82: case 0x83: case 0xC6:
					{
						// We check Extension because there is a diff
						// Reading position of bytes depend on the extension
						// 1 = read byte, 3rd position
						// 2 = read dword, 6th position

						if(Extension==1) // read 1 byte at 3rd position
						{
							FOpcode=(BYTE)(*(*Opcode+pos+3));
							wsprintf(temp,"%02X",FOpcode);
							lstrcat((*Disasm)->Opcode,temp);
						}
						else
                        { 
							if(Extension==2) // read byte at 7th position (dword read before)
                            {
								FOpcode=(BYTE)(*(*Opcode+pos+6));
								wsprintf(temp,"%02X",FOpcode);
								lstrcat((*Disasm)->Opcode,temp);
							}
                            else if(Extension==0)
                            {
                                if(reg1==REG_EBP)
                                   FOpcode=(BYTE)(*(*Opcode+pos+6));
                                else
                                   FOpcode=(BYTE)(*(*Opcode+pos+2));

                                wsprintf(temp," %02X",FOpcode);
                                lstrcat((*Disasm)->Opcode,temp);
                            }

                        }
						strcpy(Aritmathic,"");
						
                        // Opcodes with signed number
                        if(Op==0x82 || Op==0x83)
						  if(FOpcode>0x7F) // check for signed numbers
                          {
							 wsprintf(Aritmathic,"%s",Scale[0]); // '-' aritmathic
							 FOpcode = 0x100-FOpcode; // -XX (Negative the Number)
                          } 
						
                        // C6 Code Block Opcodes is Mov!
                        if(Op==0xC6)
                        {
                             /* 
                                Instruction rule: Mem,Imm ->  1100011woo000mmm,imm
                                Code Block: 1100011
                                w = Reg Size
                                oo - Mod
                                000 - Must be!
                                mmm - Reg/Mem
                                imm - Immidiant (麽弪)
                            */
                            if(reg2!=0)
                                lstrcat((*Disasm)->Remarks,"Invalid Instruction");

                            wsprintf(instruction,"%s","mov");  
                        }
                        else // Others Opcode we decode from the instruction tables
                        {                            						                                    
                            wsprintf(instruction,"%s",Instructions[REG]);
                        }
						
                        wsprintf(tempMeme,"%s %s,%s%02Xh",instruction,menemonic,Aritmathic,FOpcode);
						
						(*(*index))++;
						(*Disasm)->OpcodeSize++;
					}
					break;

                    case 0x8C: // Segments in Source Register
                    { 
                        if(REG>5)
                            lstrcat((*Disasm)->Remarks,";Invalid Segment Usage");

                        wsprintf(tempMeme,"%s %s,%s",instruction,menemonic,segs[REG]);
                    }
                    break;

                    case 0xD0: case 0xD1:
                    {
                        strcpy(instruction,ArtimaticInstructions[REG]);
                        wsprintf(tempMeme,"%s %s,1",instruction,menemonic);
                    }
                    break;

                    case 0xD2: case 0xD3:
                    {
                            strcpy(instruction,ArtimaticInstructions[REG]);
                            wsprintf(tempMeme,"%s %s,cl",instruction,menemonic);
                    }
                    break;

                    case 0xD8: case 0xDC:// FPU Instruction (unsigned instructions)
                    {
                        wsprintf(tempMeme,"%s %s",FpuInstructions[REG],menemonic);
                    }
                    break;

                    case 0xD9: // FPU Instructions Set2 (UnSigned)
                    {
                        if(REG==0 && reg2!=0) // (11011001oo[000]mmm) must have 00 else invalid fld instruction only
                            lstrcat((*Disasm)->Remarks,"Invalid Instruction");                    
                        else {
                            if(REG==1) // no such fpu instruction!
                               lstrcat((*Disasm)->Remarks,"Invalid Instruction");
                        }

                        wsprintf(tempMeme,"%s %s",FpuInstructionsSet2[REG],menemonic);
                    }
                    break;

                    case 0xDA: case 0xDE:// FPU Instructions (Signed)
                    {
                        wsprintf(tempMeme,"%s %s",FpuInstructionsSigned[REG],menemonic);
                    }
                    break;

                    case 0xDB: // FPU Instructions Set2 (Signed)
                    {
                        if(REG==1 || REG==4 || REG==6) // no such fpu instruction!
                               lstrcat((*Disasm)->Remarks,"Invalid Instruction");

                        wsprintf(tempMeme,"%s %s",FpuInstructionsSet2Signed[REG],menemonic);
                    }
                    break;

                    case 0xDD: // FPU Instructions Set3
                    {
                        if(REG==1 ||  REG==5) // no such fpu instruction!
                            lstrcat((*Disasm)->Remarks,"Invalid Instruction");
                        
                        wsprintf(tempMeme,"%s %s",FpuInstructionsSet3[REG],menemonic);
                    }
                    break;

                    case 0xDF: // Extended FPU Instructions Set2 (Signed)
                    {
                        if(REG==1) // no such fpu instruction!
                            lstrcat((*Disasm)->Remarks,"Invalid Instruction");
                        
                        wsprintf(tempMeme,"%s %s",FpuInstructionsSet2Signed_EX[REG],menemonic);
                    }
                    break;

                    case 0xF6: // MIXED Instructions (MUL,DIV,NOT...)
                    {
                       // We check Extension because there is a diff
                       // Reading position of bytes depend on the extension
                       // 1 = read byte, 3rd position
                       // 2 = read dword, 6th position
                            
                       if(Extension==1) // read 1 byte at 3rd position
                       {
                           if(reg2==0 || reg2==1) // TEST Only
                           {                            
                               FOpcode=(BYTE)(*(*Opcode+pos+3));
                               wsprintf(temp,"%02X",FOpcode);
                               lstrcat((*Disasm)->Opcode,temp);
                           }
                       }
                       else
                       { 
                           if(Extension==2) // read byte at 7th position (dword read before)
                           {
                               if(reg2==0 || reg2==1) // TEST Only
                               {                                
                                   FOpcode=(BYTE)(*(*Opcode+pos+6));
                                   wsprintf(temp,"%02X",FOpcode);
                                   lstrcat((*Disasm)->Opcode,temp);
                               }
                           }
                           else if(Extension==0)
                           {
                               if(reg2==0 || reg2==1) // TEST Only
                               {                                
                                   FOpcode=(BYTE)(*(*Opcode+pos+2));
                                   wsprintf(temp,"%02X",FOpcode);
                                   lstrcat((*Disasm)->Opcode,temp);
                               }
                           }
                             
                       }
                            
                       strcpy(Aritmathic,"");                         
                       wsprintf(instruction,"%s",InstructionsSet2[REG]);
                            
                       if(reg2==0 || reg2==1) // TEST instruction
                       {
                           wsprintf(tempMeme,"%s %s,%s%02X",instruction,menemonic,Aritmathic,FOpcode);
                           (*(*index))++;
                           (*Disasm)->OpcodeSize++;
                       }
                       else // NOT/NEG/MUL/IMUL/DIV/IDIV instruction must not have operands
                           wsprintf(tempMeme,"%s %s",instruction,menemonic);                        
                            
                    }
                    break;

                    case 0xF7:
                    {
                        // Get Instruction
                        wsprintf(instruction,"%s",InstructionsSet2[REG]);

                        //================//
                        // Get Extensions!//
                        //================//

                        if(reg2==0 || reg2==1){ // TEST Instruction
                        
						if(Extension==1) // 1 byte extersion
						{	
                            if(PrefixReg==0)
                            {   
                                SwapDword((BYTE*)(*Opcode+pos+3),&dwOp,&dwMem);
                                wsprintf(temp," %08X",dwOp);                                
                                lstrcat((*Disasm)->Opcode,temp);                                                                
                                wsprintf(temp,"%08X",dwMem);
                            }
                            else
                            {
                                SwapWord((BYTE*)(*Opcode+pos+3),&wOp,&wMem);
                                wsprintf(temp," %04X",wOp);                                
                                lstrcat((*Disasm)->Opcode,temp);                                                                
                                wsprintf(temp,"%04X",wMem);
                            }
						}
						else if(Extension==2) // 4 bytes Extensions
						{   
                            if(PrefixReg==0)
                            {   
                                SwapDword((BYTE*)(*Opcode+pos+6),&dwOp,&dwMem);
                                wsprintf(temp," %08X",dwOp);                               
                                lstrcat((*Disasm)->Opcode,temp);
                                wsprintf(temp,"%08X",dwMem);
                            }
                            else
                            {
                                SwapWord((BYTE*)(*Opcode+pos+6),&wOp,&wMem);
                                wsprintf(temp," %04X",wOp);                                
                                lstrcat((*Disasm)->Opcode,temp);                                                                
                                wsprintf(temp,"%04X",wMem);
                            }
                        }
                        else{ // No Extension!  (check ebp)

                            if(PrefixReg==0)
                            {   
                                if(reg1==REG_EBP)
                                {
                                    SwapDword((BYTE*)(*Opcode+pos+6),&dwOp,&dwMem);
                                    wsprintf(temp," %08X",dwOp);                                        
                                    lstrcat((*Disasm)->Opcode,temp);
                                    wsprintf(temp,"%08X",dwMem);                                    
                                }
                                else
                                {                                
                                    SwapDword((BYTE*)(*Opcode+pos+2),&dwOp,&dwMem);
                                    wsprintf(temp," %08X",dwOp);                                
                                    lstrcat((*Disasm)->Opcode,temp);                                                                
                                    wsprintf(temp,"%08X",dwMem);
                                }
                            }
                            else
                            {
                                if(reg1==REG_EBP)
                                {
                                    SwapWord((BYTE*)(*Opcode+pos+6),&wOp,&wMem);
                                    wsprintf(temp," %04X",wOp);                                        
                                    lstrcat((*Disasm)->Opcode,temp);                                                                                
                                    wsprintf(temp,"%04X",wMem);
                                }
                                else 
                                {                                
                                    SwapWord((BYTE*)(*Opcode+pos+2),&wOp,&wMem);
                                    wsprintf(temp," %04X",wOp);                                
                                    lstrcat((*Disasm)->Opcode,temp);                                                                
                                    wsprintf(temp,"%04X",wMem);
                                }
                            }
                        }

                        wsprintf(tempMeme,"%s %s,%s",instruction,menemonic,temp);
                        if(PrefixReg==0)
                        {                        
                            (*(*index))+=4;
                            (*Disasm)->OpcodeSize+=4;
                        }
                        else
                        {
                            (*(*index))+=2;
                            (*Disasm)->OpcodeSize+=2;
                        }
                      }
                      else
                          wsprintf(tempMeme,"%s %s",instruction,menemonic);
                    }
                    break;

                    case 0xFE: // MIX Instructions (INC,DEC,INVALID,INVALID,INVALID...)
                    {                                                
                        wsprintf(tempMeme,"%s %s",InstructionsSet3[REG],menemonic);

                        if(REG>1) // Invalid instructions
                            lstrcat((*Disasm)->Remarks,"Invalid Instruction");
                    }
                    break;

                    case 0xFF:// MIX Instructions (INC,DEC,CALL,PUSH,JMP,FAR JMP,FAR CALL,INVALID)
                    {
                        wsprintf(tempMeme,"%s %s",InstructionsSet4[REG],menemonic);
                        
                        if(REG==3)// FAR CALL
                        {
                            lstrcat((*Disasm)->Remarks,"Far Call");
                            break;
                        }
                        
                        if(REG==5) // FAR JUMP
                        {
                            lstrcat((*Disasm)->Remarks,"Far Jump");
                            break;
                        }

                        if(REG==7) // Invalid instructions
                            lstrcat((*Disasm)->Remarks,"Invalid Instruction");
                    }
                    break;

                    // Decode non imm8/16/32 source opcodes
                    // i.e: mov dword ptr[eax],eax
					default:
					{
					   wsprintf(tempMeme,"%s %s,%s",instruction,menemonic,regs[RM][REG]);					   
					}
					break;
				}

				lstrcat((*Disasm)->Assembly,tempMeme);
				
			}
			break;
			
			case 1: // (<-) Direction of decoding
			{
                switch(Op)
                {
                    case 0x8E: // Segments in Destination Register
                    {
                        wsprintf(tempMeme,"%s %s,%s",instruction,segs[REG],menemonic);
                    }
                    break;
                    
                    // POP DWORD PTR[REG/MEM/DISP]
                    case 0x8F:
                    {
                        wsprintf(tempMeme,"%s %s",instruction,menemonic);
                    }
                    break;

                    // Mixed Bit Rotation Instructions (rol/ror/shl..)
                    case 0xC0:case 0xC1:
                    {
						switch(Extension) {
							case 0:
							{
								if(reg1==REG_EBP)
								{
								  FOpcode=(BYTE)(*(*Opcode+pos+6));
								}
								else
									FOpcode=(BYTE)(*(*Opcode+pos+2)); 
							}
							break;
							case 1: FOpcode=(BYTE)(*(*Opcode+pos+3)); break;
							case 2: FOpcode=(BYTE)(*(*Opcode+pos+6)); break;
						}
						
                        wsprintf(tempMeme,"%s %s,%02X",ArtimaticInstructions[REG],menemonic,FOpcode);
						//wsprintf(tempMeme,"%s %s,cl",ArtimaticInstructions[REG],menemonic,FOpcode);
                        wsprintf(menemonic," %02X",FOpcode);
                        lstrcat((*Disasm)->Opcode,menemonic);
                        (*(*index))++;
                        (*Disasm)->OpcodeSize++;
                    }
                    break;
                    
                    case 0xC4:
                    {
                        strcpy(instruction,"les");
                        wsprintf(tempMeme,"%s %s,%s",instruction,regs[RM][REG],menemonic);
                    }
                    break;

                    case 0xC5:
                    {
                        strcpy(instruction,"lds");
                        wsprintf(tempMeme,"%s %s,%s",instruction,regs[RM][REG],menemonic);
                    }
                    break;

                    // Default Decode
                    default:
                    {
                        wsprintf(tempMeme,"%s %s,%s",instruction,regs[RM][REG],menemonic);
                    }
                    break;
                }
								
                lstrcat((*Disasm)->Assembly,tempMeme);
			}
			break;
		}
        
        return;
	}
    // ===================================================//
    //                 SIB is being used!                 //
    // ===================================================//
	else if(SIB==SIB_EX) // Found SIB, lets strip the extensions
	{
		/* 
		   Example menemonic for SIB: 
		   Opcodes:   000401  
		   Menemonic: add byte ptr [eax+ecx], al
		   Binary:    0000 0000 0000 0100 0000 0001
        */
		reg1=((BYTE)(*(*Opcode+pos+2))&0x38)>>3;  // Register A
		reg2=((BYTE)(*(*Opcode+pos+2))&0x07);     // Register B
		SCALE=((BYTE)(*(*Opcode+pos+2))&0xC0)>>6; // Scale size (0,2,4,8)

        /* 
           Check for valid/invalid pop instruction,
           pop insteruction must have reg bit 000
           pop code/ModRM:
           
           Code Block: 1000 1111
           Mod/RM: oo000mmm 
           oo - Mod
           000 - Must be 0
           mmm - <reg>
        
        */

        if(Op==0x8F)
            if( (((BYTE)(*(*Opcode+pos+1))&0x38)>>3)!=0 )// check 000
                lstrcat((*Disasm)->Remarks,"Invalid Instruction");

		// Scale look up
		switch(SCALE)
		{
			case 0:wsprintf(Aritmathic,"%s",Scale[1]);break; // +
			case 1:wsprintf(Aritmathic,"%s",Scale[2]);break; // *2+
			case 2:wsprintf(Aritmathic,"%s",Scale[3]);break; // *4+
			case 3:wsprintf(Aritmathic,"%s",Scale[4]);break; // *8+
		}

		switch(Extension) // +/+00/+00000000
		{
			case 00: // No extension of bytes
			{
                if(reg1==REG_ESP && reg2!=REG_EBP)
                {
                    if(reg2==REG_ESP) SEG=SEG_SS; // IF ESP is being used, User SS Segment Overridr
                    SwapWord((BYTE*)(*Opcode+pos+1),&wOp,&wMem);
                    wsprintf(menemonic,"%02X%04X",Op,wOp);
                    lstrcat((*Disasm)->Opcode,menemonic);
                    wsprintf(menemonic,"%s ptr %s:[%s]",RSize,segs[SEG],regs[ADDRM][reg2]);
                    (*(*index))+=2; //2 byte read				
					(*Disasm)->OpcodeSize=3; // total used opcodes
                }
				else if(reg2!=REG_EBP) // No EBP in RegMem
				{
                    if(reg2==REG_ESP) SEG=SEG_SS; // IF ESP is being used, User SS Segment Overridr
                    SwapWord((BYTE*)(*Opcode+pos+1),&wOp,&wMem);
					wsprintf(menemonic,"%02X%04X",Op,wOp);
					lstrcat((*Disasm)->Opcode,menemonic);
					wsprintf(menemonic,"%s ptr %s:[%s%s%s]",RSize,segs[SEG],regs[ADDRM][reg1],Aritmathic,regs[ADDRM][reg2]);
					(*(*index))+=2; //2 byte read				
					(*Disasm)->OpcodeSize=3; // total used opcodes
				}
				else if(reg2==REG_EBP) // Replace EBP with Dword Number
				{
					// get 4 bytes extensions for memReg addon
					// insted of Normal Registers

					// Format Opcodes (HEX)
                    SwapDword((BYTE*)(*Opcode+pos+3),&dwOp,&dwMem);
                    SwapWord((BYTE*)(*Opcode+pos+1),&wOp,&wMem);
					wsprintf(menemonic,"%02X %04X %08X",Op,wOp,dwOp);

					lstrcat((*Disasm)->Opcode,menemonic);
					// Format menemonic

                    // Check If if ESP is being Used.
                    if(reg1==REG_ESP) // Must Not Be ESP (Index)
                    {
                        wsprintf(temp,"");
                        strcpy(Aritmathic,"");
                    }
                    else
                        wsprintf(temp,regs[ADDRM][reg1]);

					wsprintf(menemonic,"%s ptr %s:[%s%s%08X]",
						RSize,      // size of regmem
						segs[SEG],  // segment
						temp,       // reg
						Aritmathic, //+,-,*2,*4,*8						
                        dwMem);     // extensions

                    Extension=2; // OverRide Extension (?????), Check toDo.txt
					(*(*index))+=6; //6 byte read				
					(*Disasm)->OpcodeSize=7; // total used opcodes
				}
			}
			break;

			case 01: // 1 byte extension
			{				
				FOpcode=(BYTE)(*(*Opcode+pos+3));
				if(FOpcode>0x7F) // check for signed numbers!!
				{
					wsprintf(tempAritmathic,"%s",Scale[0]); // '-' aritmathic
					FOpcode = 0x100-FOpcode; // -XX
				}
				
				if(/*reg2==REG_EBP ||*/ reg1==REG_ESP) // no ESP in [Mem]
                {                
					SEG=SEG_SS;
                    // added REG+Arithmatic [21.3.2004]
					//wsprintf(tempMeme,"%s ptr %s:[%s%s%s%s%02X]",RSize,segs[SEG],regs[ADDRM][reg2],tempAritmathic,regs[ADDRM][reg1],tempAritmathic,FOpcode); ; OLD
                    wsprintf(tempMeme,"%s ptr %s:[%s%s%02Xh]",RSize,segs[SEG],regs[ADDRM][reg1],tempAritmathic,FOpcode);
                }
				else
				    wsprintf(tempMeme,"%s ptr %s:[%s%s%s%s%02Xh]",RSize,segs[SEG],regs[ADDRM][reg1],Aritmathic,regs[ADDRM][reg2],tempAritmathic,FOpcode);

				(*(*index))+=3; // x + 3 byte(s) read				
				
                SwapDword((BYTE*)(*Opcode+pos),&dwOp,&dwMem);
                wsprintf(menemonic,"%08X",dwOp);
				lstrcat((*Disasm)->Opcode,menemonic);
				(*Disasm)->OpcodeSize=4; // total used opcodes
				strcpy(menemonic,tempMeme);
			}
			break;

			case 02:// Dword extension
			{
                SwapDword((BYTE*)(*Opcode+pos+3),&dwOp,&dwMem);
                SwapWord((BYTE*)(*Opcode+pos+1),&wOp,&wMem);
                
                // Menemonic decode
                if(reg1!=REG_ESP) 
                {
                    if(reg2==REG_EBP || reg2==REG_ESP)
                        SEG=SEG_SS;
                    
                    wsprintf(tempMeme,"%s ptr %s:[%s%s%s%s%08Xh]",
                        RSize,  // size of register
                        segs[SEG], // segment
                        regs[ADDRM][reg1],
                        Aritmathic,
                        regs[ADDRM][reg2],
                        tempAritmathic,
                        dwMem);	                    
                }
                else
                {// ESP Must not be as Index, Code = 100b
                    
                    if(reg2==REG_ESP)
                        SEG=SEG_SS;
                    
                    wsprintf(tempMeme,"%s ptr %s:[%s%s%08Xh]",
                        RSize,  // size of register
                        segs[SEG], // segment
                        regs[ADDRM][reg2],
                        tempAritmathic,
                        dwMem);
                    
                }
                // Format Opcode		
                wsprintf(menemonic,"%02X %04X %08X",Op,wOp,dwOp);
                
                lstrcat((*Disasm)->Opcode,menemonic);
                (*(*index))+=6; // x + 3 byte(s) read	
                (*Disasm)->OpcodeSize=7; // total used opcodes
				strcpy(menemonic,tempMeme);
            }
			break;
		}

		// Finish up the opcode with position of target register
		switch(Bit_d)
		{
			case 0: // (->) Direction
			{
				/*
				wsprintf(tempMeme,"%s %s,%s",instruction,menemonic,regs[RM][REG]);
				lstrcat((*Disasm)->Assembly,tempMeme);
				*/
				// Check for More Menemonics Addons
				switch(Op)// Check for all Cases
				{
                    case 0x6B:
                    {
                        // We check Extension because there is a diff
                        // Reading position of bytes depend on the extension
                        // 1 = read byte, 3rd position
                        // 2 = read dword, 6th position
                        
                        if(Extension==1) // read 1 byte at 3rd position
                        {
                            FOpcode=(BYTE)(*(*Opcode+pos+3));
                            wsprintf(temp,"%02X",FOpcode);
                            lstrcat((*Disasm)->Opcode,temp);
                        }
                        else
                        { 
                            if(Extension==2) // read byte at 7th position (dword read before)
                            {
                                FOpcode=(BYTE)(*(*Opcode+pos+7));
                                wsprintf(temp,"%02X",FOpcode);
                                lstrcat((*Disasm)->Opcode,temp);
                            }
                            else if(Extension==0)
                            {
                                FOpcode=(BYTE)(*(*Opcode+pos+2));
                                wsprintf(temp,"%02X",FOpcode);
                                lstrcat((*Disasm)->Opcode,temp);
                            }
                            
                        }
                        
                        if(FOpcode>0x7F) // check for signed numbers!!
                        {
                            FOpcode = 0x100-FOpcode; // -XX
                            wsprintf(Aritmathic,"%s",Scale[0]); // '-' aritmathic (Signed)                            
                        }
                        else                    
                            strcpy(Aritmathic,"");	
                        
						
						BYTE Level=(BYTE)(*(*Opcode+pos+1));
						if( (Level>=0x00 && Level<=0x07) || (Level>=0x40 && Level<=0x47) || (Level>=0x80 && Level<=0x87) )
							reg2=0;

						if( (Level>=0x08 && Level<=0x0F) || (Level>=0x48 && Level<=0x4F) || (Level>=0x88 && Level<=0x8F) )
							reg2=1;
						
						if( (Level>=0x10 && Level<=0x17) || (Level>=0x50 && Level<=0x57) || (Level>=0x90 && Level<=0x97) )
							reg2=2;

						if( (Level>=0x18 && Level<=0x1F) || (Level>=0x58 && Level<=0x5F) || (Level>=0x98 && Level<=0x9F) )
							reg2=3;

						if( (Level>=0x20 && Level<=0x27) || (Level>=0x60 && Level<=0x67) || (Level>=0xA0 && Level<=0xA7) )
							reg2=4;
						
						if( (Level>=0x28 && Level<=0x2F) || (Level>=0x68 && Level<=0x6F) || (Level>=0xA8 && Level<=0xAF) )
							reg2=5;

						if( (Level>=0x30 && Level<=0x37) || (Level>=0x70 && Level<=0x77) || (Level>=0xB0 && Level<=0xB7) )
							reg2=6;
						
						if( (Level>=0x38 && Level<=0x3F) || (Level>=0x78 && Level<=0x7F) || (Level>=0xB8 && Level<=0xBF) )
							reg2=7;

                        strcpy(instruction,"imul");                        						
                        wsprintf(tempMeme,"%s %s,%s,%s%02Xh",instruction,regs[RM][reg2],menemonic,Aritmathic,FOpcode);
                        
                        (*(*index))++;
						(*Disasm)->OpcodeSize++;
                    }
                    break;

                    case 0x81: case 0xC7: case 0x69: // Opcodes 0x81/0xC7/0x69
                    {
                        // Get Extensions!
                        //================
                        if(Extension==1) // 1 byte extersion
                        {
                            if(PrefixReg==0) // No Reg Prefix
                            {
                                SwapDword((BYTE*)(*Opcode+pos+4),&dwOp,&dwMem);
                                wsprintf(temp," %08X",dwOp);                               
                                lstrcat((*Disasm)->Opcode,temp);
                                wsprintf(temp,"%08X",dwMem);
                            }
                            else
                            {
                                SwapWord((BYTE*)(*Opcode+pos+4),&wOp,&wMem);
                                wsprintf(temp," %04X",wOp);                               
                                lstrcat((*Disasm)->Opcode,temp);
                                wsprintf(temp,"%04X",wMem);
                            }
                        }
                        else if(Extension==2) // 4 bytes Extensions
                        {    
                            if(PrefixReg==0) // No Reg Prefix
                            {   
                                SwapDword((BYTE*)(*Opcode+pos+7),&dwOp,&dwMem);
                                wsprintf(temp," %08X",dwOp);                                
                                lstrcat((*Disasm)->Opcode,temp);                                                                
                                wsprintf(temp,"%08X",dwMem);
                            }
                            else
                            {
                                SwapWord((BYTE*)(*Opcode+pos+7),&wOp,&wMem);
                                wsprintf(temp," %04X",wOp);                                
                                lstrcat((*Disasm)->Opcode,temp);                                                                
                                wsprintf(temp,"%04X",wMem);
                            }
                        }
                        else{ // No Extension!
                            
                            if(PrefixReg==0) // No Reg Prefix
                            {   
                                SwapDword((BYTE*)(*Opcode+pos+3),&dwOp,&dwMem);
                                wsprintf(temp," %08X",dwOp);                                
                                lstrcat((*Disasm)->Opcode,temp);                                                                
                                wsprintf(temp,"%08X",dwMem);
                            }
                            else
                            {
                                SwapWord((BYTE*)(*Opcode+pos+3),&wOp,&wMem);
                                wsprintf(temp," %04X",wOp);                               
                                lstrcat((*Disasm)->Opcode,temp);
                                wsprintf(temp,"%04X",wMem);
                            }
                        }
                        
                        if(Op==0xC7)
                        {
                            /* 
                                Instruction rule: Mem,Imm ->  1100011woo000mmm,imm
                                Code Block: 1100011
                                w = Reg Size
                                oo - Mod
                                000 - Must be!
                                mmm - Reg/Mem
                                imm - Immidiant (麽弪)
                            */
                            
                            reg1=((BYTE)(*(*Opcode+pos+1))&0x38)>>3; // Check for valid opcode, result must be 0
                            
                            if(reg1!=0)
                                lstrcat((*Disasm)->Remarks,"Invalid Instruction");
                            
                            wsprintf(instruction,"%s","mov");
                        }
                        else
                        {
                            if (Op==0x69) // IMUL REG,MEM,IIM
                            {
                                reg1=((BYTE)(*(*Opcode+pos+1))&0x38)>>3; // get register
                                wsprintf(instruction,"imul %s,",regs[RM][reg1]);
                            }
                            else
                                wsprintf(instruction,"%s",Instructions[REG]);
                        }
                        
                        wsprintf(tempMeme,"%s %s,%s",instruction,menemonic,temp);
                        if(PrefixReg==0) // No regPrefix
                        {                        
                            (*(*index))+=4;
                            (*Disasm)->OpcodeSize+=4;
                        }
                        else
                        {
                            (*(*index))+=2;
                            (*Disasm)->OpcodeSize+=2;
                        }
                    }
					break;

                    case 0x80:case 0x82: case 0x83: case 0xC6:
					{
						if(Extension==1)// read 1 byte at 3rd position
						{
							FOpcode=(BYTE)(*(*Opcode+pos+4));
							wsprintf(temp,"%02X",FOpcode);
							lstrcat((*Disasm)->Opcode,temp);
						}
						else
						{ 
                            if(Extension==2)
                            {
                                // read byte at 7th position (dword read before)
                                FOpcode=(BYTE)(*(*Opcode+pos+7));
                                wsprintf(temp,"%02X",FOpcode);
                                lstrcat((*Disasm)->Opcode,temp);
							}
                            else if (Extension==0)
                            {
                                FOpcode=(BYTE)(*(*Opcode+pos+3));
                                wsprintf(temp,"%02X",FOpcode);
							    lstrcat((*Disasm)->Opcode,temp);
                            }
						}
                        
						strcpy(Aritmathic,"");
						
                        if(Op==0x82 || Op==0x83)
						  if(FOpcode>0x7F) // check for signed numbers
                          {
							 wsprintf(Aritmathic,"%s",Scale[0]); // '-' aritmathic
							 FOpcode = 0x100-FOpcode; // -XX (Negative the Number)
                          }
						
                        // Code Block of C6 is Mov instruction
                        if(Op==0xC6)
                        {
                            /* 
                                Instruction rule: Mem,Imm ->  1100011woo000mmm,imm
                                Code Block: 1100011
                                w = Reg Size
                                oo - Mod
                                000 - Must be!
                                mmm - Reg/Mem
                                imm - Immidiant (麽弪)
                            */

                            // Check for valid intruction, reg1 must be 000 to be valid
                            reg1=((BYTE)(*(*Opcode+pos+1))&0x38)>>3; 
                            
                            if(reg1!=0)
                                lstrcat((*Disasm)->Remarks,"Invalid Instruction!");

                            wsprintf(instruction,"%s","mov");  
                        }
						else // Decode from instruction table
                            wsprintf(instruction,"%s",Instructions[REG]);

						wsprintf(tempMeme,"%s %s,%s%02X",instruction,menemonic,Aritmathic,FOpcode);
						
						(*(*index))++;
						(*Disasm)->OpcodeSize++;
					}
					break;

                    case 0x8C: // Segments in Source register
                    {
                        wsprintf(tempMeme,"%s %s,%s",instruction,menemonic,segs[REG]);
                    }
                    break;

                    case 0xD0: case 0xD1:
                    {
                        strcpy(instruction,ArtimaticInstructions[REG]);
                        wsprintf(tempMeme,"%s %s,1",instruction,menemonic);
                    }
                    break;

                    case 0xD2: case 0xD3:
                    {
                        strcpy(instruction,ArtimaticInstructions[REG]);
                        wsprintf(tempMeme,"%s %s,cl",instruction,menemonic);
                    }
                    break;

                    case 0xD8: case 0xDC:// FPU Instructions (UnSigned)
                    {
                      wsprintf(tempMeme,"%s %s",FpuInstructions[REG],menemonic);
                    }
                    break;

                    case 0xD9: // FPU Instructions Set2 (UnSigned)
                    {                           
                       wsprintf(tempMeme,"%s %s",FpuInstructionsSet2[REG],menemonic);
                    }
                    break;

                    case 0xDA: case 0xDE:// FPU Instructions (Signed)
                    {
                       wsprintf(tempMeme,"%s %s",FpuInstructionsSigned[REG],menemonic);
                    }
                    break;

                    case 0xDB: // FPU Instructions Set2 (Signed)
                    {
                       if(REG==1 || REG==4 || REG==6) // No such fpu instructions!
                            lstrcat((*Disasm)->Remarks,"Invalid Instruction");
                            
                        wsprintf(tempMeme,"%s %s",FpuInstructionsSet2Signed[REG],menemonic);
                    }
                    break;

                    case 0xDD: // FPU Instructions Set3
                    {
                       if(REG==1 ||  REG==5) // no such fpu instruction!
                          lstrcat((*Disasm)->Remarks,"Invalid Instruction");
                            
                       wsprintf(tempMeme,"%s %s",FpuInstructionsSet3[REG],menemonic);
                    }
                    break;

                    case 0xDF: // Extended FPU Instructions Set2 (Signed)
                    {
                      if(REG==1) // no such fpu instruction!
                         lstrcat((*Disasm)->Remarks,"Invalid Instruction");
                            
                      wsprintf(tempMeme,"%s %s",FpuInstructionsSet2Signed_EX[REG],menemonic);
                    }
                    break;

                    case 0xF6:
                    {
                        // strip Instruction Bits (1111011woo[000]mmm)
                        reg1=((BYTE)(*(*Opcode+pos+1))&0x38)>>3;

                        if(Extension==1)// read 1 byte at 3rd position
						{
                            if(reg1==0 || reg1==1) // check bites: TEST 
                            {                            
                                FOpcode=(BYTE)(*(*Opcode+pos+3));
                                wsprintf(temp,"%02X",FOpcode);
                                lstrcat((*Disasm)->Opcode,temp);
                            }
						}
						else
						{ 
                            if(Extension==2)
                            {
                                if(reg1==0 || reg1==1) // check bites: TEST 
                                {                                
                                    // read byte at 7th position (dword read before)
                                    FOpcode=(BYTE)(*(*Opcode+pos+7));
                                    wsprintf(temp,"%02X",FOpcode);
                                    lstrcat((*Disasm)->Opcode,temp);
                                }
							}
                            else if (Extension==0)
                            {
                                if(reg1==0 || reg1==1) // check bites: TEST 
                                {                                
                                    FOpcode=(BYTE)(*(*Opcode+pos+2));
                                    wsprintf(temp,"%02X",FOpcode);
                                    lstrcat((*Disasm)->Opcode,temp);
                                }
                            }
						}
                        
						strcpy(Aritmathic,"");						
                        wsprintf(instruction,"%s",InstructionsSet2[REG]);

                        if(reg1==0 || reg1==1) // TEST
                        {                        
						    wsprintf(tempMeme,"%s %s,%s%02X",instruction,menemonic,Aritmathic,FOpcode);
                            (*(*index))++;
						    (*Disasm)->OpcodeSize++;
                        }
                        else// NOT/NEG/MUL/IMUL/DIV/IDIV instruction must not have operands
                            wsprintf(tempMeme,"%s %s",instruction,menemonic);						
                    }
                    break;

                    case 0xF7:
                    {
                        // Get Instruction
                        wsprintf(instruction,"%s",InstructionsSet2[REG]);
                        reg1=((BYTE)(*(*Opcode+pos+1))&0x38)>>3;

                        //================//
                        // Get Extensions!//
                        //================//
                        
                        if(reg1==0 || reg1==1){ // TEST Instruction
                            
                            if(Extension==1) // 1 byte extersion
                            {	
                                if(PrefixReg==0)  // no 0x66 prefix
                                {   
                                    SwapDword((BYTE*)(*Opcode+pos+4),&dwOp,&dwMem);
                                    wsprintf(temp," %08X",dwOp);                                    
                                    lstrcat((*Disasm)->Opcode,temp);
                                    wsprintf(temp,"%08X",dwMem);
                                }
                                else
                                {
                                    SwapWord((BYTE*)(*Opcode+pos+4),&wOp,&wMem);
                                    wsprintf(temp," %04X",wOp);                                   
                                    lstrcat((*Disasm)->Opcode,temp);
                                    wsprintf(temp,"%04X",wMem);
                                }
                            }
                            else if(Extension==2) // 4 bytes Extensions
                            {    
                                if(PrefixReg==0)
                                {   
                                    SwapDword((BYTE*)(*Opcode+pos+7),&dwOp,&dwMem);
                                    wsprintf(temp," %08X",dwOp);                                    
                                    lstrcat((*Disasm)->Opcode,temp);
                                    wsprintf(temp,"%08X",dwMem);
                                }
                                else
                                {
                                    SwapWord((BYTE*)(*Opcode+pos+7),&wOp,&wMem);
                                    wsprintf(temp," %04X",wOp);                                    
                                    lstrcat((*Disasm)->Opcode,temp);
                                    wsprintf(temp,"%04X",wMem);
                                }
                            }
                            else{ // No Extension!
                                
                                if(PrefixReg==0)
                                {   
                                    SwapDword((BYTE*)(*Opcode+pos+3),&dwOp,&dwMem);
                                    wsprintf(temp," %08X",dwOp);                                    
                                    lstrcat((*Disasm)->Opcode,temp);
                                    wsprintf(temp,"%08X",dwMem);
                                }
                                else
                                {
                                    SwapWord((BYTE*)(*Opcode+pos+3),&wOp,&wMem);
                                    wsprintf(temp," %04X",wOp);                                    
                                    lstrcat((*Disasm)->Opcode,temp);
                                    wsprintf(temp,"%04X",wMem);
                                }
                            }
                            
                            wsprintf(tempMeme,"%s %s,%s",instruction,menemonic,temp);
                            if(PrefixReg==0) // No Reg prefix
                            {                            
                                (*(*index))+=4;
                                (*Disasm)->OpcodeSize+=4;
                            }
                            else
                            {
                                (*(*index))+=2;
                                (*Disasm)->OpcodeSize+=2;
                            }
                        }
                        else
                            wsprintf(tempMeme,"%s %s",instruction,menemonic);                                             
                    }
                    break;
                    
                    case 0xFE: // MIX Instructions (INC,DEC,INVALID,INVALID,INVALID...)
                    {                                                
                       wsprintf(tempMeme,"%s %s",InstructionsSet3[REG],menemonic);
                            
                       if(REG>1) // Invalid instructions
                           lstrcat((*Disasm)->Remarks,"Invalid Instruction");
                    }
                    break;

                    case 0xFF:// MIX Instructions (INC,DEC,CALL,PUSH,JMP,FAR JMP,FAR CALL,INVALID)
                    {
                       wsprintf(tempMeme,"%s %s",InstructionsSet4[REG],menemonic);
                    
                       if(REG==3)// FAR CALL
                       {
                           lstrcat((*Disasm)->Remarks,"Far Call");
                           break;
                       }

                       if(REG==5)
                       {
                           lstrcat((*Disasm)->Remarks,"Far Jump");
                           break;
                       }

                       if(REG==7) // Invalid instructions
                          lstrcat((*Disasm)->Remarks,"Invalid Instruction");
                    }
                    break;

					default:
					{
					   wsprintf(tempMeme,"%s %s,%s",instruction,menemonic,regs[RM][REG]);
					}
					break;
				}

				lstrcat((*Disasm)->Assembly,tempMeme);
			}
			break;

			case 1: // (<-) Direction
			{
                switch(Op) 
                {
                    case 0x8E: // Segments in Destination Register
                    {
                        wsprintf(tempMeme,"%s %s,%s",instruction,segs[REG],menemonic);
                    }
                	break;

                    // POP DWORD PTR[REG/MEM/DISP]
                    case 0x8F:
                    {
                        wsprintf(tempMeme,"%s %s",instruction,menemonic);
                    }
                    break;
                    
                    // Mixed Bit Rotation Instructions (rol/ror/shl..)
                    case 0xC0:case 0xC1:
                    {
                        switch(Extension)
                        {
                            case 0:
                            {
                                FOpcode=(BYTE)(*(*Opcode+pos+3));
                                wsprintf(tempMeme,"%s %s,%02X",ArtimaticInstructions[REG],menemonic,FOpcode);                                
                                wsprintf(menemonic," %02X",FOpcode);
                                lstrcat((*Disasm)->Opcode,menemonic);
                                (*(*index))++;
                                (*Disasm)->OpcodeSize++;
                            }
                            break;

                            case 1:
                            {
                                FOpcode=(BYTE)(*(*Opcode+pos+4));
                                wsprintf(tempMeme,"%s %s,%02X",ArtimaticInstructions[REG],menemonic,FOpcode);                                
                                //wsprintf(menemonic," %02X",(BYTE)(*(*Opcode+pos+4)));
                                wsprintf(menemonic," %02X",FOpcode);
                                lstrcat((*Disasm)->Opcode,menemonic);
                                (*(*index))++;
                                (*Disasm)->OpcodeSize++;
                            }
                            break;

                            case 2:
                            {
                                FOpcode=(BYTE)(*(*Opcode+pos+7));
                                wsprintf(tempMeme,"%s %s,%02X",ArtimaticInstructions[REG],menemonic,FOpcode);                                
                                wsprintf(menemonic," %02X",FOpcode);
                                lstrcat((*Disasm)->Opcode,menemonic);
                                (*(*index))++;
                                (*Disasm)->OpcodeSize++;
                            }
                            break;
                        }                        
                    }
                    break;
                    
                    case 0xC4:
                    {
                       strcpy(instruction,"les");
                       wsprintf(tempMeme,"%s %s,%s",instruction,regs[RM][REG],menemonic);
                    }
                    break;

                    case 0xC5:
                    {
                       strcpy(instruction,"lds");
                       wsprintf(tempMeme,"%s %s,%s",instruction,regs[RM][REG],menemonic);
                    }
                    break;

                    default:
                    {
                        wsprintf(tempMeme,"%s %s,%s",instruction,regs[RM][REG],menemonic);
                    }
                    break;
                }
				
				lstrcat((*Disasm)->Assembly,tempMeme);
			}
			break;
		}		
	}	
}

void CPayLoadDlg::Mod_RM_SIB_EX(DISASSEMBLY **Disasm, char **Opcode, int pos, bool AddrPrefix, int SEG, DWORD **index, BYTE Op, bool PrefixReg, bool PrefixSeg, bool PrefixAddr, BYTE Bit_d, BYTE Bit_w, bool RepPrefix)
{
	/*
        This Function will resolve BigSet menemonics: 
        Of MMX,3DNow! and New Set Instructions.
    */

	// Set Defaults    
    DWORD dwOp,dwMem;
    int RM=REG8,SCALE=0,SIB,ADDRM=REG32;
    WORD wOp,wMem;
    char RSize[10]="byte",Aritmathic[5]="+",tempAritmathic[5]="+";
	BYTE reg1=0,reg2=0,REG=0,Extension=0,FOpcode=0;
    char menemonic[128]="",tempMeme[128]="",Addr[15]="",temp[128]="";
	char instr[50]="";

    // Get used Register
	// Get target register, example:
	// 1. add byte ptr [ecx], -> al <-
	// 2. add -> al <- ,byte ptr [ecx]
    REG=(BYTE)(*(*Opcode+pos+1)); 
	REG>>=3;
	REG&=0x07;

    //Displacement MOD (none|BYTE/WORD|DWORD)
	Extension=(BYTE)(*(*Opcode+pos+1))>>6;


    switch((BYTE)(*(*Opcode+pos)))
    {
      case 0x00:{Bit_d=0; Bit_w=1; strcpy(RSize,regSize[2]); } break; // WORD
      case 0x01:{
                 Bit_d=0; Bit_w=1;
                 switch(REG)
                 {
                    case 0: case 1: case 2: case 3: strcpy(RSize,regSize[4]); break; // FWORD
                    case 4: case 5: case 6: strcpy(RSize,regSize[2]);         break; // WORD  
                    case 7:strcpy(RSize,regSize[3]);                          break; // BYTE
                 }
                }
                break;
      case 0x02: case 0x03:{ Bit_d=1; Bit_w=1; strcpy(RSize,regSize[1]); }    break; // DWORD
      case 0x0D: { Bit_d=1; Bit_w=1; strcpy(RSize,regSize[0]); }			  break; // DWORD
	  case 0x10: { Bit_d=1; Bit_w=1; strcpy(RSize,regSize[8]); }              break; // DQWORD
      case 0x11: { Bit_d=0; Bit_w=1; strcpy(RSize,regSize[8]); }              break; // DQWORD
      case 0x12: case 0x16: { Bit_d=1; Bit_w=1; strcpy(RSize,regSize[0]); }   break; // QWORD
      case 0x13: case 0x17: { Bit_d=0; Bit_w=1; strcpy(RSize,regSize[0]); }   break; // QWORD
      case 0x14: case 0x15: { Bit_d=1; Bit_w=1; strcpy(RSize,regSize[8]); }   break; // DQWORD
      case 0x18: { Bit_d=1; Bit_w=1; strcpy(RSize,regSize[0]); }              break; // QWORD
      case 0x28: { Bit_d=1; Bit_w=1; strcpy(RSize,regSize[8]); }              break; // DQWORD
      case 0x29: { Bit_d=0; Bit_w=1; strcpy(RSize,regSize[8]); }              break; // DQWORD
      case 0x2A: case 0x2C: case 0x2D:case 0x2E: case 0x2F:                  
      {
          Bit_d=1; Bit_w=1;
          switch((Op&0x0F)-0x08)
          {
            case 2: strcpy(RSize,regSize[0]);         break; // QWORD
            case 4: case 5: strcpy(RSize,regSize[8]); break; // DQWORD
            case 6: case 7: strcpy(RSize,regSize[1]); break; // DWORD
          }
      }
      break;      
      case 0x40:case 0x41:case 0x42:case 0x43:case 0x44:case 0x45:case 0x46:case 0x47:
      case 0x48:case 0x49:case 0x4A:case 0x4B:case 0x4C:case 0x4D:case 0x4E:case 0x4F:
          { Bit_d=1; Bit_w=1; strcpy(RSize,regSize[1]); }  // DWORD
      break;

      case 0x51:case 0x52:case 0x53:case 0x54:case 0x55:case 0x56:case 0x57:
      case 0x58:case 0x59:case 0x5C:case 0x5D:case 0x5E:case 0x5F:
          { Bit_d=1; Bit_w=1; strcpy(RSize,regSize[8]); } // DQWORD
      break;
      case 0x60:case 0x61:case 0x62:case 0x63:case 0x64:case 0x65:case 0x66:case 0x67:
      case 0x68:case 0x69:case 0x6A:case 0x6B:case 0x6E:case 0x6F:
          { Bit_d=1; Bit_w=1; if((Op&0x0F)==0x0E)strcpy(RSize,regSize[1]);else strcpy(RSize,regSize[0]); }  // DWORD/QWORD
      break;
      case 0x70:case 0x74:case 0x75:case 0x76: { Bit_d=1; Bit_w=1; strcpy(RSize,regSize[0]); } break; // QWORD
      case 0x7E: { Bit_d=0; Bit_w=1; strcpy(RSize,regSize[1]); }  break;   // DWORD
      case 0x7F: { Bit_d=0; Bit_w=1; strcpy(RSize,regSize[0]); }  break;   // QWORD
      case 0x90:case 0x91:case 0x92:case 0x93:case 0x94:case 0x95:case 0x96:case 0x97:
      case 0x98:case 0x99:case 0x9A:case 0x9B:case 0x9C:case 0x9D:case 0x9E:case 0x9F:
          { Bit_d=1; Bit_w=1; strcpy(RSize,regSize[3]); }  // BYTE
      break;
      case 0xA3:case 0xA4:case 0xA5:case 0xAB:case 0xAC:case 0xAD:{ Bit_d=0; Bit_w=1; strcpy(RSize,regSize[1]); } break;//DWORD
      case 0xAE:
      { 
          Bit_d=1; Bit_w=1;
          if(REG<2)
            strcpy(RSize,regSize[9]); // (512)Byte
          else
            strcpy(RSize,regSize[1]); // DWORD 
      } 
      break; //512Byte / DWORD  (FXSAVE)
      case 0xAF:            { Bit_d=1; Bit_w=1; strcpy(RSize,regSize[1]); }            break; // DWORD  (IMUL)
      case 0xB0:            { Bit_d=0; Bit_w=1; strcpy(RSize,regSize[3]); }            break; // BYTE   (CMPXCHG)
      case 0xB1: case 0xB3: case 0xBB:{ Bit_d=0; Bit_w=1; strcpy(RSize,regSize[1]); }  break; // DWORD  (CMPXCHG/BTC/BTR)
      case 0xB2: case 0xB4: case 0xB5:{ Bit_d=1; Bit_w=1; strcpy(RSize,regSize[4]); }  break; // FWORD  (LSS/LFS/LGS)
      case 0xB6: case 0xBE: { Bit_d=1; Bit_w=1; strcpy(RSize,regSize[3]); }            break; // BYTE   (MOVSX/MOVZX)
      case 0xB7: case 0xBF: { Bit_d=1; Bit_w=1; strcpy(RSize,regSize[2]); }            break; // WORD   (MOVSX/MOVZX)
      case 0xBC: case 0xBD: { Bit_d=1; Bit_w=1; strcpy(RSize,regSize[1]); }            break; // DWORD  (BSF/BSR)
      case 0xC0:            { Bit_d=0; Bit_w=1; strcpy(RSize,regSize[3]); }            break; // BYTE   (XADD)
      case 0xC1:            { Bit_d=0; Bit_w=1; strcpy(RSize,regSize[1]); }            break; // DWORD  (XADD)
      case 0xC2: case 0xC6: { Bit_d=1; Bit_w=1; strcpy(RSize,regSize[8]); }            break; // DQWORD (MIX)
      case 0xC4:            { Bit_d=1; Bit_w=1; strcpy(RSize,regSize[2]); }            break; // WORD   (MIX)
      case 0xC5: case 0xC7: { Bit_d=1; Bit_w=1; strcpy(RSize,regSize[0]); }            break; // QWORD  (MIX)
      
      case 0xD1:case 0xD2:case 0xD3:case 0xD5:case 0xD7:
      case 0xD8:case 0xD9:case 0xDA:case 0xDB:case 0xDC:
      case 0xDD:case 0xDE:case 0xDF:case 0xE0:case 0xE1:
      case 0xE2:case 0xE3:case 0xE4:case 0xE5:case 0xE8:
      case 0xE9:case 0xEA:case 0xEB:case 0xEC:case 0xED:
      case 0xEE:case 0xEF:
      case 0xF1:case 0xF2:case 0xF3:case 0xF5:case 0xF6:
      case 0xF7:case 0xF8:case 0xF9:case 0xFA:case 0xFC:
      case 0xFD:case 0xFE:
      {
          Bit_d=1; Bit_w=1; 
          strcpy(RSize,regSize[0]); // QWORD
      }
      break;

      case 0xE7: Bit_d=0; Bit_w=1; strcpy(RSize,regSize[0]); break; // QWORD      
      
    }

    	// check for bit register size : 16bit/32bit
	if(Bit_w==1)
	{
	   RM=REG32; // 32bit registers set                   
	}
	
    if(PrefixReg==1) // Change 32bit Data Size to 16Bit
    {
        // All Opcodes with DWORD Data Size
        BYTE DOpcodes[35]={ 
                    0x02,0x03,0x2E,0x2F,0x6E,0x7E,
                    0xA3,0xA4,0xA5,0xAB,0xAC,0xAD,
                    0xAF,0xB1,0xB3,0xBB,0xBC,0xBD,
                    0xC1,0x40,0x41,0x42,0x43,0x44,
                    0x45,0x46,0x47,0x48,0x49,0x4A,
                    0x4B,0x4C,0x4D,0x4E,0x4F
        };

        for(int i=0;i<35;i++)
            if(Op==DOpcodes[i])
            {            
                RM=REG16; // 16bit registers
                strcpy(RSize,regSize[2]); // word ptr
                break;
            }
    }
    
    if(RepPrefix==1)
    {
       strcpy(RSize,regSize[1]); // DWORD
    }

    // SCALE INDEX BASE
	SIB=(BYTE)(*(*Opcode+pos+1))&0x07; // Get SIB extension


    // ===================================================//
    //             AddrPrefix is being used!               //
    // ===================================================//
    
    if(PrefixAddr==1) // Prefix 0x67 is set, Change Segments/Addressing Modes to 16 bits
    {		
        FOpcode=((BYTE)(*(*Opcode+pos+1))&0x0F); // Get addressing Mode (8 types of mode)
        reg1=((BYTE)(*(*Opcode+pos+1))&0x38)>>3;
        
        // Choose Mode + Segment
        switch(FOpcode)
        {
            case 0x00: case 0x08: wsprintf(Addr,"%s",addr16[0]); SEG=SEG_DS; break; // Mode 0:[BX+SI]
            case 0x01: case 0x09: wsprintf(Addr,"%s",addr16[1]); SEG=SEG_DS; break; // Mode 1:[BX+DI]
            case 0x02: case 0x0A: wsprintf(Addr,"%s",addr16[2]); SEG=SEG_SS; break; // Mode 2:[BP+SI]
            case 0x03: case 0x0B: wsprintf(Addr,"%s",addr16[3]); SEG=SEG_SS; break; // Mode 3:[BP+DI]
            case 0x04: case 0x0C: wsprintf(Addr,"%s",addr16[4]); SEG=SEG_DS; break; // Mode 4:[SI]
            case 0x05: case 0x0D: wsprintf(Addr,"%s",addr16[5]); SEG=SEG_DS; break; // Mode 5:[DI]
            case 0x06: case 0x0E: // Mode 6: [BP+XX/XXXX] | [XX]
            {
                if(Extension==0) // 0x00-0x3F only! has special [XXXX]
                {
                    SEG=SEG_DS;
                    SwapWord((BYTE*)(*Opcode+pos+2),&wOp,&wMem);
                    wsprintf(Addr,"%04X",wMem);
                    (*(*index))+=2; // read 2 bytes
                }
                else{ // 0x50-0xBF has [BP+]
                    
                    SEG=SEG_SS; // SS Segment
                    wsprintf(Addr,"%s",addr16[7]);
                }
            }
            break;
            
            case 0x07: case 0x0F: wsprintf(Addr,"%s",addr16[6]); SEG=SEG_DS; break; // Mode 7: [BX]
        }
        
        // Choose used extension 
        // And Decode properly the menemonic
        switch(Extension)
        {
            case 0: // No extension of bytes to RegMem (except mode 6)
            {
                wsprintf(tempMeme,"%s ptr %s:[%s]",RSize,segs[SEG],Addr);
                SwapWord((BYTE*)(*Opcode+pos),&wOp,&wMem);
                SwapDword((BYTE*)(*Opcode+pos+2),&dwOp,&dwMem);

                if(((wOp&0x00FF)&0x0F)==0x06) // 0x00-0x3F with mode 6 only!
                {					
                    wsprintf(menemonic,"%08X",dwOp);
                    (*Disasm)->OpcodeSize=4;
                    lstrcat((*Disasm)->Opcode,menemonic);
                    FOpcode=(BYTE)(*(*Opcode+pos+4));
                }
                else{ // other modes
                    wsprintf(menemonic,"%04X",wOp);
                    (*Disasm)->OpcodeSize=2;
                    lstrcat((*Disasm)->Opcode,menemonic);
                    FOpcode=(BYTE)(*(*Opcode+pos+2));
                }
            }
            break;
            
            case 1: // 1 Byte Extension to regMem
            {
                SwapWord((BYTE*)(*Opcode+pos+1),&wOp,&wMem);
                FOpcode=wOp&0x00FF;
                
                if(FOpcode>0x7F) // check for signed numbers
                {
                    wsprintf(Aritmathic,"%s",Scale[0]); // '-' Signed Numbers
                    FOpcode = 0x100-FOpcode; // -XX
                }
                wsprintf(menemonic,"%02X%04X",Op,wOp);
                lstrcat((*Disasm)->Opcode,menemonic);
                wsprintf(tempMeme,"%s ptr %s:[%s%s%02X]",RSize,segs[SEG],Addr,Aritmathic,FOpcode);
                ++(*(*index)); // 1 byte read
                (*Disasm)->OpcodeSize=3;
                FOpcode=(BYTE)(*(*Opcode+pos+3));
            }
            break;
            
            case 2: // 2 Bytes Extension to RegMem
            {
                SwapDword((BYTE*)(*Opcode+pos),&dwOp,&dwMem);
                SwapWord((BYTE*)(*Opcode+pos+2),&wOp,&wMem);
                wsprintf(menemonic,"%08X",dwOp);
                (*Disasm)->OpcodeSize=4;
                lstrcat((*Disasm)->Opcode,menemonic);
                wsprintf(tempMeme,"%s ptr %s:[%s%s%04X]",RSize,segs[SEG],Addr,Aritmathic,wMem);
                (*(*index))+=2; // we read 2 bytes
                FOpcode=(BYTE)(*(*Opcode+pos+4));
            }
            break;
		}
        
        switch(Bit_d)
        {
            case 0: // direction (->)
			{
                switch(Op)
                {
                    case 0x00:
                    {
                        wsprintf(temp,"%s %s",NewSet[REG],tempMeme);

                        if(REG>5) // Invalid operation
                            lstrcat((*Disasm)->Remarks,"Invalid instruction");
                    }
                    break;

                    case 0x01:
                    {
                        wsprintf(temp,"%s %s",NewSet2[REG],tempMeme);
                        
                        if(REG==5) // Invalid operation
                            lstrcat((*Disasm)->Remarks,"Invalid instruction");
                    }
                    break;

                    case 0x11: 
                        {
                            if(RepPrefix==1)
                            {
                                wsprintf(temp,"movss %s,%s",tempMeme,MMXRegs[REG]);
                                strcpy((*Disasm)->Assembly,"");
                                (*Disasm)->OpcodeSize++;
                            }
                            else
                                wsprintf(temp,"movups %s,%s",tempMeme,MMXRegs[REG]); 
                        }
                        break; // MOVUPS
                    case 0x13: wsprintf(temp,"movlps %s,%s",tempMeme,MMXRegs[REG]); break; // MOVLPS
                    case 0x17: wsprintf(temp,"movhps %s,%s",tempMeme,MMXRegs[REG]); break; // MOVHPS
                    case 0x29: wsprintf(temp,"movaps %s,%s",tempMeme,MMXRegs[REG]); break; // MOVAPS
                    case 0x7E: case 0x7F:wsprintf(temp,"%s %s,%s",NewSet7[Op&0x0F],tempMeme,Regs3DNow[REG]); break; // MOVD/MOVQ
                    case 0xA3: case 0xAB:
                    {
                      wsprintf(temp,"%s %s,%s",NewSet10[Op&0x0F],tempMeme,regs[RM][REG]); 
                    }
                    break;

                    case 0xA4:case 0xAC:
                    {
                       wsprintf(temp,"%s %s,%s,%02X",NewSet10[Op&0x0F],tempMeme,regs[RM][REG],FOpcode);
                       wsprintf(menemonic," %02X",FOpcode);
                       lstrcat((*Disasm)->Opcode,menemonic);
                       (*Disasm)->OpcodeSize++;
                       (*(*index))++;
                    }
                    break;

                    case 0xA5: case 0xAD:
                    {
                      wsprintf(temp,"%s %s,%s,cl",NewSet10[Op&0x0F],tempMeme,regs[RM][REG]); 
                    }
                    break;

                    case 0xB0: case 0xB1: case 0xB3: case 0xBB:
                    {
                      if((Op&0x0F)==0x00)
                         RM=REG8;
                            
                      wsprintf(temp,"%s %s,%s",NewSet11[Op&0x0F],tempMeme,regs[RM][REG]);
                    }
                    break;

                    case 0xC0:
                    { 
                        RM=REG8; 
                        wsprintf(temp,"xadd %s,%s",tempMeme,regs[RM][REG]); 
                    } 
                    break; // XADD

                    case 0xC1:wsprintf(temp,"xadd %s,%s",tempMeme,regs[RM][REG]); break;
                    case 0xE7:wsprintf(temp,"%s %s,%s",NewSet14[Op&0x07],tempMeme,Regs3DNow[REG]); break;

                }
                lstrcat((*Disasm)->Assembly,temp);
            }
            break;

            case 1: // direction (<-)
            {
                switch(Op)
                {                
                    case 0x02: wsprintf(temp,"lar %s,%s",regs[RM][REG],tempMeme);     break; // LAR
                    case 0x03: wsprintf(temp,"lsl %s,%s",regs[RM][REG],tempMeme);     break; // LAR
                    case 0x10: 
                        {
                          if(RepPrefix==1)
                          {
                            wsprintf(temp,"movss %s,%s",MMXRegs[REG],tempMeme);
                            strcpy((*Disasm)->Assembly,"");
                            (*Disasm)->OpcodeSize++;
                          }
                          else
                            wsprintf(temp,"movups %s,%s",MMXRegs[REG],tempMeme);
                        }
                        break; // MOVUPS
                    
                    case 0x12: wsprintf(temp,"movlps %s,%s",MMXRegs[REG],tempMeme);   break; // MOVLPS
                    case 0x14: wsprintf(temp,"unpcklps %s,%s",MMXRegs[REG],tempMeme); break; // UNPCKLPS
                    case 0x15: wsprintf(temp,"unpckhps %s,%s",MMXRegs[REG],tempMeme); break; // UNPCKHPS
                    case 0x16: wsprintf(temp,"movhps %s,%s",MMXRegs[REG],tempMeme);   break; // MOVHPS
                    
                    case 0x18:
                    {
                      wsprintf(temp,"%s,%s",NewSet3[REG],tempMeme);
                      if(REG>3)
                         lstrcat((*Disasm)->Remarks,"Invalid instruction");
                    } 
                    break; // MIX
                    
                    case 0x28: wsprintf(temp,"movaps %s,%s",MMXRegs[REG],tempMeme);   break; // MOVAPS
                    
                    case 0x2A: case 0x2C: case 0x2D:case 0x2E: case 0x2F: 
                    {
                      BYTE R=((Op&0x0F)-0x08);
                      if(R==4 || R==5)
                          wsprintf(instr,"%s",Regs3DNow[REG]); // 3DNow! Regs
                      else
                          wsprintf(instr,"%s",MMXRegs[REG]); // MMX Regs
                        
                      if(RepPrefix==1)
                      {
                          if(Op==0x2A || Op==0x2C || Op==0x2D)
                          {
                              switch(Op)
                              {
                                case 0x2A:strcpy(menemonic,"cvtsi2ss");  break;
                                case 0x2C:
                                {
                                  strcpy(menemonic,"cvttss2si"); 
                                  wsprintf(instr,"%s",regs[RM][REG]);
                                }
                                break;
                                case 0x2D:
                                {
                                   strcpy(menemonic,"cvtss2si");
                                   wsprintf(instr,"%s",regs[RM][REG]);
                                }
                                break;
                              }
                              strcpy((*Disasm)->Assembly,"");
                              (*Disasm)->OpcodeSize++;
                              wsprintf(temp,"%s %s,%s",menemonic,instr,tempMeme);
                          }
                      }
                      else
                        wsprintf(temp,"%s %s,%s",NewSet4[(Op&0x0F)-0x08],instr,tempMeme); 
                    }
                    break; // MIX
                    
                    case 0x40:case 0x41:case 0x42:case 0x43:case 0x44:case 0x45:case 0x46:case 0x47:
                    case 0x48:case 0x49:case 0x4A:case 0x4B:case 0x4C:case 0x4D:case 0x4E:case 0x4F:
                    {
                       wsprintf(temp,"%s %s,%s",NewSet5[Op&0x0F],regs[RM][REG],tempMeme); 
                    }
                    break; // MIX

                    case 0x51:case 0x52:case 0x53:case 0x54:case 0x55:case 0x56:case 0x57:
                    case 0x58:case 0x59:case 0x5C:case 0x5D:case 0x5E:case 0x5F:
                    {
                        if(RepPrefix==1)
                        {
                            wsprintf(temp,"%s %s,%s",NewSet6Ex[Op&0x0F],MMXRegs[reg2],tempMeme);
                            strcpy((*Disasm)->Assembly,"");
                            (*Disasm)->OpcodeSize++;
                        }
                        else
                            wsprintf(temp,"%s %s,%s",NewSet6[Op&0x0F],MMXRegs[REG],tempMeme); 
                    }
                    break; // MIX

                    case 0x60:case 0x61:case 0x62:case 0x63:case 0x64:case 0x65:case 0x66:case 0x67:
                    case 0x68:case 0x69:case 0x6A:case 0x6B:case 0x6E:case 0x6F:
                    {
                       wsprintf(temp,"%s %s,%s",NewSet7[Op&0x0F],Regs3DNow[REG],tempMeme); 
                    }
                    break;

                    case 0x70:
                    {                       
                        wsprintf(temp,"%s %s,%s,%02X",NewSet8[Op&0x0F],Regs3DNow[reg2],tempMeme,FOpcode);
                        wsprintf(menemonic," %02X",FOpcode);
                        lstrcat((*Disasm)->Opcode,menemonic);
                        (*Disasm)->OpcodeSize++;
                        (*(*index))++;
                    }
                    break;

                    case 0x74:case 0x75:case 0x76: // MIX
                    {
                       wsprintf(temp,"%s %s,%s",NewSet8[Op&0x0F],Regs3DNow[REG],tempMeme); 
                    }
                    break;

                    case 0x90:case 0x91:case 0x92:case 0x93:case 0x94:case 0x95:case 0x96:case 0x97:
                    case 0x98:case 0x99:case 0x9A:case 0x9B:case 0x9C:case 0x9D:case 0x9E:case 0x9F:
                    {
                      wsprintf(temp,"%s %s",NewSet9[Op&0x0F],tempMeme);
                    }
                    break; // MIX

                    case 0xAE: // FXSAVE
                    {
                        wsprintf(temp,"%s %s",NewSet10Ex[REG],tempMeme);
                        if(REG>3)// Check for Invalid
                            lstrcat((*Disasm)->Remarks,"Invalid Instruction");
                    }
                    break;
                    case 0xAF:wsprintf(temp,"%s %s,%s",NewSet10[Op&0x0F],regs[RM][REG],tempMeme);break;
                    
                    case 0xB2:case 0xB4:case 0xB5:
                    case 0xB6:case 0xB7:case 0xBC: 
                    case 0xBD:case 0xBE:case 0xBF:
                    {
                      wsprintf(temp,"%s %s,%s",NewSet11[Op&0x0F],regs[RM][REG],tempMeme);
                    }
                    break;

                    case 0xC2:
                    {
                      if(FOpcode<8) // Instructions here
                      {
                          if(RepPrefix==1) // Rep Prefix is being used
                          {
                              wsprintf(temp,"%s %s,%s",NewSet12Ex[FOpcode],MMXRegs[REG],tempMeme);
                              strcpy((*Disasm)->Assembly,"");
                              (*Disasm)->OpcodeSize++;
                          }
                          else
                              wsprintf(temp,"%s %s,%s",NewSet12[FOpcode],MMXRegs[REG],tempMeme);
                      }
                      else
                      {
                          if(RepPrefix==1) // Rep Prefix is being used
                          {
                              wsprintf(temp,"cmpss %s,%s,%02X",MMXRegs[REG],tempMeme,FOpcode);
                              strcpy((*Disasm)->Assembly,"");
                              (*Disasm)->OpcodeSize++;
                          }
                          else
                              wsprintf(temp,"cmpps %s,%s,%02X",MMXRegs[REG],tempMeme,FOpcode);
                      }
                       
                      wsprintf(menemonic," %02X",FOpcode);
                      lstrcat((*Disasm)->Opcode,menemonic);
                      (*Disasm)->OpcodeSize++;
                      (*(*index))++;
                    }
                    break;

                    case 0xC4:
                    {
                        wsprintf(temp,"pinsrw %s,%s,%02X",Regs3DNow[REG],tempMeme,FOpcode);                        
                        wsprintf(menemonic," %02X",FOpcode);
                        lstrcat((*Disasm)->Opcode,menemonic);
                        (*Disasm)->OpcodeSize++;
                        (*(*index))++;
                    }
                    break;
                    
                    case 0xC5:
                    {
                        wsprintf(temp,"pextrw %s,%s,%02X",regs[RM][REG],tempMeme,FOpcode);                        
                        wsprintf(menemonic," %02X",FOpcode);
                        lstrcat((*Disasm)->Opcode,menemonic);
                        (*Disasm)->OpcodeSize++;
                        (*(*index))++;
                    }
                    break;
                    
                    case 0xC6:
                    {
                        wsprintf(temp,"shufps %s,%s,%02X",MMXRegs[REG],tempMeme,FOpcode);                        
                        wsprintf(menemonic," %02X",FOpcode);
                        lstrcat((*Disasm)->Opcode,menemonic);
                        (*Disasm)->OpcodeSize++;
                        (*(*index))++;
                    }
                    break;

                    case 0xD7:wsprintf(temp,"%s %s,%s",NewSet13[Op&0x0F],regs[RM][REG],tempMeme);break;
                    case 0xD1:case 0xD2:case 0xD3:case 0xD5:case 0xD8:case 0xDF:
                    case 0xD9:case 0xDA:case 0xDB:case 0xDC:case 0xDD:case 0xDE:                    
                    {
                        wsprintf(temp,"%s %s,%s",NewSet13[Op&0x0F],Regs3DNow[REG],tempMeme);
                    }
                    break;

                    case 0xE0:case 0xE1:case 0xE2:case 0xE3:
                    case 0xE4:case 0xE5:case 0xE8:case 0xE9:
                    case 0xEA:case 0xEB:case 0xEC:case 0xED:
                    case 0xEE:case 0xEF:
                    {
                      wsprintf(temp,"%s %s,%s",NewSet14[Op&0x0F],Regs3DNow[REG],tempMeme);
                    }
                    break;

                    case 0xF1:case 0xF2:case 0xF3:case 0xF5:case 0xF6:
                    case 0xF7:case 0xF8:case 0xF9:case 0xFA:case 0xFC:
                    case 0xFD:case 0xFE:
                    {
                      wsprintf(temp,"%s %s,%s",NewSet15[Op&0x0F],Regs3DNow[REG],tempMeme);
                    }
                    break;
                }

                lstrcat((*Disasm)->Assembly,temp);
            }
            break;

        }

        ++(*(*index)); // add 1 byte to index
        // no need to continue!! exit the function and proeed with decoding next bytes.
		return;
    }
    
    //===================================================//
    //               NO SIB Being used!                  //
    //===================================================//
    
    if(SIB!=SIB_EX) // NO SIB extension (i.e: 0x0001 = add byte ptr [ecx], al)
    {
        reg1=((BYTE)(*(*Opcode+pos+1))&0x07); // get register (we have only one)
        reg2=(((BYTE)(*(*Opcode+pos+1))&0x38)>>3);
        
        switch(Extension) // Check what extension we have (None/Byte/Dword)
        {
            case 00: // no extention to regMem
            {
                if(reg1==REG_EBP) // cannot display EBP as memoryReg, use DWORD mem location
                {
                    SwapDword((BYTE*)(*Opcode+pos+2),&dwOp,&dwMem);
                    SwapWord((BYTE*)(*Opcode+pos),&wOp,&wMem);                    
                    wsprintf(menemonic,"%04X%08X",wOp,dwOp);
                    lstrcat((*Disasm)->Opcode,menemonic);
                    wsprintf(instr,"%08X",dwMem);
                    wsprintf(menemonic,"%s ptr %s:[%s]",RSize,segs[SEG],instr);					
                    (*Disasm)->OpcodeSize=6;
                    (*(*index))+=5;
                    FOpcode=(BYTE)(*(*Opcode+pos+6));
                }
                else{
                    SwapWord((BYTE*)(*Opcode+pos),&wOp,&wMem);
                    wsprintf(menemonic,"%04X",wOp);
                    lstrcat((*Disasm)->Opcode,menemonic);
                    wsprintf(menemonic,"%s ptr %s:[%s]",RSize,segs[SEG],regs[ADDRM][reg1]);
                    ++(*(*index)); // only 1 byte read
                    (*Disasm)->OpcodeSize=2; // total used opcodes
                    FOpcode=(BYTE)(*(*Opcode+pos+2));
                }
            }
            break;
            
            case 01: // 1 btye extention to regMem
            {
                FOpcode=(BYTE)(*(*Opcode+pos+2));
                SwapWord((BYTE*)(*Opcode+pos),&wOp,&wMem);
                wsprintf(menemonic,"%04X%02X",wOp,FOpcode);
                lstrcat((*Disasm)->Opcode,menemonic);
                
                if(FOpcode>0x7F) // check for signed numbers
                {
                    wsprintf(Aritmathic,"%s",Scale[0]); // '-' aritmathic
                    FOpcode = 0x100-FOpcode; // -XX
                }
                
                if(reg1==REG_EBP && PrefixSeg==0)
                    SEG=SEG_SS;
                
                wsprintf(menemonic,"%s ptr %s:[%s%s%02X]",RSize,segs[SEG],regs[ADDRM][reg1],Aritmathic,FOpcode);
                (*(*index))+=2; // x + 1 byte(s) read
                (*Disasm)->OpcodeSize=3; // total used opcodes
                FOpcode=(BYTE)(*(*Opcode+pos+3));
            }
            break;
            
            case 02: // 4 btye extention to regMem
            {
                // if ebp and there is no prefix 0x67, use SS segment
                if(reg1==REG_EBP && PrefixSeg==0)
                    SEG=SEG_SS;
                
                SwapDword((BYTE*)(*Opcode+pos+2),&dwOp,&dwMem);
                SwapWord((BYTE*)(*Opcode+pos),&wOp,&wMem);                 
                wsprintf(menemonic,"%04X %08X",wOp,dwOp);
                lstrcat((*Disasm)->Opcode,menemonic);                
                wsprintf(instr,"%08X",dwMem);
                wsprintf(menemonic,"%s ptr %s:[%s+%s]",RSize,segs[SEG],regs[ADDRM][reg1],instr);
                (*(*index))+=5; // x + 1 + 4 byte(s) read
                (*Disasm)->OpcodeSize=6; // total used opcodes
                FOpcode=(BYTE)(*(*Opcode+pos+6));
            }
            break;            
        }
        
        switch(Bit_d)
        {            
            case 0: // direction (->)
            {
                switch(Op) // Check for all Cases Availble            
                {
                    case 0x00: // MIX Instructions
                    {
                        wsprintf(tempMeme,"%s %s",NewSet[REG],menemonic);
                        
                        if(REG>5) // Invalid operation
                            lstrcat((*Disasm)->Remarks,"Invalid instruction");
                    }
                    break;

                    case 0x01: // MIX Instructions
                    {
                        wsprintf(tempMeme,"%s %s",NewSet2[REG],menemonic);
                        
                        if(REG==5) // Invalid operation
                            lstrcat((*Disasm)->Remarks,"Invalid instruction");
                    }
                    break;

                    case 0x11: 
                        {
                            if(RepPrefix==1)
                            {
                                wsprintf(tempMeme,"movss %s,%s",menemonic,MMXRegs[reg2]);
                                strcpy((*Disasm)->Assembly,"");
                                (*Disasm)->OpcodeSize++;
                            }
                            else
                                wsprintf(tempMeme,"movups %s,%s",menemonic,MMXRegs[reg2]); 
                        }
                        break; // MOVUPS
                    case 0x13: wsprintf(tempMeme,"movlps %s,%s",menemonic,MMXRegs[reg2]); break; // MOVLPS
                    case 0x17: wsprintf(tempMeme,"movhps %s,%s",menemonic,MMXRegs[reg2]); break; // MOVHPS
                    case 0x29: wsprintf(tempMeme,"movaps %s,%s",menemonic,MMXRegs[reg2]); break; // MOVAPS
                    case 0x7E: case 0x7F:wsprintf(tempMeme,"%s %s,%s",NewSet7[Op&0x0F],menemonic,Regs3DNow[reg2]); break; // MOVD/MOVQ
                    case 0xA3: case 0xAB:
                    {
                        wsprintf(tempMeme,"%s %s,%s",NewSet10[Op&0x0F],menemonic,regs[RM][reg2]); 
                    }
                    break;
                    case 0xA4:case 0xAC:
                    {
                       wsprintf(tempMeme,"%s %s,%s,%02X",NewSet10[Op&0x0F],menemonic,regs[RM][reg2],FOpcode);
                       wsprintf(menemonic," %02X",FOpcode);
                       lstrcat((*Disasm)->Opcode,menemonic);
                       (*Disasm)->OpcodeSize++;
                       (*(*index))++;
                    }
                    break;
                    case 0xA5: case 0xAD:wsprintf(tempMeme,"%s %s,%s,cl",NewSet10[Op&0x0F],menemonic,regs[RM][reg2]); break;
                    
                    case 0xB0: case 0xB1:  case 0xB3: case 0xBB:
                    {
                        if((Op&0x0F)==0x00)
                            RM=REG8;

                        wsprintf(tempMeme,"%s %s,%s",NewSet11[Op&0x0F],menemonic,regs[RM][reg2]);
                    }
                    break;
                    case 0xC0: RM=REG8; wsprintf(tempMeme,"xadd %s,%s",menemonic,regs[RM][reg2]);break;
                    case 0xC1: wsprintf(tempMeme,"xadd %s,%s",menemonic,regs[RM][reg2]); break;
                    case 0xE7: wsprintf(tempMeme,"%s %s,%s",NewSet14[Op&0x07],menemonic,Regs3DNow[reg2]); break;
                }

                lstrcat((*Disasm)->Assembly,tempMeme); // copy the decoded assembly
            }
            break;

            case 1: // direction (<-)
            {
                switch(Op)
                {                 
                    case 0x02: wsprintf(tempMeme,"lar %s,%s",regs[RM][reg2],menemonic);     break; // LAR
                    case 0x03: wsprintf(tempMeme,"lsl %s,%s",regs[RM][reg2],menemonic);     break; // LSL
                    
					case 0x0D:
					{
						FOpcode=(BYTE)(*(*Opcode+pos+1));
						if(
							(FOpcode>=0x00 &&FOpcode<=0x0F) ||
							(FOpcode>=0x40 &&FOpcode<=0x4F) ||
							(FOpcode>=0x80 &&FOpcode<=0x8F)
						  )
						{
							if((FOpcode&0x0F)<=0x07)
								wsprintf(tempMeme,"prefetcht %s",menemonic);
							else
								wsprintf(tempMeme,"prefetchtW %s",menemonic);
						}
						else{
								strcpy(tempMeme,"???");
						}
						
					}
					break;

					case 0x10: 
                    {
						if(RepPrefix==1) // rep prefix is being used
						{
							strcpy((*Disasm)->Assembly,"");
							(*Disasm)->OpcodeSize++;
							wsprintf(tempMeme,"movss %s,%s",MMXRegs[reg2],menemonic);
						}
						else 
							wsprintf(tempMeme,"movups %s,%s",MMXRegs[reg2],menemonic); 
                    }
                    break; // MOVUPS
                    case 0x12: wsprintf(tempMeme,"movlps %s,%s",MMXRegs[reg2],menemonic);   break; // MOVLPS
                    case 0x14: wsprintf(tempMeme,"unpcklps %s,%s",MMXRegs[reg2],menemonic); break; // UNPCKLPS
                    case 0x15: wsprintf(tempMeme,"unpckhps %s,%s",MMXRegs[reg2],menemonic); break; // UNPCKHPS
                    case 0x16: wsprintf(tempMeme,"movhps %s,%s",MMXRegs[reg2],menemonic);   break; // MOVHPS
                    
                    case 0x18:
                    {
                        wsprintf(tempMeme,"%s,%s",NewSet3[REG],menemonic);
                        if(REG>3)
                            lstrcat((*Disasm)->Remarks,"Invalid instruction");
                    } 
                    break;
                    
                    case 0x28: wsprintf(tempMeme,"movaps %s,%s",MMXRegs[reg2],menemonic);   break; // MOVAPS
                    
                    case 0x2A: case 0x2C: case 0x2D:case 0x2E: case 0x2F: 
                    {
                       BYTE R=((Op&0x0F)-0x08);                       

                       if(R==4 || R==5)
                           wsprintf(temp,"%s",Regs3DNow[reg2]); // 3DNow! Regs
                       else
                           wsprintf(temp,"%s",MMXRegs[reg2]); // MMX Regs

                       if(RepPrefix==1)
                       {
                           char instruction[20];
                           if(Op==0x2A || Op==0x2C || Op==0x2D)
                           {
                               switch(Op)
                               {
                                    case 0x2A:strcpy(instruction,"cvtsi2ss"); break;
                                    case 0x2C:
                                    {
                                        wsprintf(temp,"%s",regs[RM][reg2]);
                                        strcpy(instruction,"cvttss2si"); 
                                    }
                                    break;
                                    case 0x2D:
                                    {
                                        wsprintf(temp,"%s",regs[RM][reg2]);
                                        strcpy(instruction,"cvtss2si"); 
                                    }
                                    break;
                               }
                               strcpy((*Disasm)->Assembly,"");
                               (*Disasm)->OpcodeSize++;
                               wsprintf(tempMeme,"%s %s,%s",instruction,temp,menemonic);
                           }
                      }
                      else
                          wsprintf(tempMeme,"%s %s,%s",NewSet4[(Op&0x0F)-0x08],temp,menemonic); 
                    }
                    break; // MIX

                    case 0x40:case 0x41:case 0x42:case 0x43:case 0x44:case 0x45:case 0x46:case 0x47:
                    case 0x48:case 0x49:case 0x4A:case 0x4B:case 0x4C:case 0x4D:case 0x4E:case 0x4F:
                    {
                        wsprintf(tempMeme,"%s %s,%s",NewSet5[Op&0x0F],regs[RM][reg2],menemonic); 
                    }
                    break;

                    case 0x51:case 0x52:case 0x53:case 0x54:case 0x55:case 0x56:case 0x57:
                    case 0x58:case 0x59:case 0x5C:case 0x5D:case 0x5E:case 0x5F:
                    {
                        if(RepPrefix==1)
                        {
                            wsprintf(tempMeme,"%s %s,%s",NewSet6Ex[Op&0x0F],MMXRegs[reg2],menemonic);
                            strcpy((*Disasm)->Assembly,"");
                            (*Disasm)->OpcodeSize++;
                        }
                        else
                            wsprintf(tempMeme,"%s %s,%s",NewSet6[Op&0x0F],MMXRegs[reg2],menemonic); 
                    }
                    break;

                    case 0x60:case 0x61:case 0x62:case 0x63:case 0x64:case 0x65:case 0x66:case 0x67:
                    case 0x68:case 0x69:case 0x6A:case 0x6B:case 0x6E:case 0x6F:
                    {
                        wsprintf(tempMeme,"%s %s,%s",NewSet7[Op&0x0F],Regs3DNow[reg2],menemonic); 
                    }
                    break; // MIX

                    case 0x70: // PSHUFW
                    {                       
                       wsprintf(tempMeme,"%s %s,%s,%02X",NewSet8[Op&0x0F],Regs3DNow[reg2],menemonic,FOpcode);
                       wsprintf(menemonic," %02X",FOpcode);
                       lstrcat((*Disasm)->Opcode,menemonic);
                       (*Disasm)->OpcodeSize++;
                       (*(*index))++;
                    }
                    break;

                    case 0x74:case 0x75:case 0x76:
                    {
                        wsprintf(tempMeme,"%s %s,%s",NewSet8[Op&0x0F],Regs3DNow[reg2],menemonic); 
                    }
                    break;

                    case 0x90:case 0x91:case 0x92:case 0x93:case 0x94:case 0x95:case 0x96:case 0x97:
                    case 0x98:case 0x99:case 0x9A:case 0x9B:case 0x9C:case 0x9D:case 0x9E:case 0x9F:
                    {
                        wsprintf(tempMeme,"%s %s",NewSet9[Op&0x0F],menemonic);
                    }
                    break; // MIX

                    case 0xAE:
                    {
                        wsprintf(tempMeme,"%s %s",NewSet10Ex[REG],menemonic);
                        if(REG>3)// Check for Invalid
                            lstrcat((*Disasm)->Remarks,"Invalid Instruction");
                    }
                    break;
                    case 0xAF:wsprintf(tempMeme,"%s %s,%s",NewSet10[Op&0x0F],regs[RM][reg2],menemonic);break;

                    case 0xB2:case 0xB4:case 0xB5:
                    case 0xB6:case 0xB7:case 0xBC: 
                    case 0xBD:case 0xBE:case 0xBF:
                    {
                        wsprintf(tempMeme,"%s %s,%s",NewSet11[Op&0x0F],regs[RM][reg2],menemonic);
                    }
                    break;

                    case 0xC2:
                    {
                       if(FOpcode<8) // Instructions here
                       {
                           if(RepPrefix==1) // Rep Prefix is being used
                           {
                               wsprintf(tempMeme,"%s %s,%s",NewSet12Ex[FOpcode],MMXRegs[reg2],menemonic);
                               strcpy((*Disasm)->Assembly,"");
                               (*Disasm)->OpcodeSize++;
                           }
                           else
                               wsprintf(tempMeme,"%s %s,%s",NewSet12[FOpcode],MMXRegs[reg2],menemonic);
                       }
                       else
                       {
                           if(RepPrefix==1) // Rep Prefix is being used
                           {
                               wsprintf(tempMeme,"cmpss %s,%s,%02X",MMXRegs[reg2],menemonic,FOpcode);
                               strcpy((*Disasm)->Assembly,"");
                               (*Disasm)->OpcodeSize++;
                           }
                           else 
                               wsprintf(tempMeme,"cmpps %s,%s,%02X",MMXRegs[reg2],menemonic,FOpcode);
                       }

                       wsprintf(menemonic," %02X",FOpcode);
                       lstrcat((*Disasm)->Opcode,menemonic);
                       (*Disasm)->OpcodeSize++;
                       (*(*index))++;
                    }
                    break;
                    
                    case 0xC4:
                    {
                        wsprintf(tempMeme,"pinsrw %s,%s,%02X",Regs3DNow[reg2],menemonic,FOpcode);                        
                        wsprintf(menemonic," %02X",FOpcode);
                        lstrcat((*Disasm)->Opcode,menemonic);
                        (*Disasm)->OpcodeSize++;
                        (*(*index))++;
                    }
                    break;

                    case 0xC5:
                    {
                        wsprintf(tempMeme,"pextrw %s,%s,%02X",regs[RM][reg2],menemonic,FOpcode);                        
                        wsprintf(menemonic," %02X",FOpcode);
                        lstrcat((*Disasm)->Opcode,menemonic);
                        (*Disasm)->OpcodeSize++;
                        (*(*index))++;
                    }
                    break;

                    case 0xC6:
                    {
                        wsprintf(tempMeme,"shufps %s,%s,%02X",MMXRegs[reg2],menemonic,FOpcode);
                        wsprintf(menemonic," %02X",FOpcode);
                        lstrcat((*Disasm)->Opcode,menemonic);
                        (*Disasm)->OpcodeSize++;
                        (*(*index))++;
                    }
                    break;

					case 0xC7:
					{
						wsprintf(tempMeme,"cmpxchg8b %s",menemonic);
					}
					break;

                    case 0xD7:wsprintf(tempMeme,"%s %s,%s",NewSet13[Op&0x0F],regs[RM][reg2],menemonic);break;

                    case 0xD1:case 0xD2:case 0xD3:case 0xD5:case 0xD8:case 0xDF:
                    case 0xD9:case 0xDA:case 0xDB:case 0xDC:case 0xDD:case 0xDE:                    
                    {
                        wsprintf(tempMeme,"%s %s,%s",NewSet13[Op&0x0F],Regs3DNow[reg2],menemonic);
                    }
                    break;

                    case 0xE0:case 0xE1:case 0xE2:case 0xE3:
                    case 0xE4:case 0xE5:case 0xE8:case 0xE9:
                    case 0xEA:case 0xEB:case 0xEC:case 0xED:
                    case 0xEE:case 0xEF:
                    {
                        wsprintf(tempMeme,"%s %s,%s",NewSet14[Op&0x0F],Regs3DNow[reg2],menemonic);
                    }
                    break;

                    case 0xF1:case 0xF2:case 0xF3:case 0xF5:case 0xF6:
                    case 0xF7:case 0xF8:case 0xF9:case 0xFA:case 0xFC:
                    case 0xFD:case 0xFE:
                    {
                        wsprintf(tempMeme,"%s %s,%s",NewSet15[Op&0x0F],Regs3DNow[reg2],menemonic);
                    }
                    break;

                }

                lstrcat((*Disasm)->Assembly,tempMeme);
            }
            break;
        }

        return; // safe exit
    }
    // ===================================================//
    //                 SIB is being used!                 //
    // ===================================================//
	else if(SIB==SIB_EX) // Found SIB, lets strip the extensions
	{
		/* 
		   Example menemonic for SIB: 
		   Opcodes:   000401  
		   Menemonic: add byte ptr [eax+ecx], al
		   Binary:    0000 0000 0000 0100 0000 0001
        */
		reg1=((BYTE)(*(*Opcode+pos+2))&0x38)>>3;  // Register A
		reg2=((BYTE)(*(*Opcode+pos+2))&0x07);     // Register B
		SCALE=((BYTE)(*(*Opcode+pos+2))&0xC0)>>6; // Scale size (0,2,4,8)

		// Scale look up
		switch(SCALE)
		{
			case 0:wsprintf(Aritmathic,"%s",Scale[1]);break; // +
			case 1:wsprintf(Aritmathic,"%s",Scale[2]);break; // *2+
			case 2:wsprintf(Aritmathic,"%s",Scale[3]);break; // *4+
			case 3:wsprintf(Aritmathic,"%s",Scale[4]);break; // *8+
		}

		switch(Extension) // +/+00/+00000000
		{
			case 00: // No extension of bytes
			{
                SwapWord((BYTE*)(*Opcode+pos+1),&wOp,&wMem);
                SwapDword((BYTE*)(*Opcode+pos+3),&dwOp,&dwMem);

                if(reg1==REG_ESP && reg2!=REG_EBP)
                {
                    if(reg2==REG_ESP) SEG=SEG_SS; // IF ESP is being used, User SS Segment Overridr                    
                    wsprintf(menemonic,"%02X%04X",Op,wOp);
                    lstrcat((*Disasm)->Opcode,menemonic);
                    wsprintf(menemonic,"%s ptr %s:[%s]",RSize,segs[SEG],regs[ADDRM][reg2]);
                    (*(*index))+=2; //2 byte read				
                    (*Disasm)->OpcodeSize=3; // total used opcodes
                    FOpcode=(BYTE)(*(*Opcode+pos+3));
                }
				else if(reg2!=REG_EBP) // No EBP in RegMem
				{
                    if(reg2==REG_ESP) SEG=SEG_SS; // IF ESP is being used, User SS Segment Overridr

					wsprintf(menemonic,"%02X%04X",Op,wOp);
					lstrcat((*Disasm)->Opcode,menemonic);
					wsprintf(menemonic,"%s ptr %s:[%s%s%s]",RSize,segs[SEG],regs[ADDRM][reg1],Aritmathic,regs[ADDRM][reg2]);
					(*(*index))+=2; //2 byte read				
					(*Disasm)->OpcodeSize=3; // total used opcodes
                    FOpcode=(BYTE)(*(*Opcode+pos+3));
				}
				else if(reg2==REG_EBP) // Replace EBP with Dword Number
				{
					// get 4 bytes extensions for memReg addon
					// insted of Normal Registers

					// Format Opcodes (HEX)
					wsprintf(menemonic,"%02X%04X %08X",Op,wOp,dwOp);
					lstrcat((*Disasm)->Opcode,menemonic);
					// Format menemonic

                    // Check If if ESP is being Used.
                    if(reg1==REG_ESP) // Must Not Be ESP (Index)
                    {
                        strcpy(temp,"");
                        strcpy(Aritmathic,"");
                    }
                    else
                        wsprintf(temp,"%s",regs[ADDRM][reg1]);

					wsprintf(menemonic,"%s ptr %s:[%s%s%08X]",
						RSize,      // size of regmem
						segs[SEG],  // segment
						temp,       // reg
						Aritmathic, //+,-,*2,*4,*8						
						dwMem);     // extensions

                    Extension=2; // OverRide Extension (?????), Check toDo.txt
					(*(*index))+=6; //6 byte read				
					(*Disasm)->OpcodeSize=7; // total used opcodes
                    FOpcode=(BYTE)(*(*Opcode+pos+6));
				}
			}
			break;

			case 01: // 1 byte extension
			{				
				FOpcode=(BYTE)(*(*Opcode+pos+3));
				if(FOpcode>0x7F) // check for signed numbers!!
				{
					wsprintf(tempAritmathic,"%s",Scale[0]); // '-' aritmathic
					FOpcode = 0x100-FOpcode; // -XX
				}
				
				if(reg2==REG_EBP || reg1==REG_ESP) // no ESP in [Mem]
                {                
					SEG=SEG_SS;
                    wsprintf(tempMeme,"%s ptr %s:[%s%s%02X]",RSize,segs[SEG],regs[ADDRM][reg2],tempAritmathic,FOpcode);
                }
				else
				    wsprintf(tempMeme,"%s ptr %s:[%s%s%s%s%02X]",RSize,segs[SEG],regs[ADDRM][reg1],Aritmathic,regs[ADDRM][reg2],tempAritmathic,FOpcode);

				(*(*index))+=3; // x + 3 byte(s) read				
                SwapDword((BYTE*)(*Opcode+pos),&dwOp,&dwMem);
                wsprintf(menemonic,"%08X",dwOp);
				lstrcat((*Disasm)->Opcode,menemonic);
				(*Disasm)->OpcodeSize=4; // total used opcodes
				strcpy(menemonic,tempMeme);
                FOpcode=(BYTE)(*(*Opcode+pos+4));
			}
			break;

			case 02:// Dword extension
			{
				// Menemonic decode
                SwapDword((BYTE*)(*Opcode+pos+3),&dwOp,&dwMem);
                SwapWord((BYTE*)(*Opcode+pos+3),&wOp,&wMem);
				if(reg1!=REG_ESP) 
                {
                  if(reg2==REG_EBP)
                      SEG=SEG_SS;
                                    
                  wsprintf(tempMeme,"%s ptr %s:[%s%s%s%s%08X]",
					RSize,  // size of register
					segs[SEG], // segment
					regs[ADDRM][reg1],
					Aritmathic,
					regs[ADDRM][reg2],
					tempAritmathic,
                    dwMem);	
                  
                }else{// ESP Must not be as Index, Code = 100b
                    wsprintf(tempMeme,"%s ptr %s:[%s%s%08X]",
                        RSize,  // size of register
                        segs[SEG], // segment
                        regs[ADDRM][reg2],
                        tempAritmathic,
                        dwMem);
                    
                }
				
                // Format Opcode		
				wsprintf(menemonic,"%02X%04X %08X",Op,wOp,dwOp);
				
				lstrcat((*Disasm)->Opcode,menemonic);
				(*(*index))+=6; // x + 3 byte(s) read	
				(*Disasm)->OpcodeSize=7; // total used opcodes
				strcpy(menemonic,tempMeme);
                FOpcode=(BYTE)(*(*Opcode+pos+7));
			}
			break;
		}
        switch(Bit_d)
        {            
            case 0: // direction (->)
            {
                switch(Op)// Check for all Cases Availble            
                {
                    case 0x00:
                    {
                        wsprintf(tempMeme,"%s %s",NewSet[REG],menemonic);

                        if(REG>5) // Invalid operation
                            lstrcat((*Disasm)->Remarks,"Invalid instruction");
                    }
                    break;

                    case 0x01:
                    {
                        wsprintf(tempMeme,"%s %s",NewSet2[REG],menemonic);
                        
                        if(REG==5) // Invalid operation
                            lstrcat((*Disasm)->Remarks,"Invalid instruction");
                    }
                    break;

                    case 0x11: 
                        {
                            if(RepPrefix==1)
                            {
                                wsprintf(tempMeme,"movss %s,%s",menemonic,MMXRegs[REG]);
                                strcpy((*Disasm)->Assembly,"");
                                (*Disasm)->OpcodeSize++;
                            }
                            else
                                wsprintf(tempMeme,"movups %s,%s",menemonic,MMXRegs[REG]); 
                        }
                        break; // MOVUPS
                    case 0x13: wsprintf(tempMeme,"movlps %s,%s",menemonic,MMXRegs[REG]); break; // MOVLPS
                    case 0x17: wsprintf(tempMeme,"movhps %s,%s",menemonic,MMXRegs[REG]); break; // MOVHPS
                    case 0x29: wsprintf(tempMeme,"movaps %s,%s",menemonic,MMXRegs[REG]); break; // MOVAPS
                    case 0x7E: case 0x7F:wsprintf(tempMeme,"%s %s,%s",NewSet7[Op&0x0F],menemonic,Regs3DNow[REG]); break; // MOVD/MOVQ
                    case 0xA3: case 0xAB: 
                    {
                      wsprintf(tempMeme,"%s %s,%s",NewSet10[Op&0x0F],menemonic,regs[RM][REG]); 
                    }
                    break;
                    case 0xA4:case 0xAC:
                    {
                      wsprintf(tempMeme,"%s %s,%s,%02X",NewSet10[Op&0x0F],menemonic,regs[RM][REG],FOpcode);
                      wsprintf(menemonic," %02X",FOpcode);
                      lstrcat((*Disasm)->Opcode,menemonic);
                      (*Disasm)->OpcodeSize++;
                      (*(*index))++;
                    }
                    break;
                    case 0xA5: case 0xAD:
                    {
                      wsprintf(tempMeme,"%s %s,%s,cl",NewSet10[Op&0x0F],menemonic,regs[RM][REG]); 
                    }
                    break;

                    case 0xB0: case 0xB1: case 0xB3: case 0xBB:
                    {
                      if((Op&0x0F)==0x00)
                         RM=REG8;
                            
                      wsprintf(tempMeme,"%s %s,%s",NewSet11[Op&0x0F],menemonic,regs[RM][REG]);
                    }
                    break;
                    case 0xC0: { RM=REG8; wsprintf(tempMeme,"xadd %s,%s",menemonic,regs[RM][REG]); } break; // XADD
                    case 0xC1: wsprintf(tempMeme,"xadd %s,%s",menemonic,regs[RM][REG]);              break; // XADD  
                    case 0xE7: wsprintf(tempMeme,"%s %s,%s",NewSet14[Op&0x07],menemonic,Regs3DNow[REG]); break;
                }
                
                lstrcat((*Disasm)->Assembly,tempMeme);
            }
            break;
            
            case 1: // direction (<-)
            {
                switch(Op) // Decode Instructions
                {
                    case 0x02: wsprintf(tempMeme,"lar %s,%s",regs[RM][REG],menemonic);     break; // LAR
                    case 0x03: wsprintf(tempMeme,"lsl %s,%s",regs[RM][REG],menemonic);     break; // LSL
                    case 0x10: 
                        {
                            if(RepPrefix==1) // rep prefix is being used
                            {
                                strcpy((*Disasm)->Assembly,"");
                                (*Disasm)->OpcodeSize++;
                                wsprintf(tempMeme,"movss %s,%s",MMXRegs[REG],menemonic);
                            }
                            else
                                wsprintf(tempMeme,"movups %s,%s",MMXRegs[REG],menemonic);  
                        }
                        break; // MOVUPS
                    case 0x12: wsprintf(tempMeme,"movlps %s,%s",MMXRegs[REG],menemonic);   break; // MOVLPS
                    case 0x14: wsprintf(tempMeme,"unpcklps %s,%s",MMXRegs[REG],menemonic); break; // UNPCKLPS
                    case 0x15: wsprintf(tempMeme,"unpckhps %s,%s",MMXRegs[REG],menemonic); break; // UNPCKHPS
                    case 0x16: wsprintf(tempMeme,"movhps %s,%s",MMXRegs[REG],menemonic);   break; // MOVHPS
                    case 0x18:
                    {
                      wsprintf(tempMeme,"%s,%s",NewSet3[REG],menemonic);
                      if(REG>3)// Invalid Instructions
                          lstrcat((*Disasm)->Remarks,"Invalid instruction");
                    } 
                    break;
                    case 0x28: wsprintf(tempMeme,"movaps %s,%s",MMXRegs[REG],menemonic);   break; // MOVAPS
                    
                    case 0x2A: case 0x2C: case 0x2D:case 0x2E: case 0x2F: 
                    {
                       BYTE R=((Op&0x0F)-0x08);
                       if(R==4 || R==5)
                            wsprintf(temp,"%s",Regs3DNow[REG]); // 3DNow! Regs
                       else
                            wsprintf(temp,"%s",MMXRegs[REG]); // MMX Regs
                        
                       if(RepPrefix==1)
                       {
                           char instruction[20];
                           if(Op==0x2A || Op==0x2C || Op==0x2D)
                           {
                               switch(Op)
                               {
                                   case 0x2A:strcpy(instruction,"cvtsi2ss"); break;
                                   case 0x2C:
                                   {
                                       wsprintf(temp,"%s",regs[RM][REG]);
                                       strcpy(instruction,"cvttss2si"); 
                                   }
                                   break;
                                   case 0x2D:
                                   {
                                       wsprintf(temp,"%s",regs[RM][REG]);
                                       strcpy(instruction,"cvtss2si"); 
                                   }
                                   break;
                               }
                               strcpy((*Disasm)->Assembly,"");
                               (*Disasm)->OpcodeSize++;
                               wsprintf(tempMeme,"%s %s,%s",instruction,temp,menemonic);
                           }
                      }
                      else
                           wsprintf(tempMeme,"%s %s,%s",NewSet4[(Op&0x0F)-0x08],temp,menemonic); 
                    }
                    break; // MIX
                    case 0x40:case 0x41:case 0x42:case 0x43:case 0x44:case 0x45:case 0x46:case 0x47:
                    case 0x48:case 0x49:case 0x4A:case 0x4B:case 0x4C:case 0x4D:case 0x4E:case 0x4F:
                    {
                      wsprintf(tempMeme,"%s %s,%s",NewSet5[Op&0x0F],regs[RM][REG],menemonic); 
                    }
                    break;

                    case 0x51:case 0x52:case 0x53:case 0x54:case 0x55:case 0x56:case 0x57:
                    case 0x58:case 0x59:case 0x5C:case 0x5D:case 0x5E:case 0x5F:
                    {
                        if(RepPrefix==1)
                        {
                            wsprintf(tempMeme,"%s %s,%s",NewSet6Ex[Op&0x0F],MMXRegs[reg2],menemonic);
                            strcpy((*Disasm)->Assembly,"");
                            (*Disasm)->OpcodeSize++;
                        }
                        else
                            wsprintf(tempMeme,"%s %s,%s",NewSet6[Op&0x0F],MMXRegs[REG],menemonic); 
                    }
                    break;
                    
                    case 0x60:case 0x61:case 0x62:case 0x63:case 0x64:case 0x65:case 0x66:case 0x67:
                    case 0x68:case 0x69:case 0x6A:case 0x6B:case 0x6E:case 0x6F:
                    {
                      wsprintf(tempMeme,"%s %s,%s",NewSet7[Op&0x0F],Regs3DNow[REG],menemonic); 
                    }
                    break;
                    
                    case 0x70:
                    {
                      wsprintf(tempMeme,"%s %s,%s,%02X",NewSet8[Op&0x0F],Regs3DNow[REG],menemonic,FOpcode);
                      wsprintf(menemonic," %02X",FOpcode);
                      lstrcat((*Disasm)->Opcode,menemonic);
                      (*Disasm)->OpcodeSize++;
                      (*(*index))++;
                    }
                    break;

                    case 0x74:case 0x75:case 0x76: // MIX
                    {
                       wsprintf(tempMeme,"%s %s,%s",NewSet8[Op&0x0F],Regs3DNow[REG],menemonic); 
                    }
                    break;

                    case 0x90:case 0x91:case 0x92:case 0x93:case 0x94:case 0x95:case 0x96:case 0x97:
                    case 0x98:case 0x99:case 0x9A:case 0x9B:case 0x9C:case 0x9D:case 0x9E:case 0x9F:
                    {
                      wsprintf(tempMeme,"%s %s",NewSet9[Op&0x0F],menemonic);
                    }
                    break; // MIX

                    case 0xAE:
                    {
                        wsprintf(tempMeme,"%s %s",NewSet10Ex[REG],menemonic);
                        if(REG>3)// Check for Invalid
                            lstrcat((*Disasm)->Remarks,"Invalid Instruction");
                    }
                    break;
                    case 0xAF:wsprintf(tempMeme,"%s %s,%s",NewSet10[Op&0x0F],regs[RM][REG],menemonic);break;

                    case 0xB2:case 0xB4:case 0xB5:
                    case 0xB6:case 0xB7:case 0xBC: 
                    case 0xBD:case 0xBE:case 0xBF:
                    {
                     wsprintf(tempMeme,"%s %s,%s",NewSet11[Op&0x0F],regs[RM][REG],menemonic);
                    }
                    break;

                    case 0xC2:
                    {
                      if(FOpcode<8) // Instructions here
                      {
                          if(RepPrefix==1) // Rep Prefix is being used
                          {
                              wsprintf(tempMeme,"%s %s,%s",NewSet12Ex[FOpcode],MMXRegs[REG],menemonic);
                              strcpy((*Disasm)->Assembly,"");
                              (*Disasm)->OpcodeSize++;
                          }
                          else
                              wsprintf(tempMeme,"%s %s,%s",NewSet12[FOpcode],MMXRegs[REG],menemonic);
                      }
                      else
                      {
                          if(RepPrefix==1) // Rep Prefix is being used
                          {
                              wsprintf(tempMeme,"cmpss %s,%s,%02X",MMXRegs[REG],menemonic,FOpcode);
                              strcpy((*Disasm)->Assembly,"");
                              (*Disasm)->OpcodeSize++;
                          }
                          else
                              wsprintf(tempMeme,"cmpps %s,%s,%02X",MMXRegs[REG],menemonic,FOpcode);
                      }     
                      wsprintf(menemonic," %02X",FOpcode);
                      lstrcat((*Disasm)->Opcode,menemonic);
                      (*Disasm)->OpcodeSize++;
                      (*(*index))++;
                    }
                    break;

                    case 0xC4:
                    {
                        wsprintf(tempMeme,"pinsrw %s,%s,%02X",Regs3DNow[REG],menemonic,FOpcode);                        
                        wsprintf(menemonic," %02X",FOpcode);
                        lstrcat((*Disasm)->Opcode,menemonic);
                        (*Disasm)->OpcodeSize++;
                        (*(*index))++;
                    }
                    break;

                    case 0xC5:
                    {
                        wsprintf(tempMeme,"pextrw %s,%s,%02X",regs[RM][REG],menemonic,FOpcode);                        
                        wsprintf(menemonic," %02X",FOpcode);
                        lstrcat((*Disasm)->Opcode,menemonic);
                        (*Disasm)->OpcodeSize++;
                        (*(*index))++;
                    }
                    break;

                    case 0xC6:
                    {
                        wsprintf(tempMeme,"shufps %s,%s,%02X",MMXRegs[REG],menemonic,FOpcode);                        
                        wsprintf(menemonic," %02X",FOpcode);
                        lstrcat((*Disasm)->Opcode,menemonic);
                        (*Disasm)->OpcodeSize++;
                        (*(*index))++;
                    }
                    break;

                    case 0xD7:wsprintf(tempMeme,"%s %s,%s",NewSet13[Op&0x0F],regs[RM][REG],menemonic); break;
                    
                    case 0xD1:case 0xD2:case 0xD3:case 0xD5:case 0xD8:case 0xDF:
                    case 0xD9:case 0xDA:case 0xDB:case 0xDC:case 0xDD:case 0xDE:                    
                    {
                        wsprintf(tempMeme,"%s %s,%s",NewSet13[Op&0x0F],Regs3DNow[REG],menemonic);
                    }
                    break;

                    case 0xE0:case 0xE1:case 0xE2:case 0xE3:
                    case 0xE4:case 0xE5:case 0xE8:case 0xE9:
                    case 0xEA:case 0xEB:case 0xEC:case 0xED:
                    case 0xEE:case 0xEF:
                    {
                      wsprintf(tempMeme,"%s %s,%s",NewSet14[Op&0x0F],Regs3DNow[REG],menemonic);
                    }
                    break;

                    case 0xF1:case 0xF2:case 0xF3:case 0xF5:case 0xF6:
                    case 0xF7:case 0xF8:case 0xF9:case 0xFA:case 0xFC:
                    case 0xFD:case 0xFE:
                    {
                      wsprintf(tempMeme,"%s %s,%s",NewSet15[Op&0x0F],Regs3DNow[REG],menemonic);
                    }
                    break;
                }

                lstrcat((*Disasm)->Assembly,tempMeme);
            }
            break;
        }
    }
}

void CPayLoadDlg::Mod_11_RM_EX(BYTE d, BYTE w, char **Opcode, DISASSEMBLY **Disasm, bool PrefixReg, BYTE Op, DWORD **index, bool RepPrefix)
{
	/* 
       Function Mod_11_RM Checks whatever we have
	   Both bit d (direction) and bit w (full/partial size).
	 
       There are 4 states:
	   00 - d=0 / w=0 ; direction -> (ie: DH->DL),   partial size (AL,DH,BL..)
	   01 - d=0 / w=1 ; direction -> (ie: EDX->EAX), partial size (EAX,EBP,EDI..)
	   10 - d=1 / w=0 ; direction <- (ie: DH<-DL),   partial size (AL,DH,BL..)
	   11 - d=1 / w=1 ; direction <- (ie: EDX<-EAX), partial size (EAX,EBP,EDI..)
	
       Also deals with harder opcodes which have diffrent
       Addresing type.
    */
    
	int RM,IndexAdd=1,m_OpcodeSize=2,Pos; // Register(s) Pointer
	WORD wOp,wMem;
    BYTE reg1=0,reg2=0,m_Opcode=0,REG;
	char assembly[50]="",temp[128]="",m_Bytes[128]="";
    
    Pos=(*(*index)); // Current Position
    
    m_Opcode = (BYTE)(*(*Opcode+Pos+1));// Decode registers from second byte
    
    // Strip Used Instructions / Used Segment
    REG=m_Opcode; 
    REG>>=3;
	REG&=0x07;

    // (->) / reg8
    if(d==0 && w==0)
    {    
        RM=REG8;
        reg1=(m_Opcode&0x07);
        reg2=(m_Opcode&0x38)>>3;
    }
    
    // (->) / reg32
    if(d==0 && w==1)
    {    
        RM=REG32;
        if(PrefixReg==1)
            RM=REG16; // (->) / reg16 (RegPerfix is being used)
        
        reg1=(m_Opcode&0x07);
        reg2=(m_Opcode&0x38)>>3;
    }
    
    // (<-) / reg8
    if(d==1 && w==0)
    {    
        RM=REG8;
        reg2=(m_Opcode&0x07);
        reg1=(m_Opcode&0x38)>>3;
    }
    
    // (<-) / reg32
    if(d==1 && w==1)
    {    
        RM=REG32;
        if(PrefixReg==1)
            RM=REG16; // (<-) / reg16
        
        reg2=(m_Opcode&0x07);
        reg1=(m_Opcode&0x38)>>3;
    }

    switch(Op)
    {
        case 0x00: 
        {
            RM=REG16; // FORCE 16BIT
            SwapWord((BYTE*)(*Opcode+Pos),&wOp,&wMem);
            wsprintf(assembly,"%s %s",NewSet[REG],regs[RM][reg2]);
            wsprintf(temp,"%04X",wOp);
            if(REG>5)
                lstrcat((*Disasm)->Remarks,"Invalid Instruction");
        }
        break;

        case 0x01: 
        {
            RM=REG32; // DEFAULT 32Bit

            if(REG>=4 && REG<=6) // USES 32bit
                RM=REG16;

            if(REG==7) // USES 8BIT
                RM=REG8;

            SwapWord((BYTE*)(*Opcode+Pos),&wOp,&wMem);
            wsprintf(assembly,"%s %s",NewSet2[REG],regs[RM][reg2]);
            wsprintf(temp,"%04X",wOp);
            if(REG==5)
                lstrcat((*Disasm)->Remarks,"Invalid Instruction");
        }
        break;

        case 0x02: // LAR
        {
            wsprintf(assembly,"lar %s, %s",regs[RM][reg1],regs[RM][reg2]);
            SwapWord((BYTE*)(*Opcode+Pos),&wOp,&wMem);
            wsprintf(temp,"%04X",wOp);
        }
        break;

        case 0x03: // LSL
        {
          wsprintf(assembly,"lsl %s, %s",regs[RM][reg1],regs[RM][reg2]);
          SwapWord((BYTE*)(*Opcode+Pos),&wOp,&wMem);
          wsprintf(temp,"%04X",wOp);
        }
        break;

        case 0x10: // MOVUPS
        {
           if(RepPrefix)
           {
               wsprintf(assembly,"movss %s, %s",MMXRegs[reg1],MMXRegs[reg2]);
               strcpy((*Disasm)->Assembly,"");
               m_OpcodeSize++;
           }
           else
               wsprintf(assembly,"movups %s, %s",MMXRegs[reg1],MMXRegs[reg2]);
           
           SwapWord((BYTE*)(*Opcode+Pos),&wOp,&wMem);
           wsprintf(temp,"%04X",wOp);
        }
        break;

        case 0x11: // MOVUPS
        {
           if(RepPrefix)
           {
                wsprintf(assembly,"movss %s, %s",MMXRegs[reg2],MMXRegs[reg1]);
                strcpy((*Disasm)->Assembly,"");
                m_OpcodeSize++;
           }
           else
               wsprintf(assembly,"movups %s, %s",MMXRegs[reg2],MMXRegs[reg1]);

           SwapWord((BYTE*)(*Opcode+Pos),&wOp,&wMem);
           wsprintf(temp,"%04X",wOp);
        }
        break;

        case 0x12: // MOVHLPS
        {
            wsprintf(assembly,"movhlps %s, %s",MMXRegs[reg1],MMXRegs[reg2]);
            SwapWord((BYTE*)(*Opcode+Pos),&wOp,&wMem);
            wsprintf(temp,"%04X",wOp);
        }
        break;

        case 0x13: // MOVLPS
        {
            wsprintf(assembly,"movlps %s, %s",MMXRegs[reg2],MMXRegs[reg1]);
            SwapWord((BYTE*)(*Opcode+Pos),&wOp,&wMem);
            wsprintf(temp,"%04X",wOp);
        }
        break;

        case 0x14: // UNPCKLPS
        {
           wsprintf(assembly,"unpcklps %s, %s",MMXRegs[reg1],MMXRegs[reg2]);
           SwapWord((BYTE*)(*Opcode+Pos),&wOp,&wMem);
           wsprintf(temp,"%04X",wOp);
        }
        break;

        case 0x15: // UNPCKHPS
        {
          wsprintf(assembly,"unpckhps %s, %s",MMXRegs[reg1],MMXRegs[reg2]);
          SwapWord((BYTE*)(*Opcode+Pos),&wOp,&wMem);
          wsprintf(temp,"%04X",wOp);
        }
        break;

        case 0x16: // MOVLHPS
        {
          wsprintf(assembly,"movhlhps %s, %s",MMXRegs[reg1],MMXRegs[reg2]);
          SwapWord((BYTE*)(*Opcode+Pos),&wOp,&wMem);
          wsprintf(temp,"%04X",wOp);
        }
        break;

        case 0x17: // MOVHPS
        {
           wsprintf(assembly,"movhps %s, %s",MMXRegs[reg2],MMXRegs[reg1]);
           SwapWord((BYTE*)(*Opcode+Pos),&wOp,&wMem);
           wsprintf(temp,"%04X",wOp);
        }
        break;

        case 0x18:
        {
           wsprintf(assembly,"%s, %s",NewSet3[REG],regs[RM][reg2]);
           if(REG>3)
               lstrcat((*Disasm)->Remarks,"Invalid Instruction");

           SwapWord((BYTE*)(*Opcode+Pos),&wOp,&wMem);
           wsprintf(temp,"%04X",wOp);
        }
        break;

        case 0x28: // MOVAPS
        {
           wsprintf(assembly,"movaps %s, %s",MMXRegs[reg1],MMXRegs[reg2]);
           SwapWord((BYTE*)(*Opcode+Pos),&wOp,&wMem);
           wsprintf(temp,"%04X",wOp);
        }
        break;

        case 0x29: // MOVAPS
        {
          wsprintf(assembly,"movaps %s, %s",MMXRegs[reg2],MMXRegs[reg1]);
          SwapWord((BYTE*)(*Opcode+Pos),&wOp,&wMem);
          wsprintf(temp,"%04X",wOp);
        }
        break;

        case 0x2A: // CVTPI2PS
        {
          if(RepPrefix==1)
          {
              wsprintf(assembly,"cvtsi2ss %s, %s",MMXRegs[reg1],regs[RM][reg2]);
              strcpy((*Disasm)->Assembly,"");
              m_OpcodeSize++;
          }
          else
              wsprintf(assembly,"%s %s, %s",NewSet4[(Op&0x0F)-0x08],MMXRegs[reg1],Regs3DNow[reg2]);
          
          SwapWord((BYTE*)(*Opcode+Pos),&wOp,&wMem);
          wsprintf(temp,"%04X",wOp);
        }
        break; 

        case 0x2C: case 0x2D: // CVTTPS2PI, CVTPS2PI
        {
            if(RepPrefix==1)
            {
                if(Op==0x2C)
                    strcpy(temp,"cvttss2si");
                else
                    strcpy(temp,"cvtss2si");

                wsprintf(assembly,"%s %s, %s",temp,regs[RM][reg1],MMXRegs[reg2]);
                strcpy((*Disasm)->Assembly,"");
                m_OpcodeSize++;
            }
            else
                wsprintf(assembly,"%s %s, %s",NewSet4[(Op&0x0F)-0x08],Regs3DNow[reg1],MMXRegs[reg2]);

            SwapWord((BYTE*)(*Opcode+Pos),&wOp,&wMem);
            wsprintf(temp,"%04X",wOp);
        }
        break;

        case 0x2E: case 0x2F: // UCOMISS, COMISS
        {
            wsprintf(assembly,"%s %s, %s",NewSet4[(Op&0x0F)-0x08],MMXRegs[reg1],MMXRegs[reg2]);
            SwapWord((BYTE*)(*Opcode+Pos),&wOp,&wMem);
            wsprintf(temp,"%04X",wOp);
        }
        break;

        case 0x40:case 0x41:case 0x42:case 0x43:case 0x44:case 0x45:case 0x46:case 0x47:
        case 0x48:case 0x49:case 0x4A:case 0x4B:case 0x4C:case 0x4D:case 0x4E:case 0x4F:
        {
          wsprintf(assembly,"%s %s,%s",NewSet5[Op&0x0F],regs[RM][reg1],regs[RM][reg2]); 
          SwapWord((BYTE*)(*Opcode+Pos),&wOp,&wMem);
          wsprintf(temp,"%04X",wOp);
        }
        break;

        case 0x51:case 0x52:case 0x53:case 0x54:case 0x55:case 0x56:case 0x57:
        case 0x58:case 0x59:case 0x5C:case 0x5D:case 0x5E:case 0x5F:
        {
            if(RepPrefix==1) // Rep Prefix is being used
            {
                wsprintf(assembly,"%s %s,%s",NewSet6Ex[Op&0x0F],MMXRegs[reg1],MMXRegs[reg2]);
                strcpy((*Disasm)->Assembly,"");
                m_OpcodeSize++;
            }
            else
                wsprintf(assembly,"%s %s,%s",NewSet6[Op&0x0F],MMXRegs[reg1],MMXRegs[reg2]); 
            
            SwapWord((BYTE*)(*Opcode+Pos),&wOp,&wMem);
            wsprintf(temp,"%04X",wOp);
        }
        break; // MIX
        
        case 0x60:case 0x61:case 0x62:case 0x63:case 0x64:case 0x65:case 0x66:case 0x67:
        case 0x68:case 0x69:case 0x6A:case 0x6B:case 0x6E:case 0x6F:
        {
          wsprintf(assembly,"%s %s,%s",NewSet7[Op&0x0F],Regs3DNow[reg1],Regs3DNow[reg2]); 
          SwapWord((BYTE*)(*Opcode+Pos),&wOp,&wMem);
          wsprintf(temp,"%04X",wOp);
        }
        break;

        case 0x70:
        {
            wsprintf(assembly,"%s %s,%s,%02X",NewSet8[Op&0x0F],Regs3DNow[reg1],Regs3DNow[reg2],(BYTE)(*(*Opcode+Pos+2))); 
            SwapWord((BYTE*)(*Opcode+Pos),&wOp,&wMem);
            wsprintf(temp,"%04X %02X",wOp,(BYTE)(*(*Opcode+Pos+2)));
            (*(*index))++;
            m_OpcodeSize++;
        }
        break;

        case 0x74:case 0x75:case 0x76: // MIX
        {
          wsprintf(assembly,"%s %s,%s",NewSet8[Op&0x0F],Regs3DNow[reg1],Regs3DNow[reg2]); 
          SwapWord((BYTE*)(*Opcode+Pos),&wOp,&wMem);
          wsprintf(temp,"%04X",wOp);
        }
        break;

        case 0x7E:
        {
          wsprintf(assembly,"%s %s,%s",NewSet7[Op&0x0F],regs[RM][reg2],Regs3DNow[reg1]); 
          SwapWord((BYTE*)(*Opcode+Pos),&wOp,&wMem);
          wsprintf(temp,"%04X",wOp);
        }
        break;

        case 0x7F:
        {
          wsprintf(assembly,"%s %s,%s",NewSet7[Op&0x0F],Regs3DNow[reg2],Regs3DNow[reg1]); 
          SwapWord((BYTE*)(*Opcode+Pos),&wOp,&wMem);
          wsprintf(temp,"%04X",wOp);
        }
        break;

        case 0x90:case 0x91:case 0x92:case 0x93:case 0x94:case 0x95:case 0x96:case 0x97:
        case 0x98:case 0x99:case 0x9A:case 0x9B:case 0x9C:case 0x9D:case 0x9E:case 0x9F:
        {
          RM=REG8; // FORCE 8BIT
          wsprintf(assembly,"%s %s",NewSet9[Op&0x0F],regs[RM][reg2]);
          SwapWord((BYTE*)(*Opcode+Pos),&wOp,&wMem);
          wsprintf(temp,"%04X",wOp);
        }
        break; // MIX

        case 0xA3: case 0xAB:
        {
          wsprintf(assembly,"%s %s,%s",NewSet10[Op&0x0F],regs[RM][reg2],regs[RM][reg1]); 
          SwapWord((BYTE*)(*Opcode+Pos),&wOp,&wMem);
          wsprintf(temp,"%04X",wOp);
        }
        break;

        case 0xA4: case 0xAC:
        {
            wsprintf(assembly,"%s %s,%s,%02X",NewSet10[Op&0x0F],regs[RM][reg2],regs[RM][reg1],(BYTE)(*(*Opcode+Pos+2))); 
            SwapWord((BYTE*)(*Opcode+Pos),&wOp,&wMem);
            wsprintf(temp,"%04X %02X",wOp,(BYTE)(*(*Opcode+Pos+2)));
            (*(*index))++;
            m_OpcodeSize++;
        }
        break;

        case 0xA5: case 0xAD:
        {
          wsprintf(assembly,"%s %s,%s,cl",NewSet10[Op&0x0F],regs[RM][reg2],regs[RM][reg1]); 
          SwapWord((BYTE*)(*Opcode+Pos),&wOp,&wMem);
          wsprintf(temp,"%04X",wOp);
        }
        break;

        case 0xAE:
        {            
            wsprintf(temp,"%02X%02X",(BYTE)(*(*Opcode+Pos)),(BYTE)(*(*Opcode+Pos+1))); 

            if(REG>3)// Check for Invalid
            {
                m_Opcode=(BYTE)(*(*Opcode+Pos+1));

                switch(m_Opcode) // Lone Instructions
                {
                    case 0xE8: strcpy(assembly,"lfence"); break;
                    case 0xF0: strcpy(assembly,"mfence"); break;
                    case 0xF8: strcpy(assembly,"sfence"); break;
                    default: lstrcat((*Disasm)->Remarks,"Invalid Instruction"); break;
                }                
            }
            else
                wsprintf(assembly,"%s %s",NewSet10Ex[REG],regs[RM][reg2]);
        }
        break;

        case 0xAF:
        {
          wsprintf(assembly,"%s %s,%s",NewSet10[Op&0x0F],regs[RM][reg1],regs[RM][reg2]); 
          SwapWord((BYTE*)(*Opcode+Pos),&wOp,&wMem);
          wsprintf(temp,"%04X",wOp);
        }
        break;

        case 0xB0: case 0xB1: case 0xB3: case 0xBB:
        {
          if((Op&0x0F)==0)
              RM=REG8;

          wsprintf(assembly,"%s %s,%s",NewSet11[Op&0x0F],regs[RM][reg2],regs[RM][reg1]); 
          SwapWord((BYTE*)(*Opcode+Pos),&wOp,&wMem);
          wsprintf(temp,"%04X",wOp);
        }
        break;

        case 0xB2:case 0xB4:case 0xB5:
        case 0xB6:case 0xB7:case 0xBC: 
        case 0xBD:case 0xBE:case 0xBF:
        {
          BYTE reg=Op&0x0F;
          int RM2=REG32; // default
          
          if(reg==0x06 || reg==0x0E) 
              RM2=REG8;
          
          if(reg==0x07 || reg==0x0F)
              RM2=REG16;
          
          wsprintf(assembly,"%s %s,%s",NewSet11[Op&0x0F],regs[RM][reg1],regs[RM2][reg2]); 
          SwapWord((BYTE*)(*Opcode+Pos),&wOp,&wMem);
          wsprintf(temp,"%04X",wOp);
        }
        break;

        case 0xC0: case 0xC1:
        {
          if(Op==0xC0)
              RM=REG8;

          wsprintf(assembly,"xadd %s,%s",regs[RM][reg2],regs[RM][reg1]); 
          SwapWord((BYTE*)(*Opcode+Pos),&wOp,&wMem);
          wsprintf(temp,"%04X",wOp);
        }
        break;

        case 0xC2:
        {
            if((BYTE)(*(*Opcode+Pos+2))<8) // Instructions here
            {
                if(RepPrefix==1)
                {
                    wsprintf(assembly,"%s %s,%s",NewSet12Ex[(BYTE)(*(*Opcode+Pos+2))],MMXRegs[reg1],MMXRegs[reg2]);
                    strcpy((*Disasm)->Assembly,"");
                    m_OpcodeSize++;
                }
                else
                    wsprintf(assembly,"%s %s,%s",NewSet12[(BYTE)(*(*Opcode+Pos+2))],MMXRegs[reg1],MMXRegs[reg2]);
            }
            else
            {
                if(RepPrefix==1)
                {
                    wsprintf(assembly,"cmpss %s,%s,%02X",MMXRegs[reg1],MMXRegs[reg2],(BYTE)(*(*Opcode+Pos+2)));
                    strcpy((*Disasm)->Assembly,"");
                    m_OpcodeSize++;
                }
                else
                    wsprintf(assembly,"cmpps %s,%s,%02X",MMXRegs[reg1],MMXRegs[reg2],(BYTE)(*(*Opcode+Pos+2)));
            }
            
            SwapWord((BYTE*)(*Opcode+Pos),&wOp,&wMem);
            wsprintf(temp,"%04X %02X",wOp,(BYTE)(*(*Opcode+Pos+2)));
            (*(*index))++;
            m_OpcodeSize++;
        }
        break;

        case 0xC4:
        {
            RM=REG16;
            wsprintf(assembly,"pinsrw %s,%s,%02X",Regs3DNow[reg1],regs[RM][reg2],(BYTE)(*(*Opcode+Pos+2)));
            SwapWord((BYTE*)(*Opcode+Pos),&wOp,&wMem);
            wsprintf(temp,"%04X %02X",wOp,(BYTE)(*(*Opcode+Pos+2)));
            (*(*index))++;
            m_OpcodeSize++;
        }
        break;

        case 0xC5:
        {
            wsprintf(assembly,"pextrw %s,%s,%02X",regs[RM][reg1],Regs3DNow[reg2],(BYTE)(*(*Opcode+Pos+2)));
            SwapWord((BYTE*)(*Opcode+Pos),&wOp,&wMem);
            wsprintf(temp,"%04X %02X",wOp,(BYTE)(*(*Opcode+Pos+2)));
            (*(*index))++;
            m_OpcodeSize++;
        }
        break;

        case 0xC6:
        {
            wsprintf(assembly,"pextrw %s,%s,%02X",MMXRegs[reg1],MMXRegs[reg2],(BYTE)(*(*Opcode+Pos+2)));
            SwapWord((BYTE*)(*Opcode+Pos),&wOp,&wMem);
            wsprintf(temp,"%04X %02X",wOp,(BYTE)(*(*Opcode+Pos+2)));
            (*(*index))++;
            m_OpcodeSize++;
        }
        break;

        case 0xD7: 
        {
            wsprintf(assembly,"%s %s,%s",NewSet13[Op&0x0F],regs[RM][reg1],Regs3DNow[reg2]); 
            SwapWord((BYTE*)(*Opcode+Pos),&wOp,&wMem);
            wsprintf(temp,"%04X",wOp);
        }
        break;
        case 0xD1:case 0xD2:case 0xD3:case 0xD5:case 0xD8:case 0xDF:
        case 0xD9:case 0xDA:case 0xDB:case 0xDC:case 0xDD:case 0xDE:                    
        {
            wsprintf(assembly,"%s %s,%s",NewSet13[Op&0x0F],Regs3DNow[reg1],Regs3DNow[reg2]);
            SwapWord((BYTE*)(*Opcode+Pos),&wOp,&wMem);
            wsprintf(temp,"%04X",wOp);
        }
        break;

        case 0xE0:case 0xE1:case 0xE2:case 0xE3:
        case 0xE4:case 0xE5:case 0xE8:case 0xE9:
        case 0xEA:case 0xEB:case 0xEC:case 0xED:
        case 0xEE:case 0xEF:
        {
            wsprintf(assembly,"%s %s,%s",NewSet14[Op&0x0F],Regs3DNow[reg1],Regs3DNow[reg2]);
            SwapWord((BYTE*)(*Opcode+Pos),&wOp,&wMem);
            wsprintf(temp,"%04X",wOp);
        }
        break;

        case 0xE7:
        {
            wsprintf(assembly,"%s %s,%s",NewSet14[Op&0x0F],regs[RM][reg2],Regs3DNow[reg1]);
            SwapWord((BYTE*)(*Opcode+Pos),&wOp,&wMem);
            wsprintf(temp,"%04X",wOp);
        }
        break;

        case 0xF1:case 0xF2:case 0xF3:case 0xF5:case 0xF6:
        case 0xF7:case 0xF8:case 0xF9:case 0xFA:case 0xFC:
        case 0xFD:case 0xFE:
        {
            wsprintf(assembly,"%s %s,%s",NewSet14[Op&0x0F],Regs3DNow[reg1],Regs3DNow[reg2]);
            SwapWord((BYTE*)(*Opcode+Pos),&wOp,&wMem);
            wsprintf(temp,"%04X",wOp);
        }
        break;

        case 0x0D:
        {
            BYTE NextByte = (BYTE)(*(*Opcode+Pos+1));
            
            if((NextByte&0x0F)<=7)
                strcpy(temp,"prefetch");
            else
                strcpy(temp,"prefetchw");

            if((NextByte)<=0xCF)
                wsprintf(assembly,"%s %s",temp,regs[RM][reg2]);
            else
            {
                strcpy(assembly,"???");
                lstrcat((*Disasm)->Remarks,"Invalid Instruction");
            }

            SwapWord((BYTE*)(*Opcode+Pos),&wOp,&wMem);
            wsprintf(temp,"%04X",wOp);
        }
        break;

        case 0x20:
        {
            wsprintf(assembly,"mov %s,%s",regs[RM][reg2],ControlRegs[reg1]);
            SwapWord((BYTE*)(*Opcode+Pos),&wOp,&wMem);
            wsprintf(temp,"%04X",wOp);
        }
        break;

        case 0x21:
        {
          wsprintf(assembly,"mov %s,%s",regs[RM][reg2],DebugRegs[reg1]);
          SwapWord((BYTE*)(*Opcode+Pos),&wOp,&wMem);
          wsprintf(temp,"%04X",wOp);
        }
        break;

        case 0x22:
        {
          wsprintf(assembly,"mov %s,%s",ControlRegs[reg1],regs[RM][reg2]);
          SwapWord((BYTE*)(*Opcode+Pos),&wOp,&wMem);
          wsprintf(temp,"%04X",wOp);
        }
        break;

        case 0x23:
        {
          wsprintf(assembly,"mov %s,%s",DebugRegs[reg1],regs[RM][reg2]);
          SwapWord((BYTE*)(*Opcode+Pos),&wOp,&wMem);
          wsprintf(temp,"%04X",wOp);
        }
        break;

        case 0x50:
        {
            wsprintf(assembly,"movmskps %s,%s",regs[RM][reg1],MMXRegs[reg2]);
            SwapWord((BYTE*)(*Opcode+Pos),&wOp,&wMem);
            wsprintf(temp,"%04X",wOp);
        }
        break;

        case 0x71:
        {
            BYTE NextByte = (BYTE)(*(*Opcode+Pos+1));

            if(
                (NextByte>=0xD0 && NextByte<=0xD7) ||
                (NextByte>=0xE0 && NextByte<=0xE7) ||
                (NextByte>=0xF0 && NextByte<=0xF7) 
              )
            {            
                if(NextByte>=0xD0 && NextByte<=0xD7)
                   strcpy(temp,"psrlw");
                else if (NextByte>=0xE0 && NextByte<=0xE7)
                        strcpy(temp,"psraw");
                     else
                        strcpy(temp,"psllw");
            
                wsprintf(assembly,"%s %s,%02X",temp,Regs3DNow[reg2],(BYTE)(*(*Opcode+Pos+2)));
            }
            else
            {
                strcpy(assembly,"???");
                lstrcat((*Disasm)->Remarks,"Invalid Instruction");
            }

            SwapWord((BYTE*)(*Opcode+Pos),&wOp,&wMem);
            wsprintf(temp,"%04X %02X",wOp,(BYTE)(*(*Opcode+Pos+2)));
            (*(*index))++;
            m_OpcodeSize++;
        }
        break;

        case 0x72:
        {
            BYTE NextByte = (BYTE)(*(*Opcode+Pos+1));
            
            if(
                (NextByte>=0xD0 && NextByte<=0xD7) ||
                (NextByte>=0xE0 && NextByte<=0xE7) ||
                (NextByte>=0xF0 && NextByte<=0xF7) 
                )
            {            
                if(NextByte>=0xD0 && NextByte<=0xD7)
                    strcpy(temp,"psrld");
                else if (NextByte>=0xE0 && NextByte<=0xE7)
                    strcpy(temp,"psrad");
                else
                    strcpy(temp,"pslld");                
                
                wsprintf(assembly,"%s %s,%02X",temp,Regs3DNow[reg2],(BYTE)(*(*Opcode+Pos+2)));
            }
            else
            {
                strcpy(assembly,"???");
                lstrcat((*Disasm)->Remarks,"Invalid Instruction");
            }
            
            SwapWord((BYTE*)(*Opcode+Pos),&wOp,&wMem);
            wsprintf(temp,"%04X %02X",wOp,(BYTE)(*(*Opcode+Pos+2)));
            (*(*index))++;
            m_OpcodeSize++;
        }
        break;

        case 0xBA:
        {
            BYTE NextByte = (BYTE)(*(*Opcode+Pos+1));
            
            if(NextByte>=0xE0 && NextByte<=0xFF)
            {            
                if(NextByte>=0xE0 && NextByte<=0xE7)
                    strcpy(temp,"bt");
                else if (NextByte>=0xE8 && NextByte<=0xEF)
                    strcpy(temp,"bts");
                else if (NextByte>=0xF0 && NextByte<=0xF7)
                        strcpy(temp,"btr");
                    else
                        strcpy(temp,"btc");
                
                wsprintf(assembly,"%s %s,%02X",temp,regs[RM][reg2],(BYTE)(*(*Opcode+Pos+2)));
            }
            else
            {
                strcpy(assembly,"???");
                lstrcat((*Disasm)->Remarks,"Invalid Instruction");
            }
            
            SwapWord((BYTE*)(*Opcode+Pos),&wOp,&wMem);
            wsprintf(temp,"%04X %02X",wOp,(BYTE)(*(*Opcode+Pos+2)));
            (*(*index))++;
            m_OpcodeSize++;
        }
        break;
        
    }
    
    lstrcat((*Disasm)->Assembly,assembly);
    (*Disasm)->OpcodeSize=m_OpcodeSize;
    lstrcat((*Disasm)->Opcode,temp);    
    (*(*index))++;
}

void CPayLoadDlg::ShowDecoded(DISASSEMBLY Disasm)
{
	char space[256];
	char addr[64];
	char opcode[128];
	char disasm[128];
	memset(space,0,sizeof(space));
	wsprintf(addr,"%08X:  ",Disasm.Address);
	strcat(space,addr);
	wsprintf(opcode,"%12s   ",Disasm.Opcode);
	strcat(space,opcode);
	wsprintf(disasm,"	%s",Disasm.Assembly);
	strcat(space,disasm);
	strcat(space,Disasm.Remarks);
	strcat(space,"\r\n");
	m_info+=space;
	
}

void CPayLoadDlg::OnButtonCode() 
{
	// TODO: Add your control notification handler code here
	if(m_path.IsEmpty())
	{
		MessageBox("请选择文件!","提示",MB_OK);
		return;
	}
	m_info="";
	char *Opcodes;
	DWORD lenght;
	FILE *hFile;
	char len[10];
	hFile=fopen(m_path,"r");
	if(hFile==NULL)
	{
		MessageBox("打开文件出错!","提示",MB_OK);
		return;
	}
	fseek(hFile,0,SEEK_END);
	//获取文件长度;
    lenght=ftell(hFile);
	//printf("%d",lenght);
	fseek(hFile,0,SEEK_SET);
	Opcodes=(char*)malloc(lenght);
	fread(Opcodes,1,lenght,hFile);
	wsprintf(len,"%d",lenght);
	m_info="unsigned char shellcode[";
	m_info+=len;
	m_info+="]=\r\n{";
	char opcode[1024];
	
//	wsprintf(opcode,"0x%02X",Opcodes);
//	strcat(cpucode,opcode);
	// Index of opcoded to decode
    DWORD Index=0;

	for(Index=0;Index<lenght;Index++)
	{
		if(Index%16==0)
		{
			m_info+="\r\n    ";
		}
		if(Index==lenght-1)
		{
			wsprintf(opcode,"0x%02X",(BYTE)Opcodes[Index]);
			m_info+=opcode;
		}
		else
		{
			wsprintf(opcode,"0x%02X,",(BYTE)Opcodes[Index]);
			m_info+=opcode;
			
		}
	}
//	m_info+=cpucode;
	m_info+="\r\n}";
	this->UpdateData(FALSE);
	free(Opcodes);
	fclose(hFile);
}

//DEL void CPayLoadDlg::OnButtonTest() 
//DEL {
//DEL 	// TODO: Add your control notification handler code here
//DEL 	if(m_path.IsEmpty())
//DEL 	{
//DEL 		MessageBox("请选择文件!","提示",MB_OK);
//DEL 		return;
//DEL 	}
//DEL 	char *Opcodes;
//DEL 	DWORD lenght;
//DEL 	FILE *hFile;
//DEL 	char len[10];
//DEL 	hFile=fopen(m_path,"r");
//DEL 	if(hFile==NULL)
//DEL 	{
//DEL 		MessageBox("打开文件出错!","提示",MB_OK);
//DEL 		return;
//DEL 	}
//DEL 	fseek(hFile,0,SEEK_END);
//DEL 	//获取文件长度;
//DEL     lenght=ftell(hFile);
//DEL 	//printf("%d",lenght);
//DEL 	fseek(hFile,0,SEEK_SET);
//DEL 	Opcodes=(char*)malloc(lenght);
//DEL 	fread(Opcodes,1,lenght,hFile);
//DEL 	((void (*)())Opcodes)();
//DEL }



void CPayLoadDlg::DisAssembly(char *file, int start, int lenght)
{
	DWORD Index=0;
	char *Linear="";
	DISASSEMBLY Disasssembly;
	Linear=(char*)file;     // Points to the address of array to decode.
    Disasssembly.Address=0x00401000; // Common Entry Point (usually default..)
    FlushDecoded(&Disasssembly);     // reset all content

	for(Index=start;Index<start+lenght;Index++)
    {
		// Decode instruction
        Decode(&Disasssembly,Linear,&Index);

		// Upper case Assembly (Optional)
        CharUpper(Disasssembly.Assembly);
		// Show Decoded instruction, size, remarks...
        MakeDecoded(Disasssembly);
		// Calculate total Size of an instruction + Prefixes, and
		// Fix the address of IP 
        Disasssembly.Address+=Disasssembly.OpcodeSize+Disasssembly.PrefixSize;
		// Clear all information
        FlushDecoded(&Disasssembly);
    }
//	CPEDasmDlg dlg;
//	dlg.SetStringValue(strinfo);
	
}

void CPayLoadDlg::MakeDecoded(DISASSEMBLY Disasm)
{
	char space[256];
	char addr[64];
	char opcode[128];
	char disasm[128];
	memset(space,0,sizeof(space));
	wsprintf(addr,"%08X:  ",Disasm.Address);
	strcat(space,addr);
	wsprintf(opcode,"%16s   ",Disasm.Opcode);
	strcat(space,opcode);
	wsprintf(disasm,"	%s",Disasm.Assembly);
	strcat(space,disasm);
	strcat(space,Disasm.Remarks);
	strcat(space,"\r\n");
	strinfo+=space;
}

CString CPayLoadDlg::Getstr()
{
	return strinfo;
}
