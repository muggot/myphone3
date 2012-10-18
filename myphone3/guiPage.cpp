///////////////////////////////////////////////////////////////////////////////
// guiPage.cpp : implementation file for GUI Settings Dialog class
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
#include "guiPage.h"
//
#include "Multilang.h"		// Languages
//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

COLORREF aCustom[16];
/////////////////////////////////////////////////////////////////////////////
// CguiPage property page

IMPLEMENT_DYNCREATE(CguiPage, CPropertyPage)

CguiPage::CguiPage() : CPropertyPage(CguiPage::IDD)
{
	//{{AFX_DATA_INIT(CguiPage)
	m_vidhidechk = FALSE;
	m_loghidechk = FALSE;
	m_stathidechk = FALSE;
	m_systraychk = FALSE;
	m_autoaddadrchk = FALSE;
	m_fullscrdflt = FALSE;
	m_lang = -1;
	//}}AFX_DATA_INIT
	clrInMsg = GetSysColor(COLOR_BTNFACE);
	clrOutMsg = GetSysColor(COLOR_BTNFACE);
	clrActInd = GetSysColor(COLOR_BTNFACE);
	clrSlndInd = GetSysColor(COLOR_BTNFACE);

	this->m_psp.dwFlags^=PSP_HASHELP;
}

CguiPage::~CguiPage()
{
}

void CguiPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CguiPage)
	DDX_Check(pDX, IDC_HIDEVIDEOCHK, m_vidhidechk);
	DDX_Check(pDX, IDC_HIDESYSMSG, m_loghidechk);
	DDX_Check(pDX, IDC_HIDESTAT, m_stathidechk);
	DDX_Check(pDX, IDC_SYSTRAYMIN, m_systraychk);
	DDX_Check(pDX, IDC_AUTOADDADDRESS, m_autoaddadrchk);
	DDX_CBIndex(pDX, IDC_LANGCOMBO, m_lang);
	DDX_Check(pDX, IDC_FULLSCRDEFLT, m_fullscrdflt);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CguiPage, CPropertyPage)
	//{{AFX_MSG_MAP(CguiPage)
	ON_BN_CLICKED(IDC_INMSGCLR, OnInmsgclr)
	ON_WM_DRAWITEM()
	ON_BN_CLICKED(IDC_OUTMSGCLR, OnOutmsgclr)
	ON_BN_CLICKED(IDC_ACTINDCLR, OnActindclr)
	ON_BN_CLICKED(IDC_SLNDINDCLR, OnSlndindclr)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CguiPage message handlers

void CguiPage::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// changing color for Color selection byuttons
	CRect curRc = lpDrawItemStruct->rcItem;
	switch(nIDCtl)
	{
	case IDC_INMSGCLR:
		{
		HBRUSH hBgBr = CreateSolidBrush(GetSysColor(COLOR_3DDKSHADOW));
		FillRect(lpDrawItemStruct->hDC, &curRc, (HBRUSH) hBgBr);
		curRc.DeflateRect(1,1);
		HBRUSH hBr = CreateSolidBrush(clrInMsg);
		FillRect(lpDrawItemStruct->hDC, &curRc, (HBRUSH) hBr);
		::DeleteObject(hBr);
		::DeleteObject(hBgBr);	
		}
		break;
	case IDC_OUTMSGCLR:
		{
		HBRUSH hBgBr = CreateSolidBrush(GetSysColor(COLOR_3DDKSHADOW));
		FillRect(lpDrawItemStruct->hDC, &curRc, (HBRUSH) hBgBr);
		curRc.DeflateRect(1,1);
		HBRUSH hBr = CreateSolidBrush(clrOutMsg);
		FillRect(lpDrawItemStruct->hDC, &curRc, (HBRUSH) hBr);
		::DeleteObject(hBr);	
		::DeleteObject(hBgBr);	
		}
		break;
	case IDC_ACTINDCLR:
		{
		HBRUSH hBgBr = CreateSolidBrush(GetSysColor(COLOR_3DDKSHADOW));
		FillRect(lpDrawItemStruct->hDC, &curRc, (HBRUSH) hBgBr);
		curRc.DeflateRect(1,1);
		HBRUSH hBr = CreateSolidBrush(clrActInd);
		FillRect(lpDrawItemStruct->hDC, &curRc, (HBRUSH) hBr);
		::DeleteObject(hBr);	
		::DeleteObject(hBgBr);	
		}
		break;
	case IDC_SLNDINDCLR:
		{
		HBRUSH hBgBr = CreateSolidBrush(GetSysColor(COLOR_3DDKSHADOW));
		FillRect(lpDrawItemStruct->hDC, &curRc, (HBRUSH) hBgBr);
		curRc.DeflateRect(1,1);
		HBRUSH hBr = CreateSolidBrush(clrSlndInd);
		FillRect(lpDrawItemStruct->hDC, &curRc, (HBRUSH) hBr);
		::DeleteObject(hBr);	
		::DeleteObject(hBgBr);	
		}
		break;
	}
	CPropertyPage::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

void CguiPage::OnInmsgclr() 
{
	//	Color select for incoming messages
  CColorDialog clrDlg;

  clrDlg.m_cc.lStructSize = sizeof(clrDlg.m_cc);
  clrDlg.m_cc.rgbResult = clrInMsg;
//  clrDlg.m_cc.lpCustColors = aCustom;

  clrDlg.m_cc.Flags = CC_ENABLEHOOK; // CC_FULLOPEN | CC_RGBINIT

  if(clrDlg.DoModal() == IDOK)
  {
    clrInMsg = clrDlg.GetColor();
  }
  Invalidate();
}

void CguiPage::OnOutmsgclr() 
{
	//	Color select for outgoing messages
  CColorDialog clrDlg;

  clrDlg.m_cc.lStructSize = sizeof(clrDlg.m_cc);
  clrDlg.m_cc.rgbResult = clrOutMsg;

  clrDlg.m_cc.Flags = CC_ENABLEHOOK; // CC_FULLOPEN | CC_RGBINIT

  if(clrDlg.DoModal() == IDOK)
  {
    clrOutMsg = clrDlg.GetColor();
  }
  Invalidate();
}

void CguiPage::OnActindclr() 
{
	//	Color select for active Sound level indicator
  CColorDialog clrDlg;

  clrDlg.m_cc.lStructSize = sizeof(clrDlg.m_cc);
  clrDlg.m_cc.rgbResult = clrActInd;
  clrDlg.m_cc.lpCustColors = aCustom;

  clrDlg.m_cc.Flags = CC_ENABLEHOOK; // CC_FULLOPEN | CC_RGBINIT

  if(clrDlg.DoModal() == IDOK)
  {
    clrActInd = clrDlg.GetColor();
  }
	Invalidate();
}

void CguiPage::OnSlndindclr() 
{
	//	Color select for silent level indicator
  CColorDialog clrDlg;

  clrDlg.m_cc.lStructSize = sizeof(clrDlg.m_cc);
  clrDlg.m_cc.rgbResult = clrSlndInd;

  clrDlg.m_cc.Flags = CC_ENABLEHOOK; // CC_FULLOPEN | CC_RGBINIT

  if(clrDlg.DoModal() == IDOK)
  {
    clrSlndInd = clrDlg.GetColor();
  }
  Invalidate();
}

BOOL CguiPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	if(m_lang<0) m_lang=0;
	UpdateData(FALSE);
	//
	TranslateDlg();
	//
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CguiPage::TranslateDlg()
{	// changing window languge
	GetDlgItem(IDC_HIDEVIDEOCHK)->SetWindowText((LPCTSTR)LoadStringLang(IDS_HVIDEOSTR));
	GetDlgItem(IDC_HIDESYSMSG)->SetWindowText((LPCTSTR)LoadStringLang(IDS_HLOGSTR));
	GetDlgItem(IDC_HIDESTAT)->SetWindowText((LPCTSTR)LoadStringLang(IDS_HSTATSTR));
	GetDlgItem(IDC_AUTOADDADDRESS)->SetWindowText((LPCTSTR)LoadStringLang(IDS_ADRAUTOADDSTR));
	GetDlgItem(IDC_SYSTRAYMIN)->SetWindowText((LPCTSTR)LoadStringLang(IDS_SYSTRAYMINSTR));
	GetDlgItem(IDC_LANGSTATIC)->SetWindowText((LPCTSTR)LoadStringLang(IDS_LANGSTR));	
	GetDlgItem(IDC_COLSTATIC)->SetWindowText((LPCTSTR)LoadStringLang(IDS_COLSTR));	
	GetDlgItem(IDC_COLMSGSTATIC)->SetWindowText((LPCTSTR)LoadStringLang(IDS_COLMSGSTR));	
	GetDlgItem(IDC_COLAUDIOSTATIC)->SetWindowText((LPCTSTR)LoadStringLang(IDS_COLAUDIOSTR));
	GetDlgItem(IDC_COLLSTATIC)->SetWindowText((LPCTSTR)LoadStringLang(IDS_COLLSTR));
	GetDlgItem(IDC_COLRSTATIC)->SetWindowText((LPCTSTR)LoadStringLang(IDS_COLRSTR));
	GetDlgItem(IDC_COLASTATIC)->SetWindowText((LPCTSTR)LoadStringLang(IDS_COLASTR));
	GetDlgItem(IDC_COLSSTATIC)->SetWindowText((LPCTSTR)LoadStringLang(IDS_COLSSTR));
	GetDlgItem(IDC_FULLSCRDEFLT)->SetWindowText((LPCTSTR)LoadStringLang(IDS_FULLSCREEN));
//	SetWindowText((LPCTSTR)LoadStringLang(IDS_AUDIOSTR));	
}
