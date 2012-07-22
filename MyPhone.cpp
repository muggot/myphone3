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

	CMyPhoneDlg dlg;
	m_pMainWnd = &dlg;
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
