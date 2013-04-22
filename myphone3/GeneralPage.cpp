///////////////////////////////////////////////////////////////////////////////
// GeneralPage.cpp : implementation file for General Settings Dialog class
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
#include "GeneralPage.h"
//
#include "Multilang.h"		// Languages
//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGeneralPage property page

IMPLEMENT_DYNCREATE(CGeneralPage, CPropertyPage)

CGeneralPage::CGeneralPage() : CPropertyPage(CGeneralPage::IDD)
{
	//{{AFX_DATA_INIT(CGeneralPage)
	m_UserName = _T("");
	m_RingSoundFile = _T("");
	m_AutoAnswer = FALSE;
	m_AutoMute = FALSE;
	m_NoTunneling = FALSE;
	m_DtmfAsString = FALSE;
	m_disableFastStart = FALSE;
	m_singleLine = FALSE;

	//}}AFX_DATA_INIT

	this->m_psp.dwFlags^=PSP_HASHELP;
}

CGeneralPage::~CGeneralPage()
{
}

void CGeneralPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGeneralPage)
	DDX_Control(pDX, IDC_RING_FILE_EDIT, m_RingFileCtrl);
	DDX_Control(pDX, IDC_RING_FILE_PLAY, m_PlayBtn);
	DDX_Text(pDX, IDC_USERNAME_EDIT, m_UserName);
	DDX_Text(pDX, IDC_RING_FILE_EDIT, m_RingSoundFile);
	DDX_Check(pDX, IDC_AUTO_ANSWER_CHECK, m_AutoAnswer);
	DDX_Check(pDX, IDC_AUTO_MUTE_CHECK, m_AutoMute);
	DDX_Check(pDX, IDC_DISABLE_H242_CHECK, m_NoTunneling);
	DDX_Check(pDX, IDC_DTMF_AS_STRING_CHECK, m_DtmfAsString);
	DDX_Check(pDX, IDC_DISABLE_FASTSTART, m_disableFastStart);
	DDX_Check(pDX, IDC_STRICT_SINGLE_LINE, m_singleLine);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGeneralPage, CPropertyPage)
	//{{AFX_MSG_MAP(CGeneralPage)
	ON_BN_CLICKED(IDC_RING_FILE_BROWSE, OnRingFileBrowse)
	ON_BN_CLICKED(IDC_RING_FILE_PLAY, OnRingFilePlay)
	ON_EN_CHANGE(IDC_RING_FILE_EDIT, OnChangeRingFileEdit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGeneralPage message handlers
BOOL CGeneralPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
//	m_AddAliasBtn.EnableWindow(false);
//	m_DelAliasBtn.EnableWindow(false);	
	m_PlayBtn.EnableWindow(!m_RingSoundFile.IsEmpty());
//	CRect rect;
//	m_AliasListCtrl.GetClientRect(&rect);
//	m_AliasListCtrl.InsertColumn(0,_T(""),LVCFMT_LEFT,rect.Width(),0);

//	for (int i=0;i<m_AliasList.GetSize();i++)
//	{
//		m_AliasListCtrl.InsertItem(i,m_AliasList.GetAt(i));
//	}
	TranslateDlg();	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CGeneralPage::OnRingFileBrowse() 
{
	TCHAR szFilters[]= _T("Sound Files (*.wav)|*.wav|All Files (*.*)|*.*||");

	CString File;
	m_RingFileCtrl.GetWindowText(File);
	CFileDialog fileDlg (TRUE, _T("wav"), File,OFN_FILEMUSTEXIST,szFilters, this);
	if( fileDlg.DoModal ()==IDOK )
	{
		CString File = fileDlg.GetPathName();
		m_RingFileCtrl.SetWindowText(File);
	}
	OnChangeRingFileEdit();
}

void CGeneralPage::OnRingFilePlay() 
{
	CString File;
	m_RingFileCtrl.GetWindowText(File);

	PSound::PlayFile(PString(File), FALSE);
}

void CGeneralPage::OnChangeRingFileEdit() 
{
	CString File;
	m_RingFileCtrl.GetWindowText(File);	
	m_PlayBtn.EnableWindow(!File.IsEmpty());
}

void CGeneralPage::OnOK() 
{
	UpdateData();
	m_UserName.Replace("|", "_");
	CPropertyPage::OnOK();
}

void CGeneralPage::TranslateDlg()
{	// changing window languge
	GetDlgItem(IDC_USERSTATIC)->SetWindowText((LPCTSTR)LoadStringLang(IDS_USERSTR));
	GetDlgItem(IDC_UNAMESTATIC)->SetWindowText((LPCTSTR)LoadStringLang(IDS_UNAMESTR));
	GetDlgItem(IDC_AUTO_ANSWER_CHECK)->SetWindowText((LPCTSTR)LoadStringLang(IDS_AANSWERSTR));
	GetDlgItem(IDC_AUTO_MUTE_CHECK)->SetWindowText((LPCTSTR)LoadStringLang(IDS_AUTOMUTESTR));
	GetDlgItem(IDC_DTMF_AS_STRING_CHECK)->SetWindowText((LPCTSTR)LoadStringLang(IDS_DTMFSTRSTR));
	GetDlgItem(IDC_DISABLE_H242_CHECK)->SetWindowText((LPCTSTR)LoadStringLang(IDS_D242STR));
	GetDlgItem(IDC_DISABLE_FASTSTART)->SetWindowText((LPCTSTR)LoadStringLang(IDS_DFSTARTSTR));
	GetDlgItem(IDC_RINGSTATIC)->SetWindowText((LPCTSTR)LoadStringLang(IDS_RINGFSTR));
	GetDlgItem(IDC_STRICT_SINGLE_LINE)->SetWindowText((LPCTSTR)LoadStringLang(IDS_SINGLELINE));
}
