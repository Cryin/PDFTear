; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CPDFTearDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "PDFTear.h"

ClassCount=6
Class1=CPDFTearApp
Class2=CPDFTearDlg
Class3=CAboutDlg

ResourceCount=8
Resource1=IDD_PDFTEAR_DIALOG
Resource2=IDR_MAINFRAME
Resource3=IDD_DIALOG_PAYLOAD
Resource4=IDD_DIALOG_JS
Class4=CPayLoadDlg
Resource5=IDD_ABOUTBOX
Class5=CDecodeDlg
Resource6=IDD_DIALOG_DECODE
Resource7=IDR_MENU_MAIN
Class6=CJsViewDlg
Resource8=IDR_MENU_FILE

[CLS:CPDFTearApp]
Type=0
HeaderFile=PDFTear.h
ImplementationFile=PDFTear.cpp
Filter=N

[CLS:CPDFTearDlg]
Type=0
HeaderFile=PDFTearDlg.h
ImplementationFile=PDFTearDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=CPDFTearDlg

[CLS:CAboutDlg]
Type=0
HeaderFile=PDFTearDlg.h
ImplementationFile=PDFTearDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_PDFTEAR_DIALOG]
Type=1
Class=CPDFTearDlg
ControlCount=7
Control1=IDC_STATIC,button,1342177287
Control2=IDC_LIST_VIEW,SysListView32,1350631425
Control3=IDC_STATIC,button,1342177287
Control4=IDC_EDIT_FILENAME,edit,1350633600
Control5=IDC_BUTTON_BROWSER,button,1342242816
Control6=IDC_STATIC,static,1342308352
Control7=IDC_BUTTON_DOWORK,button,1342242816

[MNU:IDR_MENU_MAIN]
Type=1
Class=?
Command1=IDR_FILE_OPENF
Command2=IDR_FILE_EXIT
Command3=IDR_PAYLOAD_TOOL
Command4=IDR_DECODE_XOR
Command5=IDR_JSVIEW_TOOL
Command6=IDR_HELP_ABOUT
CommandCount=6

[DLG:IDD_DIALOG_PAYLOAD]
Type=1
Class=CPayLoadDlg
ControlCount=8
Control1=IDC_STATIC,button,1342177287
Control2=IDC_STATIC,static,1342308352
Control3=IDC_EDIT_PATH,edit,1350633600
Control4=IDC_BUTTON_BROWSER,button,1342242816
Control5=IDC_BUTTON_DASM,button,1342242816
Control6=IDC_BUTTON_CODE,button,1342242816
Control7=IDC_EDIT_INFO,edit,1352730628
Control8=IDC_STATIC,static,1476530176

[CLS:CPayLoadDlg]
Type=0
HeaderFile=PayLoadDlg.h
ImplementationFile=PayLoadDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDR_PAYLOAD_TOOL

[DLG:IDD_DIALOG_DECODE]
Type=1
Class=CDecodeDlg
ControlCount=19
Control1=IDC_STATIC,button,1342177287
Control2=IDC_STATIC,static,1342308352
Control3=IDC_EDIT_PATH,edit,1350633600
Control4=IDC_BUTTON_BROWSER,button,1342242816
Control5=IDC_STATIC,button,1342177287
Control6=IDC_EDIT_INFO,edit,1352730628
Control7=IDC_STATIC,static,1476526080
Control8=IDC_STATIC,button,1342177287
Control9=IDC_RADIO_XOR,button,1342308361
Control10=IDC_RADIO_ADDXOR,button,1342177289
Control11=IDC_RADIO_DECXOR,button,1342177289
Control12=IDC_RADIO_BYTE,button,1342308361
Control13=IDC_RADIO_WORD,button,1342177289
Control14=IDC_RADIO_DWORD,button,1342177289
Control15=IDC_STATIC,static,1342308352
Control16=IDC_EDIT_TARGET,edit,1350631552
Control17=IDC_STATIC,static,1342308352
Control18=IDC_BUTTON_OK,button,1342242816
Control19=IDC_BUTTON_BYE,button,1342242816

[CLS:CDecodeDlg]
Type=0
HeaderFile=DecodeDlg.h
ImplementationFile=DecodeDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_RADIO_DWORD

[MNU:IDR_MENU_FILE]
Type=1
Class=?
Command1=ID_MENU_FILE
CommandCount=1

[DLG:IDD_DIALOG_JS]
Type=1
Class=CJsViewDlg
ControlCount=11
Control1=IDC_STATIC,button,1342177287
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,static,1342308352
Control4=IDC_EDIT_START,edit,1350631552
Control5=IDC_EDIT_END,edit,1350631552
Control6=IDC_BUTTON_DECODE,button,1342242816
Control7=IDC_BUTTON_CODE,button,1208025088
Control8=IDC_STATIC,button,1342177287
Control9=IDC_EDIT_INFO,edit,1352728580
Control10=IDC_STATIC,static,1476526080
Control11=IDC_STATIC,static,1476526080

[CLS:CJsViewDlg]
Type=0
HeaderFile=JsViewDlg.h
ImplementationFile=JsViewDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=IDR_JSVIEW_TOOL
VirtualFilter=dWC

