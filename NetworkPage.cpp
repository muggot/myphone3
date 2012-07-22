///////////////////////////////////////////////////////////////////////////////
// NetworkPage.cpp : implementation file for Network Settings Dialog class
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
#include "NetworkPage.h"
//
#include "Multilang.h"		// Languages
//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNetworkPage property page

IMPLEMENT_DYNCREATE(CNetworkPage, CPropertyPage)

void DDX_Hex(CDataExchange* pDX, int nIDC, long& value)
{
	HWND hWndCtrl = pDX->PrepareEditCtrl(nIDC);
	TCHAR szT[64];

	if (pDX->m_bSaveAndValidate)
	{
		// the following works for %d, %u, %ld, %lu
		::GetWindowText(hWndCtrl, szT, 63);
		TCHAR *endptr=NULL;
		value = _tcstol(szT,&endptr,16);
		if (endptr==NULL || endptr<=szT)
		{
			AfxMessageBox(AFX_IDP_PARSE_INT);
			pDX->Fail();        
		}
	}
	else
	{
		wsprintf(szT, _T("%X"), value);
		::SetWindowText(hWndCtrl, szT);
	}
}

CNetworkPage::CNetworkPage() : CPropertyPage(CNetworkPage::IDD)
{
	//{{AFX_DATA_INIT(CNetworkPage)
	m_BandwidthType = -1;
	m_Bandwidth = 0;
	m_IP_Type = 0;
	m_linterface = _T("*");
	m_natip = _T("");
	m_rtpbase = 0;
	m_rtpmax = 0;
	//}}AFX_DATA_INIT

	this->m_psp.dwFlags^=PSP_HASHELP;
}

CNetworkPage::~CNetworkPage()
{
}

void CNetworkPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNetworkPage)
	DDX_Control(pDX, IDC_BANDWIDTH_COMBO, m_BandwidthTypeCtrl);
	DDX_Control(pDX, IDC_BANDWIDTH_EDIT, m_BandwidthEdt);
	DDX_CBIndex(pDX, IDC_BANDWIDTH_COMBO, m_BandwidthType);
	DDX_Text(pDX, IDC_BANDWIDTH_EDIT, m_Bandwidth);
	DDV_MinMaxDouble(pDX, m_Bandwidth, 1., 999999.);
	DDX_Hex(pDX, IDC_IP_TYPE_EDIT, m_IP_Type);
	DDV_MinMaxLong(pDX, m_IP_Type, 0, 255);
	DDX_Text(pDX, IDC_LOCALIEDIT, m_linterface);
	DDX_Text(pDX, IDC_NATREDIT, m_natip);
	DDX_Text(pDX, IDC_RTPBEDIT, m_rtpbase);
	DDV_MinMaxUInt(pDX, m_rtpbase, 0, 65000);
	DDX_Text(pDX, IDC_RTPMEDIT, m_rtpmax);
	DDV_MinMaxUInt(pDX, m_rtpmax, 0, 65000);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNetworkPage, CPropertyPage)
	//{{AFX_MSG_MAP(CNetworkPage)
	ON_CBN_SELCHANGE(IDC_BANDWIDTH_COMBO, OnSelchangeBandwidthCombo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNetworkPage message handlers

BOOL CNetworkPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	m_BandwidthEdt.EnableWindow(m_BandwidthType == 6);

	TranslateDlg();	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CNetworkPage::OnSelchangeBandwidthCombo() 
{
	int nSel = m_BandwidthTypeCtrl.GetCurSel();
	m_BandwidthEdt.EnableWindow(nSel==6);
	if (nSel<6 && nSel>=0)
	{
		static double const bandwidths[6] = {14400, 28800, 64000, 128000, 1500000, 10000000};
		double bandwidth = bandwidths[nSel]/1000.0;
		CString value;
		value.Format(_T("%.2f"),bandwidth);
		m_BandwidthEdt.SetWindowText(value);
	}
}

void CNetworkPage::TranslateDlg()
{	// changing window languge
	GetDlgItem(IDC_BANDSTATIC)->SetWindowText((LPCTSTR)LoadStringLang(IDS_BANDSTR));
	GetDlgItem(IDC_KBSSTATIC)->SetWindowText((LPCTSTR)LoadStringLang(IDS_KBSSTR));
	GetDlgItem(IDC_RTPBSTATIC)->SetWindowText((LPCTSTR)LoadStringLang(IDS_RTPBSTR));
	GetDlgItem(IDC_RTPMSTATIC)->SetWindowText((LPCTSTR)LoadStringLang(IDS_RTPMSTR));
	GetDlgItem(IDC_NATRSTATIC)->SetWindowText((LPCTSTR)LoadStringLang(IDS_NATRSTR));
	GetDlgItem(IDC_LOCISTATIC)->SetWindowText((LPCTSTR)LoadStringLang(IDS_LOCISTR));
	GetDlgItem(IDC_IPTSTATIC)->SetWindowText((LPCTSTR)LoadStringLang(IDS_IPTSTR));	

}

void CNetworkPage::OnOK() 
{
	UpdateData();
	m_natip.TrimLeft();
	m_natip.TrimRight();
	m_linterface.TrimLeft();
	m_linterface.TrimRight();
	m_linterface = m_linterface.IsEmpty()?_T("*"):m_linterface;

	CPropertyPage::OnOK();
}
