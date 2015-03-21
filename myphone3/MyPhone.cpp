///////////////////////////////////////////////////////////////////////////////
// MyPhone.cpp : Defines the class behaviors for the application.
//
// MyPhone  -  Audio/Video IP telephony client.
// Compatible with H323 protocol, based on H323plus project.
// 
// Home Page: http://www.h323plus.org/
//
// Copyright (c) 2003 Michael Gerdov aka -=MaGGuS=-
// Copyright (c) 2007 ISVO (Asia) Pte Ltd.
//
// The contents of this file are subject to the Mozilla Public License
// Version 1.0 (the "License"); you may not use this file except in
// compliance with the License. You may obtain a copy of the License at
// http://www.mozilla.org/MPL/
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See
// the License for the specific language governing rights and limitations
// under the License.
// 
// Thanks goes to All OpenH323/H323plus members (www.h323plus.org)
//				  and especially to Yuriy Gorvitovskiy (PocketBone).
//
///////////////////////////////////////////////////////////////////////////////
    
#include "stdafx.h"
#include "MyPhone.h"
//#include "MyMainMenu.h"
#include "MyPhoneDlg.h"
//
#include "Multilang.h"		// Languages
//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
/////////////////////////////////////////////////////////////////////////////
// UI Language
int curLang=-1;

/////////////////////////////////////////////////////////////////////////////
// CMyPhoneApp

BEGIN_MESSAGE_MAP(CMyPhoneApp, CWinApp)
	//{{AFX_MSG_MAP(CMyPhoneApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyPhoneApp construction

CMyPhoneApp::CMyPhoneApp()
{
	// Setting default Languge
    curLang=GetUserDefaultLangID();
}
/////////////////////////////////////////////////////////////////////////////
// The one and only CMyPhoneApp object

CMyPhoneApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CMyPhoneApp initialization

BOOL CMyPhoneApp::InitInstance()
{
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

InitCommonControls();  // for XP Manifest only

#if _MSC_VER < 1300
#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif
#endif

//        CDialog vdlg(IDD_VIDEOBOX);
//      CVideoDlg vdlg;
//        vdlg.ShowWindow(SW_SHOW);
//        vdlg.DoModal();

//   CWnd* pWnd = new CStatic;
//   pWnd->CreateEx(WS_EX_OVERLAPPEDWINDOW, // Make a client edge label.
//      _T("STATIC"), "Hi",
//      WS_OVERLAPPED | WS_VISIBLE,
//      5, 5, 300, 300, NULL, NULL);

        CVideoDlg vdlg;
	vdlg.Create(CVideoDlg::IDD,NULL);

	CMyPhoneDlg dlg;
	m_pMainWnd = &dlg;

        dlg.vdlg = &vdlg;

	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

PString convert_cp1251_to_utf8(const PString & str)
{
  static const int table[128] = { // cp1251 -> utf8 translation based on http://www.linux.org.ru/forum/development/3968525
    0x82D0,0x83D0,  0x9A80E2,0x93D1,  0x9E80E2,0xA680E2,0xA080E2,0xA180E2,0xAC82E2,0xB080E2,0x89D0,0xB980E2,0x8AD0,0x8CD0,0x8BD0,0x8FD0,
    0x92D1,0x9880E2,0x9980E2,0x9C80E2,0x9D80E2,0xA280E2,0x9380E2,0x9480E2,0,       0xA284E2,0x99D1,0xBA80E2,0x9AD1,0x9CD1,0x9BD1,0x9FD1,
    0xA0C2,0x8ED0,  0x9ED1,  0x88D0,  0xA4C2,  0x90D2,  0xA6C2,  0xA7C2,  0x81D0,  0xA9C2,  0x84D0,0xABC2,  0xACC2,0xADC2,0xAEC2,0x87D0,
    0xB0C2,0xB1C2,  0x86D0,  0x96D1,  0x91D2,  0xB5C2,  0xB6C2,  0xB7C2,  0x91D1,  0x9684E2,0x94D1,0xBBC2,  0x98D1,0x85D0,0x95D1,0x97D1,
    0x90D0,0x91D0,  0x92D0,  0x93D0,  0x94D0,  0x95D0,  0x96D0,  0x97D0,  0x98D0,  0x99D0,  0x9AD0,0x9BD0,  0x9CD0,0x9DD0,0x9ED0,0x9FD0,
    0xA0D0,0xA1D0,  0xA2D0,  0xA3D0,  0xA4D0,  0xA5D0,  0xA6D0,  0xA7D0,  0xA8D0,  0xA9D0,  0xAAD0,0xABD0,  0xACD0,0xADD0,0xAED0,0xAFD0,
    0xB0D0,0xB1D0,  0xB2D0,  0xB3D0,  0xB4D0,  0xB5D0,  0xB6D0,  0xB7D0,  0xB8D0,  0xB9D0,  0xBAD0,0xBBD0,  0xBCD0,0xBDD0,0xBED0,0xBFD0,
    0x80D1,0x81D1,  0x82D1,  0x83D1,  0x84D1,  0x85D1,  0x86D1,  0x87D1,  0x88D1,  0x89D1,  0x8AD1,0x8BD1,  0x8CD1,0x8DD1,0x8ED1,0x8FD1
  };

  PStringStream utf8;
  for(PINDEX i = 0; i < str.GetLength(); i++)
  {
    unsigned int charcode=(BYTE)str[i];
    if(charcode&128)
    {
      if((charcode=table[charcode&127]))
      {
        utf8 << (char)charcode << (char)(charcode >> 8);
        if(charcode >>= 16) utf8 << (char)charcode;
      }
    } else {
      utf8 << (char)charcode;
    }
  }
  if(str != utf8)
  {
	utf8 += '\0'; // !!!
    PTRACE(1, "Converted cp1251->utf8: " << str << " -> " << utf8);
    return utf8;
  } else {
    return str;
  }
}
