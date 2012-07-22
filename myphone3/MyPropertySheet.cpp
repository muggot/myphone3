///////////////////////////////////////////////////////////////////////////////
// MyPropertySheet.cpp : implementation file for MySettings dialog window
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
#include "myphone.h"
#include "MyPropertySheet.h"
//
#include "Multilang.h"		// Languages
//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyPropertySheet

IMPLEMENT_DYNAMIC(CMyPropertySheet, CPropertySheet)

CMyPropertySheet::CMyPropertySheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
}

CMyPropertySheet::CMyPropertySheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
}

CMyPropertySheet::~CMyPropertySheet()
{
}


BEGIN_MESSAGE_MAP(CMyPropertySheet, CPropertySheet)
	//{{AFX_MSG_MAP(CMyPropertySheet)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyPropertySheet message handlers

BOOL CMyPropertySheet::OnInitDialog() 
{
	CPropertySheet::OnInitDialog();
	
	CTabCtrl* tab = GetTabControl();
	if(tab)
	{
		TCITEM iTab;
		CString newTitle;
		iTab.mask = TCIF_TEXT;
		newTitle = LoadStringLang(IDS_GENERALSTR);
		iTab.pszText = newTitle.GetBuffer(0);
		tab->SetItem(0, &iTab);
		//
		newTitle = LoadStringLang(IDS_NETWORKSTR);
		iTab.pszText = newTitle.GetBuffer(0);
		tab->SetItem(1, &iTab);
		//
		newTitle = LoadStringLang(IDS_GKSTR);
		iTab.pszText = newTitle.GetBuffer(0);
		tab->SetItem(2, &iTab);
		//
		newTitle = LoadStringLang(IDS_AUDIOSTR);
		iTab.pszText = newTitle.GetBuffer(0);
		tab->SetItem(3, &iTab);
		//
		newTitle = LoadStringLang(IDS_VIDEOSTR);
		iTab.pszText = newTitle.GetBuffer(0);
		tab->SetItem(4, &iTab);
		//
		newTitle = LoadStringLang(IDS_IFACESTR);
		iTab.pszText = newTitle.GetBuffer(0);
		tab->SetItem(5, &iTab);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

int CMyPropertySheet::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	EnableStackedTabs(FALSE);
	ModifyStyleEx(0, WS_EX_TOOLWINDOW);	

	if (CPropertySheet::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}
